/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\[\\n\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}{z}" 1 } } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%ymm\[0-9\]\[^\n\]*%xmm\[0-9\]\[\\n\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%ymm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmovsqb\[ \\t\]+\[^\n\]*%ymm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}{z}" 1 } } */

#include <immintrin.h>

volatile __m128i x, z;
volatile __m256i y;
volatile __mmask8 m;

void extern
avx512vl_test (void)
{
  z = _mm_cvtsepi64_epi8 (x);
  z = _mm_mask_cvtsepi64_epi8 (z, m, x);
  z = _mm_maskz_cvtsepi64_epi8 (m, x);
  z = _mm256_cvtsepi64_epi8 (y);
  z = _mm256_mask_cvtsepi64_epi8 (z, m, y);
  z = _mm256_maskz_cvtsepi64_epi8 (m, y);
}
