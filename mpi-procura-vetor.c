#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define TAMANHO 500

int X[TAMANHO];

void encontra(int to_find)
{
	int i;
	int value_to_find;
	
	for (i=0; i<TAMANHO; i++) { 
		X[i][j]=rand();
	}

        for(i = 0; i < TAMANHO; i++) {
		if(X[i] == value_to_find){
			//retorna valor
			//sai do loop
		}
	}	
}

void recebe()
{
	//recebe o buffer, ainda nao sei como e imprime
}


//main
main(int argc, char** argv){

	int nodes, rank;

	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//realiza o BCast

	//dependendo de quem for o no...
	if(rank == 0) {
		recebe();
	} else {
		encontra(rand());
	}       
 
	MPI_Finalize();
	return(0);
}
