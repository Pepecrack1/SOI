#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

static void gestion(int numeroSenhal);

int main(int argc, char **argv) 
{
      pid_t pid;
      if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");   //Modificamos comportamiento de SIGUSR1
      
      pid=fork();
      if(pid<0)
      {
          perror("ERROR en el fork\n");
      }
      else if(pid==0) //Es el hijo
      {
          pause();        //El hijo entra en pausa indefinidamente
          
          printf("Hijo terminado\n");   //Si se imprime esta linea, sabremos que la recepcion de la señal ha hecho que el hijo salga del pause() y siga su ejecucion
          return(EXIT_SUCCESS);
      }
      else  //Es el padre
      {
          sleep(4);     //Espera 4 segundos y manda SIGUSR1 al hijo
          kill(pid,SIGUSR1);
          
          return(EXIT_SUCCESS);
      }
      
}

static void gestion(int numeroSenhal)   //El manejador
{
      switch(numeroSenhal)
      {
          case SIGUSR1:     //Cuando reciba SIGURR1
            printf("Soy libreeeeeee\n");
            break;
      }
}
