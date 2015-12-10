#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALTURA 10000
#define LARGURA 10000

int int_matrix[LARGURA][ALTURA];

//Inicializa array de inteiros
int encontra_maior(int rank, int int_arr[LARGURA])
{
	int i, maior;

	maior = -1;

	//procura o maior valor no array
	for(i=0; i<LARGURA; i++){
		if(int_arr[i] > maior) { maior = int_arr[i];  }
	}

	return maior;
}

//main
main(int argc, char** argv){

	int i, j, nodes, rank, maior, maior_local, maior_global, int_arr[LARGURA];
	time_t t;

	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand((unsigned) time(&t));

	//inicializa matriz com valores randomicos
	if(rank == 0){
		for(i = 0; i < LARGURA; i++){
			for(j = 0; j < ALTURA; j++){
				int_matrix[i][j] = rand();
			}
		}
	}
	
	for(i = 0; i < LARGURA; i++){
		int_arr[i] = -1;
	} 
 
	//de 1 ate largura
	for(i = 0; i < LARGURA; i++){
		//manda um scatter para geral
		MPI_Scatter(int_matrix[i], LARGURA/nodes, MPI_INT, int_arr, LARGURA/nodes, MPI_INT, 0, MPI_COMM_WORLD);

		maior_local = encontra_maior(rank, int_arr);

		//junta tudo
		MPI_Reduce(&maior_local, &maior_global, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

		if(rank == 0){
			if(maior < maior_global) { maior = maior_global; }
			printf("%d\r", i);
			fflush(stdout);
		}
	}

	if(rank == 0){
		printf("Maior elemento encontrado: %d\r\n", maior);
		fflush(stdout);
	}

	MPI_Finalize();
	return(0);
}
