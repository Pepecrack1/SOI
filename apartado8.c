#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


static void gestionHijos(int numero_de_senhal);
static void gestionPadre(int numero_de_senhal);
void imprimirMesa(int posM,int posJ,int posB,int turno, int puntosJ, int puntosM);

FILE*file;
pid_t padre;

void leerArchivo(int*posicionJ, int*posicionM, int*posicionB,int*turno,int*puntosJ,int*puntosM)
{    
    char linea[1024];
    FILE*file = fopen("file.txt","r");
    
    if (file == NULL)
    {
        perror("Error en la apertura del archivo\n");
        kill(padre,SIGTERM);
    }
    
    fseek(file,0,SEEK_SET);
    
    while (fgets(linea, 1024, file) != NULL); //Leemos líneas del archivo hasta llegar a la última, por si acaso ha pasado algo, pero normalmente solo habrá una línea de cada vez
    
    if(sscanf(linea, "Maquina: %d | Jugador: %d | Bola: %d | Turno: %d | PuntosJ: %d | PuntosM: %d", posicionM, posicionJ, posicionB, turno, puntosJ, puntosM)!=6)
    {
        perror("Error leyendo la línea cogida del archivo (sscanf)\n");
        kill(padre,SIGTERM);
    }
    
    fclose(file);
}

void imprimirArchivo(int posicionJ,int posicionM,int posicionB,int turno,int puntosJ,int puntosM)
{
    char linea[1024];
    
    FILE*file = fopen("file.txt","w");  //Al abrir el archivo en modo w, se resetea, y cada vez tendremos solo la última línea impresa, nada más
    if (file == NULL)
    {
        perror("Error en la apertura del archivo\n");
        kill(padre,SIGTERM);
    }
    
    fseek(file,0,SEEK_END); //Movemos puntero al final del archivo
    
    snprintf(linea, sizeof(linea),"Maquina: %d | Jugador: %d | Bola: %d | Turno: %d | PuntosJ: %d | PuntosM: %d\n", posicionM, posicionJ, posicionB, turno, puntosJ, puntosM);
    
    fputs(linea,file);
    fclose(file);
}

pid_t padre;


int main(int argc, char **argv) 
{
    padre=getpid();
    pid_t J;
    pid_t M;
    
    
    srand(time(NULL));  //Fijamos semilla para los numeros aleatorios

    J=fork();
    
    if(J<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(J==0)  //-------------------------------------JUGADOR
    {   
        if (signal(SIGUSR1, gestionHijos) == SIG_ERR) printf("Error al vincular hijo1\n");
        
        while(1);
        exit(0);
    }

    M=fork();

    if(M<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(M==0)  //-------------------------------------MAQUINA
    {   
        if (signal(SIGUSR2, gestionHijos) == SIG_ERR) printf("Error al vincular hijo2\n");
        
        while(1);
        exit(0);
    }
    
    if(signal(SIGUSR1, gestionPadre) == SIG_ERR) printf("Error al vincular padre\n");
    
    int turno;    //Aleatorio, 0 si saca la maquina y 1 si saca el jugador
    int puntosJ=0;
    int puntosM=0; 
    int posicionJ;
    int posicionB;
    int posicionM;
    
     
    while( (puntosJ<11&&puntosM<11) || abs(puntosJ-puntosM) < 2 )
    {
        int puntoTerminado=0;
        turno=rand() % 2;   //Escogemos de forma aleatoria quien tiene el turno
        
        posicionM=rand() % 10; //Obtenemos posicion aleatoria entre 0 y 9 para la máquina
        
        printf("La máquina empieza en la posición %d\n",posicionM);
        do
        {   
            printf("Escoge tu posición de inicio (0-9): ");
            scanf(" %d",&posicionJ);
        }while(posicionJ<0||posicionJ>9);
        
        if(turno) //Si saca el jugador
        {
            posicionB=posicionJ;  //Colocamos la bola justo delante para que saque
            printf("Comienza el punto, saca el jugador:\n");
        }
        else  //Si la máquina
        {
            posicionB=posicionM;  //Colocamos la bola justo delante para que saque
            printf("Comienza el punto, saca la máquina:\n") ;
        }
        
        imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
        
        imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);  //Imprimimos los datos iniciales en el archivo
        
        
        
        while(!puntoTerminado)
        {   
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM);
            if(turno)
            {
                if(abs(posicionJ-posicionB)<=3)
                {
            	    kill(J,SIGUSR1);
            	    pause();
            	    //sleep(1);
                }
                else
                {
                    puntosM+=1;
                    puntoTerminado=1;
                    printf("\nPunto para la máquina\n");
                    printf("===================================\n\n");
                    printf("NUEVO PUNTO\n");
                    printf("MARCADOR: | J %d | M %d |\n\n",puntosJ,puntosM);
                }
            }
            else
            {
                if(abs(posicionM-posicionB)<=3)
                {
            	    kill(M,SIGUSR2);
            	    pause();
            	    //sleep(1);
                }
                else
                {
                    puntosJ+=1;
                    puntoTerminado=1;
                    printf("\nPunto para el jugador\n\n");
                    printf("===================================\n\n");
                    printf("NUEVO PUNTO\n");
                    printf("MARCADOR: | J %d | M %d |\n\n",puntosJ,puntosM);
                }
            }
        }
    }
    printf("\nPARTIDA TERMINADA\n");
    if(puntosM < puntosJ)
    {
        printf("Gana el jugador: %d - %d , bien jugado!\n",puntosJ, puntosM);
    }
    else printf("Gana la máquina: %d - %d\n , mala suerte!\n",puntosJ, puntosM);
    
    return (EXIT_SUCCESS);
}


void imprimirMesa(int posJ,int posM,int posB,int turno, int puntosJ, int puntosM) {

    printf("     J %d - %d M \n\n",puntosJ,puntosM);  //Imprimimos marcador

    posM = posM*2;    //Como añadimos espacios por el medio las posiciones en las que vamos a imprimir los elementos dentro del string serán el doble
    posB = posB*2;
    posJ = posJ*2;

    char linea[19];
    linea[19]='\0';

    for (int i=0;i<19;i++) {    //Recorremos primera línea, en los pares ponemos "." y en los impares " "
        if (i%2==1) linea[i] = ' ';
        else linea[i] = '.';
    }

    linea[posM] = 'M';    //Colocamos a la máquina en la posición correspondiente
    printf("|%s|\n",linea);   //Imprimimos la línea con los bordes
    
    linea[posM] = '.';  //Volvemos a poner "." donde estaba el jugador para reutilizar la línea
    
    if (!turno) {   //Si le toca a la máquina (lado arriba), tras lanzar la bola esta se imprimirá en el lado de abajo (imprimimos siempre después de lanzar)
        linea[posB] = 'o';
        printf("|%s|\n",linea);
    } else printf("|%s|\n",linea);

    linea[posB] = '.';  //Reseteamos la línea otra vez
    
    printf("|%s|\n",linea);   //Imprimimos las líneas centrales y la red
    printf("#####################\n");
    printf("|%s|\n",linea);

    if (turno) {    //Si le toca al jugador la bola se lanzará hacia la máquina, y se dibuja en el lado de arriba
        linea[posB] = 'o';
        printf("|%s|\n",linea);
        linea[posB] = '.';
    } else printf("|%s|\n",linea);

    linea[posJ] = 'J';  //Colocamos al jugador y pintamos la zona inferior
    printf("|%s|\n",linea);

}

static void gestionPadre(int numero_de_senhal)
{
    /*switch(numero_de_senhal)
    {
        case SIGUSR1:
            printf("SEÑAL PADRE\n");
            break;
    }*/
}

void moverJugador(int*posicionJ)
{
    int posicionMover;
    do
    {   
        printf("Introduce cómo te quieres mover (desde -2 a +2): ");
        scanf(" %d", &posicionMover);
    }while(posicionMover<-2||posicionMover>2);
    
    *posicionJ+=posicionMover;   //Sumamos la posicion que se mueve a la del jugador
    if(*posicionJ<0) *posicionJ=0;  //Si se pasa de los límites, colocamos al jugador en el extremo correspondiente
    if(*posicionJ>9) *posicionJ=9;
}

void moverMaquina(int*posicionM)
{
    int posicionMover = rand()%3; //Se mueve numero aleatorio entre 0 y 2
    if(rand()%2) posicionMover *= (-1); //Multiplicamos por -1 o no, aleatorio
    *posicionM += posicionMover;
    if(*posicionM<0) *posicionM=0;  //Si se sleep(1);pasa de los límites, colfflush(file);ocamos al jugador en el extremo correspondiente
    if(*posicionM>9) *posicionM=9;
}

static void gestionHijos(int numero_de_senhal)  //Manejador, NO PUEDE HACER RETURN-----------------------------
{
    int posicionJ, posicionM, posicionB, turno, puntosJ, puntosM;
    
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGUSR1:
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM);
            
            printf("La bola viene hacia ti, te toca golpear:\n");
            do
            {   
                printf("Introduce la posicion a la que quieres lanzar: ");
                scanf(" %d", &posicionB);
                printf(" %d\n", posicionB);
            }while(posicionB<0||posicionB>9);
            
            moverJugador(&posicionJ);
            moverMaquina(&posicionM);
            
            turno = (turno+1)%2;
            imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            printf("El jugador se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionJ,posicionB);
            imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            
            
            kill(padre,SIGUSR1);
            break;
            
        case SIGUSR2:
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM);
            
            posicionB= rand() % 10; //Manda bola a posición aleatoria
            
            printf("Le toca golpear a la máquina, prepárate:\n");
            moverJugador(&posicionJ);
            moverMaquina(&posicionM);
            
            turno = (turno+1)%2;
            imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            printf("La maquina se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionM,posicionB);
            imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            
            
            kill(padre,SIGUSR1);
            break;
            
        case SIGTERM:
            exit(0);
            break;
      }
}
