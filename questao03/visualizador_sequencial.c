/* visualizador_sequencial.c
 *
 * Versão sequencial do visualizador (sem threads).
 * Mesma lógica de imagens simuladas (imagem_1.png, imagem_2.png, imagem_3.png),
 * porém todas as operações são realizadas de forma bloqueante e linear.
 *
 * Compilar:
 *   gcc visualizador_sequencial.c -o seq
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_IMAGENS 3
#define W 10
#define H 10

typedef struct {
    char nome[32];
    int w, h;
    int data[H][W];
    int loaded;
} ImagemSeq;

/* fontes (os mesmos valores do paralelo para comparação coerente) */
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
ImagemSeq imagens[NUM_IMAGENS];

void inicializar() {

    /* Preenche a matriz fonte2 com valores diferentes apenas para simular dados reais.
       Cada posição recebe um valor único baseado na posição i,j. */
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            fonte2[i][j] = 200 + i*W + j;
    /* Mesmo processo para fonte3, mas iniciando em 50. */
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            fonte3[i][j] = 50 + i*W + j;


    /* Inicializa a estrutura das três imagens simuladas.
       Aqui definimos o nome (imagem_1.png, imagem_2.png, ...),
       marcamos como "nao carregada" e zeramos a matriz de pixels. */
    for (int k = 0; k < NUM_IMAGENS; ++k) {
        snprintf(imagens[k].nome, sizeof(imagens[k].nome), "imagem_%d.png", k+1);
        imagens[k].w = W;
        imagens[k].h = H;
        imagens[k].loaded = 0;
        /* Zera os dados da imagem antes do carregamento. */
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                imagens[k].data[i][j] = 0;
    }
}

/* carrega uma imagem (simulação) */
void carregar_imagem(ImagemSeq *img) {
    printf("[LOAD] Carregando %s\n", img->nome);
    for (int passo = 0; passo <= 100; passo += 20) {
        printf("[LOAD][%s] %d%%\n", img->nome, passo);
        sleep(1);
    }
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
    printf("[LOAD] %s carregada\n", img->nome);
}


/* imprime conteúdo compacto da imagem */
void imprimir_compacto(ImagemSeq *img) {
    if (!img->loaded) { printf("%s: (nao carregada)\n", img->nome); return; }
    printf("%s (%dx%d):\n", img->nome, img->w, img->h);
    for (int i = 0; i < img->h; ++i) {
        for (int j = 0; j < img->w; ++j)
            printf("%3d ", img->data[i][j]);
        printf("\n");
    }
}

int seleciona_indice() {
    int id;
    /* Pede ao usuário o número da imagem que deseja visualizar.
       O valor deve estar entre 1 e NUM_IMAGENS. */
    printf("Escolha imagem (1-%d): ", NUM_IMAGENS);
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return -1; }
    
    /* Verifica se o número está dentro da faixa permitida. */
    if (id < 1 || id > NUM_IMAGENS) return -1;

    /* Retorna o índice correto (0, 1 ou 2). */
    return id - 1;
}

int main() {
    char comando[32];
    inicializar();

    printf("=== Visualizador (Sequencial) ===\n");
    printf("Imagens disponiveis: ");
    for (int i = 0; i < NUM_IMAGENS; ++i) printf("%s ", imagens[i].nome);
    printf("\n");

    while (1) {
        printf("\nComandos:\n");
        printf("  carregar   - carregar todas as imagens (sequencial)\n");
        printf("  mostrar    - mostrar uma imagem\n");
        printf("  status     - mostra quais estao carregadas\n");
        printf("  sair       - fechar\n");
        printf("Digite comando: ");
        if (scanf("%31s", comando) != 1) break;

        if (strcmp(comando, "carregar") == 0) {
            for (int i = 0; i < NUM_IMAGENS; ++i) {
                carregar_imagem(&imagens[i]);
            }
            printf("Todas as imagens foram carregadas (sequencialmente).\n");

        } else if (strcmp(comando, "mostrar") == 0) {
            int idx = seleciona_indice();
            if (idx < 0) { printf("Indice invalido\n"); continue; }
            imprimir_compacto(&imagens[idx]);

        } else if (strcmp(comando, "status") == 0) {
            for (int i = 0; i < NUM_IMAGENS; ++i)
                printf("  %s : %s\n", imagens[i].nome, imagens[i].loaded ? "carregada" : "nao carregada");

        } else if (strcmp(comando, "sair") == 0) {
            printf("Encerrando programa...\n");
            break;

        } else {
            printf("Comando invalido\n");
        }
    }
    return 0;
}
