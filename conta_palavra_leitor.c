#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Estrutura de lista para manter a contagem de palavras
struct lista_palavras_s{

        char  palavra[1024];
        int contagem;
        struct lista_palavras_s *next;

};

typedef struct lista_palavras_s lista_palavras_t;

//lowering case
void tolowercase(char str[1024]){

        int i;

        for(i = 0; i < strlen(str); i++){
                str[i] = tolower(str[i]);
        }
}

//removendo caracteres especiais e espaços
void trim(char str[1024]){

        int i, j;
        char result[1024];

        i = 0; j = 0;

        for(i = 0; i < strlen(str); i++){
                if(isalpha(str[i])){
                        result[j] = str[i];
                        j++;
                }
        }

        result[j] = '\0';
        str[0] = '\0';
        strcpy(str, result);
}

//adiciona a contagem de uma palavra na lista
void incrementa_contagem(char* palavra, lista_palavras_t* lista){
        lista_palavras_t* cursor = NULL;

        cursor = lista;

        //nao chegou ao fim da fila
        while (cursor->next != NULL){
                if(strcmp(cursor->palavra, palavra) == 0){
                        cursor->contagem++;
                        return;
                }

                cursor = cursor->next;
        }

        //chegamos ao fim da fila e nao achamos a palavra
        cursor->next = malloc ( sizeof(lista_palavras_t ) );
        cursor->next->palavra[0] = '\0';
        strcpy(cursor->next->palavra, palavra);
        cursor->next->contagem = 1;
        cursor->next->next = NULL;
        return;
}

//lista as palavras e suas ocorrencias
void lista_contagem(lista_palavras_t* lista, FILE* outFile){
        if(lista->next != NULL){
                lista_contagem(lista->next, outFile);
        }

        if(lista->contagem > 20 && strlen(lista->palavra) > 1) {
                fprintf(outFile, "%s,%d\r\n", lista->palavra, lista->contagem);
        }
}

//cria a lista
lista_palavras_t* cria_lista(){
        lista_palavras_t* lista;

        if( (lista = malloc ( sizeof(lista_palavras_t ) ) ) == NULL ) {
                return NULL;
        } else {
                lista->contagem = 0;
                lista->palavra[0] = '\0';
                lista->next = NULL;
                return lista;
        }
}

//destroi a lista
void destroi_lista(lista_palavras_t* lista){
        if(lista->next != NULL){
                destroi_lista(lista->next);
        }

        free(lista);
}

//adiciona a lista de arquivos a serem lidos
void write_to_file(FILE *fp, const char* string){

        fprintf(fp, "%s\r\n", string);
}

//adiciona a lista de arquivos a serem lidos
void write_to_file(FILE *fp, const char* string){

        fprintf(fp, "%s\r\n", string);
}

void main(){
	//!!
}
