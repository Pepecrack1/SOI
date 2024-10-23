#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
    pid_t P=getpid();
    pid_t J;
    pid_t M;
    
    FILE*fp = fopen("file.txt","w+");
    if (file == NULL)
    {
        printf("Error en la apertura del archivo\n")
        return(EXIT_FAILURE);
    }
    
    srand(time(NULL));  //Fijamos semilla para los numeros aleatorios

    J=fork();
    
    if(J<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(J==0)  //-------------------------------------JUGADOR
    {   
        int posicionJ;
        kill(pid_padre,SIGUSR1);
        
        while(1);
        exit(0);
    }

    //############################
    if (J) M=fork();
    //############################

    if(M<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(M==0)  //-------------------------------------MAQUINA
    {   

        int turno;    //Aleatorio, 0 si saca la maquina y 1 si saca el jugador
        int posicionJ;
        int posicionM = rand() % 10; //Obtenemos posicion aleatoria entre 0 y 9
        
        
        while(puntosJ<10&&puntosM<10)
        {
            turno=rand() % 2;
            puntoTerminado=false;
            while(!puntoTerminado)
            {
                leerArchivo(&posicionJ,&posicionM,&posicionB);
                turno = (turno+1)%2;
                if(turno)
                {
                    if(abs(posicionM-posicionB)<=3)
                    {
                	    kill(J,SIGUSR2);
                    }
                    else
                    {
                        puntosJ+=1;
                        printf("Punto para el jugador\n");
                        printf("MARCADOR: | J %d | M %d |\n");
                    }
                }
                else
                {
                    if(abs(posicionJ-posicionB)<=3)
                    {
                	    kill(K,SIGUSR1);
                    }
                    else
                    {
                        puntosM+=1;
                        printf("Punto para la máquina\n");
                        printf("MARCADOR: | J %d | M %d |\n");
                    }
                }
            }    
        }
    }
    return 0;
}


void imprimir(int posM,int posJ,int posB,int turno) {

    //printf("######## 2-1 ########\n\n");
    printf("     J %d - 1 %d \n\n");

    posM = posM*2;
    posB = posB*2;
    posJ = posJ*2;

    char linea[19];

    for (int i=0;i<19;i++) {
        if (i%2) linea[i] = ' ';
        else linea[i] = '.';
    }

    linea[posM] = 'M';
    printf("|%s|\n",linea);
    linea[posM] = '.';

    if (!turno) {
        linea[posB] = 'o';
        printf("|%s|\n",linea);
        linea[posB] = '.';
    } else printf("|%s|\n",linea);

    printf("|%s|\n",linea);
    printf("#####################\n");
    printf("|%s|\n",linea);

    if (turno) {
        linea[posB] = 'o';
        printf("|%s|\n",linea);
        linea[posB] = '.';
    } else printf("|%s|\n",linea);

    linea[posJ] = 'J';
    printf("|%s|\n",linea);

}


static void gestion(int numero_de_senhal)   //Manejador
{
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGUSR1:
            int posicionJ, posicionM, posicionB;
            if(fscanf(file, "Maquina: %d | Jugador: %d | Bola %d\n", &posicionM, &posicionJ, &posicionB) == EOF) 
            {
                perror("ERROR leyendo del archivo\n");
            }
    
            printf("Introduce la posicion a la que quieres lanzar: ");
            scanf(" %d", &posicionB);
            printf("Introduce la posicion a la que te deseas mover: ");
            scanf(" %d", &posicionJ);
            
            printf("El jugador se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionM,posicionB);
            //Mostrar mesa
            break;
            
        case SIGUSR2:
            int posicionJ, posicionM, posicionB;
            if(fscanf(file, "Maquina: %d | Jugador: %d | Bola %d\n", &posicionM, &posicionJ, &posicionB) == EOF) 
            {                perror("ERROR leyendo del archivo\n");
            }
            posicionB= rand() % 10;
            posicionM += (rand()%3) * pow(-1, rand()%2);
            
            printf("La maquina se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionM,posicionB);
            //Mostrar mesa
            break;
            
        case SIGTERM:
            exit(0);
            break;
      }
