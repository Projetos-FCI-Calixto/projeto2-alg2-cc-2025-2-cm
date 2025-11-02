#include <stdio.h>
#include <string.h>

#define MAX_COLUMN 100

char getQuadrant(char matriz[][MAX_COLUMN], int height, int widht){
    //dividir os quadrantes em diferentes matrizes
    char quadrant[MAX_COLUMN][MAX_COLUMN];

    for (int line = 0; line < height;line++){
        for (int column = 0; column < widht;column++){
            quadrant[line][column] = matriz[line][column];
        }
    }

    return quadrant;
}

char* getHashCode(char matriz[][MAX_COLUMN]){
    // calcula a altura da matriz
    int widht = sizeof(matriz[0]) / sizeof(matriz[0][0]);
    //calcula a largura da matriz
    int height = sizeof(matriz) / sizeof(matriz[0]);

    // calcula o tamanho da imagem em qtd de bits
    int totalBits = sizeof(matriz[0][0]) * height * widht;
    char hashCode[totalBits];

    //percorre toda a matriz buscando se ela é toda preta ou branca
    int allTheSame = 1;
    char baseColor = 'B';
    for (int counter = 0; counter < widht; counter++)
    {
        for (int sub_counter = 0;counter < height;counter++){
            if(matriz[counter][sub_counter] != baseColor){
                //testa se é o primeiro elemento de todos, se for há tempo de mudar para outra cor
                if(counter == 0 && sub_counter == 0) baseColor = 'W';
                else allTheSame = 0;
            }
            if(allTheSame == 0) break;
        }
        if(allTheSame == 0) break;
    }

    //valida se são todos da mesma cor, caso contrário é divido
    if(allTheSame == 1) return ("%c", baseColor);
    else{
        // consigo os valores dos quais serao capazes de dividir a matriz em 4
        int half_height = height / 2;
        int half_widht = widht / 2;

        //first 
        getHashCode(getQuadrant(matriz, half_height, half_widht));
        //second
        getHashCode(getQuadrant(matriz, height - half_height, half_widht));
        //thierd
        getHashCode(getQuadrant(matriz, half_height, widht - half_widht));
        //fourth
        getHashCode(getQuadrant(matriz, height - half_height, widht - half_widht));

        return hashCode;
    }
}

int main(){

    return 0;
}