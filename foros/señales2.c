#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

static void gestion(int numero_de_senhal, siginfo_t *infor, void *mensaje;

int main() {
    pid_t pid, pid_padre=getpid();
    struct sigaction sa;

    sa.sa_sigaction = gestion;
    sa.sa_flags = SA_SIGINFO;
    
    printf("El PID del padre es: %d\n", getpid());
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
        sleep(1);
        kill(pid_padre,SIGUSR2);
        sleep(1);
        kill(pid_padre,SIGTERM);
        exit(0);
    }

    if (sigaction(SIGUSR1, &sa, NULL)<0)
    {
        perror("Error asociando sigaction a SIGUSR1\n");
        return(EXIT_FAILURE);
    }

    if (sigaction(SIGUSR2, &sa, NULL)<0)
    {
        perror("Error asociando sigaction a SIGUSR2\n");
        return(EXIT_FAILURE);
    }
    
    if (sigaction(SIGTERM, &sa, NULL)<0)
    {
        perror("Error asociando sigaction a SIGTERM\n");
        return(EXIT_FAILURE);
    }

    while (1);

    return (EXIT_SUCCESS);
}

static void gestion(int numero_de_senhal, siginfo_t *infor, void *mensaje) {
    printf("\nSe ha recibido señal: %s\n",strsignal(numero_de_senhal));

    psiginfo(infor, "Detalles");

    if (numero_de_senhal==SIGUSR1) {
        printf("Terminada ejecución SIGUSR1\n");
    } else if (numero_de_senhal==SIGUSR2) {
        printf("Terminada ejecución SIGUSR2\n");
    }
    else if (numero_de_senhal==SIGTERM) {
        printf("Terminada ejecución SIGTERM\n");
    }
}

//psiginfo ha mostrado el tipo de la señal, cómo se ha enviado, el PID del proceso emisor (8594) y el usuario efectivo (1000).
