#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMN 100
#define MAX 1000

int ler_pbm(const char *nome_arquivo, char imagem[MAX_COLUMN][MAX_COLUMN], int *altura, int *largura) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    char linha[256];

    // Lê o cabeçalho "P1"
    fgets(linha, sizeof(linha), arquivo);
    if (strncmp(linha, "P1", 2) != 0) {
        printf("Formato inválido (esperado P1)\n");
        fclose(arquivo);
        return 0;
    }

    // Ignora comentários e lê largura e altura
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '#') continue; // ignora comentários
        if (sscanf(linha, "%d %d", largura, altura) == 2) break;
    }

    // Lê os pixels (0 = branco, 1 = preto)
    int valor;
    for (int i = 0; i < *altura; i++) {
        for (int j = 0; j < *largura; j++) {
            if (fscanf(arquivo, "%d", &valor) == 1) {
                if (valor == 0)
                    imagem[i][j] = 'B'; // branco
                else
                    imagem[i][j] = 'P'; // preto
            } else {
                printf("Erro na leitura dos pixels.\n");
                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);
    return 1;
}

void getQuadrant(char matriz[][MAX_COLUMN], int beginL, int beginC, int height, int widht, char quadrant[][MAX_COLUMN]) {
    //dividir os quadrantes em diferentes matrizes
    for (int line = 0; line < height; line++) {
        for (int column = 0; column < widht; column++) {
            quadrant[line][column] = matriz[beginL + line][beginC + column];
        }
    }
}

void getHashCode(char matriz[][MAX_COLUMN], int height, int widht, char hashCode[]){
    // calcula o tamanho da imagem em qtd de bits --- necessário colocar *10 para margem de segurança (pesquisado devido a erro)
    int totalBits = sizeof(matriz[0][0]) * height * widht * 10;
    int positionHashCode = 0;

    //percorre toda a matriz buscando se ela é toda preta ou branca
    int allTheSame = 1;
    char baseColor = 'B';
    for (int counter = 0; counter < widht && allTheSame; counter++)
    {
        for (int sub_counter = 0;sub_counter < height && allTheSame;sub_counter++){
            if(matriz[counter][sub_counter] != baseColor){
                //testa se é o primeiro elemento de todos, se for há tempo de mudar para outra cor
                if(counter == 0 && sub_counter == 0) {
                    baseColor = 'W';
                }
                else {
                    allTheSame = 0;
                    break;
                }
            }
        }
    }

    //valida se são todos da mesma cor, caso contrário é divido
    if(allTheSame == 1){
        hashCode[positionHashCode] = baseColor;
        positionHashCode++;
        hashCode[positionHashCode] = '\0';
    } 
    else{
        hashCode[positionHashCode++] = 'X';
        hashCode[positionHashCode] = '\0';
        positionHashCode++;
        // consigo os valores dos quais serao capazes de dividir a matriz em 4
        int half_height = (height + 1) / 2; 
        int half_widht = (widht + 1) / 2;
        
        // Quadrantes
        char q1[MAX_COLUMN][MAX_COLUMN];
        char q2[MAX_COLUMN][MAX_COLUMN];
        char q3[MAX_COLUMN][MAX_COLUMN];
        char q4[MAX_COLUMN][MAX_COLUMN];

        //first 
        getQuadrant(matriz, 0, 0, half_height, half_widht, q1);
        //second
        getQuadrant(matriz, half_height, 0, height - half_height, half_widht, q2);
        //third
        getQuadrant(matriz, 0, half_widht, half_height, widht - half_widht, q3);
        //fourth
        getQuadrant(matriz, half_height, half_widht, height - half_height, widht - half_widht, q4);

	// Hashes de cada quadrante
        char h1[totalBits];
        char h2[totalBits];
        char h3[totalBits];
        char h4[totalBits];

        getHashCode(q1, half_height, half_widht, h1);
        getHashCode(q2, height - half_height, half_widht, h2);
        getHashCode(q3, half_height, widht - half_widht, h3);
        getHashCode(q4, height - half_height, widht - half_widht, h4);

        strcat(hashCode, h1);
        strcat(hashCode, h2);
        strcat(hashCode, h3);
        strcat(hashCode, h4);
    }
}

int main(int argc, char *argv[]) {
    char imagem[MAX_COLUMN][MAX_COLUMN];
    int altura, largura;

    // Verifica se foi passado o argumento -f e o nome do arquivo
    if (argc != 3 || strcmp(argv[1], "-f") != 0) {
        printf("Uso: %s -f arquivo.pbm\n", argv[0]);
        return 1;
    }

    // Lê o arquivo PBM (sua função)
    if (!ler_pbm(argv[2], imagem, &altura, &largura)) {
        printf("Falha ao ler o arquivo PBM.\n");
        return 1;
    }

    // Cria a string para armazenar o código
    char hashCode[10000] = "";

    // Chama a função do seu amigo
    getHashCode(imagem, altura, largura, hashCode);

    // Mostra o resultado
    printf("\nCódigo gerado: %s\n", hashCode);
    return 0;
}