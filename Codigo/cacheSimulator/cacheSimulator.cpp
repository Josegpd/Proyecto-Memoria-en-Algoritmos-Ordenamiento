#include "cacheSimulator.hpp"
#include <iostream>
#include <cstddef>

// Constructor: Inicializa la cache con los tamaños especificados (en bytes)
CacheSimulator::CacheSimulator(size_t cache_size, size_t block_size) 
    : cacheSize(cache_size), blockSize(block_size) {
    
    // Calculamos cuántas líneas tendrá la cache en total
    numLines = cacheSize / blockSize;
    
    // Redimensionamos el vector para representar las lineas físicas
    cache.resize(numLines);
}

// Metodo principal que simula un acceso a memoria (lectura o escritura)
void CacheSimulator::accessMemory(uint64_t address) {
    // 1. Descomposicion de la direccion fisica
    // Primero, descartamos el desplazamiento dividiendo por el tamaño del bloque
    uint64_t blockAddress = address / blockSize; 
    
    // Calculamos el indice
    uint64_t index = blockAddress % numLines;    
    
    // Calculamos la etiqueta
    uint64_t tag = blockAddress / numLines;      

    // 2. Comprobación de acierto o fallo
    if (cache[index].valid && cache[index].tag == tag) {
        // El dato esta en la cache y la etiqueta coincide
        hits++;
    }else{
        // La línea esta vacia o tiene un bloque de otra direccion de memoria
        misses++;
        
        // 3. Simulacion de carga desde la memoria principal
        // Reemplazamos la línea con el nuevo bloque
        cache[index].valid = true;
        cache[index].tag = tag;
    }
}

// Metodos para obtener los resultados del analisis
uint64_t CacheSimulator::getHits() const { return hits; }
uint64_t CacheSimulator::getMisses() const { return misses; }

double CacheSimulator::getMissRate() const {
    uint64_t totalAccesses = hits + misses;
    if (totalAccesses == 0) return 0.0;
    return static_cast<double>(misses) / totalAccesses;
}

double CacheSimulator::getHitRate() const {
    uint64_t totalAccesses = hits + misses;
    if(totalAccesses == 0) return 0.0;
    return static_cast<double>(hits) / totalAccesses;
}

void CacheSimulator::printStats() const {
    std::cout << "--- Estadísticas de Caché ---\n";
    std::cout << "Accesos Totales: " << (hits + misses) << "\n";
    std::cout << "Aciertos: " << hits << "\n";
    std::cout << "Fallos: " << misses << "\n";
    std::cout << "Tasa de Aciertos (Hit Rate): " << getHitRate() * 100.0 << "%\n";
    std::cout << "Tasa de Fallos (Miss Rate): " << getMissRate() * 100.0 << "%\n";
    std::cout << "-----------------------------\n";
}