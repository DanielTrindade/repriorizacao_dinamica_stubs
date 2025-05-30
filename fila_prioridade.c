
#include "fila_prioridade.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 100

static Pedido fila[TAM_MAX];
static int tamanho = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void inicializar_fila() {
    tamanho = 0;
}

void inserir(Pedido p) {
}

Pedido remover() {
    return resultado;
}

int repriorizar(int id_dispositivo, int nova_prioridade) {
    return 0;
}
