import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation
import numpy as np

class Visualizador:
    def __init__(self, archivo='simulacion.txt'):
        self.archivo = archivo
        self.caja_ancho = 100
        self.caja_alto = 100
        self.obstaculos = []
        self.datos = {}
        self.tiempos = []
        self.colisiones = []
        
    def leer_datos(self):
        """Lee el archivo de simulación"""
        print("Leyendo datos de simulación...")
        
        with open(self.archivo, 'r') as f:
            for linea in f:
                linea = linea.strip()
                
                # Ignorar comentarios y líneas vacías
                if linea.startswith('#') or not linea:
                    # Extraer información de la caja
                    if '# CAJA:' in linea:
                        partes = linea.split(':')[1].strip().split('x')
                        self.caja_ancho = float(partes[0])
                        self.caja_alto = float(partes[1])
                    # Extraer obstáculos
                    elif '# OBSTACULO:' in linea:
                        coords = linea.split(':')[1].strip().split(',')
                        x, y, lado = float(coords[0]), float(coords[1]), float(coords[2])
                        self.obstaculos.append((x, y, lado))
                    continue
                
                partes = linea.split(',')
                tiempo = float(partes[0])
                tipo = partes[1]
                
                if tipo == 'POSICION':
                    id_part = int(partes[2])
                    x, y = float(partes[3]), float(partes[4])
                    radio = float(partes[5])
                    masa = float(partes[6])
                    
                    if id_part not in self.datos:
                        self.datos[id_part] = {'tiempos': [], 'x': [], 'y': [], 
                                               'radio': [], 'masa': []}
                    
                    self.datos[id_part]['tiempos'].append(tiempo)
                    self.datos[id_part]['x'].append(x)
                    self.datos[id_part]['y'].append(y)
                    self.datos[id_part]['radio'].append(radio)
                    self.datos[id_part]['masa'].append(masa)
                    
                    if tiempo not in self.tiempos:
                        self.tiempos.append(tiempo)
                
                elif tipo in ['COLISION_PARED', 'COLISION_OBSTACULO', 'COLISION_PARTICULAS']:
                    self.colisiones.append((tiempo, tipo, partes[2:]))
        
        self.tiempos = sorted(list(set(self.tiempos)))
        print(f"Datos cargados: {len(self.datos)} partículas, {len(self.tiempos)} frames")
        print(f"Colisiones detectadas: {len(self.colisiones)}")
        
    def crear_animacion(self, guardar=False, nombre='simulacion.mp4'):
        """Crea una animación de la simulación"""
        fig, ax = plt.subplots(figsize=(10, 10))
        
        def init():
            ax.clear()
            ax.set_xlim(0, self.caja_ancho)
            ax.set_ylim(0, self.caja_alto)
            ax.set_aspect('equal')
            ax.set_xlabel('X (unidades)')
            ax.set_ylabel('Y (unidades)')
            ax.set_title('Simulación de Colisiones Múltiples')
            ax.grid(True, alpha=0.3)
            
            # Dibujar obstáculos
            for obs in self.obstaculos:
                x, y, lado = obs
                rect = patches.Rectangle((x - lado/2, y - lado/2), lado, lado,
                                        linewidth=2, edgecolor='red', 
                                        facecolor='lightcoral', alpha=0.5)
                ax.add_patch(rect)
            
            return []
        
        def animate(frame):
            ax.clear()
            init()
            
            tiempo_actual = self.tiempos[frame]
            ax.set_title(f'Simulación de Colisiones - Tiempo: {tiempo_actual:.2f}s')
            
            # Dibujar partículas activas en este frame
            particulas_dibujadas = []
            for id_part, datos in self.datos.items():
                # Encontrar el índice más cercano a este tiempo
                idx = None
                for i, t in enumerate(datos['tiempos']):
                    if abs(t - tiempo_actual) < 0.001:
                        idx = i
                        break
                
                if idx is not None:
                    x = datos['x'][idx]
                    y = datos['y'][idx]
                    radio = datos['radio'][idx]
                    masa = datos['masa'][idx]
                    
                    # Color basado en la masa
                    color = plt.cm.viridis(masa / 10.0)
                    
                    circle = patches.Circle((x, y), radio, 
                                           linewidth=2, edgecolor='black',
                                           facecolor=color, alpha=0.7)
                    ax.add_patch(circle)
                    
                    # Etiqueta con ID y masa
                    ax.text(x, y, f'{id_part}\nm={masa:.1f}', 
                           ha='center', va='center', fontsize=8, 
                           fontweight='bold', color='white')
                    
                    particulas_dibujadas.append(id_part)
            
            # Mostrar colisiones recientes
            colisiones_recientes = [c for c in self.colisiones 
                                   if abs(c[0] - tiempo_actual) < 0.1]
            if colisiones_recientes:
                texto_col = f"Colisiones: {len(colisiones_recientes)}"
                ax.text(self.caja_ancho * 0.02, self.caja_alto * 0.98, 
                       texto_col, fontsize=10, color='red',
                       bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.5))
            
            return []
        
        # Crear animación con menos frames para mejor rendimiento
        frames = range(0, len(self.tiempos), max(1, len(self.tiempos) // 500))
        
        anim = FuncAnimation(fig, animate, init_func=init, 
                           frames=frames, interval=20, 
                           blit=False, repeat=True)
        
        if guardar:
            print(f"Guardando animación en {nombre}...")
            anim.save(nombre, writer='ffmpeg', fps=30, dpi=100)
            print("Animación guardada!")
        
        plt.show()
    
    def graficar_trayectorias(self):
        """Grafica las trayectorias de todas las partículas"""
        fig, ax = plt.subplots(figsize=(12, 10))
        
        ax.set_xlim(0, self.caja_ancho)
        ax.set_ylim(0, self.caja_alto)
        ax.set_aspect('equal')
        ax.set_xlabel('X (unidades)')
        ax.set_ylabel('Y (unidades)')
        ax.set_title('Trayectorias de las Partículas')
        ax.grid(True, alpha=0.3)
        
        # Dibujar obstáculos
        for obs in self.obstaculos:
            x, y, lado = obs
            rect = patches.Rectangle((x - lado/2, y - lado/2), lado, lado,
                                    linewidth=2, edgecolor='red', 
                                    facecolor='lightcoral', alpha=0.3)
            ax.add_patch(rect)
        
        # Dibujar trayectorias
        colores = plt.cm.tab10(np.linspace(0, 1, len(self.datos)))
        for i, (id_part, datos) in enumerate(self.datos.items()):
            ax.plot(datos['x'], datos['y'], '-', color=colores[i], 
                   alpha=0.6, linewidth=1.5, label=f'Partícula {id_part}')
            
            # Marcar inicio y fin
            ax.plot(datos['x'][0], datos['y'][0], 'o', color=colores[i], 
                   markersize=8, markeredgecolor='black', markeredgewidth=1.5)
            ax.plot(datos['x'][-1], datos['y'][-1], 's', color=colores[i], 
                   markersize=8, markeredgecolor='black', markeredgewidth=1.5)
        
        ax.legend(loc='upper left', fontsize=8)
        plt.tight_layout()
        plt.savefig('trayectorias.png', dpi=150, bbox_inches='tight')
        print("Trayectorias guardadas en: trayectorias.png")
        plt.show()
    
    def analizar_colisiones(self):
        """Analiza y muestra estadísticas de colisiones"""
        print("\n=== ANÁLISIS DE COLISIONES ===")
        
        tipos = {}
        for col in self.colisiones:
            tipo = col[1]
            if tipo not in tipos:
                tipos[tipo] = 0
            tipos[tipo] += 1
        
        for tipo, cantidad in tipos.items():
            print(f"{tipo}: {cantidad}")
        
        print(f"\nTotal de colisiones: {len(self.colisiones)}")
        
        # Gráfica de colisiones vs tiempo
        tiempos_col = [c[0] for c in self.colisiones]
        
        fig, ax = plt.subplots(figsize=(12, 5))
        ax.hist(tiempos_col, bins=50, edgecolor='black', alpha=0.7)
        ax.set_xlabel('Tiempo (s)')
        ax.set_ylabel('Número de colisiones')
        ax.set_title('Distribución de Colisiones en el Tiempo')
        ax.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('analisis_colisiones.png', dpi=150, bbox_inches='tight')
        print("Análisis guardado en: analisis_colisiones.png")
        plt.show()

def main():
    print("=== VISUALIZADOR DE SIMULACIÓN DE COLISIONES ===\n")
    
    viz = Visualizador('simulacion.txt')
    viz.leer_datos()
    
    print("\nOpciones:")
    print("1. Ver animación")
    print("2. Graficar trayectorias")
    print("3. Analizar colisiones")
    print("4. Todo lo anterior")
    
    opcion = input("\nSeleccione una opción (1-4): ")
    
    if opcion == '1':
        viz.crear_animacion()
    elif opcion == '2':
        viz.graficar_trayectorias()
    elif opcion == '3':
        viz.analizar_colisiones()
    elif opcion == '4':
        viz.graficar_trayectorias()
        viz.analizar_colisiones()
        viz.crear_animacion()
    else:
        print("Opción inválida")

if __name__ == '__main__':
    main()