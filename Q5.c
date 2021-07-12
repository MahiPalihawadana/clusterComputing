#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
void print_array(int* arr, int no_of_elements);
void merge(int* arr, int start, int mid, int end);
void merge_sort(int* arr, int start, int end);
int main(int argc, char** argv) {
// Input the number of elements
int no_of_elements = 10; //default is 10
//Pass the number of elements as the first argument
if (argv[1])
no_of_elements = atoi(argv[1]);
int* arr_unsorted = (int*)malloc(sizeof(int) * no_of_elements);
int* arr_sorted = NULL;
//Generate the random numbers for array
srand(time(NULL));
for (int i = 0; i < no_of_elements; i++)
{
arr_unsorted[i] = rand() % 100;
}
int numprocs, rank;
int root = 0;
double local_time, max_time;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
int size = no_of_elements / numprocs;
int* arr_sub = (int*)malloc(sizeof(int) * size);
MPI_Scatter(arr_unsorted, size, MPI_INT, arr_sub, size, MPI_INT, root,
MPI_COMM_WORLD);
MPI_Barrier(MPI_COMM_WORLD);
local_time = MPI_Wtime();
merge_sort(arr_sub, 0, (size - 1));
local_time = MPI_Wtime() - local_time;
if (rank == root)
arr_sorted = (int*)malloc(sizeof(int) * no_of_elements);
MPI_Gather(arr_sub, size, MPI_INT, arr_sorted, size, MPI_INT, root,
MPI_COMM_WORLD);
MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, root,
MPI_COMM_WORLD);if (rank == root) {
printf("\nNumber of Elements in the Array : %d\n", no_of_elements);
printf("Unsorted Array \t");
print_array(arr_unsorted, no_of_elements);
//Measure Execution time
clock_t begin = clock();
merge_sort(arr_unsorted, 0, no_of_elements - 1);
clock_t end = clock();
printf("Sorted Array \t");
print_array(arr_unsorted, no_of_elements);
double duration = (double)(end - begin) / (double)CLOCKS_PER_SEC;
printf("\nDuration for Execution: %f s", duration);
printf("\n");
}
MPI_Finalize();
}
// Print arrays
void print_array(int* arr, int no_of_elements)
{
if (no_of_elements > 20)
{
no_of_elements = 10;
printf(" of first 10 Elements");
}
printf("\n");
for (int i = 0; i < no_of_elements; i++) {
printf(" %d,", arr[i]);
}
printf("\n");
}
// Merge sort
void merge_sort(int* arr, int start, int end)
{
if (start < end){
int mid = start + (end - start) / 2;
merge_sort(arr, start, mid);
merge_sort(arr, (mid + 1), end);
merge(arr, start, mid, end);
}
}
// Merge
void merge(int* arr, int start, int mid, int end)
{
int i, j, k;
int half_1 = mid - start + 1;
int half_2 = end - mid;
int* Left = (int*)malloc(sizeof(int) * half_1);
int* Right = (int*)malloc(sizeof(int) * half_2);
for (i = 0; i < half_1; i++)Left[i] = arr[start + i];
for (j = 0; j < half_2; j++)
Right[j] = arr[mid + 1 + j];
i = 0; j = 0; k = start;
// Sort subarray
while (i < half_1 && j < half_2)
{
if (Left[i] <= Right[j]) {
arr[k] = Left[i];
i++;
}
else
{
arr[k] = Right[j];
j++;
}
k++;
}
while (i < half_1) {
arr[k] = Left[i];
i++;
k++;
}
while (j < half_2) {
arr[k] = Right[j];
j++;
k++;
}
}