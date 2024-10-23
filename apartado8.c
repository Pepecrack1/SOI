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
        
        exit(0);
    }
    
    if(M<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(M==0)  //-------------------------------------MAQUINA
    {   
        int posicionM = rand() % 10; //Obtenemos posicion aleatoria entre 0 y 9
        kill(pid_padre,SIGUSR1);
        
        exit(0);
    }
    
    int puntosM=0;
    int puntosJ=0;
    int saque;    //Aleatorio, 0 si saca la maquina y 1 si saca el jugador
    while(puntosJ<10&&puntosM<10)
    {
        saque=rand() % 2;
    }
}
