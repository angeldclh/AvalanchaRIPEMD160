/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <string.h>

// Calcular distancia de Hamming de dos strings de igual tamaño: número de 1s de su XOR a nivel de bit
int hammingDist(unsigned char *a, unsigned char *b){
	int dist = 0;
	int i;
	for(i=0;i<strlen(a);i++){
		if(a[i] ^ b[i])
			dist++;
	}
	return dist;
}

int main(int argc, char **argv){

	// Comprobar número argumentos
	if(argc != 1 && argc != 2){
		fprintf(stderr, "Número incorrecto de argumentos.\n./avalancha\n./avalancha numIteraciones\n");
		exit(1);
	}

	// Hayar número iteraciones y bits del texto y de la clave a cambiar en cada iteración
	int numIteraciones;
	if(argc == 1){
		numIteraciones = 50; //PONER ESTE VALOR BIEN, ESTÁ INVENTADO. TAMBIÉN EN EL PRINTF
		printf("No se especifican argumentos, se asumen 50 iteraciones.\n");
	}
	else{
		numIteraciones = atoi(argv[1]);
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
	int contDHC[65], contDHK[257];

	//int i, j, dhc, dhk;
	int i=5;
	//for(i=0;i<numIteraciones;i++){
		// Calcular aleatoriamente la clave y el primer número
		RAND_bytes(k, 32);
		RAND_bytes(x, 8);
		// Alterar 1 bit de k y x para obtener k2 y x2
		*k2 = *k ^ 1 << (i%256);
		*x2 = *x ^ 1 << (i%64);
		// Cifrar x y x2 con la clave k y x con la clave k2
		/*y = gost(x, k);
		y2 = gost(x2, k);
		y3 = gost(x, k2);*/
		// Calcular distancias de Hamming
		int dhc = hammingDist(x, x2);
		int dhk = hammingDist(k, k2);
		printf("dhc = %i; dhk = %i", dhc, dhk);
		return 0;
/*
		// Actualizar contadores
		for(j=0;j<65;j++){
			if (dhc==j) contDHC[j]++;
		}
		for(j=0;j<257;j++){
			if (dhk==j) contDHK[j]++;
		}
	//}

	// Imprimir contadores
	for(j=0;j<65;j++){
		printf("Distancia Hamming entre criptogramas alterando 1 bit del texto = %d: %d veces.\n", j, contDHC[j]);
	}
	for(j=0;j<257;j++){
		printf("Distancia Hamming entre criptogramas alterando 1 bit de la clave = %d: %d veces.\n", j, contDHK[j]);
	}

	// Liberar memoria asignada
	free(x);
	free(x2);
	free(y);
	free(y2);
	free(y3);
	free(k);
	free(k2);

	return 0;
	*/
}
