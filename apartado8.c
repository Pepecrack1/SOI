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
pid_t padre;

//Función para leer la última línea, con los datos más recientes, del archivo
void leerArchivo(int*posicionJ, int*posicionM, int*posicionB,int*turno,int*puntosJ,int*puntosM)
{    
    char linea[1024];
    FILE*file = fopen("file.txt","r");  //Abrimos el archivo en modo lectura
    
    if (file == NULL) //comprobamos errores
    {
        perror("Error en la apertura del archivo\n");
        kill(padre,SIGTERM);
    }
    
    fseek(file,0,SEEK_SET); //Movemos el puntero al principio del archivo, por seguridad
    
    while (fgets(linea, 1024, file) != NULL); //Leemos líneas del archivo hasta llegar a la última, por si acaso ha pasado algo, pero normalmente solo habrá una línea de cada vez
    
    if(sscanf(linea, "Maquina: %d | Jugador: %d | Bola: %d | Turno: %d | PuntosJ: %d | PuntosM: %d", posicionM, posicionJ, posicionB, turno, puntosJ, puntosM)!=6)  //Leemos la última línea y comprobamos errores
    {
        perror("Error leyendo la línea cogida del archivo (sscanf)\n");
        kill(padre,SIGTERM);  //Terminamos al padre su hay error
    }
    
    fclose(file); //Cerramos el archivo
}


//Función para imprimir una línea, con los datos de un instante, en el archivo
void imprimirArchivo(int posicionJ,int posicionM,int posicionB,int turno,int puntosJ,int puntosM)
{
    char linea[1024];
    
    FILE*file = fopen("file.txt","w");  //Al abrir el archivo en modo w, se resetea, y cada vez tendremos solo la última línea impresa, nada más
    if (file == NULL)
    {
        perror("Error en la apertura del archivo\n");
        kill(padre,SIGTERM);
    }
    
    fseek(file,0,SEEK_END); //Movemos puntero al final del archivo, por seguridad
    
    snprintf(linea, sizeof(linea),"Maquina: %d | Jugador: %d | Bola: %d | Turno: %d | PuntosJ: %d | PuntosM: %d\n", posicionM, posicionJ, posicionB, turno, puntosJ, puntosM);
    //Formateamos la línea que se va a imprimir en el archivo
    
    fputs(linea,file);  //La escribimos
    fflush(file);   //Nos aseguramos de que es escribe todo en el archivo
    fclose(file);   //Cerramos la conexión con el archivo
}


int main(int argc, char **argv) 
{
    padre=getpid();
    pid_t J;
    pid_t M;
    
    
    srand(time(NULL));  //Fijamos semilla para los numeros aleatorios

    J=fork(); //Primer fork, genera el hijo jugador
    
    if(J<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(J==0)  //-------------------------------------JUGADOR
    {   
        if (signal(SIGUSR1, gestionHijos) == SIG_ERR) printf("Error al vincular hijo1\n");  //Asignamos manejador a SIGUSR1 en el jugador
        
        while(1); //Entra en bucle infinito, interrumpido ocacionalmente por las señales que lleguen
        exit(0);
    }

    M=fork(); //Fork para generar el segundo hijo, la máquina

    if(M<0)
    {
        printf("ERROR en el fork\n");
        return(EXIT_FAILURE);
    }
    else if(M==0)  //-------------------------------------MAQUINA
    {   
        if (signal(SIGUSR2, gestionHijos) == SIG_ERR) printf("Error al vincular hijo2\n");  //Le asignamos manejador a SIGUSR2 (usamos el mismo que antes, pero con una señal distinta)
        
        while(1);
        exit(0);
    }
    
    if(signal(SIGUSR1, gestionPadre) == SIG_ERR) printf("Error al vincular padre\n"); //Usaremos SIGUSR1 también en el padre, pero en este caso la señal no hará nada, simplemente se envía por parte de los hijos para indicar al padre que debe actuar, terminando su espera
    
    int turno;    //Aleatorio al principio, 0 si golpea la maquina y 1 si es el jugador
    int puntosJ=0;
    int puntosM=0; 
    int posicionJ;
    int posicionB;
    int posicionM;
    
     
    while(puntosJ<2&&puntosM<2) //El partido es a 10 puntos, cuando alguno llegue a 10, se sale del bucle
    {
        int puntoTerminado=0; //Se usará esta variable para indicar si se ha terminado un punto o no
        turno=rand() % 2;   //Escogemos de forma aleatoria quien tiene el turno
        
        posicionM=rand() % 10; //Obtenemos posicion aleatoria entre 0 y 9 para la máquina, su posición inicial
        printf("La máquina empieza en la posición %d\n",posicionM);
        
        do  //Pedimos al jugador su posición inicial, y la restringimos al valor adecuado
        {   
            printf("Escoge tu posición de inicio (0-9): ");
            scanf(" %d",&posicionJ);
        }while(posicionJ<0||posicionJ>9);
        
        if(turno) //Si saca el jugador: turno==1
        {
            posicionB=posicionJ;  //Colocamos la bola justo delante para que saque
            printf("Comienza el punto, saca el jugador:\n");
        }
        else  //Si la máquina
        {
            posicionB=posicionM;  //Colocamos la bola justo delante para que saque
            printf("Comienza el punto, saca la máquina:\n") ;
        }
        
        imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);  //Imprimimos estado de la mesa al inicio del punto
        
        imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);  //Imprimimos los datos iniciales en el archivo
        
        
        
        while(!puntoTerminado)  //Mientras no acaba el punto
        {   
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM); //Leemos información del archivo
            if(turno) //Si es el turno del jugador
            {
                if(abs(posicionJ-posicionB)<=3) //Si el jugador puede llegar a la bola, se la manda SIGUSR1 para que actúe
                {
            	    kill(J,SIGUSR1);
            	    pause();
                }
                else  //Si no llega, se le suma un punto a la máquina, se termina el punto, y se muestra el marcador
                {
                    puntosM+=1;
                    puntoTerminado=1;
                    printf("\nPunto para la máquina\n");
                    printf("===================================\n\n");
                    printf("NUEVO PUNTO\n");
                    printf("MARCADOR: | J %d | M %d |\n\n",puntosJ,puntosM);
                }
            }
            else  //Si le toca a la máquina
            {
                if(abs(posicionM-posicionB)<=3)   //si llega a la bola, se manda SIGUSR2 para que actúe
                {
            	    kill(M,SIGUSR2);
            	    pause();
                }
                else    //Si no, pierde el punto, y se acaba
                {
                    puntosJ+=1;
                    puntoTerminado=1;
                    printf("\nPunto para el jugador\n\n");
                    printf("===================================\n\n");
                    printf("NUEVO PUNTO\n");
                    printf("MARCADOR: | J %d | M %d |\n\n",puntosJ,puntosM);
                }
            }
        //Ahora, si el punto no ha terminado, se volvería a entrar en el bucle, para seguir con los demás golpes
        }
    //Si ninguno de los 2 ha llegado a 10, se inicia un nuevo punto (se da otra vuelta al bucle)
    }
    
    //En cuanto uno llegue a 10, se termina la partida
    printf("\nPARTIDA TERMINADA\n");
    
    if(puntosM < puntosJ) //Imprimimos el resultado en función de quién gana
    {
        printf("Gana el jugador: %d - %d , bien jugado!\n",puntosJ, puntosM);
    }
    else printf("Gana la máquina: %d - %d , mala suerte!\n",puntosJ, puntosM);
    
    return (EXIT_SUCCESS);
}


//Función para imprimir el estado de la mesa en un momento dado
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

//Función de gestión para el padre, asociada a SIGUSR1, no hace nada, simplemente es para romper la espera
static void gestionPadre(int numero_de_senhal)  
{
}

//FUnción para mover al jugador
void moverJugador(int*posicionJ)
{
    int posicionMover;
    do  //Pedimos cómo se quiere mover
    {   
        printf("Introduce cómo te quieres mover (desde -2 a +2): ");
        scanf(" %d", &posicionMover);
    }while(posicionMover<-2||posicionMover>2);
    
    *posicionJ+=posicionMover;   //Sumamos la posicion que se mueve a la del jugador
    if(*posicionJ<0) *posicionJ=0;  //Si se pasa de los límites, colocamos al jugador en el extremo correspondiente
    if(*posicionJ>9) *posicionJ=9;
}

//Función para mover a la máquina
void moverMaquina(int*posicionM)
{
    int posicionMover = rand()%3; //Se mueve numero aleatorio entre 0 y 2
    if(rand()%2) posicionMover *= (-1); //Multiplicamos por -1 o no, aleatorio, para ver hacia qué lado va
    *posicionM += posicionMover;
    if(*posicionM<0) *posicionM=0;  //Si se sleep(1);pasa de los límites, colfflush(file);ocamos al jugador en el extremo correspondiente
    if(*posicionM>9) *posicionM=9;
}

//Función manejadora para los hijos
static void gestionHijos(int numero_de_senhal)
{
    int posicionJ, posicionM, posicionB, turno, puntosJ, puntosM;
    
    switch (numero_de_senhal)   //En funcion del numero de señal hariamos una cosa u otra
    {
        case SIGUSR1: //Para el hijo 1 (jugador)
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM); //Leemos última línea del archivo
            
            printf("Jugador, te toca golpear:\n");
            do    //Se le pide posición a la que va a lanzar la bola
            {   
                printf("Introduce la posicion a la que quieres lanzar: ");
                scanf(" %d", &posicionB);
                printf(" %d\n", posicionB);
            }while(posicionB<0||posicionB>9);
            
            moverJugador(&posicionJ);   //Movemos jugador y máquina
            moverMaquina(&posicionM);
            
            turno = (turno+1)%2;  //Obtenemos el siguiente turno
            imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);   //Imprimimos los datos en el archivo, y mostramos por pantalla el estado de la partida
            printf("El jugador se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionJ,posicionB);
            imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            
            
            kill(padre,SIGUSR1);  //Indicamos al padre que vuelva a trabajar
            break;
            
        case SIGUSR2:   //Para el hijo 2 (maquina)
        
            leerArchivo(&posicionJ,&posicionM,&posicionB,&turno,&puntosJ,&puntosM);   //leemos información del archivo
            
            posicionB= rand() % 10; //Manda bola a posición aleatoria de 0 a 9
            
            printf("Le toca golpear a la máquina, prepárate:\n");
            
            moverJugador(&posicionJ);   //Movemos al jugador y a la máquina
            moverMaquina(&posicionM);
            
            turno = (turno+1)%2;    //Pasamos turno
            imprimirArchivo(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);   //imprimimos información en el archivo
            printf("La maquina se mueve a la posicion %d y envía la bola hacia la posicion %d\n",posicionM,posicionB);    //Sacamos por pantalla el estado de la partida
            imprimirMesa(posicionJ,posicionM,posicionB,turno,puntosJ,puntosM);
            
            
            kill(padre,SIGUSR1);    //Enviamos SIGUSR1 al padre, para que continúe
            break;
            
      }
}
