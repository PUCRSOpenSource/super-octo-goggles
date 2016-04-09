#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 1000
#define COLUMNS 100000

int vet[ROWS][COLUMNS];

int master()
{
	return 0;
}

int slave()
{
	return 0;
}

int compare(const void* a, const void* b)
{
	const int* ia = (const int*)a;
	const int* ib = (const int*)b;
	return *ia  - *ib;
}

int main(int argc, char** argv)
{
	int my_rank;
	int proc_n;

	MPI_Init(&argc , &argv);

	double t1,t2;
	t1 = MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	if ( my_rank == 0 )
		master();
	else
		slave();

	t2 = MPI_Wtime();
	printf("\nTempo de execucao: %f\n\n", t2-t1);

	MPI_Finalize();
}
