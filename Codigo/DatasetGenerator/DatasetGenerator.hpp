#pragma once

#include <vector>
#include <string>
#include <cstddef>

namespace DatasetGenerator{
    // 1. DATOS SINTETICOS
    std::vector<int> generateSorted(size_t size);
    std::vector<int> generateReverseSorted(size_t size);

    // 2. DATOS ALEATORIOS
    std::vector<int> generateUniformRandom(size_t size, int min_val = 0, int max_val = 100000);
    std::vector<int> generateNormalRandom(size_t size, double mean = 50000.0, double stddev = 15000.0);

    // 3. DATOS REALES
    std::vector<int> loadFromFile(const std::string& filename);
}