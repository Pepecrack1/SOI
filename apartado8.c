#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
    pid_t P=getpid();
    pid_t J;
    pid_t M;
    
    srand(time(NULL));  //Fijamos semilla para los numeros aleatorios

    J=fork();
    
    if(J<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(J==0)  //-------------------------------------JUGADOR
    {   
        int posicionJ;
        kill(pid_padre,SIGUSR1);
        
        while(1);
        exit(0);
    }

    //############################
    if (J) M=fork();
    //############################

    if(M<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(M==0)  //-------------------------------------MAQUINA
    {   
        int posicionM = rand() % 10; //Obtenemos posicion aleatoria entre 0 y 9
        kill(pid_padre,SIGUSR1);

        while(1) {
            pause();
            // instrucciones
        }

    }

    int puntosM=0;
    int puntosJ=0;


    int turno;    //Aleatorio, 0 si saca la maquina y 1 si saca el jugador
    while(puntosJ<10&&puntosM<10)
    {
        turno=rand() % 2;


//##############################################
        if (turno) {
            kill(J,SIGUSR1);
        }
        else {
            kill(M,SIGUSR2);
        }
//##############################################
    }

    return 0;
}


void imprimir(int posM,int posJ,int posB,int turno) {

    //printf("######## 2-1 ########\n\n");
    printf("     J %d - 1 %d \n\n");

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

/*

"#...M......#"
        o
"#----------#"


*/