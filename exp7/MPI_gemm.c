/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#define C(i,j) c[(i) * ldc + (j)]
#define A(i,j) a[(i) * lda + (j)]
#define B(i,j) b[(i) * ldb + (j)]
#define M 8
#define N 8
#define K 8
#define LDA 8
#define LDB 8
#define LDC 8

void MPI_dotx(double *a, double *b, double *c, int m, int n, int k, int lda, int ldb, int ldc, int i, int j)
{
    for(int l=0;l<k;l++)
    {
        C(i,j) += A(i,l) * B(l,j);
    }
}

void PrintM(int m, int n, double *a)
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            printf("%f ",a[i*n+j]);
        }
        printf("\n");
    }
}
// double f(double);

// double f(double a)
// {
//     return (4.0 / (1.0 + a * a));
// }

int main(int argc, char *argv[])
{
    int myid, numprocs;
    double c[M*N]={0}, mc[M*N]={0};
    double a[M*K]={
1.900078e-02, 8.548636e-02, -3.314322e-01, 2.440604e-02, -6.461445e-01, -7.460251e-01, 5.706861e-01, -6.391164e-02, 
-3.681220e-01, 3.359572e-01, -8.358069e-01, 1.730207e-01, -7.762620e-01, -9.918196e-01, -1.155304e-01, -6.060053e-01, 
4.034051e-01, -6.577163e-01, -4.593707e-01, 4.775171e-01, 7.053617e-01, -9.989944e-01, -4.241922e-01, -1.056461e-01, 
-2.655067e-01, 6.615583e-01, -8.514753e-01, 7.433776e-01, 6.223437e-01, -6.140962e-01, 7.067059e-01, 4.568796e-01, 
7.526247e-01, 7.238108e-01, 3.524515e-01, 1.559346e-01, 9.097746e-01, 1.735603e-01, 5.194394e-02, 9.321860e-01, 
-2.580107e-01, 5.384423e-01, 6.203171e-01, 9.294072e-01, 8.085664e-01, -2.500817e-01, 4.568334e-01, 5.136816e-01, 
5.050480e-02, -8.948411e-01, -7.157754e-02, -3.329414e-01, -7.221695e-01, -9.464281e-01, -4.673714e-01, -6.788623e-02, 
-1.443309e-01, 9.334593e-01, 1.383701e-01, -2.143441e-01, 3.723747e-01, -7.046941e-02, 5.669680e-01, 7.379329e-01
};
    double b[K*N]={
-7.319215e-01, 8.165651e-01, -4.029224e-02, 8.852518e-01, 5.361704e-01, -2.435351e-01, -5.578819e-01, -3.161686e-01, 
6.921052e-01, 5.488294e-01, -9.377434e-01, -2.239507e-01, 5.038825e-01, -1.625671e-01, 5.086909e-02, -8.637625e-02, 
-9.638247e-01, -2.678706e-01, 2.168604e-01, 8.610245e-01, -8.182718e-01, -7.231037e-01, -1.094956e-02, 1.762435e-01, 
8.817096e-01, 8.885760e-01, 5.845223e-01, 8.227684e-01, 7.079334e-01, -2.383066e-01, 7.650440e-01, 1.702999e-01, 
3.401642e-01, 7.696433e-01, -6.350064e-01, -3.245633e-01, -6.507111e-01, -4.004399e-01, 5.476602e-01, -8.601459e-01, 
5.999431e-01, -1.096578e-01, 5.024259e-02, 6.562465e-02, -9.604593e-01, -9.579758e-01, -8.819950e-01, 6.378629e-01, 
-7.135664e-01, -6.739431e-01, 7.975177e-01, 4.041545e-01, -1.618930e-01, -5.223948e-01, -7.603972e-01, -1.936786e-01, 
2.209206e-01, -1.503654e-01, -3.824202e-01, 9.420213e-01, -4.072602e-01, -9.257813e-01, 9.560251e-02, 1.709661e-01
};
    // double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    fprintf(stdout, "Process %d of %d is on %s\n", myid, numprocs, processor_name);
    fflush(stdout);

    if (myid == 0)
    {
        startwtime = MPI_Wtime();
        // for(int i=0;i<M*K;i++)
        // {
        //     a[i] = i%4+1;
        // }
        // for(int i=0;i<K*N;i++)
        // {
        //     b[i] = 4-i%4;
        // }
    }

    MPI_Bcast(a, M*K, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, K*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Bcast(c, M*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(myid!=0)
    {
        // printf("id:%d\n",myid);
        // printf("A:\n");
        // PrintM(M,K,a);
        // printf("B:\n");
        // PrintM(K,N,b);
        for(int i=myid-1;i<M*N;i+=(numprocs-1))
        {
            MPI_dotx(a, b, mc, M, N, K, LDA, LDB, LDC, i/N, i%N);
        }
        printf("id:%d\n",myid);
        PrintM(M,N,mc);
    }

    // for (i = myid + 1; i <= n; i += numprocs) {
    //     x = h * ((double) i - 0.5);
    //     sum += f(x);
    // }
    // mypi = h * sum;

    MPI_Reduce(mc, c, M*N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        endwtime = MPI_Wtime();
        // printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        printf("C=[\n");
        PrintM(M, N, c);
        printf("]\n");
        printf("wall clock time = %f\n", endwtime - startwtime);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}