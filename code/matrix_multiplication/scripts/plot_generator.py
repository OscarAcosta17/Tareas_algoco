import os
import matplotlib.pyplot as plt
from collections import defaultdict

RUTA_MEDICIONES = "../data/measurements/mediciones.txt"
RUTA_GRAFICOS = "../plots/"

def leer_datos():
    datos = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: defaultdict(list))))
    dim, tipo = 0, ""

    with open(RUTA_MEDICIONES, 'r') as f:
        for linea in f:
            linea = linea.strip()
            if not linea: continue
            
            partes = linea.split(": ")
            if len(partes) != 2: continue
            
            clave, valor = partes[0].strip(), partes[1].strip()

            if clave == "caso":
                p_caso = valor.split('_')
                dim, tipo = int(p_caso[0]), p_caso[1]
            else:
                algo, metrica = clave.split('_')
                val_num = float(valor.split(' ')[0]) if metrica == 'tiempo' else int(valor.split(' ')[0])
                datos[metrica][algo][tipo][dim].append(val_num)
                
    return datos

def promediar(datos_metrica):
    promedios = defaultdict(lambda: defaultdict(dict))
    for algo, tipos in datos_metrica.items():
        for tipo, dims in tipos.items():
            for dim, vals in dims.items():
                promedios[algo][tipo][dim] = sum(vals) / len(vals)
    return promedios

def crear_grafico(promedios, titulo, ylabel, nombre_archivo):
    plt.figure(figsize=(12, 7))
    
    dimensiones_set = set()
    for algo in promedios:
        for tipo in promedios[algo]:
            dimensiones_set.update(promedios[algo][tipo].keys())
    
    dims_sorted = sorted(list(dimensiones_set))

    estilos_tipo = {'densa': ('o', '-'), 'diagonal': ('s', '--'), 'dispersa': ('^', ':')}
    colores_algo = {'naive': 'blue', 'strassen': 'red'}

    for algo in ['naive', 'strassen']:
        for tipo in ['densa', 'diagonal', 'dispersa']:
            if tipo in promedios.get(algo, {}) and promedios[algo][tipo]:
                dict_dims = promedios[algo][tipo]
                x_vals = sorted(dict_dims.keys())
                y_vals = [dict_dims[x] for x in x_vals]
                
                marcador, linea = estilos_tipo[tipo]
                color = colores_algo[algo]
                
                plt.plot(x_vals, y_vals, marker=marcador, linestyle=linea, color=color, 
                         label=f'{algo.capitalize()} ({tipo.capitalize()})')

    plt.title(titulo, fontsize=14, fontweight='bold')
    plt.xlabel('Dimensión N (Escala Log2)', fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.xscale('log', base=2)
    plt.yscale('log')
    plt.xticks(dims_sorted, dims_sorted)
    plt.grid(True, which="both", ls="--", alpha=0.6)
    plt.legend(fontsize=10)
    
    ruta = os.path.join(RUTA_GRAFICOS, nombre_archivo)
    plt.savefig(ruta, bbox_inches='tight')
    plt.close()
    print(f"[*] Gráfico guardado: {ruta}")

def graficar():
    if not os.path.exists(RUTA_GRAFICOS):
        os.makedirs(RUTA_GRAFICOS)

    datos = leer_datos()
    promedios_tiempo = promediar(datos['tiempo'])
    promedios_memoria = promediar(datos['memoria'])

    crear_grafico(promedios_tiempo, 'Tiempo Promedio vs N', 'Tiempo (ms) [Log10]', 'tiempo_vs_n_matrices.png')
    crear_grafico(promedios_memoria, 'Consumo Peak de Memoria Promedio vs N', 'Memoria (Bytes) [Log10]', 'memoria_vs_n_matrices.png')

if __name__ == "__main__":
    graficar()