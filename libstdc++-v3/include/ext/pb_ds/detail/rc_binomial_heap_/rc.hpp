// -*- C++ -*-

// Copyright (C) 2005, 2006, 2009, 2011 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/**
 * @file rc.hpp
 * Contains a redundant (binary counter).
 */

#ifndef PB_DS_RC_HPP
#define PB_DS_RC_HPP

#define PB_DS_ASSERT_VALID(X)						\
  _GLIBCXX_DEBUG_ONLY(X.assert_valid(__FILE__, __LINE__);)

#define PB_DS_DEBUG_VERIFY(_Cond)					\
  _GLIBCXX_DEBUG_VERIFY_AT(_Cond,					\
			   _M_message(#_Cond" assertion from %1;:%2;")	\
			   ._M_string(__FILE__)._M_integer(__LINE__)	\
			   ,__file,__line)

namespace __gnu_pbds
{
  namespace detail
  {

#define PB_DS_CLASS_T_DEC \
    template<typename Node, class Allocator>

#define PB_DS_CLASS_C_DEC \
    rc<Node, Allocator>

    template<typename Node, class Allocator>
    class rc
    {
    private:
      typedef Allocator allocator_type;

      typedef typename allocator_type::size_type size_type;

      typedef Node node;

      typedef
      typename allocator_type::template rebind<
	node>::other::pointer
      node_pointer;

      typedef
      typename allocator_type::template rebind<
	node_pointer>::other::pointer
      entry_pointer;

      typedef
      typename allocator_type::template rebind<
	node_pointer>::other::const_pointer
      const_entry_pointer;

      enum
	{
	  max_entries = sizeof(size_type) << 3
	};

    public:
      typedef node_pointer entry;

      typedef const_entry_pointer const_iterator;

    public:
      rc();

      rc(const PB_DS_CLASS_C_DEC& other);

      inline void
      swap(PB_DS_CLASS_C_DEC& other);

      inline void
      push(entry p_nd);

      inline node_pointer
      top() const;

      inline void
      pop();

      inline bool
      empty() const;

      inline size_type
      size() const;

      void
      clear();

      const const_iterator
      begin() const;

      const const_iterator
      end() const;

#ifdef _GLIBCXX_DEBUG
      void
      assert_valid(const char* file, int line) const;
#endif 

#ifdef PB_DS_RC_BINOMIAL_HEAP_TRACE_
      void
      trace() const;
#endif 

    private:
      node_pointer m_a_entries[max_entries];

      size_type m_over_top;
    };

    PB_DS_CLASS_T_DEC
    PB_DS_CLASS_C_DEC::
    rc() : m_over_top(0)
    { PB_DS_ASSERT_VALID((*this)) }

    PB_DS_CLASS_T_DEC
    PB_DS_CLASS_C_DEC::
    rc(const PB_DS_CLASS_C_DEC& other) : m_over_top(0)
    { PB_DS_ASSERT_VALID((*this)) }

    PB_DS_CLASS_T_DEC
    inline void
    PB_DS_CLASS_C_DEC::
    swap(PB_DS_CLASS_C_DEC& other)
    {
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)

      const size_type over_top = std::max(m_over_top, other.m_over_top);

      for (size_type i = 0; i < over_top; ++i)
	std::swap(m_a_entries[i], other.m_a_entries[i]);

      std::swap(m_over_top, other.m_over_top);
      PB_DS_ASSERT_VALID((*this))
      PB_DS_ASSERT_VALID(other)
     }

    PB_DS_CLASS_T_DEC
    inline void
    PB_DS_CLASS_C_DEC::
    push(entry p_nd)
    {
      PB_DS_ASSERT_VALID((*this))
      _GLIBCXX_DEBUG_ASSERT(m_over_top < max_entries);
      m_a_entries[m_over_top++] = p_nd;
      PB_DS_ASSERT_VALID((*this))
    }

    PB_DS_CLASS_T_DEC
    inline void
    PB_DS_CLASS_C_DEC::
    pop()
    {
      PB_DS_ASSERT_VALID((*this))
      _GLIBCXX_DEBUG_ASSERT(!empty());
      --m_over_top;
      PB_DS_ASSERT_VALID((*this))
    }

    PB_DS_CLASS_T_DEC
    inline typename PB_DS_CLASS_C_DEC::node_pointer
    PB_DS_CLASS_C_DEC::
    top() const
    {
      PB_DS_ASSERT_VALID((*this))
      _GLIBCXX_DEBUG_ASSERT(!empty());
      return *(m_a_entries + m_over_top - 1);
    }

    PB_DS_CLASS_T_DEC
    inline bool
    PB_DS_CLASS_C_DEC::
    empty() const
    {
      PB_DS_ASSERT_VALID((*this))
      return m_over_top == 0;
    }

    PB_DS_CLASS_T_DEC
    inline typename PB_DS_CLASS_C_DEC::size_type
    PB_DS_CLASS_C_DEC::
    size() const
    { return m_over_top; }

    PB_DS_CLASS_T_DEC
    void
    PB_DS_CLASS_C_DEC::
    clear()
    {
      PB_DS_ASSERT_VALID((*this))
      m_over_top = 0;
      PB_DS_ASSERT_VALID((*this))
    }

    PB_DS_CLASS_T_DEC
    const typename PB_DS_CLASS_C_DEC::const_iterator
    PB_DS_CLASS_C_DEC::
    begin() const
    { return& m_a_entries[0]; }

    PB_DS_CLASS_T_DEC
    const typename PB_DS_CLASS_C_DEC::const_iterator
    PB_DS_CLASS_C_DEC::
    end() const
    { return& m_a_entries[m_over_top]; }

#ifdef _GLIBCXX_DEBUG
    PB_DS_CLASS_T_DEC
    void
    PB_DS_CLASS_C_DEC::
    assert_valid(const char* __file, int __line) const
    { PB_DS_DEBUG_VERIFY(m_over_top < max_entries); }
#endif 

#ifdef PB_DS_RC_BINOMIAL_HEAP_TRACE_
    PB_DS_CLASS_T_DEC
    void
    PB_DS_CLASS_C_DEC::
    trace() const
    {
      std::cout << "rc" << std::endl;
      for (size_type i = 0; i < m_over_top; ++i)
	std::cerr << m_a_entries[i] << std::endl;
      std::cout << std::endl;
    }
#endif 

#undef PB_DS_CLASS_T_DEC
#undef PB_DS_CLASS_C_DEC

} // namespace detail
} // namespace __gnu_pbds

#undef PB_DS_DEBUG_VERIFY
#undef PB_DS_ASSERT_VALID
#endif 
