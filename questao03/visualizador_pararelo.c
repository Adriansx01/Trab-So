/* visualizador_paralelo.c
 *
 * Visualizador de imagens (simulado) — versão paralela usando pthreads.
 * Três imagens internas (imagem_1.png, imagem_2.png, imagem_3.png),
 * cada uma representada por uma matriz 10x10 de inteiros.
 *
 * Operações:
 *  - carregar (cria várias threads para carregar imagens em paralelo)
 *  - mostrar  (imprime matriz reduzida no terminal)
 *  - sair
 *
 * Compilar:
 *   gcc visualizador_paralelo.c -o par -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_IMAGENS 3
#define W 10
#define H 10

typedef struct {
    char nome[32];
    int w, h;
    int data[H][W];    // matriz que representa a "imagem"
    int loaded;        // 0 = não carregada, 1 = carregada
    pthread_mutex_t m; // protege acesso/modificação da imagem
} Imagem;

/* imagens "fonte" (conteúdo que será copiado ao carregar) */
static int fonte1[H][W] = {
    {10,20,30,40,50,60,70,80,90,100},
    {11,21,31,41,51,61,71,81,91,101},
    {12,22,32,42,52,62,72,82,92,102},
    {13,23,33,43,53,63,73,83,93,103},
    {14,24,34,44,54,64,74,84,94,104},
    {15,25,35,45,55,65,75,85,95,105},
    {16,26,36,46,56,66,76,86,96,106},
    {17,27,37,47,57,67,77,87,97,107},
    {18,28,38,48,58,68,78,88,98,108},
    {19,29,39,49,59,69,79,89,99,109}
};
static int fonte2[H][W];
static int fonte3[H][W];

Imagem imagens[NUM_IMAGENS];

/* função auxiliar para imprimir uma versão compacta da imagem */
void imprimir_imagem_compacta(Imagem *img) {
    pthread_mutex_lock(&img->m);
    if (!img->loaded) {
        printf("  %s: (nao carregada)\n", img->nome);
        pthread_mutex_unlock(&img->m);
        return;
    }
    printf("  %s: %dx%d (conteudo parcial)\n", img->nome, img->w, img->h);
    // imprime apenas uma grade reduzida (cada valor)
    for (int i = 0; i < img->h; ++i) {
        for (int j = 0; j < img->w; ++j) {
            printf("%3d ", img->data[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&img->m);
}

/* Thread que "carrega" uma imagem: simula leitura com sleep e progresso */
void* thread_carregar(void* arg) {
    Imagem *img = (Imagem*) arg;

    printf("[LOAD] Thread iniciada para %s\n", img->nome);

    // Simula etapas de carregamento (5 etapas)
    for (int passo = 0; passo <= 100; passo += 20) {
        printf("[LOAD][%s] Progresso: %d%%\n", img->nome, passo);
        sleep(1); // simula I/O
    }

    // copia os dados da fonte correspondente (simula leitura)
    pthread_mutex_lock(&img->m);
    if (strcmp(img->nome, "imagem_1.png") == 0) {
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                img->data[i][j] = fonte1[i][j];
    } else if (strcmp(img->nome, "imagem_2.png") == 0) {
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                img->data[i][j] = fonte2[i][j];
    } else {
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                img->data[i][j] = fonte3[i][j];
    }
    img->loaded = 1;
    pthread_mutex_unlock(&img->m);

    printf("[LOAD] %s carregada com sucesso\n", img->nome);
    return NULL;
}




/* função que inicializa as imagens "vazias" e as fontes 2 e 3 */
void inicializar() {
    // fonte2: valores 200..209 etc (para diferenciar)
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            fonte2[i][j] = 200 + i*W + j;

    // fonte3: valores 50..59 etc
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            fonte3[i][j] = 50 + i*W + j;

    for (int k = 0; k < NUM_IMAGENS; ++k) {
        snprintf(imagens[k].nome, sizeof(imagens[k].nome), "imagem_%d.png", k+1);
        imagens[k].w = W;
        imagens[k].h = H;
        imagens[k].loaded = 0;
        pthread_mutex_init(&imagens[k].m, NULL);
        // preencher com zeros inicialmente
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                imagens[k].data[i][j] = 0;
    }
}

/* busca índice válido a partir do input do usuário */
int seleciona_indice() {
    int id;
    printf("Escolha imagem (1-%d): ", NUM_IMAGENS);
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n'); // limpa entrada
        return -1;
    }
    if (id < 1 || id > NUM_IMAGENS) return -1;
    return id - 1;
}

int main() {
    char comando[32];

    inicializar();

    printf("=== Visualizador (Paralelo) ===\n");
    printf("Imagens disponiveis: ");
    for (int i = 0; i < NUM_IMAGENS; ++i) printf("%s ", imagens[i].nome);
    printf("\n");

    while (1) {
        printf("\nComandos:\n");
        printf("  carregar   - carregar todas as imagens em paralelo\n");
        printf("  mostrar    - mostrar uma imagem (conteudo)\n");
        printf("  status     - mostra quais estao carregadas\n");
        printf("  sair       - fechar\n");
        printf("Digite comando: ");
        if (scanf("%31s", comando) != 1) break;

        if (strcmp(comando, "carregar") == 0) {
            pthread_t t[NUM_IMAGENS];
            for (int i = 0; i < NUM_IMAGENS; ++i) {
                // inicia thread de carregamento para cada imagem
                pthread_create(&t[i], NULL, thread_carregar, &imagens[i]);
            }
            // espera todas terminarem (sincroniza ponto de medição)
            for (int i = 0; i < NUM_IMAGENS; ++i)
                pthread_join(t[i], NULL);

            printf("Todas as imagens foram carregadas.\n");

        } else if (strcmp(comando, "mostrar") == 0) {
            int idx = seleciona_indice();
            if (idx < 0) { printf("Índice inválido\n"); continue; }
            imprimir_imagem_compacta(&imagens[idx]);

        }else if (strcmp(comando, "status") == 0) {
            for (int i = 0; i < NUM_IMAGENS; ++i) {
                pthread_mutex_lock(&imagens[i].m);
                printf("  %s : %s\n", imagens[i].nome, imagens[i].loaded ? "carregada" : "nao carregada");
                pthread_mutex_unlock(&imagens[i].m);
            }

        } else if (strcmp(comando, "sair") == 0) {
            printf("Encerrando programa...\n");
            break;

        } else {
            printf("Comando invalido\n");
        }
    }

    // cleanup básico
    for (int i = 0; i < NUM_IMAGENS; ++i) pthread_mutex_destroy(&imagens[i].m);
    return 0;
}
