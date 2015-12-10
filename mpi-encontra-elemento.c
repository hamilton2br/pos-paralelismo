#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 10000

void generate_vector(int *vector, int length){

	int i;

	//iniciando um vetor de inteiros crescente
	for(i = 0; i < length; i++){
		vector[i] = i;
	}

	//embaralhando o vetor
	for(i = length - 1; i > 0; i--){
		int w = rand()%i;
		int temp = vector[i];
		vector[i] = vector[w];
		vector[w] = temp;
	}
}

//encontrando o maior elemento
int find_bigger(int *vector, int length){

	int i;
	int bigger = vector[0];

	for(i = 0; i < length; i++){
		if(vector[i] > bigger) { bigger = vector[i]; }
	}

	return bigger;
}


main(int argc, char** argv){

	int nodes, rank, k, i;
	MPI_Status status;
	int begin,end;
	int *vector = malloc( sizeof(int) * SIZE);
	
	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
	//acertando os valores limites do array
	k=SIZE/nodes;
	begin=rank*k;
	end=begin+k;

	if (rank == 0) {

		//iniciando o vetor
		generate_vector(vector, SIZE);
	
		//splitting the vector among the processes
		for (i = 1; i < nodes; i++)
		{
			MPI_Send(&vector[k*i],k,MPI_INT,i,10,MPI_COMM_WORLD);
			printf("sent to rank %d\r\n",i);
			fflush(stdout);
		}
        
	} else {

          	MPI_Recv(vector,k,MPI_INT,0,10,MPI_COMM_WORLD,&status);
		printf("rank=%d received\r\n",rank);
		fflush(stdout);
	}

	//looking for the biggest in our slice
        int bigger = find_bigger(vector, k);
	int their_bigger = 0;

	if(rank == 0) {

		for(i = 1; i < nodes; i++){
			MPI_Recv(&their_bigger, 1, MPI_INT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status);
			
			if(their_bigger > bigger) { bigger = their_bigger; }

		}
	
		//Se estiver funcionando corretamente vai sempre voltar 99	
		printf("bigger = %d\r\n", bigger);
		fflush(stdout);

	} else {
		
		//cada um retorna seu maior
		printf("rank=%d bigger=%d\r\n", rank, bigger);
		fflush(stdout);
           	MPI_Send(&bigger, 1, MPI_INT, 0, 11, MPI_COMM_WORLD);
	
	}

	free(vector);	
	MPI_Finalize();
}
