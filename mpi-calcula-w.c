#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALTURA 500
#define LARGURA 500

//imprime a matriz
void imprime_matriz(double M[LARGURA][ALTURA]){

	int i, j;

	for(i = 0; i < LARGURA; i++){
		for(j = 0; j < ALTURA; j++){
			printf("%.0f - ", M[i][j]);
			fflush(stdout);
		}

		printf("\r\n");
		fflush(stdout);
	}
}

//Inicializa matrizes
void  inicializa_matriz(double matriz[LARGURA][ALTURA], int matrizid){
	
	int i, j, valor_init;

	switch(matrizid){
		case 'a':
			valor_init = 1;
			break;

		case 'b':
			valor_init = 2;
			break;

		case 'c':
			valor_init = 3;
			break;

		case 'd':
			valor_init = 4;
			break;

		case 'e':
			valor_init = 5;
			break;

		case 'f':
			valor_init = 6;
			break;

		case 't':
			valor_init = 7;
			break;
	}
	
	for(i = 0; i < LARGURA; i++){
		for(j = 0; j < ALTURA; j++){		
			matriz[i][j] = valor_init;
		}
	}
}

//multiplica duas matrizes
void multiplica_matriz(double multiplicando[LARGURA][ALTURA], double multiplicador[LARGURA][ALTURA], double produto[LARGURA][ALTURA]){

	int i, j, k;

	for(i = 0; i < LARGURA; i++){
		for(j = 0; j < ALTURA; j++){
			for(k = 0; k < LARGURA; k++){
				produto[i][j] = produto[i][j] + (multiplicando[i][k] * multiplicador[k][j]);
			}
		}
	}
}

//somando um array a uma linha da matriz
void soma_linha(int linha, double M[LARGURA][ALTURA], double arr[LARGURA]){

	int i;

	for(i = 0; i < LARGURA; i++){
		M[linha][i] = M[linha][i] + arr[i];
	}
}

//Tarefa 1 inicializa A, B e calcula X = A x B
void tarefa1(int rank){
	double A[LARGURA][ALTURA];
	double B[LARGURA][ALTURA];
	double X[LARGURA][ALTURA];
	MPI_Request request = MPI_REQUEST_NULL;
	MPI_Status status;
	int i;

	inicializa_matriz(A, 'a');
	inicializa_matriz(B, 'b');

	multiplica_matriz(A, B, X);

	//envia linha por linha para 3
	for(i = 0; i < LARGURA; i++){
		MPI_Isend(X[i], ALTURA, MPI_DOUBLE, 3, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
	}
}

//Tarefa 2 inicializa C, D e calcula Y = C x D
void tarefa2(int rank){
	double C[LARGURA][ALTURA];
	double D[LARGURA][ALTURA];
	double Y[LARGURA][ALTURA];
	MPI_Request request = MPI_REQUEST_NULL;
	MPI_Status status;
	int i;

	inicializa_matriz(C, 'c');
	inicializa_matriz(D, 'd');

	multiplica_matriz(C, D, Y);

	//envia linha por linha para 4
	for(i = 0; i < LARGURA; i++){
		MPI_Isend(Y[i], ALTURA, MPI_DOUBLE, 3, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
	}
}

//tarefa 3 inicializa E, F e calcula Z = E x F
void tarefa3(int rank){
	double E[LARGURA][ALTURA];
	double F[LARGURA][ALTURA];
	double Z[LARGURA][ALTURA];
	MPI_Request request = MPI_REQUEST_NULL;
	MPI_Status status;
	int i;

	inicializa_matriz(E, 'e');
	inicializa_matriz(F, 'f');

	multiplica_matriz(E, F, Z);

	//envia linha por linha para 4
	for(i = 0; i < LARGURA; i++){
		MPI_Isend(Z[i], ALTURA, MPI_DOUBLE, 3, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
	}
}

//tarefa 4 inicializa T e calcula W = (X + Y + Z) x T
void tarefa4(int rank){
	double T[LARGURA][ALTURA];
	double S[LARGURA][ALTURA];
	double W[LARGURA][ALTURA];
	double buff[LARGURA];
	MPI_Request request = MPI_REQUEST_NULL;
	MPI_Status status;
	int i;

	inicializa_matriz(T, 't');

	//para cada linha recebida
	for(i = 0; i <  LARGURA; i++){
		MPI_Irecv(buff, LARGURA, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);

		soma_linha(i, S, buff);

		MPI_Irecv(buff, LARGURA, MPI_DOUBLE, 1, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		
		soma_linha(i, S, buff);
		
		MPI_Irecv(buff, LARGURA, MPI_DOUBLE, 2, i, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		
		soma_linha(i, S, buff);

		printf("%d\r", i);
	}

	multiplica_matriz(S, T, W);
}

//main
main(int argc, char** argv){

	int nodes, rank;

	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	switch(rank){
		case 0:
			tarefa1(rank);
			break;

		case 1: 
			tarefa2(rank);
			break;

		case 2:
			tarefa3(rank);
			break;

		case 3:
			tarefa4(rank);
			break;
	}

	MPI_Finalize();
}
