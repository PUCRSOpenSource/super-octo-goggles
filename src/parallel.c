#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 1000
#define COLUMNS 100000
#define TAG 1

int vet[ROWS][COLUMNS];

int compare(const void* a, const void* b)
{
	return *((const int*) a)  - *((const int*) b);
}

int master()
{
	int ntasks;
	/*int rank;*/
	/*int work;*/

	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

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

	MPI_Send(vet[0], COLUMNS, MPI_INT, 1, TAG, MPI_COMM_WORLD);

	int* result = malloc(COLUMNS * sizeof(int));

	MPI_Recv(result, COLUMNS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	for (i = 0; i < COLUMNS; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");

	return 0;
}

int slave()
{
	int* work = malloc(COLUMNS * sizeof(int));
	MPI_Status status;

	MPI_Recv(work, COLUMNS, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	qsort(work, COLUMNS, sizeof(int), compare);

	MPI_Send(&work, COLUMNS, MPI_INT, 0, 0, MPI_COMM_WORLD);

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

	return 0;
}
