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

void getHashCode(char matriz[][MAX_COLUMN], int beginL, int beginC, int height, int width, char hashCode[]){
    hashCode[0];

    //valida se é uma matriz inexistente
    if (height == 0 || width == 0) {
        strcpy(hashCode, ""); 
        return;
    }
    //percorre toda a matriz buscando se ela é branca
    int allWhite = 1;
    for (int counter = beginL; counter <  beginL + height && allWhite; counter++){
        for (int sub_counter = beginC;sub_counter < beginC + width && allWhite;sub_counter++){
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
    for (int counter = beginL; counter < beginL + height && allBlack; counter++){
        for (int sub_counter = beginC;sub_counter < beginC + width && allBlack;sub_counter++){
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
    // Calcula dimensões e posições dos 4 sub-quadrantes
    int h1 = (height + 1) / 2; // Altura Sup (Q1, Q3)
    int h2 = height - h1;      // Altura Inf (Q2, Q4)
    int w1 = (width + 1) / 2;  // Largura Esq (Q1, Q2)
    int w2 = width - w1;       // Largura Dir (Q3, Q4)

    // Hashes de cada quadrante (alocados na pilha, mas são pequenos)
    char h1_hash[MAX_HASH_CODE_LEN];
    char h2_hash[MAX_HASH_CODE_LEN];
    char h3_hash[MAX_HASH_CODE_LEN];
    char h4_hash[MAX_HASH_CODE_LEN];

    // A ordem de concatenação do seu código original era h1, h3, h2, h4
    getHashCode(matriz, beginL, beginC, h1, w1, h1_hash);
    getHashCode(matriz, beginL, beginC + w1, h1, w2, h3_hash);   
    getHashCode(matriz, beginL + h1, beginC, h2, w1, h2_hash);
    getHashCode(matriz, beginL + h1, beginC + w1, h2, w2, h4_hash);


    strcat(hashCode, h1_hash);
    strcat(hashCode, h3_hash); 
    strcat(hashCode, h2_hash); 
    strcat(hashCode, h4_hash);
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
    getHashCode(imagem, 0, 0, altura, largura, hashCode);

    printf("\nCódigo gerado: %s\n", hashCode);
    return 0;
}