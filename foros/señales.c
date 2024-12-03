#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>

static void gestion(int numero_de_senhal);

int main(int argc, char**argv)
{
    pid_t pid, pid_padre=getpid();
    
    printf("El PID del padre es: %d\n",pid_padre);
    
    
    pid=fork();
    if(pid<0)
    {
        perror("Error en el fork\n");
        return(EXIT_FAILURE);
    }
    else if (pid==0) //Es el hijo
    {
        sleep(1);
        kill(pid_padre,SIGUSR1);
        printf("SIGUSR1 enviada | %d\n",getpid());
        sleep(1);
        kill(pid_padre,SIGUSR2);
        printf("SIGUSR2 enviada | %d\n",getpid());
        exit(0);
    }
    
    if(signal(SIGUSR1,gestion)==SIG_ERR) printf("Error asociando manejador a SIGUSR1\n");
    if(signal(SIGUSR2,gestion)==SIG_ERR) printf("Error asociando manejador a SIGUSR2\n");
    
    while(1);
    return(EXIT_SUCCESS);
    
}

static void gestion(int numero_de_senhal)
{
    switch(numero_de_senhal)
    {
        case SIGUSR1:
            printf("SIGUSR1 recibida | %d\n",getpid());
            
            double res=0;
            for(int i=1;i<1e8;i++)
            {
               res+=tan(sqrt(i));
            }
            printf("Terminada ejecución SIGUSR1 | %d | Resultado: %lf\n",getpid(), res);
            break;
        case SIGUSR2:
            printf("SIGUSR2 recibida | %d\n",getpid());
            sigset_t pending;
            sigpending(&pending);
            if(sigismember(&pending,SIGUSR1)) printf("SIGUSR1 está pendiente\n");
            sleep(5);
            printf("Terminada ejecución SIGUSR2 | %d \n",getpid());
            break;
    }
}
