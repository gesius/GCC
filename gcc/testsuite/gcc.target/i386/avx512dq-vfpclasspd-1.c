/* { dg-do compile } */
/* { dg-options "-mavx512dq -mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%zmm\[0-7\]\[^\n^k\]*%k\[1-7\]\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%ymm\[0-7\]\[^\n^k\]*%k\[1-7\]\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%xmm\[0-7\]\[^\n^k\]*%k\[1-7\]\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%zmm\[0-7\]\[^\n^k\]*%k\[1-7\]\{" 1 } } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%ymm\[0-7\]\[^\n^k\]*%k\[1-7\]\{" 1 } } */
/* { dg-final { scan-assembler-times "vfpclasspd\[ \\t\]+\[^\n\]*%xmm\[0-7\]\[^\n^k\]*%k\[1-7\]\{" 1 } } */

#include <immintrin.h>

volatile __m512d x512;
volatile __m256d x256;
volatile __m128d x128;
volatile __mmask8 m;

void extern
avx512dq_test (void)
{
  m = _mm512_fpclass_pd_mask (x512, 13);
  m = _mm256_fpclass_pd_mask (x256, 13);
  m = _mm_fpclass_pd_mask (x128, 13);
  m = _mm512_mask_fpclass_pd_mask (2, x512, 13);
  m = _mm256_mask_fpclass_pd_mask (2, x256, 13);
  m = _mm_mask_fpclass_pd_mask (2, x128, 13);
}
