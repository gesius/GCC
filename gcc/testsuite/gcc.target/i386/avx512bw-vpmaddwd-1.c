/* { dg-do compile } */
/* { dg-options "-mavx512bw -mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%zmm\[0-9\]" 3 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%zmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%zmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */
/* { dg-final { scan-assembler-times "vpmaddwd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */

#include <immintrin.h>

volatile __m512i x;
volatile __m256i xq;
volatile __m128i xw;

void extern
avx512bw_test (void)
{
  x = _mm512_madd_epi16 (x, x);
  x = _mm512_mask_madd_epi16 (x, 2, x, x);
  x = _mm512_maskz_madd_epi16 (2, x, x);
  xq = _mm256_mask_madd_epi16 (xq, 2, xq, xq);
  xq = _mm256_maskz_madd_epi16 (2, xq, xq);
  xw = _mm_mask_madd_epi16 (xw, 2, xw, xw);
  xw = _mm_maskz_madd_epi16 (2, xw, xw);
}
