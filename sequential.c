#include <stdio.h>
#include <stdlib.h>

#define ROWS 1000
#define COLUMNS 100000

int vet[ROWS][COLUMNS];

int main(int argc, const char* argv[])
{
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
	return 0;
}