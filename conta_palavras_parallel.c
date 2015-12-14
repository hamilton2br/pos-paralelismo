#include <mpi.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "mede_time.h"

#define MAX_PROCS 4

//verifica se estamos lhe dando com as referencias ao diretõrio atual ou o pai (. e ..)
bool is_reference_dir(char* val){

        if(strcmp(val, ".") == 0) return true;
        if(strcmp(val, "..") == 0) return true;
        return false;
}

//adiciona a lista de arquivos a serem lidos
void write_to_file(FILE *fp, const char* string){

        fprintf(fp, "%s\r\n", string);
}

//Listando os arquivos a serem lidos
void file_list(FILE *fp, char* basedir){

	DIR* dir;
	DIR* subdir;
	struct dirent* ent;
	struct dirent* subent;

	char path[255];
	char subpath[255];

	dir = opendir(basedir);

	if (dir != NULL ){
		while ((ent = readdir(dir)) != NULL) {
			//descarta . e .. 
			if( !(is_reference_dir(ent->d_name)) ){
				strcpy(path, basedir);
				strcat(path, ent->d_name);

				subdir = opendir(path);

				if(subdir != NULL){
					while((subent = readdir(subdir)) != NULL){
						//descarta . e ..
						if( !(is_reference_dir(subent->d_name)) ){
							strcpy(subpath, path);
							strcat(subpath, "/");
							strcat(subpath, subent->d_name);
							write_to_file(fp, subpath);
							subpath[0] = '\0';
						}
					}

					closedir(subdir);

				}

				path[0] = '\0';
			}
		}

	closedir(dir);

	}
}

//lendo os arquivos e disparando os processos filhos
void read_files(FILE* fpread, MPI_Comm leitores){
	char buffer[1024];
	int i = 0;

	//spawna um ajuntador que vai esperar a resposta da galera	

	for (i = 0; i < MAX_PROCS; i++){
		//espawna os leitores e manda para cada um deles o nome de um arquivo para ler
	}

	while (fscanf(fpread, "%s", buffer) != EOF){
	
		//espera algum processos sinalizar que terminou o processamento
		//envia mais uma linha para ele ler
	}
}

void main(int argc, char* argv[]){

	TIMER_CLEAR;
	TIMER_START;

	FILE *fp;
	FILE *fpread;

	int nodes, rank;

	MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm leitores;
        MPI_Comm escritor;

	//lista os arquivos a serem lidos	
	fp=fopen("./files.txt", "w");
	file_list(fp, argv[1]);
	fclose(fp);

	printf("File created...\r\n");

	//efetivamente ler os arquivos. Nesta funcao realizaremos a paralelizacao
	fpread = fopen("./files.txt", "r");
	read_files(fpread, leitores);
	fclose(fpread);

	TIMER_STOP;
	printf("Duracao: %.3f \r\n", TIMER_ELAPSED);
	fflush(stdout);
}
