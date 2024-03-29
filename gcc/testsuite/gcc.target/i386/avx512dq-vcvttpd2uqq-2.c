/* { dg-do run } */
/* { dg-options "-O2 -mavx512dq -DAVX512DQ" } */
/* { dg-require-effective-target avx512dq } */

#include "avx512f-helper.h"

#define SIZE (AVX512F_LEN / 64)
#include "avx512f-mask-type.h"

void
CALC (double *s, unsigned long long *r)
{
  int i;

  for (i = 0; i < SIZE; i++)
    {
      r[i] = (unsigned long long) s[i];
    }
}

void
TEST (void)
{
  UNION_TYPE (AVX512F_LEN, d) s;
  UNION_TYPE (AVX512F_LEN, i_q) res1, res2, res3;
  MASK_TYPE mask = MASK_VALUE;
  unsigned long long res_ref[SIZE];
  int i;

  for (i = 0; i < SIZE; i++)
    {
      s.a[i] = 123.456 * (i + 2000);
      res2.a[i] = DEFAULT_VALUE;
    }

  res1.x = INTRINSIC (_cvttpd_epu64) (s.x);
  res2.x = INTRINSIC (_mask_cvttpd_epu64) (res2.x, mask, s.x);
  res3.x = INTRINSIC (_maskz_cvttpd_epu64) (mask, s.x);

  CALC (s.a, res_ref);

  if (UNION_CHECK (AVX512F_LEN, i_q) (res1, res_ref))
    abort ();

  MASK_MERGE (i_q) (res_ref, mask, SIZE);
  if (UNION_CHECK (AVX512F_LEN, i_q) (res2, res_ref))
    abort ();

  MASK_ZERO (i_q) (res_ref, mask, SIZE);
  if (UNION_CHECK (AVX512F_LEN, i_q) (res3, res_ref))
    abort ();
}
