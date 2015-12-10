#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINHA 500
#define COLUNA 500

double resultado[LINHA][COLUNA];

//imprime a matriz
void imprime_matriz(double M[LINHA][COLUNA]){

        int i, j;

        for(i = 0; i < LINHA; i++){
                for(j = 0; j < COLUNA; j++){
                        printf("%.0f - ", M[i][j]);
                        fflush(stdout);
                }

                printf("\r\n");
                fflush(stdout);
        }
}


void main(int argc, char** argv){

	int nodes, rank, i, j;
	double buff[LINHA];

        MPI_Status status;
        MPI_Comm intercom1;
	MPI_Comm intercom2;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_spawn("./mpi-multiplicador-worker-a", MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercom1, MPI_ERRCODES_IGNORE);
	MPI_Comm_spawn("./mpi-multiplicador-worker-b", MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercom2, MPI_ERRCODES_IGNORE);

	printf("nos filhos disparados\r\n");
	fflush(stdout);

	i = 0; j = 0;

	while(i < LINHA){
		MPI_Recv(buff, LINHA, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, intercom1, &status);

		for(j = 0; j < COLUNA; j++){
			resultado[status.MPI_TAG][j] = resultado[status.MPI_TAG][j] + buff[j];
		}

		MPI_Recv(buff, LINHA, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, intercom2, &status);

		for(j = 0; j < COLUNA; j++){
			resultado[status.MPI_TAG][j] = resultado[status.MPI_TAG][j] + buff[j];
		}

		i++;

		printf("%d\r", i);
		fflush(stdout);
	}	

	//imprime_matriz(resultado);

	MPI_Finalize();
}
