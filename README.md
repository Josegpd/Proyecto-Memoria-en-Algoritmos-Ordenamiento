# Análisis de Accesos a Memoria y Simulación de Caché en Algoritmos de Ordenamiento

Este proyecto implementa un **simulador de memoria caché de hardware** en **C++20** diseñado para medir, comparar y analizar el comportamiento y la eficiencia en el uso de memoria de algoritmos clásicos de ordenamiento (**QuickSort** y **MergeSort**) frente a diversas distribuciones de datos.

---

## ¿De qué trata este proyecto?

Cuando un programa se ejecuta, el procesador (CPU) necesita leer y escribir datos constantemente en la memoria principal (RAM). Sin embargo, acceder a la memoria RAM es un proceso relativamente lento en comparación con la velocidad de procesamiento de la CPU.

Para solucionar este cuello de botella, los procesadores modernos cuentan con una memoria intermedia extremadamente rápida llamada **Memoria Caché**:

* **Principio de Localidad:**
  * **Localidad Temporal:** Si un dato se utiliza, es muy probable que vuelva a consultarse pronto.
  * **Localidad Espacial:** Si se accede a una posición de memoria, es muy probable que pronto se acceda a las posiciones contiguas (por ello, la caché transfiere información en bloques de varios bytes, no dato por dato).
* **Acierto de Caché (Cache Hit):** El dato solicitado por la CPU ya está cargado en la caché, por lo que se obtiene de forma casi instantánea.
* **Fallo de Caché (Cache Miss):** El dato no está en la caché; se debe pausar la operación para traer el bloque correspondiente desde la RAM, lo que introduce latencia.

En la teoría de algoritmos, la eficiencia suele medirse únicamente por la cantidad de comparaciones matemáticas u operaciones lógicas (complejidad $O(n \log n)$). No obstante, en sistemas reales, **la forma en que un algoritmo accede a la memoria y aprovecha la caché puede marcar una diferencia drástica en el rendimiento**. Este proyecto permite observar y cuantificar ese impacto de forma experimental.

---

## Características y Algoritmos Evaluados

### 1. Algoritmos de Ordenamiento
* **QuickSort:** Algoritmo de particionamiento *in-place* (reordena los elementos dentro del mismo arreglo sin requerir memoria auxiliar sustancial).
* **MergeSort:** Algoritmo divide y vencerás que divide el arreglo sucesivamente y fusiona las partes ordenadas utilizando un arreglo auxiliar en memoria.

### 2. Modelo de Simulación de Caché
* **Caché de Mapeo Directo (Direct-Mapped Cache):** Cada bloque de memoria principal tiene asignada una única línea posible en la caché.
  * Cálculo de bloque: $\text{Dirección} / \text{Tamaño del Bloque}$
  * Índice de línea: $\text{Bloque} \pmod{\text{Número de Líneas}}$
  * Etiqueta (*Tag*): $\text{Bloque} / \text{Número de Líneas}$
* Configuración de referencia en simulación: Líneas de caché con etiquetas de validez, bloques de 64 Bytes y tamaño total configurable (por defecto 32 KB).

### 3. Generación y Variedad de Datasets
El simulador evalúa el rendimiento sobre cinco escenarios de datos:
1. **Arreglo Ordenado (Sorted):** Evalúa el comportamiento en el mejor/peor caso estructural.
2. **Arreglo Inversamente Ordenado (Reverse Sorted):** Evalúa el impacto de reorganizaciones extremas.
3. **Distribución Uniforme Aleatoria:** Simula un flujo de datos sin correlación previa.
4. **Distribución Normal / Gaussiana:** Modela concentraciones realistas de valores.
5. **Datos Reales (`datos_reales.csv`):** Evaluación sobre un conjunto de datos externos provisto en formato CSV.

---

## Arquitectura del Software

El código está estructurado modularmente en C++20 bajo el directorio `Codigo/`:

```plaintext
Codigo/
├── main.cpp                     # Orquestación de pruebas, benchmarks y algoritmos
├── makefile                     # Automatización de compilación y enlace
├── datos_reales.csv             # Dataset real de prueba
├── cacheSimulator/
│   ├── cacheSimulator.hpp       # Definición de CacheLine y CacheSimulator
│   └── cacheSimulator.cpp       # Lógica de cálculo de tag/índice y detección de hit/miss
├── DatasetGenerator/
│   ├── DatasetGenerator.hpp     # Interfaz del generador y cargador de datos
│   └── DatasetGenerator.cpp     # Implementación de distribuciones sintéticas y lector CSV
└── TrackedArray/
    └── TrackedArray.hpp         # Contenedor wrapper con patrón Proxy para intercepción
