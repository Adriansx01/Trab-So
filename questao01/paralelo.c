// produto_escalar_par.c
// Versão paralela do cálculo de produto escalar usando pthreads
// 1. COMPILAR O PROGRAMA
//gcc paralelo.c -o paralelo -O2 -pthread

// 2. RODAR O PROGRAMA PARALELO

// ./paralelo 10000000 4
// Forma 2 — Deixando o programa perguntar
// ./paralelo

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Estrutura com os dados que cada thread precisa
typedef struct {
    double *vetorA;
    double *vetorB;
    long long inicio;     // índice inicial (inclusive)
    long long fim;        // índice final (exclusivo)
    double soma_parcial;  // resultado parcial calculado pela thread
} DadosThread;

// Retorna o tempo atual em segundos
double tempo_em_segundos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Função que cada thread executa: calcula uma parte do produto escalar
void *calcular_parcial(void *arg) {
    DadosThread *dados = (DadosThread *) arg;
    double soma = 0.0;

    for (long long i = dados->inicio; i < dados->fim; i++) {
        soma += dados->vetorA[i] * dados->vetorB[i];
    }

    dados->soma_parcial = soma;
    return NULL;
}

int main(int argc, char *argv[]) {
    long long tamanho;
    int num_threads;

    // Parâmetros podem vir da linha de comando:
    // argv[1] = tamanho do vetor, argv[2] = número de threads
    if (argc >= 3) {
        tamanho = atoll(argv[1]);
        num_threads = atoi(argv[2]);
    } else {
        printf("Digite o tamanho dos vetores: ");
        if (scanf("%lld", &tamanho) != 1 || tamanho <= 0) {
            fprintf(stderr, "Tamanho inválido.\n");
            return 1;
        }

        printf("Digite a quantidade de threads: ");
        if (scanf("%d", &num_threads) != 1 || num_threads <= 0) {
            fprintf(stderr, "Número de threads inválido.\n");
            return 1;
        }
    }

    if (tamanho <= 0) {
        fprintf(stderr, "Tamanho inválido.\n");
        return 1;
    }
    if (num_threads <= 0) {
        fprintf(stderr, "Número de threads inválido.\n");
        return 1;
    }

    // Evita criar mais threads do que elementos no vetor
    if (num_threads > tamanho) {
        printf("Aviso: quantidade de threads maior que o tamanho do vetor.\n");
        printf("Ajustando numero de threads para %lld.\n", tamanho);
        num_threads = (int) tamanho;
    }

    // Alocação dos vetores
    double *vetorA = (double *) malloc(tamanho * sizeof(double));
    double *vetorB = (double *) malloc(tamanho * sizeof(double));

    if (vetorA == NULL || vetorB == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vetores.\n");
        free(vetorA);
        free(vetorB);
        return 1;
    }

    // Mesma semente da versão sequencial, para facilitar comparação
    srand(42);
    for (long long i = 0; i < tamanho; i++) {
        vetorA[i] = (double)(rand() % 1000) / 10.0;
        vetorB[i] = (double)(rand() % 1000) / 10.0;
    }

    // Alocação dos vetores de threads e de estruturas de dados
    pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    DadosThread *dados = (DadosThread *) malloc(num_threads * sizeof(DadosThread));

    if (threads == NULL || dados == NULL) {
        fprintf(stderr, "Erro ao alocar memória para estruturas de threads.\n");
        free(vetorA);
        free(vetorB);
        free(threads);
        free(dados);
        return 1;
    }

    // Divide o trabalho de forma balanceada entre as threads
    long long base = tamanho / num_threads;
    long long sobra = tamanho % num_threads;
    long long pos_atual = 0;

    // Medição do tempo apenas da fase paralela
    double inicio = tempo_em_segundos();

    for (int t = 0; t < num_threads; t++) {
        long long bloco = base + (t < sobra ? 1 : 0);

        dados[t].vetorA = vetorA;
        dados[t].vetorB = vetorB;
        dados[t].inicio = pos_atual;
        dados[t].fim = pos_atual + bloco;
        dados[t].soma_parcial = 0.0;

        int rc = pthread_create(&threads[t], NULL, calcular_parcial, &dados[t]);
        if (rc != 0) {
            fprintf(stderr, "Erro ao criar thread %d (codigo %d).\n", t, rc);
            // Em caso de erro, interrompe a criação de novas threads
            num_threads = t;
            break;
        }

        pos_atual += bloco;
    }

    double produto_final = 0.0;

    // Aguarda todas as threads terminarem e soma os resultados parciais
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
        produto_final += dados[t].soma_parcial;
    }

    double fim = tempo_em_segundos();
    double tempo_gasto = fim - inicio;

    printf("=== VERSAO PARALELA (pthreads) ===\n");
    printf("Tamanho do vetor: %lld\n", tamanho);
    printf("Threads utilizadas: %d\n", num_threads);
    printf("Produto escalar (paralelo) = %.4f\n", produto_final);
    printf("Tempo de execucao (paralelo) = %.6f segundos\n", tempo_gasto);
    printf("Use este tempo junto com o da versao sequencial para calcular o speedup Sp = Ts / Tp.\n");

    // Liberação da memória
    free(vetorA);
    free(vetorB);
    free(threads);
    free(dados);

    return 0;
}

