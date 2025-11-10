#include <stdio.h>
#include <string.h>

#define MAX_COLUMN 100
#define MAX_HASH_CODE_LEN 1000

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
            if(matriz[counter][sub_counter] != 'B'){
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
            if(matriz[counter][sub_counter] != 'P'){
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

    // Quadrantes
    char q1[MAX_COLUMN][MAX_COLUMN];
    char q2[MAX_COLUMN][MAX_COLUMN];
    char q3[MAX_COLUMN][MAX_COLUMN];
    char q4[MAX_COLUMN][MAX_COLUMN];

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

int main(){
    char matriz[MAX_COLUMN][MAX_COLUMN] = {
        {'P', 'P', 'B', 'P', 'B'},
        {'P', 'P', 'B', 'B', 'P'},
        {'B', 'B', 'P', 'P', 'P'}
    };

    char hashCode[MAX_HASH_CODE_LEN];
    getHashCode(matriz, 3, 5, hashCode);

    printf("Hash gerado: %s\n", hashCode);
    return 0;
}
