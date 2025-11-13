#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMN 768 // O limite para a ALTURA (linhas)
#define MAX_WIDTH 1024 // O limite para a LARGURA (colunas)
#define MAX_HASH_CODE_LEN 200 //// Tamanho máximo permitido para o código hash

int ler_manual(char imagem[MAX_WIDTH][MAX_COLUMN], int *altura, int *largura) {
    printf("--- Modo de Entrada Manual ---\n");
    printf("Digite a altura da matriz (max %d): ", MAX_WIDTH);
    // Leitura da Altura
    if (scanf("%d", altura) != 1 || *altura <= 0 || *altura > MAX_WIDTH) {
        printf("Altura inválida ou fora do limite (1 a %d).\n", MAX_WIDTH);
        return 0;
    }

    printf("Digite a largura da matriz (max %d): ", MAX_COLUMN);
    // Leitura da Largura
    if (scanf("%d", largura) != 1 || *largura <= 0 || *largura > MAX_COLUMN) {
        printf("Largura inválida ou fora do limite (1 a %d).\n", MAX_COLUMN);
        return 0;
    }

    printf("Digite os pixels (0s e 1s) linha por linha, separados por espaco:\n");
    
    // Leitura dos Pixels
    for (int i = 0; i < *altura; i++) {
        printf("Linha %d (%d pixels): ", i + 1, *largura);
        for (int j = 0; j < *largura; j++) {
            int valor;
            if (scanf("%d", &valor) != 1) {
                printf("Erro na leitura do pixel na linha %d, coluna %d.\n", i + 1, j + 1);
                return 0;
            }
            if (valor != 0 && valor != 1) {
                printf("Pixel inválido na linha %d, coluna %d. Use apenas 0 ou 1.\n", i + 1, j + 1);
                return 0;
            }
            // Converte o inteiro lido para o caractere ('0' ou '1')
            imagem[i][j] = (valor == 0) ? '0' : '1';
        }
    }

    return 1;
}

// Lê um arquivo PBM (formato P1) e preenche a matriz 'imagem'
int ler_pbm(const char *nome_arquivo, char imagem[MAX_WIDTH][MAX_COLUMN], int *altura, int *largura) {
    // Abre o arquivo para leitura
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }
    // Lê a primeira linha do arquivo (deve ser "P1")
    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    // Verifica se o formato é válido
    if (strncmp(linha, "P1", 2) != 0) {
        printf("Formato inválido (esperado P1)\n");
        fclose(arquivo);
        return 0;
    }
    // Procura a linha que contém a largura e altura
    while (fgets(linha, sizeof(linha), arquivo)) {
        if  (linha[0] == '#') continue;
        if (sscanf(linha, "%d %d", largura, altura) == 2) break;
    }

    // Lê cada pixel da imagem PBM
    int valor;
    for (int i = 0; i < *altura; i++) {
        for (int j = 0; j < *largura; j++) {
            if (fscanf(arquivo, "%d", &valor) == 1)
                imagem[i][j] = (valor == 0) ? '0' : '1';
            else {
                printf("Erro na leitura dos pixels.\n");
                fclose(arquivo);
                return 0;
            }
        }
    }
    // Fecha o arquivo e retorna sucesso
    fclose(arquivo);
    return 1;
}

void getQuadrant(char matriz[][MAX_COLUMN], int beginL, int beginC, int height, int width, char quadrant[][MAX_COLUMN]) {
    //dividir os quadrantes em diferentes matrizes
    for (int line = 0; line < height; line++) {
        for (int column = 0; column < width; column++) {
            quadrant[line][column] = matriz[beginL + line][beginC + column];
        }
    }
}

void getHashCode(char matriz[][MAX_COLUMN], int height, int width, char hashCode[]){
    hashCode[0];

    //valida se é uma matriz inexistente
    if (height == 0 || width == 0) {
        strcpy(hashCode, ""); 
        return;
    }
    //percorre toda a matriz buscando se ela é branca
    int allWhite = 1;
    for (int counter = 0; counter < height && allWhite; counter++){
        for (int sub_counter = 0;sub_counter < width && allWhite;sub_counter++){
            if(matriz[counter][sub_counter] != '0'){
                allWhite = 0;
                break;
            }
        }
        if(!allWhite) break;
    }
    if(allWhite == 1){
        strcpy(hashCode, "B");
        return;
    }

    //percorre toda a matriz buscando se ela é preta
    int allBlack = 1;
    for (int counter = 0; counter < height && allBlack; counter++){
        for (int sub_counter = 0;sub_counter < width && allBlack;sub_counter++){
            if(matriz[counter][sub_counter] != '1'){
                allBlack = 0;
                break;
            }
        }
        if(!allBlack) break;
    }
    if(allBlack == 1){
        strcpy(hashCode, "P");
        return;
    }

    strcpy(hashCode, "X");
    // consigo os valores dos quais serao capazes de dividir a matriz em 4
    int half_height = (height + 1) / 2;
    int half_width = (width + 1) / 2;

    int altura_q1_q2 = (height + 1) / 2; // Altura superior (Q1, Q2)
    int altura_q3_q4 = height - altura_q1_q2; // Altura inferior (Q3, Q4)

    int largura_q1_q3 = (width + 1) / 2; // Largura esquerda (Q1, Q3)
    int largura_q2_q4 = width - largura_q1_q3; // Largura direita (Q2, Q4)

    // Quadrantes
    char q1[altura_q1_q2][largura_q1_q3]; //Sup-Esq
    char q2[altura_q1_q2][largura_q2_q4]; //Sup-Dir
    char q3[altura_q3_q4][largura_q1_q3]; //Inf-Esq
    char q4[altura_q3_q4][largura_q2_q4]; //Inf-Dir

    //first
    getQuadrant(matriz, 0, 0, half_height, half_width, q1);
    //second
    getQuadrant(matriz, half_height, 0, height - half_height, half_width, q2);
    //third
    getQuadrant(matriz, 0, half_width, half_height, width - half_width, q3);
    //fourth
    getQuadrant(matriz, half_height, half_width, height - half_height, width - half_width, q4);

// Hashes de cada quadrante
    char h1[MAX_HASH_CODE_LEN];
    char h2[MAX_HASH_CODE_LEN];
    char h3[MAX_HASH_CODE_LEN];
    char h4[MAX_HASH_CODE_LEN];

    getHashCode(q1, half_height, half_width, h1);
    getHashCode(q2, height - half_height, half_width, h2);
    getHashCode(q3, half_height, width - half_width, h3);
    getHashCode(q4, height - half_height, width - half_width, h4);

    strcat(hashCode, h1);
    strcat(hashCode, h3);
    strcat(hashCode, h2);
    strcat(hashCode, h4);
}

int main(int argc, char *argv[]) {
    char imagem[MAX_WIDTH][MAX_COLUMN];
    int altura, largura, sucesso;
    

    // Verifica se o usuário pediu ajuda
    if (argc == 1 || (argc == 2 && (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0))) {
        printf("Uso: %s [opções]\n", argv[0]);
        printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.\n\n");
        printf("Opções:\n");
        printf("  -?, --help     : mostra esta ajuda na tela.\n");
        printf("  -m, --manual   : ativa o modo de entrada manual (via teclado).\n");
        printf("  -f <arquivo>   : lê e codifica a imagem do arquivo PBM informado.\n");
        return 0;
    }

    //estrutura de controle para os modos de entrada

    // 1. MODO MANUAL (-m)
    if (argc == 2 && (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--manual") == 0)) {
        sucesso = ler_manual(imagem, &altura, &largura);
    }
    // 2. MODO ARQUIVO (-f <arquivo>)
    else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        sucesso = ler_pbm(argv[2], imagem, &altura, &largura);
    }
    // 3. ARGUMENTOS INVÁLIDOS
    else {
        printf("Uso inválido.\n");
        printf("Uso para arquivo: %s -f arquivo.pbm\n", argv[0]);
        printf("Uso para entrada manual: %s -m\n", argv[0]);
        return 1; // Sai do programa com erro
    }

    // Verifica se a leitura (de qualquer modo) foi bem-sucedida
    if (!sucesso) {
        printf("Falha ao ler a imagem.\n");
        return 1; // Sai do programa com erro
    }

    char hashCode[MAX_HASH_CODE_LEN] = "";
    getHashCode(imagem, altura, largura, hashCode);

    printf("\nCódigo gerado: %s\n", hashCode);
    return 0;
}