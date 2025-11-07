#include <stdio.h>
#include <string.h>

#define MAX_COLUMN 100

//REMOVER DEPOIS
//REMOVER DEPOIS
//REMOVER DEPOIS
//REMOVER DEPOIS
//REMOVER DEPOIS
void printMatriz(char matriz[][MAX_COLUMN], int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", matriz[i][j]); // imprime cada caractere
        }
        printf("\n"); // quebra de linha ao fim de cada linha da matriz
    }
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
    // calcula o tamanho da imagem em qtd de bits --- necessário colocar *10 para margem de segurança (pesquisado devido a erro)
    int totalBits = sizeof(matriz[0][0]) * height * width * 10;
    int positionHashCode = 0;

    //REMOVER DEPOIS
    //REMOVER DEPOIS
    //REMOVER DEPOIS
    //REMOVER DEPOIS
    //REMOVER DEPOIS
    printMatriz(matriz, height, width);

    //percorre toda a matriz buscando se ela é toda preta ou branca
    int allTheSame = 1;
    char baseColor = 'B';
    for (int counter = 0; counter < width && allTheSame; counter++)
    {
        for (int sub_counter = 0;sub_counter < height && allTheSame;sub_counter++){
            if(matriz[counter][sub_counter] != baseColor){
                //testa se é o primeiro elemento de todos, se for há tempo de mudar para outra cor
                if(counter == 0 && sub_counter == 0) {
                    baseColor = 'W';
                }
                else {
                    baseColor = 'W';
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
        int half_widht = (width + 1) / 2;
        
        // Quadrantes
        char q1[MAX_COLUMN][MAX_COLUMN];
        char q2[MAX_COLUMN][MAX_COLUMN];
        char q3[MAX_COLUMN][MAX_COLUMN];
        char q4[MAX_COLUMN][MAX_COLUMN];

        printf("---------\n");

        //first 
        getQuadrant(matriz, 0, 0, half_height, half_widht, q1);
        //second
        getQuadrant(matriz, half_height, 0, height - half_height, half_widht, q2);
        //third
        getQuadrant(matriz, 0, half_widht, half_height, width - half_widht, q3);
        //fourth
        getQuadrant(matriz, half_height, half_widht, height - half_height, width - half_widht, q4);

	// Hashes de cada quadrante
        char h1[totalBits];
        char h2[totalBits];
        char h3[totalBits];
        char h4[totalBits];

        getHashCode(q1, half_height, half_widht, h1);
        getHashCode(q2, height - half_height, half_widht, h2);
        getHashCode(q3, half_height, width - half_widht, h3);
        getHashCode(q4, height - half_height, width - half_widht, h4);

        strcat(hashCode, h1);
        strcat(hashCode, h2);
        strcat(hashCode, h3);
        strcat(hashCode, h4);
    }
}

int main(){
    /*char matriz[4][MAX_COLUMN] = {
        {'B', 'B', 'W', 'B', 'W'},
        {'B', 'B', 'W', 'W', 'B'},
        {'W', 'W', 'B', 'B', 'B'}
    };*/

    char matriz[4][MAX_COLUMN] = {
        {'W', 'W', 'B'},
        {'W', 'B', 'B'},
        {'W', 'W', 'B'} 
    };

    char hashCode[1000];
    getHashCode(matriz, 3, 3, hashCode);

    printf("Hash gerado: %s\n", hashCode);

    return 0;
}