//soma todos os elementos de um vetor de inteiro
#include<stdio.h>//printf
#include<stdlib.h>//alocacao e conversoes
#include<pthread.h>
#include"timer.h"

//variaveis globais
float *vetor; //vetor de numeros reais
long long int dim;//dimensao do vetor
int NTHREADS; //numero de threads
float MENOR = 100000;
float MAIOR = 0;
/**
typedef struct {
    float menor, maior;
} t_Saida;
*/

//fluxo das threads
void * tarefa(void *arg){
    //t_Saida *retorno; //guarda as informacoes do menor e maior encontrado
    /**retorno = (t_Saida *)malloc(sizeof(t_Saida)); // guarda um endereco de uma estrutura do tipo t_Saida
    if(retorno == NULL){
        fprintf(stderr, "Erro -- malloc\n"); 
        exit(1);
    }

    retorno->maior = 0;
    retorno->menor = 100000;*/

    long long int id = (long int) arg; //identificador da thread
    for(long long int i = id; i < dim; i+=NTHREADS){
        if(vetor[i] > MAIOR) MAIOR = vetor[i];
        if(vetor[i] < MENOR) MENOR = vetor[i];
    }
    pthread_exit(NULL);
}

//programa principal
int main(int argc, char *argv[]){
    float menor = 100000;
    float maior = 0;
    double inicio, fim, delta;
    pthread_t *tid; //identificadores das threads no sistema

    //recebe os parametros de entrada e os trata
    if(argc<3){
        fprintf(stderr,"Digite %s <numero elementos do vetor> <numero de threads>\n",argv[0]);
        return 1;
    }

    dim = atoll(argv[1]);
    NTHREADS = atoi(argv[2]);

    //aloca memoria 
    vetor = (float *) malloc(sizeof(float)*dim);
    if(vetor == NULL){
        fprintf(stderr, "Erro -- malloc\n"); 
        return 2;
    }
    
    //inicializa vetor 
    for(long long int i = 0; i < dim; i++){
        vetor[i] = 111.1/(i+1);
        if(i==15) vetor[i] = -1000.2;
        //if(i==100) vetor[i] = 22.2;
        //maior valor = 1000.2
        //menor valor = -1000.2
    }

    GET_TIME(inicio); //tempo sequencial
    //funcao sequencial 
    for(long long int i = 0; i < dim; i++){
        if(vetor[i]>maior) maior = vetor[i];
        if(vetor[i]<menor) menor = vetor[i];
    }
    GET_TIME(fim);//fim inicializacao
    delta = fim - inicio; 
    printf("Tempo sequencial: %lf ms\n",delta);

    GET_TIME(inicio);//inicio parte concorrente
    
    //funcao concorrente
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
    if(tid==NULL){
        fprintf(stderr, "Erro -- malloc\n"); 
        return 2;
    }

    //cria as threads
    for(long long int i = 0; i < NTHREADS;i++){
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "Erro -- pthread_create\n"); 
            return 3;
        }
    }

    //espera as threads finalizarem
    for(long long int i = 0; i < NTHREADS;i++){
        if(pthread_join(*(tid+i), NULL)){
            fprintf(stderr, "Erro -- pthread_join\n"); 
            return 3;
        }
    }
    
    //exibe os resultados
    
    printf("Sequencial\n");
    printf("Menor: %f Maior: %f\n",menor, maior);
    printf("Concorrente\n");
    printf("Menor: %f Maior: %f\n",MENOR, MAIOR);

    GET_TIME(fim);
    delta = fim - inicio; 
    printf("Tempo concorrente: %lf ms\n",delta);

    //corretude
    if((menor==MENOR)&&(maior==MAIOR)) printf("Saida correta");
    else printf("Saida incorreta");
   
    
    //libera memoria
    free(vetor);
    free(tid);
    return 0;
}