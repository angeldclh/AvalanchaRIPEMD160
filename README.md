# AvalanchaRIPEMD160
Estudio del efecto avalancha en el algoritmo criptográfico RIPEMD-160

## Compilación
Es necesaria la librería OpenSSL. Se incluye un Makefile. Es posible que en el mismo haya que cambiar la ubicación de esta librería y de sus ficheros de cabecera todo depende de dónde tengamos la instalación de OpenSSL.

## Uso
El programa recibe, opcionalmente, un argumento. En él se indica un número que, multiplicado por la constante del programa *RANDOM_SIZE*, nos da el número total de datos aleatorios (de tamaño *RANDOM_SIZE*) a generar. 

## Uso con funciones hash diferentes a RIPEMD-160
Para medir el efecto avalancha de otras funciones hash, se debe modificar adecuadamente la constante *HASHSIZE* con el tamaño en bytes del hash en cuestión. Por ejemplo, para SHA-256 el valor de *HASHSIZE* debe ser 32. Además, en la línea 113 se debe cambiar EVP_ripemd160() por la función adecuada. Una lista con las funciones disponibles se puede encontrar [aquí](https://www.openssl.org/docs/manmaster/crypto/EVP_DigestInit.html).
