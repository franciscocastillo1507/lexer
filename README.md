# Analizador léxico

Para poder correr el proyecto es necesario correr el script llamado lexico.sh de la siguiente manera:

**./lexico.sh [nombre del archivo]**

**Es necesario solo poner el nombre del archivo y que se encuentre en la misma carpeta, no se debe de poner la ruta del archivo**

En el cual obtiene o actualiza g++ para despues compilar el programa lexico.cpp el cual genera la salida en el archivo a.out y se correrá el proyecto.

*o en caso de tener problemas compilar de la siguiente manera: g++ lexico.cpp, correr la salida ./a.out [nombre del archivo]

## Descripción
Dada una gramática en EBNF, imprimir el listado de first y follows de la gramática asi como si es una gramática LL(1).

## Entrada

La lectura tanto de las producciones como el total de las mismas se hace desde el programa al obtener el archivo test, el usuario solo debe de poner el nombre del archivo con lo requisitos anteriores mencionados.

Ejemplo:
./lexico.sh input1.txt

input1.txt =

5 <br>
goal -> A<br>
A -> ( A )<br>
A -> two<br>
two -> a<br>
two -> b<br>

La primera línea contiene un número entero, N, que indica el número de producciones a analizar. El formato de las producciones es el siguiente:

id -> letra (letra | dígito)*

Restricciones a considerar:

El analizador léxico es sensible a minúsculas y mayúsculas. Es decir, que A es diferente de a.
“->” separa el no terminal del encabezado de la producción del cuerpo de la misma.
Asegúrate de colocar espacios entre las cosas que no deseas leer como un sólo símbolo. Por ejemplo ( A ) != (A).
Los caracteres válidos para un identificador son [ A..Z, a..z, _, - ]. El símbolo 
ϵ se representa con la cadena ' '.

## Salida
Imprimir el conjunto de firsts y follows, además de si es una gramática LL(1).

goal => FIRST = {(, a, b}, FOLLOW = {$}<br>
A => FIRST = {(, a, b}, FOLLOW = {$, )}<br>
two => FIRST = {a, b}, FOLLOW = {$, )}<br>
LL(1)? Yes<br>

