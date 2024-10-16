#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int veces=0;    //Declaramos veces como variable global para que el manejador pueda acceder a ella

static void gestion(int);

int main(int argc, char **argv) 
{
    if (signal(SIGINT, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");    //Cambiamos el comportamiento de la señal de interrupcion para que al recibirla ejecute el manejador
    
    for(;;);    //Bucle infinito
    
    return(EXIT_SUCCESS);
}


static void gestion(int numero_de_senhal)   //Manejador
{
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGINT:
            char opcion;
            veces+=1;
            printf("Señal de interrupcion recibida.\n");
            
            if(veces>=3)    //Si se ha recibido la señal de interrupcion 3 veces o mas, preguntamos si el usuario quiere volver al comportamiento habitual
            {
                  printf("Quieres volver a comportamiento habitual? S/N\n");
                  scanf(" %c",&opcion);
                  
                  if(opcion=='S')
                  {
                        if (signal(SIGINT, SIG_DFL) == SIG_ERR) printf("Error al crear gestor 1\n");    //Si quisiera, la reseteamos a comportamiento DEFAULT
                  }
            }
            break;
      }
}
