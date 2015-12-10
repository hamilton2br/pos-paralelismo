#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define SLAVEREADYTAG 0
#define WORKRESULTTAG 1
#define DIETAG 2
#define FUNCS 10

void (*tab_func[FUNCS])();

void func0(){ int result = 0; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func1(){ int result = 1; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func2(){ int result = 2; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func3(){ int result = 3; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func4(){ int result = 4; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func5(){ int result = 5; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func6(){ int result = 6; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func7(){ int result = 7; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func8(){ int result = 8; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }

void func9(){ int result = 9; MPI_Send(&result, 1, MPI_INT, 0, WORKRESULTTAG, MPI_COMM_WORLD); }


//função executada pelo master
void master(int nodes){
	int i = 0;
	int num_respostas = 0;
	int a_result = 0;
	MPI_Status status;

	while(i < FUNCS){
		//espera a resposta do primeiro slave
		MPI_Recv(&a_result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		//0 e o sinal usado pelo escravo para dizer que ele esta pronto
		if(status.MPI_TAG == WORKRESULTTAG){
			printf("resposta de %d recebida e valor = %d\r\n", status.MPI_SOURCE, a_result);
			fflush(stdout);
			num_respostas++;
		}
		
		//envia tarefa
		MPI_Send(&i, 1, MPI_INT, status.MPI_SOURCE, 10, MPI_COMM_WORLD);
		i++;
	}

	//todas as tarefas foram disparadas, aguardando os retornos
	while(num_respostas < FUNCS){
		MPI_Recv(&a_result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		num_respostas++;
		printf("resposta de %d recebida e valor = %d\r\n", status.MPI_SOURCE, a_result);
		fflush(stdout);
	}

	for(i = 1; i < nodes; i++){
		//envia sinal de kill
		printf("Matando %d \r\n", i);
		fflush(stdout);
		MPI_Send(0, 0, MPI_INT, i, DIETAG, MPI_COMM_WORLD);
	}	
}


//função executada pelo slave
void slave(){
	MPI_Status status;
	int func_id = 0;
	
	//sinaliza master que esta pronto 
	MPI_Send(0, 0, MPI_INT, 0, SLAVEREADYTAG, MPI_COMM_WORLD);

	while(true){
		//espera recebimento da mensagem
		MPI_Recv(&func_id, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		//se nao for para morrer
		if(status.MPI_TAG == DIETAG) break;

		//executa tarefa e retorna resultado
		(*tab_func[func_id])();
	}
}

//main
main(int argc, char** argv){

	int nodes, rank;

	tab_func[0] = func0;
	tab_func[1] = func1;
	tab_func[2] = func2;
	tab_func[3] = func3;
	tab_func[4] = func4;
	tab_func[5] = func5;
	tab_func[6] = func6;
	tab_func[7] = func7;
	tab_func[8] = func8;
	tab_func[9] = func9;
	
	//os procedimentos de inicializacao
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
	if (rank == 0) {
		//dispara a funcao master
		master(nodes);
	} else {
		//dispara a função slave
		slave();	
	}

	MPI_Finalize();
	return(0);
}
