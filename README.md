# Analizador léxico


## Descripción
Dada una gramática en EBNF, imprimir el listado de terminales y no terminales de la gramática.

## Entrada

La lectura tanto de las producciones como el total de las mismas se hace desde la consola, dentro del programa corriendo, es necesario que el usuario escriba los datos.

Ejemplo:
5


goal -> A<br />
A -> ( A )<br />
A -> two <br />
two -> a <br />
two -> b<br />

*No se tiene la implementación con lectura de archivo por lo tanto solo se puede por entrada de usuario dentro de la consola (estipulado en la descripcion del proyecto)

La primera línea contiene un número entero, N, que indica el número de producciones a analizar. El formato de las producciones es el siguiente:

id -> letra (letra | dígito)*

Restricciones a considerar:

El analizador léxico es sensible a minúsculas y mayúsculas. Es decir, que A es diferente de a.
“->” separa el no terminal del encabezado de la producción del cuerpo de la misma.
Asegúrate de colocar espacios entre las cosas que no deseas leer como un sólo símbolo. Por ejemplo ( A ) != (A).
Los caracteres válidos para un identificador son [ A..Z, a..z, _, - ]. El símbolo 
ϵ se representa con la cadena ' '.

## Salida
Imprimir el conjunto de terminales y no terminales.

Terminales: a, b, (, )


No terminales: goal, A, two

