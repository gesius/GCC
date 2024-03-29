/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\[^\{\]" 3 } } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\{\]" 3 } } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vrsqrt14pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m256d x1;
volatile __m128d x2;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  x1 = _mm256_rsqrt14_pd (x1);
  x1 = _mm256_mask_rsqrt14_pd (x1, m, x1);
  x1 = _mm256_maskz_rsqrt14_pd (m, x1);

  x2 = _mm_rsqrt14_pd (x2);
  x2 = _mm_mask_rsqrt14_pd (x2, m, x2);
  x2 = _mm_maskz_rsqrt14_pd (m, x2);
}
