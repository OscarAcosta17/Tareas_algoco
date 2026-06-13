import os
import pandas as pd
import matplotlib.pyplot as plt

MEASUREMENTS_DIR = os.path.join(os.path.dirname(__file__), '..', 'data', 'measurements')
OUTPUTS_DIR = os.path.join(os.path.dirname(__file__), '..', 'data', 'outputs')
PLOTS_DIR = os.path.join(os.path.dirname(__file__), '..', 'data', 'plots')

os.makedirs(PLOTS_DIR, exist_ok=True)

archivos_medicion = {
    'Brute Force': 'measurements_brute_force.txt',
    'Dynamic Prog': 'measurements_dynamic_programming.txt',
    'Greedy 1': 'measurements_greedy1.txt',
    'Greedy 2': 'measurements_greedy2.txt'
}

def leer_y_procesar_mediciones():
    datos = []
    for algoritmo, filename in archivos_medicion.items():
        filepath = os.path.join(MEASUREMENTS_DIR, filename)
        if not os.path.exists(filepath): continue
            
        with open(filepath, 'r') as file:
            lineas = file.readlines()[1:]
            for linea in lineas:
                partes = linea.split()
                if len(partes) >= 3:
                    nombre_archivo = partes[0]
                    tiempo = float(partes[1])
                    memoria = float(partes[2])
                    n = int(nombre_archivo.split('_')[1])
                    
                    datos.append({
                        'Algoritmo': algoritmo,
                        'N': n,
                        'Tiempo (ms)': tiempo,
                        'Memoria (KB)': memoria,
                        'Archivo': nombre_archivo
                    })
    return pd.DataFrame(datos)

def plot_calidad_solucion(df):
    calidad = []
    casos = df['Archivo'].unique()
    
    for caso in casos:
        try:
            with open(os.path.join(OUTPUTS_DIR, f"dp_{caso}"), 'r') as f:
                optimo = float(f.read().strip())
            
            with open(os.path.join(OUTPUTS_DIR, f"g1_{caso}"), 'r') as f:
                g1_val = float(f.read().strip())
            with open(os.path.join(OUTPUTS_DIR, f"g2_{caso}"), 'r') as f:
                g2_val = float(f.read().strip())
                
            n = int(caso.split('_')[1])
            res_g1 = (g1_val/optimo)*100 if optimo > 0 else 0
            res_g2 = (g2_val/optimo)*100 if optimo > 0 else 0
            
            calidad.append({'N': n, 'Greedy 1 (%)': res_g1, 'Greedy 2 (%)': res_g2})
            
        except (FileNotFoundError, ValueError):
            continue

    if not calidad:
        print("No se pudieron procesar archivos de salida.")
        return

    df_calidad = pd.DataFrame(calidad).groupby('N').mean()
    
    plt.figure(figsize=(10, 6))
    df_calidad.plot(kind='bar', ax=plt.gca())
    
    plt.title('Calidad de Heurísticas Greedy vs Óptimo')
    plt.xlabel('Cantidad de Animes (N)')
    plt.ylabel('Satisfacción (% del óptimo)')
    plt.axhline(y=100, color='r', linestyle='--', label='Óptimo (100%)')
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, 'calidad_solucion.png'))
    print("Gráfico de calidad generado con éxito.")

def plot_tiempo(df):
    plt.figure(figsize=(10, 6))
    df_mean = df.groupby(['Algoritmo', 'N'])['Tiempo (ms)'].mean().unstack(level=0)
    df_mean.plot(marker='o', logy=True, ax=plt.gca())
    
    plt.title('Tiempo de Ejecución vs Cantidad de Animes (N)')
    plt.xlabel('Cantidad de Animes (N)')
    plt.ylabel('Tiempo Promedio (ms) [Escala Logarítmica]')
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend(title='Algoritmo')
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, 'tiempo_ejecucion.png'))
    print("Gráfico de tiempo generado.")

def plot_memoria(df):
    plt.figure(figsize=(10, 6))
    # Convertimos de KB a MB
    df['Memoria (MB)'] = df['Memoria (KB)'] / 1024
    df_mean = df.groupby(['Algoritmo', 'N'])['Memoria (MB)'].mean().unstack(level=0)
    
    df_mean.plot(marker='s', ax=plt.gca())
    
    plt.title('Uso de Memoria vs Cantidad de Animes (N)')
    plt.xlabel('Cantidad de Animes (N)')
    plt.ylabel('Memoria Promedio (MB)')
    plt.grid(True, ls="--", alpha=0.5)
    plt.legend(title='Algoritmo')
    plt.tight_layout()
    plt.savefig(os.path.join(PLOTS_DIR, 'uso_memoria.png'))
    print("Gráfico de memoria generado.")

if __name__ == "__main__":
    print("Iniciando generación de gráficos...")
    df_mediciones = leer_y_procesar_mediciones()
    if not df_mediciones.empty:
        plot_tiempo(df_mediciones)
        plot_memoria(df_mediciones)
        plot_calidad_solucion(df_mediciones)
        print("Todos los gráficos han sido guardados en data/plots/")
    else:
        print("No se encontraron datos en data/measurements/")