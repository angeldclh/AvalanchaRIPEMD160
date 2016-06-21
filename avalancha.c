/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

#define NUMBITS 160   //Número de bits del texto en claro
#define HASHSIZE 20  //Número de bytes del hash (160 bits)

// Calcular distancia de Hamming de dos unsigned int: número de 1s de su XOR a nivel de bit
int hammingDist(unsigned int a, unsigned int b){ 
	unsigned int c = a ^ b;
	return __builtin_popcount(c);  //Función de gcc/llvm  
}

int main(int argc, char **argv){
	
	// Comprobar número argumentos
	if(argc > 2){
		fprintf(stderr, "Número incorrecto de argumentos.\n./avalancha [numIteraciones]\n");
		return 1;
	}

	// Hayar número iteraciones y bits del texto a cambiar en cada iteración
	int numIteraciones;
	if(argc == 1){
		numIteraciones = 160; //PONER ESTE VALOR BIEN, ESTÁ INVENTADO. TAMBIÉN EN EL PRINTF
		printf("No se especifican argumentos, se asumen 160 iteraciones (número de bits del hash).\n");
	}
	else{
		numIteraciones = atoi(argv[1]);
	}


	// Buffers donde guardar los bits aleatorios generados (VER LONGITUD) y sus hashes
	unsigned char *x = (unsigned char*)malloc(NUMBITS/8);
	unsigned char *h = (unsigned char*)malloc(HASHSIZE); //hash: 160b=20B. 
	unsigned char *x2 = (unsigned char*)malloc(NUMBITS/8);
	unsigned char *h2 = (unsigned char*)malloc(HASHSIZE); //hash: 160b=20B. 

	unsigned int *ix = (unsigned int*)malloc(NUMBITS/8);
	unsigned int *ix2 = (unsigned int*)malloc(NUMBITS/8);	
	
	unsigned int *ih = (unsigned int*)malloc(NUMBITS/8/*HASHSIZE*/);
	unsigned int *ih2 = (unsigned int*)malloc(NUMBITS/8/*HASHSIZE*/);	
	
	/*distancias[z] contiene las veces que la distancia de Hamming entre hashes ha sido igual a z. Al tener el hash 160 bits,
	  la distancia estará siempre en el intervalo [0,160]*/
	int *distancias = (int*) malloc(161*sizeof(int));

	int itsTotales = numIteraciones*NUMBITS;
	int i, totalDH=0;
	/* Cada iteración altera un bit distinto del texto en claro. Tantas como indique el parámetro * nº bits de texto
	   en claro (NUMBITS)*/
	for(i=0;i<itsTotales;i++){
		// Calcular aleatoriamente el número
		RAND_bytes(x, NUMBITS/8);

		memcpy(ix,x,NUMBITS/8);
		*ix2 = *ix^(1<<(i%NUMBITS));
		
		// Alterar 1 bit de x para obtener x2. 
		//		*x2 = *x ^= 1 << (i%NUMBITS);

		////////////////////////////////////////
		//memcpy(ih,x,NUMBITS/8);
		//memcpy(ih2,x2,NUMBITS/8);
		int d = hammingDist(*ix,*ix2);
		printf("Debería ser 1 y es %d\n",d);
		return 1;
		////////////////////////////////////////
		// Hash de x y de x2
		/*h = hash(x);
		  h2 = hash(x2);
		*/
		ih = (unsigned int *) &h;
		ih2 = (unsigned int *) &h2;
		
		// Calcular distancia de Hamming y guardarla
		distancias[hammingDist(*ih, *ih2)]++;
		totalDH += distancias[i];
    
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
