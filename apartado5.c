#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

static void gestion(int);
pid_t* H;
int input;

int main (int argc, char** argv) {

    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");
    if (signal(SIGTERM, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");

    printf("Escoja el numero de hijos -> "); scanf(" %d",&input);
    input++; // esto es porque luego almaceno el pid del padre como H[0]

    pid_t aux;
    H = (pid_t *) malloc (input * sizeof(pid_t));

    H[0] = getpid();

    for (int i=1;i<input;i++) {
        aux = fork();
        if (aux){
            H[i] = aux;
        }
        if (!aux) { printf("Soy H[%i] pid: %i\n",i,getpid()); for(;;); }
    }

    if(H[0] == getpid()) waitpid(H[1],NULL,0);

    return 0;
}

static void gestion(int numero_de_senhal) {
    switch (numero_de_senhal) {
    case SIGUSR2:
        printf("Señal SIGUSR2 recibida\n");
        for (int i=2;i<input;i++) {
            printf("%d\n",H[i]);
            if (H[i] == 0)
                kill(H[i-1],SIGTERM);
        }
        break;
    case SIGTERM:
        printf("Soy %d y me muero",getpid());
        exit(EXIT_SUCCESS);
        break;
    }
}