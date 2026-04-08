# Documentación

## Entrega

La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

## Multiplicación de matrices
Algoritmos: Clásico (Naive) y Strassen.

### Programa principal
El código principal está en "matrix_multiplication.cpp." Este programa lee las matrices desde la carpeta "data/matrix_input/", las multiplica y luego guarda los tiempos y el consumo de memoria en "data/measurements/mediciones.txt".

Para medir el tiempo usé la librería `<chrono>`. Para medir la memoria que pide cada algoritmo y evitar que el reciclaje de memoria de WSL ensuciara los datos, sobrecargué globalmente los operadores `new` y `delete`. Antes de cada ejecución los contadores se reinician a cero, así solo mido la memoria Heap auxiliar que pide el algoritmo en el momento (como las submatrices de Strassen) y no las matrices iniciales.

Para compilar y ejecutar todo, simplemente hay que correr: `make run` dentro de la carpeta que corresponde.

### Scripts
el script que hice genera los graficos, funciona leyendo el archivo `mediciones.txt` y agrupando los datos sacando promedios, los graficos se guardan en la carpeta plots como una imagen `.png`. Usé escalas logarítmicas (base 2 para el eje X, base 10 para el Y) para que las diferencias de rendimiento se noten bien. Requiere tener instalada la librería matplotlib

## Ordenamiento de arreglo unidimensional

Algoritmos: MergeSort, QuickSort, std::sort.

### Programa principal
Toda la lógica está en sorting.cpp. El programa lee los arreglos (dominios D1 y D7, aleatorios, ascendentes y descendentes) desde data/array_input/, los ordena con los tres algoritmos y guarda las métricas en el mismo `mediciones.txt`.

A diferencia de las matrices, acá calculé la memoria de forma algorítmica y estática para demostrar qué algoritmos son in-place. Simplemente tomé el tamaño base del contenedor original (capacity) y le sumé la memoria teórica auxiliar que usa cada algoritmo (como el arreglo temporal que requiere Merge Sort).

Nota importante: En este programa decidí no completar la ejecución de los casos de 10 millones de datos `10^7` en Quick Sort cuando los arreglos ya venían ordenados. Al caer en su peor caso de O(n^2), cada prueba demoraba más de 4 horas, y la tendencia ya estaba clara con los tamaños anteriores.

Para compilar y ejecutar todo, simplemente hay que correr: `make run` dentro de la carpeta que corresponde.:

### Scripts
el script que hice genera los graficos, funciona leyendo el archivo `mediciones.txt` y agrupando los datos sacando promedios, los graficos se guardan en la carpeta plots como una imagen `.png`.

### EXTRA
borre algunos casos de prueba que no use para sorting para no superar los 50mb que tiene de tope moodle para subir un archivo.