/* Incluye las librerias necesarias*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

static void gestion(int); /*Declaracion de la funcion de gestion de señales recibidas*/

int main(int argc, char** argv) {
    /* Declara las variables necesarias*/
    pid_t padre, hijo;

    if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");
    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");
    sleep(2);
    padre = getpid();
    if ((hijo = fork()) == 0) { /*Trabajo del hijo*/
        kill(padre,SIGUSR1); /*Envia señal al padre*/
        for (;;); /*Espera una señal del padre indefinidamente*/
    }
    else { /*Trabajo del padre*/
        /*** Completa el código del padre para enviar las señales SIGUSR2 y SIGTERM al hijo,
        y espera a que acabe con wait ***/
        kill(hijo,SIGUSR2);
        sleep(2);
        kill(hijo,SIGTERM);
        wait(NULL);
    }
}
    static void gestion(int numero_de_senhal) { /* Funcion de gestion de señales*/
        printf("Entramos al switch\n");
        switch (numero_de_senhal) {
            case SIGUSR1: /*Entra señal SIGUSR1*/
            printf("Señal tipo 1 recibida. Soy %d\n",getpid()); break;
            case SIGUSR2:
            printf("Señal tipo 2 recibida. Soy %d\n",getpid()); break;
        }
        /***Completa para el resto de las señales usadas***/
        return;
}
