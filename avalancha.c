/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>

#define NUMBITS 160   //Número de bits del texto en claro

// Calcular distancia de Hamming de dos strings de igual tamaño: número de 1s de su XOR a nivel de bit
int hammingDist(unsigned char *a, unsigned char *b){ //REVISAR

  int dist = 0;
  int i;
  for(i=0;i<strlen((char*)a);i++){
    if(a[i] ^ b[i])
      dist++;
  }
  return dist;

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
  unsigned char *h = (unsigned char*)malloc(20); //hash: 160b=20B. Ver si es unsigned char o qué
  unsigned char *x2 = (unsigned char*)malloc(NUMBITS/8);
  unsigned char *h2 = (unsigned char*)malloc(20); //hash: 160b=20B. Ver si es unsigned char o qué


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
    
    // Alterar 1 bit de x para obtener x2. 
    *x2 = *x ^ 1 << (i%NUMBITS);
    // Hash de x y de x2
    /*h = hash(x);
      h2 = hash(x2);
    */
    // Calcular distancia de Hamming y guardarla
    distancias[hammingDist(h, h2)]++;
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
  free(distancias);

  return 0;

}
