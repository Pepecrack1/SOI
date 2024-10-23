#include <stdio.h>
#include <stdlib.h>

void imprimir(int posM,int posJ,int posB,int turno) {

    //printf("######## 2-1 ########\n\n");
    printf("     J 2 - 1 M \n\n");

    posM = posM*2;
    posB = posB*2;
    posJ = posJ*2;

    char linea[19];

    for (int i=0;i<19;i++) {
        if (i%2) linea[i] = ' ';
        else linea[i] = '.';
    }

    linea[posM] = 'M';
    printf("|%s|\n",linea);
    linea[posM] = '.';

    if (!turno) {
        linea[posB] = 'o';
        printf("|%s|\n",linea);
        linea[posB] = '.';
    } else printf("|%s|\n",linea);

    printf("|%s|\n",linea);
    printf("#####################\n");
    printf("|%s|\n",linea);

    if (turno) {
        linea[posB] = 'o';
        printf("|%s|\n",linea);
        linea[posB] = '.';
    } else printf("|%s|\n",linea);

    linea[posJ] = 'J';
    printf("|%s|\n",linea);

}

int main() {
    imprimir(2,5,3,1);
    return 0;
}
/*

LADO MAQUINA

|. ----- . . . . . .|
|. . . . . . . . . .|
|. . . . . . . . . .|
#####################
|. . . . . . . . . .|
|. . . o . . . . . .|
|. . . . ----- . . .|

LADO JUGADOR



*/