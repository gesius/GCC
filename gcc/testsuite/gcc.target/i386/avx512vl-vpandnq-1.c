/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%ymm\[0-9\]\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpandnq\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m256i y;
volatile __m128i x;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  y = _mm256_mask_andnot_epi64 (y, m, y, y);
  y = _mm256_maskz_andnot_epi64 (m, y, y);

  x = _mm_mask_andnot_epi64 (x, m, x, x);
  x = _mm_maskz_andnot_epi64 (m, x, x);
}
