// produto_escalar_seq.c
// Versão sequencial do cálculo de produto escalar entre dois vetores
// 1. COMPILAR O PROGRAMA
// gcc sequencial.c -o sequencial -O2

// 2. RODAR O PROGRAMA SEQUENCIAL

// /sequencial 10000000
//  Forma 2 — Deixando o programa perguntar
// ./sequencial


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Retorna o tempo atual em segundos (com boa resolução)
// Usado para medir apenas o tempo do cálculo do produto escalar
double tempo_em_segundos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char *argv[]) {
    long long tamanho;

    // Tamanho do vetor pode vir da linha de comando ou ser lido do teclado
    if (argc >= 2) {
        tamanho = atoll(argv[1]);
    } else {
        printf("Digite o tamanho dos vetores: ");
        if (scanf("%lld", &tamanho) != 1 || tamanho <= 0) {
            fprintf(stderr, "Tamanho inválido.\n");
            return 1;
        }
    }

    if (tamanho <= 0) {
        fprintf(stderr, "Tamanho inválido.\n");
        return 1;
    }

    // Alocação dinâmica dos vetores
    double *vetorA = (double *) malloc(tamanho * sizeof(double));
    double *vetorB = (double *) malloc(tamanho * sizeof(double));

    if (vetorA == NULL || vetorB == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vetores.\n");
        free(vetorA);
        free(vetorB);
        return 1;
    }

    // Semente fixa para facilitar repetição dos experimentos
    srand(42);

    // Preenche os vetores com valores de ponto flutuante
    for (long long i = 0; i < tamanho; i++) {
        vetorA[i] = (double)(rand() % 1000) / 10.0;  // 0.0 a 99.9
        vetorB[i] = (double)(rand() % 1000) / 10.0;
    }

    // Medição do tempo apenas do laço de cálculo
    double inicio = tempo_em_segundos();

    double produto = 0.0;
    for (long long i = 0; i < tamanho; i++) {
        produto += vetorA[i] * vetorB[i];
    }

    double fim = tempo_em_segundos();
    double tempo_gasto = fim - inicio;

    printf("=== VERSAO SEQUENCIAL ===\n");
    printf("Tamanho do vetor: %lld\n", tamanho);
    printf("Produto escalar (sequencial) = %.4f\n", produto);
    printf("Tempo de execucao (sequencial) = %.6f segundos\n", tempo_gasto);

    // Liberação da memória
    free(vetorA);
    free(vetorB);

    return 0;
}

