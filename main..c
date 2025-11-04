#include <stdio.h>
#include <string.h>

#define MAX_COLUMN 100

void getQuadrant(char matriz[][MAX_COLUMN], int beginL, int beginC, int height, int widht, char quadrant[][MAX_COLUMN]) {
    //dividir os quadrantes em diferentes matrizes
    for (int line = 0; line < height; line++) {
        for (int column = 0; column < widht; column++) {
            quadrant[line][column] = matriz[beginL + line][beginC + column];
        }
    }
}

void getHashCode(char matriz[][MAX_COLUMN], int height, int widht, char hashCode[]){
    // calcula o tamanho da imagem em qtd de bits
    int totalBits = sizeof(matriz[0][0]) * height * widht;
    int positionHashCode = 0;

    //percorre toda a matriz buscando se ela é toda preta ou branca
    int allTheSame = 1;
    char baseColor = 'B';
    for (int counter = 0; counter < widht && allTheSame; counter++)
    {
        for (int sub_counter = 0;sub_counter < height && allTheSame;counter++){
            if(matriz[counter][sub_counter] != baseColor){
                //testa se é o primeiro elemento de todos, se for há tempo de mudar para outra cor
                if(counter == 0 && sub_counter == 0) {
                    baseColor = 'W';
                }
                else {
                    allTheSame = 0;
                    hashCode[positionHashCode] = 'X';
                    positionHashCode++;
                    break;
                }
            }
        }
    }

    //valida se são todos da mesma cor, caso contrário é divido
    if(allTheSame == 1){
        hashCode[positionHashCode] = baseColor;
        positionHashCode++;
        return ("%s", hashCode);
    } 
    else{
        // consigo os valores dos quais serao capazes de dividir a matriz em 4
        int half_height = height / 2;
        int half_widht = widht / 2;

        //first 
        char *firstQuadrant[MAX_COLUMN] = getHashCode(getQuadrant(matriz, 0, 0, half_height, half_widht));
        //second
        char *secondQuadrant[MAX_COLUMN] = getHashCode(getQuadrant(matriz, half_height, 0, height - half_height, half_widht));
        //third
        char *thirdQuadrant[MAX_COLUMN] = getHashCode(getQuadrant(matriz, 0, half_widht, half_height, widht - half_widht));
        //fourth
        char *fourthQuadrant[MAX_COLUMN] = getHashCode(getQuadrant(matriz, half_height, half_widht, height - half_height, widht - half_widht));

        strcat(hashCode, firstQuadrant);
        strcat(hashCode, secondQuadrant);
        strcat(hashCode, thirdQuadrant);
        strcat(hashCode, fourthQuadrant);

        return hashCode;
    }
}

int main(){
    char matriz[4][MAX_COLUMN] = {
        {'B', 'B', 'W', 'W'},
        {'B', 'B', 'W', 'W'},
        {'W', 'W', 'B', 'B'},
        {'W', 'W', 'B', 'B'}
    };

    char hashCode[1000];
    getHashCode(matriz, 4, 4, hashCode);

    printf("Hash gerado: %s\n", hashCode);

    return 0;
}