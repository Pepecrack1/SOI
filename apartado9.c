#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

int primo = 2;
double raiz = 0;

int esPrimo (int num);
void mostrarValor (int primo, double raiz);

static void gestionarSennales (int sennal);

int main (int argc , char** argv)
{

    if (signal (SIGALRM, gestionarSennales) == SIG_ERR) perror ("Error al crear gestor SIGALRM\n");
    if (signal (SIGUSR1, gestionarSennales) == SIG_ERR) perror ("Error al crear gestor SIGUSR1\n");

    printf ("El programa empieza, el PID es %d\n", getpid());

    struct timeval t0,t1;
    gettimeofday (&t0,NULL);

    for (int num; ; num++)
    {
        if (esPrimo (num))
        {
            primo = num;        // hacemos la asignacion para asegurarnos de que primo es SIEMPRE un numero primo
            raiz = sqrt (primo);
        }
        gettimeofday (&t1,NULL);
        if (1. < t1.tv_sec - t0.tv_sec)
        {
            alarm (1);
            t0.tv_sec = t1.tv_sec;
        }

    }

}

int esPrimo (int num)
{
    for (int i = 2; i< num ; i++)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

void mostrarValor (int primo, double raiz)
{
    printf ("Ultimo primo : %d\tRaiz : %lf\n", primo, raiz);
}

static void gestionarSennales (int sennal)
{
    switch (sennal)
    {
        case SIGALRM:
            mostrarValor (primo, raiz);
            break;
        case SIGUSR1:
            printf("\nEl programa finaliza\n\t-> ");
            alarm (1);
            pause ();
            exit (0);
            break;
    }
}