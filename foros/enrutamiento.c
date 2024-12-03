#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>



int main(int argc, char** argv) {
    typedef struct 
    {
        struct in_addr ip;
        int mascara;
        int interfaz;
    }iptabla;
    
    int sufijo,ip_copia, interfaz =-1, max=-1;
    char filename[50];
    char ip[INET_ADDRSTRLEN];
    struct in_addr ip_entrada;
    iptabla *tabla=(iptabla*)malloc(10*sizeof(iptabla));
    int i=0, mascara;
    FILE *file;
    

    if (argc!=3){ 
        printf("Tienes que introducir la IP y el puerto del servidor por linea de comandos");
        exit (EXIT_FAILURE);
    }
    else{
        strcpy(filename,argv[1]);
        if ( inet_pton (AF_INET , argv[2] , (void *) &ip_entrada ) <0) {
            fprintf ( stderr , "Formato de direccion incorrecto " ) ;
            exit ( EXIT_FAILURE ) ;
        }
    }

    file = fopen (filename,"r");
    if (file==NULL){
            printf("Error al leer el archivo de entrada");
            exit(EXIT_FAILURE);
    } 

    while(fscanf(file,"%[^,],%d \n", ip, &tabla[i].interfaz)==2){

        if ((sufijo =inet_net_pton(AF_INET, ip, (void *) &tabla[i].ip, sizeof(struct in_addr)))<0){
            perror("Formato de dirección incorrecto\n");
            return (EXIT_FAILURE);
        }
        
        mascara = 0xFFFFFFFF;
        ip_copia=ip_entrada.s_addr;
        if(sufijo==0) mascara=0x00000000;
        else mascara = mascara<<(32-sufijo);
        
        mascara = htonl(mascara);
        
        ip_copia = ip_copia & mascara;

        if (tabla[i].ip.s_addr == ip_copia){
            if(sufijo>max){
                interfaz = tabla[i].interfaz;
                max=sufijo;
            }
        }
        i++;

    }

    if (interfaz == -1){
        printf ("Error en la asignación de interfaz");
    }
    else{
        printf("La interfaz de la salida es la %d, con sufijo /%d\n", interfaz, max);
    }



    free(tabla);
    return (EXIT_SUCCESS);
}
