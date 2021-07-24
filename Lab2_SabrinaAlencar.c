//mult de matriz x vetor (considerando matrizes quadradas)
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

//escopo global para que todos fluxos de execucao os enxerguem
float *mat1; //matriz de entrada
float *mat2; //segunda matriz entrada
float *msaida; //matriz de saida 
int NTHREADS;// numero de threads

typedef struct{
  int id; //identificador do elemento que a thread ira processar
  int dim; //dimensao das estruturas de entrada
} tArgs;

  
//funcao que as threads vao executar
void * tarefa(void *arg){
  tArgs *args = (tArgs*) arg;
  //printf("Thread %d\n",args->id);
  //for que percorre as linhas da matriz 1
  for(int i = args->id; i < args->dim; i+=NTHREADS){
    // for que percorre as colunas da matriz 2
    for(int j = 0; j < args->dim; j++){
      // for responsavel pelos produtos dos elementos da linha e coluna correspondente 
      for(int k = 0; k < args->dim; k++){
        msaida[i*(args->dim)+j] += mat1[i*(args->dim)+k] * mat2[k*(args->dim)+j];
      } 
    }
  }
  pthread_exit(NULL);
}


int main(int argc, char* argv[]){
  int n; //dimensao matriz de entrada
  pthread_t *tid; //id das threads
  tArgs *args; // id locais das threads e dimensao
  int corretude = 1;
  double inicio, fim, delta;

  GET_TIME(inicio); //medindo tempo da inicializacao
  
  //leitura e avaliacao dos parametros de entrada
  if(argc<3){ //nome do programa + dimensao + num threads
  printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
  return 1;
  }
  n = atoi(argv[1]); 
  NTHREADS = atoi(argv[2]);
  if(NTHREADS > n) NTHREADS = n; //para evitar criar threads a mais
  
  //alocacao de memoria p as estruturas de dados 
  //malloc me retorna void faço typecast pra ponteiro pra float
  mat1 = (float *) malloc(sizeof(float) * n * n);//qtd de bytes que preciso
  if(mat1 == NULL) { printf("Erro malloc\n"); return 2;}
  mat2 = (float *) malloc(sizeof(float) * n * n);
  if(mat2 == NULL) { printf("Erro malloc\n"); return 2;}
  msaida = (float *) malloc(sizeof(float) * n * n);
  if(msaida == NULL) { printf("Erro malloc\n"); return 2;} 
  
  //inicializacao das estruturas de dados
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      mat1[i*n+j] = 1; //equivalente a mat[i][j] 
      mat2[i*n+j] = 1;
      msaida[i*n+j] = 0;
    }
  }
  
  GET_TIME(fim);//fim inicializacao
  delta = fim - inicio; 
  printf("Tempo inicializacao (sequencial): %lf ms\n",delta);

  GET_TIME(inicio);//inicio parte concorrente

  //operacao de multiplicacao 
  //alocacao das estruturas
  tid = (pthread_t*) malloc(sizeof(pthread_t)*NTHREADS);
  if(tid == NULL){ puts("Erro malloc"); return 2;}
  args = (tArgs*) malloc(sizeof(tArgs)*NTHREADS);
  if(args == NULL){ puts("Erro malloc"); return 2;}
  
  //criacao das threads
  for(int i = 0; i < NTHREADS; i++){
    //args é o endereco do primeiro byte
    (args+i)->id = i;
    (args+i)->dim = n;
    if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){ puts("Erro pthread create"); return 3;}
  }
  //espera pelo termino das threads
  for(int i = 0; i < NTHREADS; i++){
    //*(tid+i) = valor do id 
    pthread_join(*(tid+i), NULL);
  }

  GET_TIME(fim);
  delta = fim - inicio; 
  printf("Tempo multiplicacao (concorrente): %lf ms\n",delta);

  GET_TIME(inicio);//inicio finalizacao

  //exibicao dos resultados
  /**
  puts("Matriz de entrada 1");
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      printf("%.1f ", mat1[i*n+j]);
    puts(" ");
  }

  puts("Matriz de entrada 2");
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      printf("%.1f ", mat2[i*n+j]);
    puts(" ");
  }

  puts("Matriz de saida");
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      printf("%.1f ", msaida[i*n+j]);
    puts(" ");
  }*/
  //corretude matriz
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
        for(int k = 0; k < n; k++) 
        if(msaida[i*n+j]!=n){ printf("Erro -- saida incorreta\n");return 4;}
    }
  }
  if(corretude) printf("Saida correta\n");

  //liberacao da memoria
  free(mat1);
  free(mat2);
  free(msaida);
  free(args);
  free(tid);

  GET_TIME(fim);
  delta = fim - inicio; 
  printf("Tempo finalizacao (sequencial): %lf ms\n",delta);

  return 0;
}
  
  
