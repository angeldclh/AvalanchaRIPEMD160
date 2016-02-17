/* gcc avalancha.c -o avalancha -lcrypto */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rand.h>


int hammingDist(unsigned char *a, unsigned char *b){
  return 3;
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


  // Buffers donde guardar los bits aleatorios generados (VER LONGITUD) y sus hashes
  unsigned char *x = (unsigned char*)malloc(8);
  unsigned char *h = (unsigned char*)malloc(20); //hash: 160b=20B. Ver si es unsigned char o qué
  unsigned char *x2 = (unsigned char*)malloc(8);
  unsigned char *h2 = (unsigned char*)malloc(20); //hash: 160b=20B. Ver si es unsigned char o qué


  // Arrays con contadores para las distancias de Hamming
  int contDH[161];

  int i, j, dh;
  for(i=0;i<numIteraciones;i++){
    // Calcular aleatoriamente el primer número
    RAND_bytes(x, 8);
    // Alterar 1 bit de x para obtener x2
    *x2 = *x ^ 1 << (i%160);
    // Hash de x y de x2
    /*h = hash(x);
      h2 = hash(x2);
    */
    // Calcular distancias de Hamming
    dhc = hammingDist(h, h2);
    // Actualizar contadores
    for(j=0;j<161;j++){
      if (dh==j) contDH[j]++;
    }
    
  }

  // Imprimir contadores
  for(j=0;j<161;j++){
    printf("Distancia Hamming entre hashes alterando 1 bit del texto = %d: %d veces.\n", j, contDH[j]);
  }

  // Liberar memoria asignada
  free(x);
  free(x2);
  free(h);
  free(h2);

  return 0;
}
