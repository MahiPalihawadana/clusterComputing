#include <mpi.h>
#include <stdio.h>
int main(int argc, char** argv) {
int size, rank;
int local_proc_data;
int sum;
const int root = 0;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//generating a local data
local_proc_data = (rank * 5) + 1;
printf("Process %d : Local Data is : %d\n", rank, local_proc_data);
MPI_Reduce(&local_proc_data, &sum, 1, MPI_INT, MPI_SUM, root,MPI_COMM_WORLD);
if (rank == root)
{
printf("Process %d: Sum of data is : %d", rank, sum);
printf("\n");
}
MPI_Finalize();
return 0;
}