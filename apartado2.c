#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    int casa;
    printf("INTRODUCE ALGO, mi PID es %d : ",getpid()); //Devolvemos el pid del proceso para saber cuál es
    scanf("%d",&casa);  //Hacemos scanf, que esperará indefinidamente una entrada del usuario
    return(EXIT_SUCCESS);
}

//Desde otra terminal llamamos a kill -SIGKILL PID, y así terminamos el proceso
