#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// declaramos variables globales
static void gestion(int);
pid_t* H;
int input;

int main (int argc, char** argv) {

    // llamadas a la funcion gestion
    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");
    if (signal(SIGTERM, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");

    printf("Escoja el numero de hijos -> "); scanf(" %d",&input);
    input++; // esto es porque se almacena el pid del padre como H[0]

    // reservamos la memoria segun el input
    pid_t aux;
    H = (pid_t *) malloc (input * sizeof(pid_t));

    // la primera posicion al padre
    H[0] = getpid();

    // a partir del H[1] vamos añadiendo los hijos
    for (int i=1;i<input;i++) {
        aux = fork();
        if (aux){
            H[i] = aux;
        }
        if (!aux) {
            H[i] = getpid();
            printf("Soy H[%i] pid: %i\n",i,getpid());
            for(;;);
        }
    }

    // el padre espera al primer hijo, cuando H[1] muera, el programa finaliza
    if(H[0] == getpid()) waitpid(H[1],NULL,0);

    return 0;
}

static void gestion(int numero_de_senhal) {
    switch (numero_de_senhal) {

    // recorre el vector hasta encontrarse a si mismo y elimina al anterior
    case SIGUSR2:
        printf("Señal SIGUSR2 recibida\n");
        for (int i=2;i<input;i++) {
            if (H[i] == getpid()) {
                kill(H[i-1],SIGTERM);
                break;
            }
        }
        break;

    // antes de morir, el proceso anuncia su muerte
    case SIGTERM:
        printf("Soy %d y me muero\n",getpid());
        exit(EXIT_SUCCESS);
        break;
    }
}