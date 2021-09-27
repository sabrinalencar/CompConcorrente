#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  4

//Variaveis Globais
int contador = 0; // contador de quantas threads foram executadas
sem_t condt1, condt4, condt3;     //semaforos para sincronizar a ordem de execucao das threads
pthread_mutex_t cont;


//T1: Fique a vontade
void * T1 (void * arg){
  //printf("Sou a Thread 1\n");
  sem_wait(&condt1); 
    
  printf("Fique a vontade!\n");
  pthread_mutex_lock(&cont);
  contador++;
  pthread_mutex_unlock(&cont);

  //se t4 ja tiver executado damos post em t3 (ultima a executar)
  if(contador>2) sem_post(&condt3); 
  else sem_post(&condt4); //senao damos post em t4
  
  pthread_exit(NULL);
}

//T2: Seja bem vindo (primeira thread)
void * T2 (void * arg){
  //printf("Sou a Thread 2\n");
  printf("Seja bem-vindo!\n");
  pthread_mutex_lock(&cont);
  contador++;
  pthread_mutex_unlock(&cont);
  
  //permite que T1 e T4 executem sem importar a ordem
  sem_post(&condt1);  
  sem_post(&condt4);  

  pthread_exit(NULL);
}

//T3: Volte sempre (ultima thread)
void * T3 (void * arg){
  //printf("Sou a Thread 3\n");
  sem_wait(&condt3);

  printf("Volte sempre!\n");
  pthread_mutex_lock(&cont);
  contador++;
  pthread_mutex_unlock(&cont);
  
    
  pthread_exit(NULL);
}

//t4: sente-se por favor
void * T4 (void * arg){
  //printf("Sou a Thread 4\n");
  sem_wait(&condt4);

  printf("Sente-se por favor.\n");
  pthread_mutex_lock(&cont);
  contador++;
  pthread_mutex_unlock(&cont);

  //se t1 ja tiver executado damos post em t3(ultima a executar)
  if(contador>2) sem_post(&condt3); 
  else sem_post(&condt1); //senao damos post em t1
    

  pthread_exit(NULL);
}


//funcao principal
int main(){
  int i;
  pthread_t threads[NTHREADS];

  //inicia os semaforos
  sem_init(&condt1, 0, 0);
  sem_init(&condt3, 0, 0);
  sem_init(&condt4, 0, 0);

  pthread_mutex_init(&cont, NULL);

  //cria as threads
  pthread_create(&threads[0], NULL, T1, NULL);
  pthread_create(&threads[1], NULL, T2, NULL);
  pthread_create(&threads[2], NULL, T3, NULL);
  pthread_create(&threads[3], NULL, T4, NULL);
    
  // Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&cont);

  return 0;
    
}