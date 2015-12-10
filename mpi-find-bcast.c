#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAMANHO 100000 
#define NON_ROOT_NODES 3

int int_arr[TAMANHO];

//Inicializa array de inteiros
int buscadores(int rank, int a_procurar)
{
	int i;
	int size;
	time_t t;
	int nao_encontrou = -1;
	MPI_Status status;

	//Inicializa o gerador randomico	
	srand((unsigned) time(&t));

	//gera o array com valores randomicos
	for (i=0; i<TAMANHO; i++) {
		int_arr[i] = (rand() % 1000) * rank;
	}

	//procura o valor no array
	for(i=0; i<TAMANHO; i++){
		if(int_arr[i] == a_procurar) {
			//retorna a posicao que foi encontrado o valor
			MPI_Send(&i, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
			return i;
		}
	}

	return -1;
}

//main
main(int argc, char** argv){

	int i, nodes, rank, a_procurar, posicao, arr_posicao[NON_ROOT_NODES];

	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     
	a_procurar = 67;
	posicao = -1;

	for(i = 0; i < NON_ROOT_NODES; i++){
		arr_posicao[i] = -1;
	}

	//manda um bcast para todos os nos com o valor a procurar
	MPI_Bcast(&a_procurar, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//nos filhos processam
	if(rank != 0){
		posicao = buscadores(rank, a_procurar);
	}

	MPI_Gather(&posicao, 1, MPI_INT, arr_posicao, NON_ROOT_NODES, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0){

		for(i = 0; i < NON_ROOT_NODES; i++){
			if(arr_posicao[i] > 0){
				printf("no %d, encontrou %d na posicao: %d\r\n", i+1, a_procurar, arr_posicao[i]);
				fflush(stdout);
			} else {
				printf("no %d, nao encontrou %d no array.\r\n", i+1, a_procurar);
				fflush(stdout);
			}
		}

	}
 
	MPI_Finalize();
	return(0);
}
