#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

// Representa una linea individual dentro de la cache
struct CacheLine {
    bool valid{false};    // Indica si la linea contiene datos reales (1) o basura (0)
    uint64_t tag{0};      // Identificador unico del bloque de memoria
};

class CacheSimulator{
private:
    size_t cacheSize;
    size_t blockSize;
    size_t numLines;

    std::vector<CacheLine> cache;

    // Contadores de rendimiento
    uint64_t hits{0};
    uint64_t misses{0};
    
public:
    CacheSimulator(size_t cache_size, size_t block_size);
    void accessMemory(uint64_t address);
    void printStats() const;
    uint64_t getHits() const;
    uint64_t getMisses() const;
    double getMissRate() const;
    double getHitRate() const;
};