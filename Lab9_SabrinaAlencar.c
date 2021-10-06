/*
  * Lab 9: Variacao da implementacao do problema produtor/consumidor onde o
  * consumidor consome o buffer inteiro a cada execução
  * Aluna: Sabrina Alencar | DRE: 119057413
  * Disciplina: Computacao Concorrente - 2021.1
*/
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // tamanho do buffer

sem_t buffercheio, espacovazio; 
sem_t produtor; // exclusao mutua dos produtores
int buffer[N]; 

// Função que inicializa o buffer
void inicializaBuffer(int n) {
  for (int i = 0; i < n; i ++)
    buffer[i] = 0;
}

// Função que imprime o buffer
void imprimeBuffer(int n) {
  printf("--- Buffer ");
  for (int i = 0; i < n; i ++)
    printf("%d ", buffer[i]);
  printf(" ---\n\n");
}

// Funcao que os produtores executam ao inserir no buffer
void inserir(int item, int id) {
  static int in = 0;
  sem_wait(&espacovazio); // aguarda slot vazio
  sem_wait(&produtor); // exclusao mutua entre produtores
  buffer[in] = item;
  printf("--- Produtora %d vai inserir no buffer ---\n", id);
  printf("--- Elemento %d inserido ---\n", item);
 
  // quando último elemento for inserido sinaliza o buffer cheio
  if (in == (N - 1)) {
    printf("\n--- Buffer esta cheio ---\n");
    imprimeBuffer(5);
    sem_post(&buffercheio);
  }

  in = (in + 1) % N;
  sem_post(&produtor);
}

// Funcao que os consumidores executam ao remover dados no buffer 
void remover(int id) {
  int item;
  sem_wait(&buffercheio); // aguarda buffer cheio
  printf("--- Consumidora %d vai remover o buffer ---\n", id);
  
  for(int i = 0; i < N; i++) {
    item = buffer[i];
    printf("--- Elemento %d retirado ---\n", item);
    sem_post(&espacovazio); 
  }
  printf("\n");
  
}

// Funcao thread produtora
void* produtora(void * arg) {
  int *id = (int *) arg;
  while(1) {
    inserir(*id, *id);
    sleep(1);
  }
  free(arg);
  pthread_exit(NULL);
}

// Funcao thread consumidora
void* consumidora(void * arg) {
  int *id = (int *) arg;
  while(1) {
    remover(*id);
    sleep(1);
  }
  free(arg);
  pthread_exit(NULL);
}

// Funcao para criar as threads
void createThreads(pthread_t *tids, int numprodutoras, int numconsumidoras) {
  int *id[numprodutoras + numconsumidoras];

  // preenche ids das threads
  for (int i = 0; i < numprodutoras + numconsumidoras; i++) {
    id[i] = malloc(sizeof(int));
    if(id[i] == NULL) exit(-1);
    *id[i] = i + 1;
  }

  // cria threads produtoras
  for (int i = 0; i < numprodutoras; i++) {
    if (pthread_create(tids + i, NULL, produtora, (void *) id[i])) exit(-1); 
  }

  // cria threads consumidoras
  for (int i = 0; i < numconsumidoras; i++) {
    if (pthread_create(tids + i + numprodutoras, NULL, consumidora, (void *) id[i + numprodutoras])) exit(-1); 
  }
}

// aguarda as threads terminarem
void joinThreads(pthread_t *tids, int numThreads) {
  for (int i = 0; i < numThreads; i++) {
    if (pthread_join(*(tids + i), NULL)) {
      fprintf(stderr, "--Erro no Join \n");
      exit(2);
    }
  }
}

// Funcao para inicializar tudo
void init(sem_t *produtor, sem_t *buffercheio, sem_t *espacovazio, pthread_t **tids, int numThreads) {
  sem_init(produtor, 0, 1);
  sem_init(buffercheio, 0, 0);
  sem_init(espacovazio, 0, N);

  (*tids) = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);
  if ((*tids) == NULL) {
    printf("--Erro alocacao de memoria \n");
    exit(2);
  }
}


// Funcao principal
int main(int argc, char *argv[]) {
  int numprodutoras, numconsumidoras; // numero de threads 
  pthread_t *tids; // identificadores 

  if (argc < 3) {
	printf("Digite %s <quantidade de produtoras> <quantidade de consumidoras> \n", argv[0]);
	return 1;
  }

  numprodutoras = atoi(argv[1]);
  numconsumidoras = atoi(argv[2]);


  srand(time(NULL));
  inicializaBuffer(N);
  init(&produtor, &buffercheio, &espacovazio, &tids, numprodutoras + numconsumidoras);


  //Criando as threads
  createThreads(tids, numprodutoras, numconsumidoras);

  //Esperando as threads terminarem 
  joinThreads(tids, numprodutoras + numconsumidoras);


  sem_destroy(&produtor);
  sem_destroy(&buffercheio);
  sem_destroy(&espacovazio);
  free(tids);

  return 0;
}
