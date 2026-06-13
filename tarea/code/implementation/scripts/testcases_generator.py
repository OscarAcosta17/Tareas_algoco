"""
Este codigo debe generar casos de prueba para los algoritmos implementados en C++, en la carpeta code/implementation/data/intputs/
"""
import os
import random
import string

OUTPUT_DIR = os.path.join(os.path.dirname(__file__), '..', 'data', 'inputs')
os.makedirs(OUTPUT_DIR, exist_ok=True)

def generar_nombre_anime(longitud=8):
    caracteres = string.ascii_lowercase + string.digits + '_'
    return ''.join(random.choice(caracteres) for _ in range(longitud))

def generar_caso_prueba(n, id_caso):
    nombre_archivo = f"testcases_{n}_{id_caso}.txt"
    ruta_archivo = os.path.join(OUTPUT_DIR, nombre_archivo)
    
    M = random.randint(1, 3000)
    E = random.randint(1, 500)
    
    animes = []
    total_capitulos = 0
    limite_capitulos_total = 700
    
    for _ in range(n):
        max_qi_posible = min(30, limite_capitulos_total - total_capitulos - (n - 1 - len(animes)))
        if max_qi_posible < 1:
            max_qi_posible = 1
            
        qi = random.randint(1, max_qi_posible)
        total_capitulos += qi
        
        nombre = generar_nombre_anime()
        bi = random.randint(0, 10**9)
        
        capitulos = []
        for _ in range(qi):
            ti = random.randint(1, 300)
            ci = random.randint(1, 100)
            vi = random.randint(1, 10**9)
            capitulos.append(f"{ti} {ci} {vi}")
            
        animes.append({
            "header": f"{nombre} {qi} {bi}",
            "capitulos": capitulos
        })

    with open(ruta_archivo, 'w') as f:
        f.write(f"{n} {M} {E}\n")
        for anime in animes:
            f.write(f"{anime['header']}\n")
            for cap in anime['capitulos']:
                f.write(f"{cap}\n")
                
    print(f"Generado: {ruta_archivo}")

def main():
    tamanos_pequenos = [3, 5, 8]
    tamanos_medianos = [20, 40, 80]
    tamanos_grandes = [100, 150, 200]
    
    todos_los_tamanos = tamanos_pequenos + tamanos_medianos + tamanos_grandes
    for n in todos_los_tamanos:
        for i in range(1, 4):
            generar_caso_prueba(n, i)

if __name__ == "__main__":
    main()