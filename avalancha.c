/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

#define RANDOMSIZE 20   //Número de bytes de los números aleatorios a generar
#define HASHSIZE 20     //Número de bytes del hash (160 bits)

// Calcular distancia de Hamming de dos unsigned int: número de 1s de su XOR a nivel de bit
int hammingDist(unsigned int a, unsigned int b){ 
	unsigned int c = a ^ b;
	return __builtin_popcount(c);  //Función de gcc/llvm  
}

int main(int argc, char **argv){
	
	// Comprobar número argumentos
	if(argc > 2){
		fprintf(stderr, "Número incorrecto de argumentos.\n./avalancha [numRondas]\n");
		return 1;
	}

	// Hayar número rondas
	int numRondas;
	if(argc == 1){
		numRondas = 1; 
		printf("No se especifican argumentos, se asume 1 ronda.\n");
	}
	else{
		numRondas = atoi(argv[1]);
	}


	// Buffers donde guardar los bits aleatorios generados (VER LONGITUD) y sus hashes
	unsigned char *x;
	unsigned char *h;
	unsigned char *x2;
	unsigned char *h2;
	// Para cambiar 1 bit y para el popcount mejor unsigned int
	unsigned int *ix;
	unsigned int *ix2;
	unsigned int *ih;
	unsigned int *ih2;

	if(
	   ((x = (unsigned char*)malloc(RANDOMSIZE)) == NULL) ||
	   ((h = (unsigned char*)malloc(HASHSIZE)) == NULL) || //hash: 160b=20B. 
	   ((x2 = (unsigned char*)malloc(RANDOMSIZE)) == NULL) ||
	   ((h2 = (unsigned char*)malloc(HASHSIZE)) == NULL) || //hash: 160b=20B. 
	   ((ix = (unsigned int*)malloc(RANDOMSIZE)) == NULL) ||
	   ((ix2 = (unsigned int*)malloc(RANDOMSIZE)) == NULL) ||
	   ((ih = (unsigned int*)malloc(HASHSIZE)) == NULL) ||
	   ((ih2 = (unsigned int*)malloc(HASHSIZE)) == NULL)){
		
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		free(h);
		free(x2);
		free(h2);
		free(ix);
		free(ix2);
		free(ih);
		free(ih2);
	}
	
	/*distancias[z] contiene las veces que la distancia de Hamming entre hashes ha sido igual a z. Al tener el hash 160 bits,
	  la distancia estará siempre en el intervalo [0,160]*/
	int *distancias = (int*) malloc(161*sizeof(int));

	int itsTotales = numRondas*RANDOMSIZE*8;
	int i, dist, totalDH=0;
	/* Cada iteración altera un bit distinto del texto en claro. Tantas como indique el parámetro * nº bits de texto
	   en claro (NUMBITS)*/
	for(i=0;i<itsTotales;i++){
		// Calcular aleatoriamente el número
		RAND_bytes(x, RANDOMSIZE);
		// Copiarlo a un unsigned int y cambiar 1 bit para calcular ambos hashes
		memcpy(ix,x,RANDOMSIZE);
		*ix2 = *ix^(1<<(i%NUMBITS));
		memcpy(x2,ix2,RANDOMSIZE);



		// Hash de x y de x2
		/*h = hash(x);
		  h2 = hash(x2);
		*/
		
		// Calcular distancia de Hamming y guardarla
		dist = hammingDist(*ih, *ih2);
		distancias[dist]++;
		totalDH += dist;
    
	}

	//Calcular distancia de Hamming media
	double dhmedia = (double) totalDH / (double) itsTotales;
  
      
	// Imprimir contadores
	printf("Distancias de Hamming entre hashes alterando 1 bit del texto:\n");
	for(i=0;i<161;i++){
		printf("%d: %d veces.\n", i, distancias[i]);
	}
	printf("La distancia de Hamming media es %f\n", dhmedia);

	// Liberar memoria asignada
	free(x);
	free(x2);
	free(h);
	free(h2);
	free(ix);
	free(ix2);
	free(ih);
	free(ih2);
	free(distancias);

	return 0;

}
