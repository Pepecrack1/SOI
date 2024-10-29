#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

static void gestion(int);

void mostrarHora()
{
    struct timeval tiempo;
    struct tm* tiempoLocal;
    char string_tiempo[40];
    long milisegundos;
    
    gettimeofday(&tiempo, NULL);  //Obtenemos el tiempo
    tiempoLocal = localtime(&tiempo.tv_sec); //Obtenemos la hora local a partir de los segundos

    strftime(string_tiempo, sizeof(string_tiempo), " %Y-%m-%d %H:%M:%S", tiempoLocal); //Obtenemos un string con el tiempo formateado
    milisegundos = tiempo.tv_usec / 1000; //Pasamos los microsegundos a milisegundos
    
    printf("[%s.%03ld]\n", string_tiempo, milisegundos);   //Imprimimos el string anterior y le concatenamos los milisegundos
}

int main(int argc, char **argv) 
{   
    
    pid_t pid1;
    int status1;
    pid_t pid2;
    int status2;
    pid_t pid_padre=getpid();
    
    sigset_t set, pending;  //Declaramos sigsets para las señales a bloquear y las señales pendientes
    sigemptyset(&set);         //Creamos un set vacio, para las señales que vamos a bloquear
    sigaddset(&set, SIGUSR1); //Añadimos al set SIGUSR1
    
    struct sigaction sa1, sa2;  //Creamos estructuras sigaction para las señales
    
    //SIGRUSR1
    sa1.sa_handler = gestion;  //Asignamos manejador
    sa1.sa_flags = 0;
    sigemptyset(&sa1.sa_mask);  //Mask estará vacío, no queremos bloquear ninguna señal mientras se maneja esta
    sigaction(SIGUSR1, &sa1, NULL);
    
    //SIGUSR2
    sa2.sa_handler = gestion; //Asignamos manejador
    sa2.sa_flags = 0;
    sigemptyset(&sa2.sa_mask);  //Mask estará vacío, no queremos bloquear nada
    sigaction(SIGUSR2, &sa2, NULL);
    
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) printf("Error al crear gestor 1\n");    //Cambiamos el comportamiento de SIGINT para que al recibirla la ignore
    
    if (sigprocmask(SIG_BLOCK, &set, NULL) < 0) {  //Bloqueamos el set de señales antes de que el hijo1 envie SIGUSR1
        perror("Error al bloquear SIGUSR1\n");
        return (EXIT_FAILURE);
    }
    
    pid1=fork();
    if(pid1<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(pid1==0)  //Es el hijo 1------------------------
    {   
        kill(pid_padre,SIGUSR1);  //Envía SIGUSR1 al padre
        printf("Hijo1| Envía SIGUSR1 y pasa a dormir 20s|");
        mostrarHora();
        sleep(20);
        kill(pid_padre,SIGUSR2);  //Envía SIGUSR2 al padre
        printf("Hijo1| Envía SIGUSR2 y pasa a dormir 20s|");
        mostrarHora();
        sleep(20);
        printf("Hijo1| Termina|");
        mostrarHora();
        exit(0);
    }
    
    //Es el padre---------------------------------------
    pid2=fork();
    
    if(pid2<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(pid2==0)  //Es el hijo 2--------------------------------
    {
        sleep(5); //Espera 5 segundos
        kill(getppid(),SIGINT); //Envía SIGINT al padre, y este, en lugar de terminar, ignora la señal, ya que hemos modificado su comportamiento
        printf("Hijo2| Envía SIGINT y termina|");
        mostrarHora();
        return(EXIT_SUCCESS);
    }
    
    //Es el padre------------------------------------------------
    
    printf("Padre| Espera 30 segundos|");
    mostrarHora();
    sleep(30);  //Espera 30 segundos, para que le de tiempo a llegar a SIGUSR1 y despues SIGUSR2
    
    if(sigpending(&pending)<0)  //Obtenemos lista de señales pendientes
    {
        perror("Error al comprobar señales pendientes\n");
        return (EXIT_FAILURE);
    }
    
    if (sigismember(&pending, SIGUSR1)) { //Comprobamos si SIGUSR1 está en ese conjunto
        printf("Padre| SIGUSR1 está pendiente ");
        mostrarHora();
    } else {
        printf("Padre| SIGUSR1 no está pendiente ");
        mostrarHora();
    }
    
    if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0) {  //Desbloqueamos las señales del set (SIGUSR1) nada más despertarse el padre
        perror("Error al desbloquear SIGUSR1\n");
        return (EXIT_FAILURE);
    }
    
    printf("Padre| Ejecuta wait al hijo1|");
    mostrarHora();
    
    waitpid(pid1, &status1, 0);   //Esperamos al hijo1
    int exit_code = WEXITSTATUS(status1);
    printf("Padre| Salida del primer hijo: %d ", exit_code);  //La salida es el codigo de salida del primer hijo
    mostrarHora();
    
    return(EXIT_SUCCESS);
}


static void gestion(int numero_de_senhal)   //Manejador
{
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGUSR1:
            printf("Padre| SIGUSR1 recibida| ");
            mostrarHora();
            break;
        case SIGUSR2:
            printf("Padre| SIGUSR2 recibida| ");
            mostrarHora();
            break;
      }
}

//El padre recibe primero SIGUSR2, pues SIGUSR1 está bloqueada, y tarda algo en desbloquearse
