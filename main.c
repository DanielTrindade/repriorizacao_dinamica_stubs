#include "fila_prioridade.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_PROD 2
#define N_CONS 4
#define N_REPR 1

void* produtor(void* arg) {
    // implemente o código do produtor
    // use a função usleep() para criar um ritmo
    // a escala do usleep() é de microsegundos
    
    for (int i = 0; i < 50; i++) {
        Pedido p;
        p.id_dispositivo = rand() % 100 + 1;
        p.timestamp = time(NULL);
        p.prioridade = rand() % 100 + 1;
        
        inserir(p);
        usleep(100000); // 0.1 segundo
    }
    
    return NULL;
}

void* consumidor(void* arg) {
    // implemente o código do consumidor
    // use a função usleep() para criar um ritmo
    // a escala do usleep() é de microsegundos

    for (int i = 0; i < 50; i++) {
        remover();
        usleep(200000); // 0.2 segundo
    }
    
    return NULL;
}

void* atualizador(void* arg) {
    // implemente o código do atualizador
    // use a função usleep() para criar um ritmo
    // a escala do usleep() é de microsegundos
    // use o rand() para gerar as entradas por exemplo:
    //      int id = rand() % 50;
    //      int nova_prioridade = rand() % 100;

    for (int i = 0; i < 20; i++) {
        int id = rand() % 50;
        int nova_prioridade = rand() % 100;
        
        repriorizar(id, nova_prioridade);
        usleep(500000); // 0.5 segundo
    }
    
    return NULL;
}

int main() {
    pthread_t prod[N_PROD], cons[N_CONS], repr[N_REPR];
    inicializar_fila();
    srand(time(NULL));

    printf("=== INICIANDO TESTE ===\n");

    for (int i = 0; i < N_PROD; i++)
        pthread_create(&prod[i], NULL, produtor, NULL);
    for (int i = 0; i < N_CONS; i++)
        pthread_create(&cons[i], NULL, consumidor, NULL);
    for (int i = 0; i < N_REPR; i++)
        pthread_create(&repr[i], NULL, atualizador, NULL);

    for (int i = 0; i < N_PROD; i++)
        pthread_join(prod[i], NULL);
    for (int i = 0; i < N_CONS; i++)
        pthread_join(cons[i], NULL);
    for (int i = 0; i < N_REPR; i++)
        pthread_join(repr[i], NULL);

    imprimir_estatisticas();

    return 0;
}