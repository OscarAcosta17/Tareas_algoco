import os
import matplotlib.pyplot as plt

RUTA_MEDICIONES = "../data/measurements/mediciones.txt"
RUTA_GRAFICOS = "../plots/"

def leer_datos():
    datos = {}
    dim, tipo, dom = 0, "", ""
    
    with open(RUTA_MEDICIONES, 'r') as f:
        for linea in f:
            linea = linea.strip()
            if not linea or ": " not in linea: continue
            
            clave, valor = linea.split(": ")
            
            if clave == "caso":
                p = valor.split('_')
                dim, tipo, dom = int(p[0]), p[1], p[2]
            else:
                if clave.startswith("std_sort"):
                    algo, metrica = "std_sort", clave.split('_')[2]
                else:
                    p = clave.split('_')
                    algo, metrica = p[0], p[1]
                    
                val_num = float(valor.split(' ')[0]) if metrica == 'tiempo' else int(valor.split(' ')[0])
                
                # Inicialización segura de los diccionarios paso a paso
                datos.setdefault(metrica, {})
                datos[metrica].setdefault(dom, {})
                datos[metrica][dom].setdefault(algo, {})
                datos[metrica][dom][algo].setdefault(tipo, {})
                datos[metrica][dom][algo][tipo].setdefault(dim, [])
                
                datos[metrica][dom][algo][tipo][dim].append(val_num)
                
    return datos

def promediar(datos_metrica):
    promedios = {}
    for dom, algos in datos_metrica.items():
        promedios.setdefault(dom, {})
        for algo, tipos in algos.items():
            promedios[dom].setdefault(algo, {})
            for tipo, dims in tipos.items():
                promedios[dom][algo].setdefault(tipo, {})
                for dim, vals in dims.items():
                    promedios[dom][algo][tipo][dim] = sum(vals) / len(vals)
    return promedios

def crear_grafico(promedios_dom, titulo, ylabel, nombre_archivo):
    plt.figure(figsize=(12, 7))
    dims_set = set()
    
    for algo in promedios_dom:
        for tipo in promedios_dom[algo]:
            dims_set.update(promedios_dom[algo][tipo].keys())
    
    dims_sorted = sorted(list(dims_set))

    estilos = {'aleatorio': ('o', '-'), 'ascendente': ('s', '--'), 'descendente': ('^', ':')}
    colores = {'mergesort': 'blue', 'quicksort': 'red', 'std_sort': 'green'}

    for algo in ['mergesort', 'quicksort', 'std_sort']:
        for tipo in ['aleatorio', 'ascendente', 'descendente']:
            if algo in promedios_dom and tipo in promedios_dom[algo] and promedios_dom[algo][tipo]:
                d_dims = promedios_dom[algo][tipo]
                x_vals = sorted(d_dims.keys())
                y_vals = [d_dims[x] for x in x_vals]
                
                m, l = estilos[tipo]
                c = colores[algo]
                
                plt.plot(x_vals, y_vals, marker=m, linestyle=l, color=c, 
                         label=f'{algo} ({tipo})')

    plt.title(titulo, fontsize=14, fontweight='bold')
    plt.xlabel('Dimensión N (Escala Log10)', fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    
    plt.xscale('log', base=10)
    
    if 'Memoria' in titulo:
        plt.yscale('symlog', linthresh=10)
    else:
        plt.yscale('log')

    plt.xticks(dims_sorted, dims_sorted)
    plt.grid(True, which="both", ls="--", alpha=0.6)
    
    plt.legend(fontsize=9, bbox_to_anchor=(1.05, 1), loc='upper left')
    
    ruta = os.path.join(RUTA_GRAFICOS, nombre_archivo)
    plt.savefig(ruta, bbox_inches='tight')
    plt.close()
    print(f"[*] Gráfico guardado: {ruta}")

def graficar():
    if not os.path.exists(RUTA_GRAFICOS):
        os.makedirs(RUTA_GRAFICOS)

    datos = leer_datos()
    p_tiempo = promediar(datos['tiempo'])
    p_memoria = promediar(datos['memoria'])

    for dom in p_tiempo.keys():
        crear_grafico(p_tiempo[dom], f'Tiempo Promedio vs N (Dominio {dom})', 'Tiempo (ms) [Log]', f'tiempo_vs_n_{dom}.png')
        crear_grafico(p_memoria[dom], f'Memoria Peak Promedio vs N (Dominio {dom})', 'Memoria (Bytes) [SymLog]', f'memoria_vs_n_{dom}.png')

if __name__ == "__main__":
    graficar()