/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vpermq\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vpermq\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */

#include <immintrin.h>

volatile __m256i x;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  x = _mm256_maskz_permutexvar_epi64 (m, x, x);
  x = _mm256_mask_permutexvar_epi64 (x, m, x, x);
}
