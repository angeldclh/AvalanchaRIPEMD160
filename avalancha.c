#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>


int main(int argc, char **argv){

// Obtener argumentos
	if(argc != 1 || argc != 3){
		fprintf(stderr, "Número incorrecto de argumentos.\n./avalancha\n./avalancha numIteraciones bitsACambiar\n");
		exit(1);
	}

	int numIteraciones, bitsACambiar;
	if(argc == 1){
		numIteraciones = 50000; //PONER ESTE VALOR BIEN, ESTÁ INVENTADO
		bitsACambiar = 1;
	}
	else{
		numIteraciones = atoi(argv[1]);
		bitsACambiar = atoi(argv[2]);
	}



	int contadoresDH[65];

	int i;
	for(i=0;i<numIteraciones;i++){
		//bucle
	}
}
