/* Incluye las librerias necesarias*/
static void gestion(int); /*Declaracion de la funcion de gestion de señales recibidas*/
main() {
/* Declara las variables necesarias*/
if (signal(SIGUSR1, gestion) == SIG_ERR) printf(“Error al crear gestor 1\n”);
if (signal(SIGUSR2, gestion) == SIG_ERR) printf(“Error al crear gestor 2\n”);
padre=getpid();
if ((hijo = fork()) == 0) { /*Trabajo del hijo*/
kill(padre,SIGUSR1); /*Envia señal al padre*/
for (;;); /*Espera una señal del padre indefinidamente*/
}
else { /*Trabajo del padre*/
/*** Completa el código del padre para enviar las señales SIGUSR2 y SIGTERM al hijo,
y espera a que acabe con wait ***/
}
}
static void gestion(int numero_de_senhal) { /* Funcion de gestion de señales*/
switch (numero_de_senhal) {
case SIGUSR1: /*Entra señal SIGUSR1*/
printf(“Señal tipo 1 recibida. Soy %d\n”,getpid()); break;
/***Completa para el resto de las señales usadas***/
return; }
