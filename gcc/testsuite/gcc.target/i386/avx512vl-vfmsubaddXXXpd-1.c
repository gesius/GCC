/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vfmsubadd...pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vfmsubadd...pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vfmsubadd231pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfmsubadd231pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfmsubadd...pd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vfmsubadd...pd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m256d yy, y2, y3;
volatile __m128d xx, x2, x3;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  yy = _mm256_mask_fmsubadd_pd (yy, m, y2, y3);
  xx = _mm_mask_fmsubadd_pd (xx, m, x2, x3);

  y3 = _mm256_mask3_fmsubadd_pd (yy, y2, y3, m);
  x3 = _mm_mask3_fmsubadd_pd (xx, x2, x3, m);

  yy = _mm256_maskz_fmsubadd_pd (m, yy, y2, y3);
  xx = _mm_maskz_fmsubadd_pd (m, xx, x2, x3);
}
