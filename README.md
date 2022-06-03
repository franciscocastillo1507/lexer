# Analizador léxico

Para poder correr el proyecto es necesario correr el script llamado lexico.sh de la siguiente manera:

**./lexico.sh [nombre del archivo]**

**Es necesario solo poner el nombre del archivo y que se encuentre en la misma carpeta, no se debe de poner la ruta del archivo**

En el cual obtiene o actualiza g++ para despues compilar el programa lexico.cpp el cual genera la salida en el archivo a.out y se correrá el proyecto.

*o en caso de tener problemas compilar de la siguiente manera: g++ lexico.cpp, correr la salida ./a.out [nombre del archivo]

## Descripción
En esta última entrega, deberás desarrollar, de forma individual, una herramienta un analizador sintáctico LL(1). El programa recibe como entrada un archivo de texto conteniendo la gramática y una serie de cadenas a analizar. La salida será la tabla de análisis LL(1) e indica cuáles cadenas han sido aceptadas o no. Este proceso se realiza siempre y cuando la gramática sea LL(1). En caso de recibir una gramática que no sea LL(1), el programa deberá terminar e indicar el error en consola.

## Entrada

La primera línea del archivo de entrada contiene dos números enteros, N y M, que representan el número de producciones y el número de cadenas a analizar, respectivamente. Las siguientes N líneas contienen las diferentes producción en el mismo formato usado para las entregas anteriores. A continuación, las siguiente M líneas contienen las cadenas analizar.

Ejemplo:
./lexico.sh test.txt

test.txt =
<br>
goal -> A <br>
A -> ( A )<br>
A -> two<br>
two -> a<br>
two -> b<br>
( ( a ) )<br>
( a ) )<br>
( ( ( ( ( b ) ) ) ) )<br>
( ( ( ( ( a b ) ) ) ) )<br>


## Salida
La tabla de análisis LL(1). Además, para cada una de las cadenas, deberá indicar si la cadena es aceptada o no. 

<img width="906" alt="Screenshot 2022-06-03 at 11 52 33" src="https://user-images.githubusercontent.com/54192394/171910262-03dd1419-0f2f-4212-8307-26dc88933b9b.png">



