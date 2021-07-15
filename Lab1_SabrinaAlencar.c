#include<stdio.h>
#include<pthread.h>

#define NTHREADS 2
int vet[10000], vetsaida[10000];//vetor global


void * quadrado (void * arg){
  //funcao que as threas vao executar
  int id_local = * (int *) arg;
  int aux;
  //thread 0 vai incrementar Ai com i par, thread 1 vai incrementar Ai com i impar
  for(int i = id_local; i < 10000; i+=2){
    vetsaida[i] = vet[i]*vet[i];
  }
}

int main(){
  int i ;
  
  //inicializando vetor 
  for(i = 0; i < 10000; i++){
    vet[i] = i+1;
  }
  //inicalizando vetor
  for(i = 0; i < 10000; i++){
    vetsaida[i] = 0;
  }

  pthread_t id[NTHREADS];
  int id_local[NTHREADS];

  //criando as threads
  for(i = 0; i < NTHREADS; i++){
    id_local[i]=i;
    printf("--Cria a thread %d\n", i+1);
    //arg de create: id, atributos, funcao, arg da funcao
    if(pthread_create(&id[i], NULL, quadrado, (void *)&id_local[i]))
      printf("Erro pthread create\n");
  }

  //espera as threads terminarem
  for(i = 0; i < NTHREADS; i++){
    //só uso o valor, nao altero entao passo o valor de tid ao inves do endereco
    if(pthread_join(id[i], NULL)) 
    printf("Erro pthread join %d\n",i+1);
  }

  //imprime vetor entrada 
  printf("Vetor Entrada\n");
  /**for(i = 0; i < 10000; i++){
    printf("%d ",vet[i]);
  }
  */
  printf("\n");

  //imprime vetor saida
  printf("Vetor Saida\n");
  /**for(i = 0; i < 10000; i++){
    printf("%d ",vetsaida[i]);
  }*/

  printf("\n");

  //fazendo a verificacao do vetor (corretude)
  for(i = 0; i < 10000; i++){
    if(vetsaida[i]!=(vet[i]*vet[i])){ 
      printf("Erro"); 
      return 0;
    }
  }
  printf("Operacao realizada com sucesso");

  return 0;
}