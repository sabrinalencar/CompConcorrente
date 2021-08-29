#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5
#define PASSOS  2

/* Variaveis globais */
int *vetor; //vetor de inteiros
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//funcao barreira
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
      //ultima thread a chegar na barreira
      pthread_cond_broadcast(&x_cond);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg) {
  int id = *(int*)arg;
  int *somaLocal; //ponteiro p variavel local de soma dos elementos
  
  somaLocal = (int*) malloc(sizeof(int));//guarda endereco de um inteiro
    if(somaLocal == NULL){
        fprintf(stderr, "Erro -- malloc\n"); 
        exit(1);
    }
    *somaLocal = 0;

  for (int passo=0; passo < PASSOS; passo++) {
    printf("Thread %d: passo=%d\n", id, passo);

    //passo 1 soma
    if(passo == 0){
        for(int i = 0; i < NTHREADS; i++){
          *somaLocal += vetor[i];
        }
    }

    //passo 2 escrita
    if(passo == 1){
        vetor[id] = 10;
    }

    //sincronizacao condicional
    barreira(NTHREADS);
  }
  pthread_exit((void *) somaLocal);//retorna o resultado da soma local
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];
  int id[NTHREADS];
  int *retorno; //valor retorno das threads
  int somas[NTHREADS];
  int diferente = 0;

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  //aloca memoria 
  vetor = (int *) malloc(sizeof(int)*NTHREADS);
    if(vetor == NULL){
      fprintf(stderr, "Erro -- malloc\n"); 
      return 2;
    }
    
  //inicializa vetor 
    printf("Vetor inicial ");
    for(int i = 0; i < NTHREADS; i++){
      vetor[i] = (i+5)%10;
      printf("%d ", vetor[i]);
    }
    printf("\n");
  
  // Cria as threads 
  for(int i=0;i<NTHREADS;i++) {
    id[i]=i;
    if(pthread_create(&threads[i], NULL, tarefa, (void *) &id[i])){
      fprintf(stderr, "Erro -- pthread_create\n"); 
      return 3;
    }
  }

  // Espera todas as threads completarem 
  for (int i = 0; i < NTHREADS; i++) {
    if(pthread_join(threads[i], (void**) &retorno)){
      fprintf(stderr, "Erro -- pthread_join\n"); 
      return 3;
    }
    somas[i] = *retorno;
    free(retorno); 
  }

  printf("Somas retornadas\n");
  //Verifica se todas somas retornadas são iguais
  for (int i = 0; i < NTHREADS; i++){
    if(somas[i] != somas[0]) diferente = 1;
    printf("%d ", somas[i]);
  }

  if(diferente) printf("Somas diferentes\n");
  else printf("Somas iguais\n");

  printf ("FIM.\n");

  // Desaloca variaveis e termina 
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  
  return 0;
}