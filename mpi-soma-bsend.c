/*
Soma de Elementos de um vetor usando MPI_Bsend.
Numero de elementos do vetor: multiplo do número de processos.

 */

#include <mpi.h>
#include <stdio.h>

main(argc, argv)

int			argc;
char			*argv[];

{
	int		n,n_nos, rank;
	MPI_Status	status;
        int inicio,fim,vetor[100000],soma_parcial,i,k;
        int size;
        int soma_total,soma;
/*
 * Initialize MPI.
 */
	MPI_Init(&argc, &argv);
/*
 * Error check the number of processes.
 * Determine my rank in the world group.
 * The sender will be rank 0 and the receiver, rank 1.
 */
	MPI_Comm_size(MPI_COMM_WORLD, &n_nos);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        n=100000;
        k=n/n_nos;
        inicio=rank*k;
        fim=inicio+k;
        if (rank==0) {
          for(i=0;i<n;i++)
            vetor[i]=1;
        size=(n-n/n_nos)*4+MPI_BSEND_OVERHEAD*(n_nos-1);
        int* buf=malloc(size);  
        MPI_Buffer_attach(buf,size);
          for (i=1;i<n_nos;i++)
          {
             MPI_Bsend(&vetor[0+k*i],k,MPI_INT,i,10,MPI_COMM_WORLD);
             printf("rank=%d APOS SEND\n",rank);
             fflush(stdout);
          }  
        MPI_Buffer_detach(buf,&size);
        }
        else {
          sleep(10);
          MPI_Recv(vetor,k,MPI_INT,0,10,MPI_COMM_WORLD,&status);
        printf("vetor[0]=%d rank=%d\n",vetor[0],rank);
        fflush(stdout);
        }
        soma_parcial=0;
        for(i=0;(i<k);i++)
          soma_parcial+=vetor[i];
        printf("soma_parcial=%d rank=%d\n",soma_parcial,rank);
        fflush(stdout);
        if (rank==0) {
           soma_total=soma_parcial;
           for(i=1;i<n_nos;i++){
             MPI_Recv(&soma,1,MPI_INT,MPI_ANY_SOURCE,11,MPI_COMM_WORLD,&status);
             soma_total+=soma;}
           printf("RESULTADO=%d\n",soma_total);
           fflush(stdout);
           }
        else {
           MPI_Send(&soma_parcial,1,MPI_INT,0,11,MPI_COMM_WORLD);
             }
	MPI_Finalize();
	return(0);
}
