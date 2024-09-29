# 1

cpu型号：13th Gen Intel(R) Core(TM) i9-13900HX

核数：24

频率：最大2200MHz，最小800MHz

avx指令集版本：avx2, avx_vnni

内存大小：16G

# 2

$FLOPS=4\times 2\times 2\times 1500M\times 24=576000M=576gflops$

# 3

操作系统版本：Ubuntu 22.04.5 LTS

MPI版本：gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

GNU MPFR版本：libmpfr6:amd64 4.1.0-3build3

GNU MPC版本：libmpc3:amd64 1.2.1-2build1

# 4

首先利用参数配置网站得到初步的参数，尝试后发现矩阵规模太大，无法运行，于是从4096开始逐步增大矩阵规模。当矩阵规模达到36864时，接近所能处理的矩阵规模的极限，gflops值也达到最大的1.9355e+01，比利用cblas计算矩阵乘法时的最大值还要大。最终得到与CPU峰值性能比为0.0336.

# 5

碰到的问题及解决方案：在改参数尝试运行xhpl时一直出现线程数不对的报错，然而我在配置中输入的数据和我运行时的参数是匹配的。后来发现我指定的mpi库地址是mpich的地址，而在先前使用mpi的过程中发现这个库是不能用的，将地址换到先前试过能用的地址上后xhpl就可以正常运行了。