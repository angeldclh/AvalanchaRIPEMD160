/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

#define RANDOMSIZE 200   //Tamaño en bytes del texto aleatorio a generar
#define HASHSIZE 20    //Tamaño en bytes del hash (RIPEMD-160, 160 b = 20 B)



/* Distancia de Hamming de dos elementos = número de 1s de su XOR a nivel de bit.
   __builtin_popcount(unsigned long x) calcula el número de bits a 1 en x. Las funciones usadas
   para calcular valores hash devuelven este como unsigned char *, por lo que su valor hay que
   convertirlo a unsigned long. El tamaño del hash será muy probablemente mayor que los 8 B que
   ocupa un unsigned long. Así, guardamos varios unsigned long consecutivos en cada argumento 
   y calculamos la distancia de Hamming entre cada "pareja". Sumando estas distancias tenemos la
   distancia de Hamming total entre los hashes*/ 
  
int hammingDist(unsigned long *a, unsigned long *b, int elems){
	int dist = 0;
	unsigned long c;
	int i;
	for(i=0; i<elems; i++){
		c = a[i] ^ b[i];
		dist +=  __builtin_popcountl(c);  //Función de gcc/llvm
	}
	return dist;
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

	
	/* Para  calcular distancias de Hamming, unsigned long. Suficientes números para que quepan todos los bytes del hash.
	   Ej: RIPEMD-160 son 20 B -> Hacen falta 3 unsigned long (8 B cada uno) y quedarían "vacías" 4 posiciones*/
	int nUL = HASHSIZE/sizeof(unsigned long);
	if (HASHSIZE%sizeof(unsigned long) != 0) nUL ++;
	
	unsigned long *lh, *lh2;
	if (( lh = (unsigned long *) calloc(nUL, sizeof(unsigned long))) == NULL){ //calloc para evitar basura en posiciones vacías
		fprintf(stderr, "Error en calloc.\n");
		free(x);
		free(h);
		free(x2);
		free(h2);
		return 1;
	}
	if (( lh2 = (unsigned long *) calloc(nUL, sizeof(unsigned long))) == NULL){
		fprintf(stderr, "Error en calloc.\n");
		free(x);
		free(h);
		free(x2);
		free(h2);
		free(lh);
		return 1;
	}

	
	/*distancias[z] contiene las veces que la distancia de Hamming entre hashes ha sido igual a z. Al tener el hash 160 bits,
	  la distancia estará siempre en el intervalo [0,160]*/
	unsigned long *distancias = (unsigned long*) calloc(HASHSIZE*8+1, sizeof(unsigned long));

	// Inicializar hash
	EVP_MD_CTX *mdctx = EVP_MD_CTX_create(); //reservar memoria
	const EVP_MD *md = /*EVP_sha512();*/EVP_ripemd160(); //Hash a emplear


	int numBits = RANDOMSIZE*8;
	int itsTotales = numRondas*numBits;
	int i, dist;
	unsigned long totalDH=0;
	/* Cada iteración altera un bit distinto del texto en claro. */
	for(i=0;i<itsTotales;i++){
		// Calcular aleatoriamente el número
		if (!RAND_bytes(x, RANDOMSIZE)){ //error
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;	
		}
		// Cambiar 1 bit para calcular ambos hashes
		*x2 = *x^(1<<(i%numBits));

		// Hash de x y de x2
		if (!EVP_DigestInit_ex(mdctx, md, NULL)){ 
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestUpdate(mdctx, x, RANDOMSIZE)){ //hashear x
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestFinal_ex(mdctx, h, NULL)){ //Escribir el hash de x en h
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}

		//Lo mismo con x2
		if (!EVP_DigestInit_ex(mdctx, md, NULL)){
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestUpdate(mdctx, x2, RANDOMSIZE)){ //hashear x2
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		if (!EVP_DigestFinal_ex(mdctx, h2, NULL)){ //Escribir el hash de x2 en h2
			free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 
			return 2;
		}
		
		// Distancia de Hamming entre hashes
		memcpy(lh,h,HASHSIZE);
		memcpy(lh2,h2,HASHSIZE);
	
		dist = hammingDist(lh,lh2, nUL); 
		distancias[dist]++;
		totalDH += dist;
    
	}

	

	      
	// Imprimir contadores
	printf("Distancias de Hamming entre hashes alterando 1 bit del texto:\n");
	for(i=0;i<161;i++){
		printf("%d: %lu veces.\n", i, distancias[i]);	
	}
	fflush(stdout);
	
	//Calcular distancia de Hamming media
	double dhmedia = ((double) totalDH / (double) itsTotales);
	printf("La distancia de Hamming media es %lf\n", dhmedia);


	// Liberar memoria asignada
	free(x); free(x2);free(h); free(h2); free(lh); free(lh2); free(distancias); EVP_MD_CTX_destroy(mdctx); 

	return 0;

}
