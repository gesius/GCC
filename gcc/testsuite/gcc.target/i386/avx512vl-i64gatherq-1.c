/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vpgatherqq\[ \\t\]+\[^\n\]*ymm\[0-9\]\[^\n\]*ymm\[0-9\]{%k\[1-7\]}" 2} } */
/* { dg-final { scan-assembler-times "vpgatherqq\[ \\t\]+\[^\n\]*xmm\[0-9\]\[^\n\]*xmm\[0-9\]{%k\[1-7\]}" 2 } } */

#include <immintrin.h>

volatile __m256i x1, idx1;
volatile __m128i x2, idx2;
volatile __mmask8 m8;
long long *base;

void extern
avx512vl_test (void)
{
  x1 = _mm256_mmask_i64gather_epi64 (x1, 0xFF, idx1, base, 8);
  x1 = _mm256_mmask_i64gather_epi64 (x1, m8, idx1, base, 8);
  x2 = _mm_mmask_i64gather_epi64 (x2, 0xFF, idx2, base, 8);
  x2 = _mm_mmask_i64gather_epi64 (x2, m8, idx2, base, 8);
}
