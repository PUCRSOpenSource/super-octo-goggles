#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 1000
#define COLUMNS 100000

int vet[ROWS][COLUMNS];

int compare(const void* a, const void* b)
{
	const int* ia = (const int*)a;
	const int* ib = (const int*)b;
	return *ia  - *ib;
}

int master()
{
	/*int ntasks, rank, work;*/

	/*MPI_Status status;*/
	/*MPI_Comm_size(MPI_COMM_WORLD, &ntasks);*/

	int i, j, k = COLUMNS;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			vet[i][j] = k;
			k--;
		}
		k = COLUMNS;
	}
	printf("I am the master!\n");

	return 0;
}

int slave()
{
	printf("Please me master!\n");
	return 0;
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
