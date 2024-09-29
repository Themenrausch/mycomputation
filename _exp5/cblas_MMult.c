#include <cblas.h>
#include <stdio.h>
#include "defs.h"

void cblasversion(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc){
  double alpha = 1, beta = 1;
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
}