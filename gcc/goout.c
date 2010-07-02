/* Output Go language descriptions of types.
   Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc.
   Written by Ian Lance Taylor <iant@google.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This file is used during the build process to emit Go language
   descriptions of declarations from C header files.  It uses the
   debug info hooks to emit the descriptions.  The Go language
   descriptions then become part of the Go runtime support
   library.

   All global names are output with a leading underscore, so that they
   are all hidden in Go.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "ggc.h"
#include "pointer-set.h"
#include "output.h"
#include "debug.h"

/* A queue of decls to output.  */

static GTY(()) tree queue;

/* A hash table of macros we have seen.  Using macro_hash_node is
   awkward but I don't know how to avoid it for the GTY machinery.  */

struct GTY(()) macro_hash_node
{
  char *name;
};

struct GTY(()) goout_container
{
  /* DECLs that we have already seen.  */
  struct pointer_set_t *decls_seen;

  /* Types which may potentially have to be defined as dummy
     types.  */
  struct pointer_set_t *pot_dummy_types;

  /* Go keywords.  */
  htab_t keyword_hash;

  /* Global type definitions.  */
  htab_t type_hash;
};

static GTY ((param_is (struct macro_hash_node))) htab_t macro_hash;

#ifdef GO_DEBUGGING_INFO

/* For the macro hash table.  */

static hashval_t
macro_hash_hash (const void *x)
{
  return htab_hash_string (((const struct macro_hash_node *) x)->name);
}

static int
macro_hash_eq (const void *x1, const void *x2)
{
  return strcmp ((((const struct macro_hash_node *) x1)->name),
		 (const char *) x2) == 0;
}

/* Initialize.  */

static void
go_init (const char *filename ATTRIBUTE_UNUSED)
{
  macro_hash = htab_create (100, macro_hash_hash, macro_hash_eq, NULL);
}

/* A macro definition.  */

static void
go_define (unsigned int lineno ATTRIBUTE_UNUSED, const char *buffer)
{
  const char *p;
  const char *name_end;
  char *out_buffer;
  char *q;
  char *copy;
  void **slot;

  /* Skip macro functions.  */
  for (p = buffer; *p != '\0' && *p != ' '; ++p)
    if (*p == '(')
      return;

  if (*p == '\0')
    return;

  name_end = p;

  ++p;
  if (*p == '\0')
    return;

  copy = XNEWVEC (char, name_end - buffer + 1);
  memcpy (copy, buffer, name_end - buffer);
  copy[name_end - buffer] = '\0';

  slot = htab_find_slot_with_hash (macro_hash, copy, htab_hash_string (copy),
				   NO_INSERT);
  if (slot != NULL)
    {
      XDELETEVEC (copy);
      return;
    }

  /* For simplicity, we force all names to be hidden by adding an
     initial underscore, and let the user undo this as needed.  */
  out_buffer = XNEWVEC (char, strlen (p) * 2 + 1);
  q = out_buffer;
  while (*p != '\0')
    {
      if (ISALPHA (*p) || *p == '_')
	{
	  const char *start;
	  char *n;

	  start = p;
	  while (ISALNUM (*p) || *p == '_')
	    ++p;
	  n = (char *) alloca (p - start + 1);
	  memcpy (n, start, p - start);
	  n[p - start] = '\0';
	  slot = htab_find_slot_with_hash (macro_hash, n,
					   htab_hash_string (n),
					   NO_INSERT);
	  if (slot == NULL
	      || ((struct macro_hash_node *) *slot)->name == NULL)
	    {
	      /* This is a reference to a name which was not defined
		 as a macro.  */
	      fprintf (asm_out_file, "#GO unknowndefine %s\n", buffer);
	      return;
	    }

	  *q++ = '_';
	  memcpy (q, start, p - start);
	  q += p - start;
	}
      else if (ISDIGIT (*p)
	       || (*p == '.' && ISDIGIT (p[1])))
	{
	  const char *start;
	  bool is_hex;

	  start = p;
	  is_hex = false;
	  if (*p == '0' && (p[1] == 'x' || p[1] == 'X'))
	    {
	      p += 2;
	      is_hex = true;
	    }
	  while (ISDIGIT (*p) || *p == '.' || *p == 'e' || *p == 'E'
		 || (is_hex
		     && ((*p >= 'a' && *p <= 'f')
			 || (*p >= 'A' && *p <= 'F'))))
	    ++p;
	  memcpy (q, start, p - start);
	  q += p - start;
	  while (*p == 'u' || *p == 'U' || *p == 'l' || *p == 'L'
		 || *p == 'f' || *p == 'F'
		 || *p == 'd' || *p == 'D')
	    {
	      /* Go doesn't use any of these trailing type
		 modifiers.  */
	      ++p;
	    }
	}
      else if (ISSPACE (*p)
	       || *p == '+' || *p == '-'
	       || *p == '*' || *p == '/' || *p == '%'
	       || *p == '|' || *p == '&'
	       || *p == '>' || *p == '<'
	       || *p == '!'
	       || *p == '(' || *p == ')'
	       || *p == '"' || *p == '\'')
	*q++ = *p++;
      else
	{
	  /* Something we don't recognize.  */
	  fprintf (asm_out_file, "#GO unknowndefine %s\n", buffer);
	  return;
	}
    }
  *q = '\0';

  slot = htab_find_slot_with_hash (macro_hash, copy, htab_hash_string (copy),
				   INSERT);
  *slot = XNEW (struct macro_hash_node);
  ((struct macro_hash_node *) *slot)->name = copy;

  fprintf (asm_out_file, "#GO const _%s = %s\n", copy, out_buffer);

  XDELETEVEC (out_buffer);
}

/* A macro undef.  */

static void
go_undef (unsigned int lineno ATTRIBUTE_UNUSED,
	  const char *buffer ATTRIBUTE_UNUSED)
{
  void **slot;

  slot = htab_find_slot_with_hash (macro_hash, buffer,
				   htab_hash_string (buffer),
				   NO_INSERT);
  if (slot == NULL)
    return;
  fprintf (asm_out_file, "#GO undef _%s\n", buffer);
  /* We don't delete the slot from the hash table because that will
     cause a duplicate const definition.  */
}

/* A function or variable decl.  */

static void
go_decl (tree decl)
{
  if (!TREE_PUBLIC (decl)
      || DECL_IS_BUILTIN (decl)
      || DECL_NAME (decl) == NULL_TREE)
    return;
  queue = tree_cons (NULL_TREE, decl, queue);
}

/* A type declaration.  */

static void
go_type_decl (tree decl, int local)
{
  if (local || DECL_IS_BUILTIN (decl))
    return;
  if (DECL_NAME (decl) == NULL_TREE
      && (TYPE_NAME (TREE_TYPE (decl)) == NULL_TREE
	  || TREE_CODE (TYPE_NAME (TREE_TYPE (decl))) != IDENTIFIER_NODE)
      && TREE_CODE (TREE_TYPE (decl)) != ENUMERAL_TYPE)
    return;
  queue = tree_cons (NULL_TREE, decl, queue);
}

/* Output a type.  */

static void
go_output_type (struct goout_container *container, tree type,
		bool in_struct_or_func)
{
  if (TYPE_NAME (type) != NULL_TREE
      && (pointer_set_contains (container->decls_seen, type)
	  || pointer_set_contains (container->decls_seen, TYPE_NAME (type)))
      && (AGGREGATE_TYPE_P (type)
	  || POINTER_TYPE_P (type)
	  || TREE_CODE (type) == FUNCTION_TYPE))
    {
      tree name;

      name = TYPE_NAME (type);
      if (TREE_CODE (name) == IDENTIFIER_NODE)
	{
	  fprintf (asm_out_file, "_%s", IDENTIFIER_POINTER (name));
	  return;
	}
      else if (TREE_CODE (name) == TYPE_DECL)
	{
	  fprintf (asm_out_file, "_%s",
		   IDENTIFIER_POINTER (DECL_NAME (name)));
	  return;
	}
    }

  pointer_set_insert (container->decls_seen, type);

  switch (TREE_CODE (type))
    {
    case ENUMERAL_TYPE:
	    fprintf (asm_out_file, "int");
	    break;

    case TYPE_DECL:
      fprintf (asm_out_file, "_%s",
	       IDENTIFIER_POINTER (DECL_NAME (type)));
      break;

    case INTEGER_TYPE:
      {
	const char *s;
	char buf[100];

	switch (TYPE_PRECISION (type))
	  {
	  case 8:
	    s = TYPE_UNSIGNED (type) ? "uint8" : "int8";
	    break;
	  case 16:
	    s = TYPE_UNSIGNED (type) ? "uint16" : "int16";
	    break;
	  case 32:
	    s = TYPE_UNSIGNED (type) ? "uint32" : "int32";
	    break;
	  case 64:
	    s = TYPE_UNSIGNED (type) ? "uint64" : "int64";
	    break;
	  default:
	    snprintf (buf, sizeof buf, "INVALID-int-%u%s",
		      TYPE_PRECISION (type),
		      TYPE_UNSIGNED (type) ? "u" : "");
	    s = buf;
	    break;
	  }
	fprintf (asm_out_file, "%s", s);
      }
      break;

    case REAL_TYPE:
      {
	const char *s;
	char buf[100];

	switch (TYPE_PRECISION (type))
	  {
	  case 32:
	    s = "float32";
	    break;
	  case 64:
	    s = "float64";
	    break;
	  case 80:
	    s = "float80";
	    break;
	  default:
	    snprintf (buf, sizeof buf, "INVALID-float-%u",
		      TYPE_PRECISION (type));
	    s = buf;
	    break;
	  }
	fprintf (asm_out_file, "%s", s);
      }
      break;

    case BOOLEAN_TYPE:
      fprintf (asm_out_file, "bool");
      break;

    case POINTER_TYPE:
      if (in_struct_or_func
          && TYPE_NAME (TREE_TYPE (type)) != NULL_TREE
          && (RECORD_OR_UNION_TYPE_P (TREE_TYPE (type))
	      || (POINTER_TYPE_P (TREE_TYPE (type))
                  && (TREE_CODE (TREE_TYPE (TREE_TYPE (type)))
		      == FUNCTION_TYPE))))
        {
	  tree name;
	  name = TYPE_NAME (TREE_TYPE (type));
	  if (TREE_CODE (name) == IDENTIFIER_NODE)
	    {
	      fprintf (asm_out_file, "*_%s", IDENTIFIER_POINTER (name));
	      /* If pointing to a struct or union, then the pointer
		 here can be used without the struct or union definition.
		 So this struct or union is a can be a potential dummy
		 type.  */
	      if (RECORD_OR_UNION_TYPE_P (TREE_TYPE (type)))
		pointer_set_insert (container->pot_dummy_types,
				    IDENTIFIER_POINTER (name));
	      return;
	    }
	  else if (TREE_CODE (name) == TYPE_DECL)
	    {
	      fprintf (asm_out_file, "*_%s",
		       IDENTIFIER_POINTER (DECL_NAME (name)));
	      if (RECORD_OR_UNION_TYPE_P (TREE_TYPE (type)))
		pointer_set_insert (container->pot_dummy_types,
				    IDENTIFIER_POINTER (DECL_NAME (name)));
	      return;
	    }
        }
      if (TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
	fprintf (asm_out_file, "func");
      else
	fprintf (asm_out_file, "*");
      if (VOID_TYPE_P (TREE_TYPE (type)))
	fprintf (asm_out_file, "byte");
      else
	go_output_type (container, TREE_TYPE (type), in_struct_or_func);
      break;

    case ARRAY_TYPE:
      fprintf (asm_out_file, "[");
      if (TYPE_DOMAIN (type) != NULL_TREE
	  && TREE_CODE (TYPE_DOMAIN (type)) == INTEGER_TYPE
	  && TYPE_MIN_VALUE (TYPE_DOMAIN (type)) != NULL_TREE
	  && TREE_CODE (TYPE_MIN_VALUE (TYPE_DOMAIN (type))) == INTEGER_CST
	  && tree_int_cst_sgn (TYPE_MIN_VALUE (TYPE_DOMAIN (type))) == 0
	  && TYPE_MAX_VALUE (TYPE_DOMAIN (type)) != NULL_TREE
	  && TREE_CODE (TYPE_MAX_VALUE (TYPE_DOMAIN (type))) == INTEGER_CST
	  && host_integerp (TYPE_MAX_VALUE (TYPE_DOMAIN (type)), 0))
	fprintf (asm_out_file, HOST_WIDE_INT_PRINT_DEC "+1",
		 tree_low_cst (TYPE_MAX_VALUE (TYPE_DOMAIN (type)), 0));
      fprintf (asm_out_file, "]");
      go_output_type (container, TREE_TYPE (type), in_struct_or_func);
      break;

    case UNION_TYPE:
    case RECORD_TYPE:
      {
	tree field;

	fprintf (asm_out_file, "struct { ");
	for (field = TYPE_FIELDS (type);
	     field != NULL_TREE;
	     field = TREE_CHAIN (field))
	  {
	    if (DECL_NAME (field) != NULL)
              {
		const char *var_name;
		void **slot;

		/* Start variable name with an underscore if a keyword.  */
		var_name = IDENTIFIER_POINTER (DECL_NAME (field));
		slot = htab_find_slot (container->keyword_hash, var_name,
				       NO_INSERT);
		if (slot == NULL)
		  fprintf (asm_out_file, "%s ", var_name);
		else
		  fprintf (asm_out_file, "_%s ", var_name);
	      }
	    else
	      fprintf (asm_out_file, "INVALID-unnamed ");
	    if (DECL_BIT_FIELD (field))
	      fprintf (asm_out_file, "INVALID-bit-field");
	    else
              {
		/* Do not expand type if a record or union type or a
		   function pointer.  */
		if (TYPE_NAME (TREE_TYPE (field)) != NULL_TREE
		    && (RECORD_OR_UNION_TYPE_P (TREE_TYPE (field))
			|| (POINTER_TYPE_P (TREE_TYPE (field))
			    && (TREE_CODE (TREE_TYPE (TREE_TYPE (field)))
                                == FUNCTION_TYPE))))
		  {
		    tree name = TYPE_NAME (TREE_TYPE (field));
		    if (TREE_CODE (name) == IDENTIFIER_NODE)
		      fprintf (asm_out_file, "_%s", IDENTIFIER_POINTER (name));
		    else if (TREE_CODE (name) == TYPE_DECL)
		      fprintf (asm_out_file, "_%s",
			       IDENTIFIER_POINTER (DECL_NAME (name)));
		  }
		else
		  go_output_type (container, TREE_TYPE (field), true);
              }
	    fprintf (asm_out_file, "; ");

	    /* Only output the first field of a union, and hope for
	       the best.  */
	    if (TREE_CODE (type) == UNION_TYPE)
	      break;
	  }
	fprintf (asm_out_file, "}");
      }
      break;

    case FUNCTION_TYPE:
      {
	tree args;
	bool is_varargs;
	tree result;

	fprintf (asm_out_file, "(");
	is_varargs = true;
	for (args = TYPE_ARG_TYPES (type);
	     args != NULL_TREE;
	     args = TREE_CHAIN (args))
	  {
	    if (VOID_TYPE_P (TREE_VALUE (args)))
	      {
		gcc_assert (TREE_CHAIN (args) == NULL);
		is_varargs = false;
		break;
	      }
	    if (args != TYPE_ARG_TYPES (type))
	      fprintf (asm_out_file, ", ");
	    go_output_type (container, TREE_VALUE (args), true);
	  }
	if (is_varargs)
	  {
	    if (TYPE_ARG_TYPES (type) != NULL_TREE)
	      fprintf (asm_out_file, ", ");
	    fprintf (asm_out_file, "...");
	  }
	fprintf (asm_out_file, ")");

	result = TREE_TYPE (type);
	if (!VOID_TYPE_P (result))
	  {
	    fprintf (asm_out_file, " ");
	    go_output_type (container, result, in_struct_or_func);
	  }
      }
      break;

    default:
      fprintf (asm_out_file, "INVALID-type");
      break;
    }
}

/* Output a function declaration.  */

static void
go_output_fndecl (struct goout_container *container, tree decl)
{
  fprintf (asm_out_file, "#GO func _%s ",
	   IDENTIFIER_POINTER (DECL_NAME (decl)));
  go_output_type (container, TREE_TYPE (decl), false);
  fprintf (asm_out_file, " __asm__(\"%s\")\n",
	   IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl)));
}

/* Output a typedef or something like a struct definition.  */

static void
go_output_typedef (struct goout_container *container, tree decl)
{
  /* If we have an enum type, output the enum constants
     separately.  */
  if (TREE_CODE (TREE_TYPE (decl)) == ENUMERAL_TYPE
      && TYPE_SIZE (TREE_TYPE (decl)) != 0
      && !pointer_set_contains (container->decls_seen, TREE_TYPE (decl))
      && (TYPE_CANONICAL (TREE_TYPE (decl)) == NULL_TREE
	  || !pointer_set_contains (container->decls_seen,
				    TYPE_CANONICAL (TREE_TYPE (decl)))))
    {
      tree element;

      for (element = TYPE_VALUES (TREE_TYPE (decl));
	   element != NULL_TREE;
	   element = TREE_CHAIN (element))
	fprintf (asm_out_file, "#GO const _%s = " HOST_WIDE_INT_PRINT_DEC "\n",
		 IDENTIFIER_POINTER (TREE_PURPOSE (element)),
		 tree_low_cst (TREE_VALUE (element), 0));
      pointer_set_insert (container->decls_seen, TREE_TYPE (decl));
      if (TYPE_CANONICAL (TREE_TYPE (decl)) != NULL_TREE)
	pointer_set_insert (container->decls_seen, TYPE_CANONICAL (TREE_TYPE (decl)));
    }

  if (DECL_NAME (decl) != NULL_TREE)
    {
      void **slot;
      const char *type;

      type = IDENTIFIER_POINTER (DECL_NAME (decl));
      /* If type defined already, skip.  */
      slot = htab_find_slot (container->type_hash, type, INSERT);
      if (*slot != NULL)
	return;
      *slot = CONST_CAST (void *, (const void *) type);

      fprintf (asm_out_file, "#GO type _%s ",
	       IDENTIFIER_POINTER (DECL_NAME (decl)));
      go_output_type (container, TREE_TYPE (decl), false);
      pointer_set_insert (container->decls_seen, decl);
    }
  else if (RECORD_OR_UNION_TYPE_P (TREE_TYPE (decl)))
    {
       void **slot;
       const char *type;

       type = IDENTIFIER_POINTER (TYPE_NAME (TREE_TYPE ((decl))));
       /* If type defined already, skip.  */
       slot = htab_find_slot (container->type_hash, type, INSERT);
       if (*slot != NULL)
         return;
       *slot = CONST_CAST (void *, (const void *) type);

       fprintf (asm_out_file, "#GO type _%s ",
	       IDENTIFIER_POINTER (TYPE_NAME (TREE_TYPE (decl))));
       go_output_type (container, TREE_TYPE (decl), false);
    }
  else
    return;

  fprintf (asm_out_file, "\n");
}

/* Output a variable.  */

static void
go_output_var (struct goout_container *container, tree decl)
{
  if (pointer_set_contains (container->decls_seen, decl)
      || pointer_set_contains (container->decls_seen, DECL_NAME (decl)))
    return;
  pointer_set_insert (container->decls_seen, decl);
  pointer_set_insert (container->decls_seen, DECL_NAME (decl));
  fprintf (asm_out_file, "#GO var _%s ",
	   IDENTIFIER_POINTER (DECL_NAME (decl)));
  go_output_type (container, TREE_TYPE (decl), false);
  fprintf (asm_out_file, "\n");
}

/* For the type and keywords hash tables.  */

static int
string_hash_eq (const void *y1, const void *y2)
{
  return strcmp ((const char *) y1, (const char *) y2) == 0;
}

/* Build a hash table with the Go keywords.  */

static const char * const keywords[] = {
  "__asm__", "break", "case", "chan", "const", "continue", "default",
  "defer", "else", "fallthrough", "for", "func", "go", "goto", "if",
  "import", "interface", "map", "package", "range", "return", "select",
  "struct", "switch", "type", "var"
};

static void
keyword_hash_init (struct goout_container *container)
{
  size_t i;
  size_t count = sizeof (keywords) / sizeof (keywords[0]);
  void **slot;

  for (i = 0; i < count; i++)
    {
      slot = htab_find_slot (container->keyword_hash, keywords[i], INSERT);
      *slot = CONST_CAST (void *, (const void *) keywords[i]);
    }
}

/* Traversing the pot_dummy_types and seeing which types are present in the
   global types hash table and creating dummy definitions if not found.
   This function is invoked by pointer_set_traverse.  */

static bool
find_dummy_types (const void *ptr, void *adata)
{
  struct goout_container *data = (struct goout_container *) adata;
  void **slot;
  const char *type = (const char *) ptr;

  slot = htab_find_slot (data->type_hash, type, NO_INSERT);
  if (slot == NULL)
    fprintf (asm_out_file, "#GO type _%s struct {};\n", type);
  return true;
}

/* Output symbols.  */

static void
go_finish (const char *filename ATTRIBUTE_UNUSED)
{
  struct goout_container container;

  tree q;

  container.decls_seen = pointer_set_create ();
  container.pot_dummy_types = pointer_set_create ();
  container.type_hash = htab_create (100, htab_hash_string,
                                     string_hash_eq, NULL);
  container.keyword_hash = htab_create (50, htab_hash_string,
                                        string_hash_eq, NULL);

  keyword_hash_init (&container);

  q = nreverse (queue);
  queue = NULL_TREE;
  for (; q != NULL_TREE; q = TREE_CHAIN (q))
    {
      tree decl;

      decl = TREE_VALUE (q);
      switch (TREE_CODE (decl))
	{
	case FUNCTION_DECL:
	  go_output_fndecl (&container, decl);
	  break;

	case TYPE_DECL:
	  go_output_typedef (&container, decl);
	  break;

	case VAR_DECL:
	  go_output_var (&container, decl);
	  break;

	default:
	  gcc_unreachable();
	}
    }
  /* To emit dummy definitions.  */
  pointer_set_traverse (container.pot_dummy_types, find_dummy_types,
                        (void *) &container);

  pointer_set_destroy (container.decls_seen);
  pointer_set_destroy (container.pot_dummy_types);
  htab_delete (container.type_hash);
  htab_delete (container.keyword_hash);
}

/* The debug hooks structure.  */

const struct gcc_debug_hooks go_debug_hooks =
{
  go_init,				/* init */
  go_finish,				/* finish */
  debug_nothing_void,			/* assembly_start */
  go_define,				/* define */
  go_undef,				/* undef */
  debug_nothing_int_charstar,		/* start_source_file */
  debug_nothing_int,			/* end_source_file */
  debug_nothing_int_int,		/* begin_block */
  debug_nothing_int_int,		/* end_block */
  debug_true_const_tree,		/* ignore_block */
  debug_nothing_int_charstar_int_bool,	/* source_line */
  debug_nothing_int_charstar,		/* begin_prologue */
  debug_nothing_int_charstar,		/* end_prologue */
  debug_nothing_int_charstar,		/* end_epilogue */
  debug_nothing_tree,			/* begin_function */
  debug_nothing_int,			/* end_function */
  go_decl,				/* function_decl */
  go_decl,				/* global_decl */
  go_type_decl,				/* type_decl */
  debug_nothing_tree_tree_tree_bool,	/* imported_module_or_decl */
  debug_nothing_tree,			/* deferred_inline_function */
  debug_nothing_tree,			/* outlining_inline_function */
  debug_nothing_rtx,			/* label */
  debug_nothing_int,			/* handle_pch */
  debug_nothing_rtx,			/* var_location */
  debug_nothing_void,			/* switch_text_section */
  debug_nothing_tree,	        	/* direct_call */
  debug_nothing_tree_int,		/* virtual_call_token */
  debug_nothing_rtx_rtx,		/* copy_call_info */
  debug_nothing_uid,			/* virtual_call */
  debug_nothing_tree_tree,		/* set_name */
  0                             	/* start_end_main_source_file */
};

#endif /* defined(GO_DEBUG_INFO) */

#include "gt-goout.h"
