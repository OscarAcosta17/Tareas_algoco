# Documentación

## Implementación
La tarea se desarrolló utilizando `C++` (estándar C++17) para el núcleo algorítmico y `Python 3` para la generación de datos y análisis visual, utlizando entornos WSL/Linux en windows

Para compilar y ejecutar el proyecto, hay que entrar en la carpeta `code/implementation` y poner el comando `make`.

### Programa principal
El principal es `general.cpp` en este archivose hizo como menú interactivo y controlador de flujo en donde poder elegir el algoritmo a ejecutar, esto se hizo para poder ejecutar un algoritmo a la vez y no tener que ejecutarlos en orden o todos juntos, las caracteristicas que tiene:

1. Lectura y Parseo: 
Extrae los parámetros globales ($M$, $E$) y procesa los detalles de cada anime desde la carpeta `data/inputs/`. Antes de iterar, ordena numéricamente los archivos (`testcases_...`  asegurando una ejecución progresiva desde los más livianos a los más pesados.

2. Selección Algorítmica: 
Es un menú para ejecutar uno de los cuatro algoritmos de la tarea:
   * Fuerza Bruta (`algorithms/brute-force.cpp`): Algoritmo de *Backtracking* que evalúa exhaustivamente todas las combinaciones posibles de prefijos. Modificado para evaluar dos restricciones simultáneas y controlar el bono de completación.

   * Greedy 1 (`algorithms/greedy1.cpp`): Heurística subóptima basada en el *Fractional Knapsack Problem*. Ordena los prefijos de forma decreciente según la rentabilidad calculada como la satisfacción Total dividida por la suma del costo combinado (Tiempo + Energía).

   * Greedy 2 (`algorithms/greedy2.cpp`): Segunda heurística subóptima. Similar a Greedy 1, pero el criterio de ordenamiento evalúa la rentabilidad dividiendo la satisfacción puramente por el tiempo consumido, utilizando el límite de energía solo como filtro de validación final.

   * Programación Dinámica (`algorithms/dynamic-programming.cpp`): Solución óptima pseudo-polinomial basada en *2D Knapsack Problem*. Utiliza una matriz de estados para maximizar el valor dentro de los topes $M$ y $E$, garantizando la elección de un único prefijo por anime.

3. Rendimiento y Salida: 
Mide el tiempo (usando `std::chrono`) y el uso máximo de memoria RAM (usando `<sys/resource.h>`). Guarda la satisfacción óptima en `data/outputs/` (con un prefijo indicando el algoritmo usado) y consolida el tiempo y memoria en archivos únicos dentro de `data/measurements/` para facilitar su posterior análisis.

### Scripts
Se hicieron los dos scripts en Python para automatizar el ciclo de vida de los datos, ubicados en la carpeta `scripts/`.

1. `testcases_generator.py`: 
Se encarga de generar los casos de prueba en la carpeta `data/inputs/`. Generando casos de prueba aleatorios apegados a las restricciones de dominio establecidas en la tarea, son 3 archvos distintos para cada una de las categorías de tamaño:
    * pequeños: con 3,5,8 animes
    * medianos: con 20,40,80 animes
    * grandes: 100,159, 200 animes

2. `plot_generator.py`: Actúa como la herramienta de análisis post-ejecución. Lee los archivos consolidados `.txt` de la carpeta `measurements/` y los datos de `outputs/`. Utilizando la librería `matplotlib`, genera y guarda tres gráficos en formato `.png` dentro de la carpeta `data/plots/`:

   * `tiempo_ejecucion.png`: Contraste temporal que evidencia la diferencia de tiempo de ejecución de los 4 algoritmos.

   * `uso_memoria.png`: Muestra el consumo espacial de los 4 algoritmos.

   * `calidad_solucion.png`: Gráfico de barras que compara porcentualmente qué tan cerca estuvieron las heurísticas Greedy 1 y Greedy 2 de la solución óptima obtenida.