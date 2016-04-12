#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 1000
#define COLUMNS 100000
#define WORKTAG 1
#define DIETAG 2

int vet[ROWS][COLUMNS];

int compare(const void* a, const void* b)
{
	return *((const int*) a)  - *((const int*) b);
}

int save_sorted(int save_path, int* sorted_array)
{
	int i;
	for (i = 0; i < COLUMNS; i++)
		vet[save_path][i] = sorted_array[i];
	return 0;
}

int master()
{
	int proc_n;
	int rank;
	int work = 0;

	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	int i, j, k;
	for (i = 0; i < ROWS; i++)
	{
		k = COLUMNS;
		for (j = 0; j < COLUMNS; j++)
		{
			vet[i][j] = k;
			k--;
		}
	}

	//Seed the slaves
	for (rank = 1; rank < proc_n; rank++)
	{
		MPI_Send(vet[work], COLUMNS, MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
		work++;
	}

	//Receive a result from any slave and dispatch a new work request
	int save_path = 0;
	int* result = malloc(COLUMNS * sizeof(int));
	while (work < ROWS)
	{
		MPI_Recv(result, COLUMNS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Send(vet[work], COLUMNS, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
		save_sorted(save_path, result);
		work++;
		save_path++;
	}

	//Receive last results
	for (rank = 1; rank < proc_n; rank++)
	{
		MPI_Recv(result, COLUMNS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		save_sorted(save_path, result);
		save_path++;
	}

	free(result);

	//Kill all the slaves
	for (rank = 1; rank < proc_n; rank++)
	{
		MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
	}

	//Test checking first 10 elements of the first 10 arrays
	/*for (i = 0; i < 10; i++)*/
	/*{*/
		/*for (j = 0; j < 10; j++)*/
		/*{*/
			/*printf("%d ", vet[i][j]);*/
		/*}*/
		/*printf("\n");*/
	/*}*/

	return 0;
}

int slave()
{
	int* work = malloc(COLUMNS * sizeof(int));
	MPI_Status status;

	while (1)
	{
		MPI_Recv(work, COLUMNS, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		if (status.MPI_TAG == DIETAG)
		{
			free(work);
			return 0;
		}

		qsort(work, COLUMNS, sizeof(int), compare);

		MPI_Send(work, COLUMNS, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	return 1;
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
	fprintf(stderr, "\nTempo de execucao: %f\n\n", t2-t1);

	MPI_Finalize();

	return 0;
}
