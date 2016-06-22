/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

#define RANDOMSIZE 200   //Tamaño en bytes de los números aleatorios a generar
#define HASHSIZE 20    //Tamaño en bytes del hash (RIPEMD-160, 160 b = 20 B)


	


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


	// Buffers donde guardar los bits aleatorios generados y sus hashes
	unsigned char *x, *h, *x2, *h2;
	unsigned int *ih, *ih2;
	if (( x = (unsigned char*)malloc(RANDOMSIZE)) == NULL){
		fprintf(stderr, "Error en malloc.\n");
		return 1;
	}
	if (( h = (unsigned char*)malloc(HASHSIZE)) == NULL){
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		return 1;
	}
	if (( x2 = (unsigned char*)malloc(RANDOMSIZE)) == NULL){
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		free(h);
		return 1;
	}
	if (( h2 = (unsigned char*)malloc(HASHSIZE)) == NULL){ 
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		free(h);
		free(x2);
		return 1;
	}
	// Para  calcular distancias de Hamming, unsigned int
	if (( ih = (unsigned int*)malloc(HASHSIZE)) == NULL){
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		free(h);
		free(x2);
		free(h2);
		return 1;
	}
	if (( ih2 = (unsigned int*)malloc(HASHSIZE)) == NULL){
		fprintf(stderr, "Error en malloc.\n");
		free(x);
		free(h);
		free(x2);
		free(h2);
		free(ih);
		return 1;
	}

	
	/*distancias[z] contiene las veces que la distancia de Hamming entre hashes ha sido igual a z. Al tener el hash 160 bits,
	  la distancia estará siempre en el intervalo [0,160]*/
	int *distancias = (int*) calloc(HASHSIZE*8+1, sizeof(int));

	// Inicializar hash
	EVP_MD_CTX *mdctx = EVP_MD_CTX_create(); //reservar memoria
	const EVP_MD *md = EVP_ripemd160(); //Hash a emplear


	int numBits = RANDOMSIZE*8;
	int itsTotales = numRondas*numBits;
	int i, dist, totalDH=0;
	/* Cada iteración altera un bit distinto del texto en claro. */
	for(i=0;i<itsTotales;i++){
		// Calcular aleatoriamente el número
		if (!RAND_bytes(x, RANDOMSIZE)){ //error
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;	
		}
		// Cambiar 1 bit para calcular ambos hashes
		*x2 = *x^(1<<(i%numBits));

		// Hash de x y de x2
		if (!EVP_DigestInit_ex(mdctx, md, NULL)){ 
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestUpdate(mdctx, x, RANDOMSIZE)){ //hashear x
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestFinal_ex(mdctx, h, NULL)){ //Escribir el hash de x en h
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}

		//Lo mismo con x2
		if (!EVP_DigestInit_ex(mdctx, md, NULL)){
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestUpdate(mdctx, x2, RANDOMSIZE)){ //hashear x2
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestFinal_ex(mdctx, h2, NULL)){ //Escribir el hash de x2 en h2
			free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		
		// Distancia de Hamming entre hashes
		memcpy(ih,h,HASHSIZE);
		memcpy(ih2,h2,HASHSIZE);

		dist = hammingDist(*ih, *ih2); 
		distancias[dist]++;
		totalDH += dist;
    
	}

	

	      
	// Imprimir contadores
	printf("Distancias de Hamming entre hashes alterando 1 bit del texto:\n");
	for(i=0;i<161;i++){
		printf("%d: %d veces.\n", i, distancias[i]);	
	}
	fflush(stdout);
	
	//Calcular distancia de Hamming media
	double dhmedia = ((double) totalDH / (double) itsTotales);
	printf("La distancia de Hamming media es %lf\n", dhmedia);


	// Liberar memoria asignada
	free(x); free(x2);free(h); free(h2); free(ih); free(ih2); free(distancias); EVP_MD_CTX_destroy(mdctx); 

	return 0;

}
