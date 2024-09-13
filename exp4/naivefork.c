#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#define M 2048
#define N 2048
#define K 2048
void random_matrix(int m, int n, double *a, int lda)
{
  /* drand48() generate pseudo-random numbers using the linear congruential algorithm and
     48-bit integer arithmetic. return nonnegative double-precision floating-point values
     uniformly distributed over the interval [0.0, 1.0). */
  double drand48();
  int i, j;

  for (j = 0; j < n; j++)
    for (i = 0; i < m; i++)
      // A(i, j) = drand48();
      A(i, j) = 2.0 * drand48() - 1.0;
}

void naive(int m, int n, int k, double *a, int lda,
               double *b, int ldb,
               double *c, int ldc)
{
  int i, j, p;

  for (i = 0; i < m; i++)
  {
    for (j = 0; j < n; j++)
    {
      for (p = 0; p < k; p++)
      {
        C(i, j) = C(i, j) + A(i, p) * B(p, j);
      }
    }
  }
}

void copy(int m, int n, double *a, double *b, int lda, int ldb)
{
  int i, j;

  for (i = 0; i < m; i++)
  {
    for (j = 0; j < n; j++)
    {
      // A(i,j) = B(i,j);
      A(i,j) = 0;
    }
  }
}


int main()
{
  unsigned int id;
  double *a, *b, *c, *cc;
  a = (double *)malloc(N * K * sizeof(double));
  b = (double *)malloc(K * N * sizeof(double));
  c = (double *)malloc(K * N * sizeof(double));
  cc = (double *)malloc(K * N * sizeof(double));
  // double a[M * K], b[K * N], c[M * N], cc[M * N];

  random_matrix(M, N, a, N);
  random_matrix(N, K, b, K);
  random_matrix(M, K, c, K);
  random_matrix(M, K, cc, K);
  copy(M, N, cc, c, M, N);
  id = fork();
  if(id < 0)
  {
    printf("error");
    exit(1);
  }
  else if(id == 0)
  {
    printf("this is child:%d\n", (int)getpid());
    naive(M, N, K, a, K, b, K, cc, N);
  }
  else
  {
    printf("this is father of %d:%d\n", id, (int)getpid());
    naive(M, N, K, a, K, b, K, c, N);
  }
}
