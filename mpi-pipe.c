#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAMANHO 500

int X[TAMANHO][TAMANHO], Y[TAMANHO][TAMANHO], Z[TAMANHO][TAMANHO];

void tarefa1()
{
	int i,j;
	int size;
	int term_signal;
	MPI_Status status;

	for (i=0; i<TAMANHO; i++) {
		for (j=0; j<TAMANHO; j++){
			X[i][j]=i*2-j;
		}
	}

	size = ( TAMANHO * sizeof(int) ) + MPI_BSEND_OVERHEAD;

	for (i=0; i<TAMANHO; i++) {
		//utilizando a linha da matriz no campo MPI_TAG
		int* buffer = malloc(size);
		MPI_Buffer_attach(buffer, size);
		MPI_Bsend(&X[i], TAMANHO, MPI_INT, 3, i, MPI_COMM_WORLD);
		MPI_Buffer_detach(buffer, &size);
		free(buffer);
	}
}

void tarefa2()
{
	int i,j;
	int size;
	int term_signal;
	MPI_Status status;

	for (i=0; i<TAMANHO; i++){
		for (j=0; j<TAMANHO; j++) {
			Y[i][j]=i*3-j;
		}
	}

	size = ( TAMANHO * sizeof(int) ) + MPI_BSEND_OVERHEAD;

	for (i=0;i<TAMANHO;i++) {
		//utlilizando a linha da matriz no campo MPI_TAG
		int* buffer = malloc(size);
		MPI_Buffer_attach(buffer, size);
		MPI_Bsend(&Y[i], TAMANHO, MPI_INT, 3, i, MPI_COMM_WORLD);
		MPI_Buffer_detach(buffer, &size);
		free(buffer);
	}
}

void tarefa3()
{
	int i,j;
	int size;
	int term_signal;
	MPI_Status status;

	for (i=0;i<TAMANHO;i++){
		for (j=0;j<TAMANHO;j++) {
			Z[i][j]=i*2+j;
		}
	}
	
	size = ( TAMANHO * sizeof(int) ) + MPI_BSEND_OVERHEAD;

	for (i=0;i<TAMANHO;i++) {
		//utilizando a linha da matriz no campo MPI_TAG
		int* buffer = malloc(size);
		MPI_Buffer_attach(buffer, size);
		MPI_Bsend(&Z[i], TAMANHO, MPI_INT, 3, i, MPI_COMM_WORLD);
		MPI_Buffer_detach(buffer, &size);
		free(buffer);
	}
}

void tarefa4()
{
	int buffer[TAMANHO];
	int W[TAMANHO][TAMANHO];
	int x,y,z,i;
	int term_signal = 1;
	MPI_Status status;

	x = 0; y = 0; z = 0;

	while(x < TAMANHO || y < TAMANHO || z < TAMANHO){
	
        	MPI_Recv(buffer, TAMANHO, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		printf("4 recebendo linha %d de no %d, x=%d, y=%d, z=%d\r\n", status.MPI_TAG, status.MPI_SOURCE, x, y, z);
		fflush(stdout);

		//usando a MPI_TAG como identificador da linha da matriz que foi recebida
		for(i = 0; i < TAMANHO; i++){
			W[status.MPI_TAG][i] = W[status.MPI_TAG][i] + buffer[i];
		}
		
		switch(status.MPI_SOURCE){
		
			case 0 :
				x++;
				break;

			case 1 :
				y++;
				break;

			case 2 :
				z++;
				break;
		}
		
	}

	printf("W[0][0] = %d, W[0][499] = %d, W[499][0] = %d, W[499][499] = %d\r\n", W[0][0], W[0][TAMANHO-1], W[TAMANHO-1][0], W[TAMANHO-1][TAMANHO-1]);
	fflush(stdout);
}


//main
main(int argc, char** argv){

	int nodes, rank;

	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
       
	switch(rank){
	
		case 0 :
			tarefa1();
			break;

		case 1 :
			tarefa2();
			break;

		case 2 :
			tarefa3();
			break;

		case 3:
			tarefa4();
			break;
	}
 
	MPI_Finalize();
	return(0);
}
