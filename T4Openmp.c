//compilar com gcc -fopenmp T4Openmp.c -o T4Openmp
//para rodar apos compilado ./T4Openmp
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>


// Tamanho da linha do arquivo e quantidade de palavras chaves
#define TAM_LINHA 315
#define QUANT_CHAVES 4


// Array de palavras que serão buscadas e de resultados
char palavras_chave[QUANT_CHAVES][25] = {"Jesus", "Davi", "do", "this"};
int ocorrencias_palavra_chave[QUANT_CHAVES] = {0};


// Funcao que conta o numero de ocorrencias de uma palavra
int num_ocorrencias(char *linha, char *palavra)
{
    int cont = 0;
    int i;
    int j = 0;
    for (i = 0; i < strlen(linha); i++)
    {
        if (linha[i] == palavra[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        if (j == strlen(palavra))
        {
            cont++;
        }
    }
    // Contabiliza numero de ocorrencias de uma palavra
    return cont;
}

int main()
{
    // Setando Num Max de Threads
    omp_set_num_threads(QUANT_CHAVES);

    // Pegando tempo Inicial
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    // Lendo arquivo
    FILE *arquivo_texto;
    if ((arquivo_texto = fopen("program.txt","r")) == NULL){
       printf("Erro ao ler arquivo");
    };

    // Conta ocorrencias
    char linha[TAM_LINHA];
    while (fgets(linha, TAM_LINHA, arquivo_texto) != NULL) {
        int i;
        // Regiao Paralela
        #pragma omp for
        for (i=0; i<QUANT_CHAVES; i++) {
            ocorrencias_palavra_chave[i] += num_ocorrencias(linha, palavras_chave[i]);
        };
    };

    // Fecha arquivo
    fclose(arquivo_texto);

    // Pegando tempo final
    gettimeofday(&t2, NULL);
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec -t1.tv_usec)/1000000.0);
    printf("tempo total = %f\n", t_total);

    // Printando os resultados
    int i;
    // Outra região paralela
    #pragma omp for
    for (i=0;i<QUANT_CHAVES;i++) {
        printf("%s:%d\n", palavras_chave[i], ocorrencias_palavra_chave[i]);
    }
}