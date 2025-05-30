#include "fila_prioridade.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX 100

static Pedido fila[TAM_MAX];
static int tamanho = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Estatísticas básicas
static int total_insercoes = 0;
static int total_remocoes = 0;
static int total_repriorizacoes = 0;
static int repriorizacoes_sucesso = 0;
static time_t inicio_execucao;

// Funções auxiliares para o heap
static void subir(int indice) {
    while (indice > 0) {
        int pai = (indice - 1) / 2;
        if (fila[indice].prioridade <= fila[pai].prioridade) break;
        
        Pedido temp = fila[indice];
        fila[indice] = fila[pai];
        fila[pai] = temp;
        indice = pai;
    }
}

static void descer(int indice) {
    while (2 * indice + 1 < tamanho) {
        int filho_esq = 2 * indice + 1;
        int filho_dir = 2 * indice + 2;
        int maior = filho_esq;
        
        if (filho_dir < tamanho && fila[filho_dir].prioridade > fila[filho_esq].prioridade) {
            maior = filho_dir;
        }
        
        if (fila[indice].prioridade >= fila[maior].prioridade) break;
        
        Pedido temp = fila[indice];
        fila[indice] = fila[maior];
        fila[maior] = temp;
        indice = maior;
    }
}

static int encontrar_por_id(int id_dispositivo) {
    for (int i = 0; i < tamanho; i++) {
        if (fila[i].id_dispositivo == id_dispositivo) {
            return i;
        }
    }
    return -1;
}

void inicializar_fila() {
    pthread_mutex_lock(&mutex);
    tamanho = 0;
    total_insercoes = 0;
    total_remocoes = 0;
    total_repriorizacoes = 0;
    repriorizacoes_sucesso = 0;
    inicio_execucao = time(NULL);
    pthread_mutex_unlock(&mutex);
}

void inserir(Pedido p) {
    pthread_mutex_lock(&mutex);
    
    if (tamanho >= TAM_MAX) {
        printf("ERRO: Fila cheia!\n");
        pthread_mutex_unlock(&mutex);
        return;
    }
    
    fila[tamanho] = p;
    subir(tamanho);
    tamanho++;
    total_insercoes++;
    
    printf("Inserido: ID=%d, Prioridade=%d\n", p.id_dispositivo, p.prioridade);
    
    pthread_mutex_unlock(&mutex);
}

Pedido remover() {
    pthread_mutex_lock(&mutex);
    
    Pedido resultado = {0, 0, 0}; // Pedido vazio
    
    if (tamanho == 0) {
        printf("AVISO: Fila vazia!\n");
        pthread_mutex_unlock(&mutex);
        return resultado;
    }
    
    resultado = fila[0];
    fila[0] = fila[--tamanho];
    if (tamanho > 0) {
        descer(0);
    }
    total_remocoes++;
    
    printf("Removido: ID=%d, Prioridade=%d\n", resultado.id_dispositivo, resultado.prioridade);
    
    pthread_mutex_unlock(&mutex);
    return resultado;
}

int repriorizar(int id_dispositivo, int nova_prioridade) {
    pthread_mutex_lock(&mutex);
    
    int indice = encontrar_por_id(id_dispositivo);
    total_repriorizacoes++;
    
    if (indice == -1) {
        printf("Repriorização falhou: ID %d não encontrado\n", id_dispositivo);
        pthread_mutex_unlock(&mutex);
        return 0; // Falha
    }
    
    int prioridade_antiga = fila[indice].prioridade;
    fila[indice].prioridade = nova_prioridade;
    repriorizacoes_sucesso++;
    
    // Reorganiza o heap baseado na mudança de prioridade
    if (nova_prioridade > prioridade_antiga) {
        subir(indice);
    } else if (nova_prioridade < prioridade_antiga) {
        descer(indice);
    }
    
    printf("Repriorizado: ID=%d, Prioridade: %d -> %d\n", 
           id_dispositivo, prioridade_antiga, nova_prioridade);
    
    pthread_mutex_unlock(&mutex);
    return 1; // Sucesso
}

void imprimir_estatisticas() {
    pthread_mutex_lock(&mutex);
    time_t fim_execucao = time(NULL);
    int tempo_execucao = (int)(fim_execucao - inicio_execucao);
    
    printf("\n=== ESTATÍSTICAS ===\n");
    printf("Tempo de execução: %d segundos\n", tempo_execucao);
    printf("Total de inserções: %d\n", total_insercoes);
    printf("Total de remoções: %d\n", total_remocoes);
    printf("Total de repriorizações: %d\n", total_repriorizacoes);
    printf("Repriorizações bem-sucedidas: %d\n", repriorizacoes_sucesso);
    if (total_repriorizacoes > 0) {
        printf("Taxa de sucesso: %.1f%%\n", (100.0 * repriorizacoes_sucesso) / total_repriorizacoes);
    }
    printf("Tamanho final da fila: %d\n", tamanho);
    printf("==================\n");
    
    pthread_mutex_unlock(&mutex);
}