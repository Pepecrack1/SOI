#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

static void gestion(int);

int main(int argc, char **argv) 
{
    pid_t pid1;
    pid_t pid2;
    
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) printf("Error al crear gestor 1\n");    //Cambiamos el comportamiento de SIGINT para que al recibirla la ignore
    if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");   //Con SIGUSR1/2 se ejecutará el manejador
    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");
    
    pid1=fork();
    if(pid1<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_SUCESS);
    }
    else if(pid1==0)  //Es el hijo 1
    {
        return(EXIT_SUCCESS);
    }
    
    pid2=fork();
    if(pid2<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_SUCESS);
    }
    else if(pid2==0)  //Es el hijo 2
    {
        
        return(EXIT_SUCCESS);
    }
    
    waitpid(pid1, &status1, 0);
    int exit_code = WEXITSTATUS(status1);
    printf("Salida del primer hijo: %d\n", exit_code);  //La salida es el codigo de salida del primer hijo
    
    return(EXIT_SUCCESS);
}


static void gestion(int numero_de_senhal)   //Manejador
{
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGINT:
            printf("Señal de interrupcion recibida.\n");
            break;
      }
}
