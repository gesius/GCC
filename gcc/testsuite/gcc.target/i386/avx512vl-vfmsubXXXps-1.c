/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vfmsub231ps\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfmsub231ps\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m256 yy, y2, y3;
volatile __m128 xx, x2, x3;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  yy = _mm256_mask_fmsub_ps (yy, m, y2, y3);
  xx = _mm_mask_fmsub_ps (xx, m, x2, x3);

  y3 = _mm256_mask3_fmsub_ps (yy, y2, y3, m);
  x3 = _mm_mask3_fmsub_ps (xx, x2, x3, m);

  yy = _mm256_maskz_fmsub_ps (m, yy, y2, y3);
  xx = _mm_maskz_fmsub_ps (m, xx, x2, x3);
}
