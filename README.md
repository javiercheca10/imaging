# Imaging — Tratamiento Digital de Imágenes en C++

[![C++](https://img.shields.io/badge/C%2B%2B-14-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Status-Completado-success)](https://github.com/javiercheca10/imaging)
[![Universidad](https://img.shields.io/badge/UGR-DGIIM-E03A3E)](https://www.ugr.es/)
[![Documentation](https://img.shields.io/badge/Docs-Doxygen-blue)](https://www.doxygen.nl/)

Biblioteca orientada a objetos en C++ para el procesamiento, análisis y manipulación de imágenes digitales en escala de grises bajo formato **PGM** (*Portable GrayMap*). Proyecto desarrollado para la asignatura **Metodología de la Programación (MP)** en el Doble Grado en Ingeniería Informática y ADE (**UGR**).

---

## 📌 Tabla de Contenidos
- [Características Principales](#-características-principales)
- [Arquitectura de Software](#-arquitectura-de-software)
  - [Módulo Byte](#1-módulo-byte-byteh-bytecpp)
  - [Módulo Histogram](#2-módulo-histogram-histogramh-histogramcpp)
  - [Módulo Image](#3-módulo-image-imageh-imagecpp)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Compilación y Uso](#-compilación-y-uso)
- [Principios de Diseño y Calidad](#-principios-de-diseño-y-calidad)
- [Autor](#-autor)

---

## 🚀 Características Principales

* **Entrada/Salida PGM:** Lectura y escritura de imágenes en formato Netpbm PGM (texto `P2` y binario `P5`).
* **Manipulación a nivel de bit:** Control granular de planos de bits, máscaras lógicas y desplazamientos.
* **Análisis de Frecuencia:** Generación de histogramas de 256 niveles de gris con cálculo de balance/mediana.
* **Procesamiento Digital de Imágenes:**
  * Binarización automática mediante cálculo de umbral óptimo equilibrado.
  * Generación y dibujo gráfico del histograma normalizado embebido en una nueva imagen.
  * Segmentación morfológica y extracción de objetos basada en tolerancia de histograma.
  * Operaciones espaciales: recorte de regiones de interés (ROI) y fusión con alfa blending (`pasteArea`).

---

## 🧱 Arquitectura de Software

El proyecto se estructura en tres módulos complementarios de alta cohesión y bajo acoplamiento:

```
┌────────────────────────────────────────────────────────┐
│                        Image                           │
│  - Matriz de píxeles (_data)                          │
│  - Binarización, recorte, fusión, segmentación         │
└──────────────┬──────────────────────────┬──────────────┘
               │                          │
               ▼                          ▼
┌──────────────────────────────┐  ┌──────────────────────┐
│          Histogram           │  │         Byte         │
│  - Frecuencias de gris (256) │  │  - Píxel de 8 bits   │
│  - Cálculo de balance        │  │  - Operadores bit    │
│  - Tolerancia de corte       │  │  - Máscaras y shifts │
└──────────────────────────────┘  └──────────────────────┘
```

### 1. Módulo Byte (`Byte.h`, `Byte.cpp`)
Abstracción de un píxel de 8 bits ($0 \le v \le 255$):
* **Operaciones bit a bit:** `onBit(pos)`, `offBit(pos)`, `getBit(pos)`.
* **Desplazamientos y rotaciones:** `shiftLByte(n)`, `shiftRByte(n)`.
* **Codificación y descomposición:** Conversión de/a vectores booleanos y descomposición de posiciones activas (`encodeByte`, `decodeByte`, `decomposeByte`).
* **Fusión:** Interpolación lineal ponderada entre dos bytes (`mergeByte`).

### 2. Módulo Histogram (`Histogram.h`, `Histogram.cpp`)
Análisis estadístico de la distribución de tonos:
* Almacenamiento de frecuencias para los 256 niveles de luminosidad.
* Métodos analíticos: `getMaxLevel()`, `getAverageLevel()`, `getBalancedLevel()` (mediana acumulada).
* Hashing estructural para validación e inspección rápida.

### 3. Módulo Image (`Image.h`, `Image.cpp`)
Núcleo del procesamiento matricial de imagen:
* **Gestión de Memoria:** Almacenamiento continuo bidimensional con protección de límites de acceso (`getPixel`, `setPixel`, `flatten`).
* **Regla de los Tres:** Garantiza copia profunda y destrucción segura evitando fugas de memoria (*memory leaks*) o punteros colgantes.
* **Filtros y Algoritmia:**
  * `threshold(int t)`: Binarización adaptativa usando el nivel de balance del histograma si no se especifica umbral.
  * `depictsHistogram()`: Dibuja un gráfico del histograma normalizado con eje de escala de grises.
  * `extractObjects(Image set[], int &n, int max, double tol)`: Identifica y separa objetos tonales continuos por tolerancia.
  * `copyArea(x, y, w, h)` y `pasteArea(x, y, from, toneup, merge)`: Composición espacial de subimágenes.

---

## 📂 Estructura del Proyecto

```
imaging/
├── include/              # Cabeceras públicas
│   ├── Byte.h
│   ├── Histogram.h
│   ├── Image.h
│   └── MPTools.h
├── src/                  # Implementación de los módulos
│   ├── Byte.cpp
│   ├── Histogram.cpp
│   ├── Image.cpp
│   └── main.cpp          # Punto de entrada / Pipeline de prueba
├── data/                 # Imágenes PGM de prueba y calibración
│   ├── .hidden_flat.pgm
│   ├── .hidden_gradient.pgm
│   ├── .hidden_histoflat.pgm
│   └── .hidden_histograd.pgm
├── tests/                # Batería de pruebas unitarias y de integración
├── doc/                  # Configuración Doxygen
├── Makefile              # Automatización de compilación
├── hidden1.pgm           # Imagen de muestra
└── README.md
```

---

## 🛠️ Compilación y Uso

### Requisitos
* Compilador C++ compatible con **C++14** o superior (`g++` / `clang++`).
* GNU Make.

### Compilar el proyecto
```bash
make
```
El ejecutable resultante se genera en `bin/imaging`.

### Ejecutar
```bash
make run
# o directamente:
./bin/imaging
```

### Limpiar artefactos de compilación
```bash
make clean
```

---

## 📐 Principios de Diseño y Calidad

1. **Separación Interfaz/Implementación:** Contratos claros en `include/*.h` e implementaciones aisladas en `src/*.cpp`.
2. **Robustez de Entrada:** Validación de encabezados PGM (`P2`/`P5`), comprobación de dimensiones máximas y gestión de errores de archivo.
3. **Documentación Doxygen:** Comentarios de documentación exhaustivos con `@brief`, `@param`, `@return`, `@pre` y `@post`.

---

## 👤 Autor

* **Francisco Javier Checa Casas**
  * Doble Grado en Ingeniería Informática y ADE (DGIIM) — Universidad de Granada
  * GitHub: [@javiercheca10](https://github.com/javiercheca10)
  * Web: [javiercheca.me](https://javiercheca.me)
