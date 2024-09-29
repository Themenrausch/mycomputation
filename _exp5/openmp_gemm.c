#include "defs.h"
#include <omp.h>
#include <stdio.h>
/* Routine for computing C = A * B + C */
void OMP_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  
#pragma omp parallel for
  for (int i = 0; i < m; i++) /* Loop over the rows of C */
  {
    for (int j = 0; j < n; j++) /* Loop over the columns of C */
    {
      for (int p = 0; p < k; p++)
      { /* Update C( i,j ) with the inner product of the ith row of A and the jth column of B */
        C(i, j) = C(i, j) + A(i, p) * B(p, j);
      }
    }
  }
}
