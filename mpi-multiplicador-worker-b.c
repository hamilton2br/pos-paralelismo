#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINHA 500
#define COLUNA 500

#define PROCS 4

void init_matriz(double matriz[LINHA][COLUNA]){

	int i,j;

	#pragma omp parallel for num_threads(PROCS) shared(matriz)
	for(i = 0; i < LINHA; i++){
		for(j = 0; j < COLUNA; j++){
			matriz[i][j] = i + j;
		}
	}
}

void multiplica(double mat1[LINHA][COLUNA], double mat2[LINHA][COLUNA], MPI_Comm parent){

	int i,j,k;
	double result[LINHA][COLUNA];

	#pragma omp parallel for num_threads(PROCS) shared(mat1, mat2, result)
	for(i = 0; i < LINHA; i++){
		for(j = 0; j < COLUNA; j++){
			for(k = 0; k < LINHA; k++){
				result[i][j] = result[i][j] + (mat1[i][k] * mat2[k][j]);
			}
		}

		//envia a linha para o no principal para adicao da linha da matriz
		MPI_Send(result[i], LINHA, MPI_DOUBLE, 0, i, parent);
	}
}

void main(int argc, char** argv){

	int nodes, rank;
	double c[LINHA][COLUNA];
	double d[LINHA][COLUNA];

	MPI_Status status;
	MPI_Comm parent;

	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&parent);

	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//inicializa as matrizes C e D
	init_matriz(c);
	init_matriz(d);

	//multiplica as matrizes C e D
	multiplica(c, d, parent);

	MPI_Finalize();	
}
