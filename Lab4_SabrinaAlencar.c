#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

//Variaveis Globais
int qtd_threads_exe = 0;
pthread_mutex_t qtd_mutex;
pthread_cond_t qtd_cond;

//Threads incrementam qtd_thread_exe
//T1: Fique a vontade
void * T1 (void * arg){
    //printf("Sou a Thread 1\n");

    //testando se posso executar
    pthread_mutex_lock(&qtd_mutex);
    while(qtd_threads_exe == 0){
        //printf("T1: vai se bloquear...\n");
        pthread_cond_wait(&qtd_cond, &qtd_mutex);
        //printf("T1: sinal recebido e mutex realocado\n");
    }
    printf("Fique a vontade!\n");
    qtd_threads_exe++;
    //printf("Enviando broadcast\n");
    pthread_cond_broadcast(&qtd_cond);//desbloqueando possiveis threads bloqueadas
    pthread_mutex_unlock(&qtd_mutex);

    pthread_exit(NULL);
}

//T2: Seja bem vindo 
void * T2 (void * arg){
    //printf("Sou a Thread 2\n");
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&qtd_mutex);//exclusao mutua pra acessar qtd_threads_exe
    qtd_threads_exe++;
    //printf("Enviando broadcast\n");
    pthread_cond_broadcast(&qtd_cond);//desbloqueando possiveis threads bloqueadas
    pthread_mutex_unlock(&qtd_mutex);

    pthread_exit(NULL);
}

//T3: Volte sempre
void * T3 (void * arg){
    //printf("Sou a Thread 3\n");

    //testando se posso executar
    pthread_mutex_lock(&qtd_mutex);
    while(qtd_threads_exe < 3){
        //printf("T3: vai se bloquear...\n");
        pthread_cond_wait(&qtd_cond, &qtd_mutex);
        //printf("T3: sinal recebido e mutex realocado\n");
    }
    printf("Volte sempre!\n");
    qtd_threads_exe++;
    //printf("Enviando broadcast\n");
    pthread_cond_broadcast(&qtd_cond);//desbloqueando possiveis threads bloqueadas
    pthread_mutex_unlock(&qtd_mutex);

    pthread_exit(NULL);
}

//t4: sente-se por favor
void * T4 (void * arg){
    //printf("Sou a Thread 4\n");

    //testando se posso executar
    pthread_mutex_lock(&qtd_mutex);
    while(qtd_threads_exe == 0){
        //printf("T4: vai se bloquear...\n");
        pthread_cond_wait(&qtd_cond, &qtd_mutex);
        //printf("T4: sinal recebido e mutex realocado\n");
    }
    printf("Sente-se por favor.\n");
    qtd_threads_exe++;
    //printf("Enviando broadcast\n");
    pthread_cond_broadcast(&qtd_cond);//desbloqueando possiveis threads bloqueadas
    pthread_mutex_unlock(&qtd_mutex);

    pthread_exit(NULL);
}


//funcao principal
int main(){
    int i;
    pthread_t threads[NTHREADS];

    //inicializa mutex e var de condicao 
    pthread_mutex_init(&qtd_mutex, NULL);
    pthread_cond_init (&qtd_cond, NULL);

    //cria as threads
    pthread_create(&threads[0], NULL, T1, NULL);
    pthread_create(&threads[1], NULL, T2, NULL);
    pthread_create(&threads[2], NULL, T3, NULL);
    pthread_create(&threads[3], NULL, T4, NULL);
    
    // Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
    }

    //desaloca variaveis e termina
    pthread_mutex_destroy(&qtd_mutex);
    pthread_cond_destroy(&qtd_cond);
    return 0;
    
}





