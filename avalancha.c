#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>


int main(int argc, char **argv){

	// Comprobar número argumentos
	if(argc != 1 && argc != 3){
		fprintf(stderr, "Número incorrecto de argumentos.\n./avalancha\n./avalancha numIteraciones bitsACambiar\n");
		exit(1);
	}

	// Hayar número iteraciones y bits del texto y de la clave a cambiar en cada iteración
	int numIteraciones, bitsACambiar;
	if(argc == 1){
		numIteraciones = 50; //PONER ESTE VALOR BIEN, ESTÁ INVENTADO
		bitsACambiar = 1;
	}
	else{
		numIteraciones = atoi(argv[1]);
		bitsACambiar = atoi(argv[2]);
	}


	// Buffers donde guardar los bits aleatorios generados (64 bits = 8 bytes)
	unsigned char *x = (unsigned char*)malloc(8);
	unsigned char *x2 = (unsigned char*)malloc(8);
	unsigned char *y = (unsigned char*)malloc(8);
	unsigned char *y2 = (unsigned char*)malloc(8);
	unsigned char *y3 = (unsigned char*)malloc(8);

	// Buffers donde guardar las claves (256 bits = 32 bytes)
	unsigned char *k = (unsigned char*)malloc(32);
	unsigned char *k2 = (unsigned char*)malloc(32);

	// Arrays con contadores para las distancias de Hamming
	int contadoresDHTexto[65], contadoresDHClave[65];


	int i;
	for(i=0;i<numIteraciones;i++){
		// Calcular aleatoriamente la clave y el primer número
		RAND_bytes(k, 32);
		RAND_bytes(x, 8);
		
	}
}
