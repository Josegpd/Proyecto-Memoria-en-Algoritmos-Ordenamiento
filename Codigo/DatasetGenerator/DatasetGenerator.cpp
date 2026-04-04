#include "DatasetGenerator.hpp"
#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>
#include <fstream>
#include <stdexcept>

namespace DatasetGenerator {

    // ====================
    // 1. DATOS SINTETICOS 
    // ====================

    // Genera una secuencia perfectamente ordenada: 0, 1, 2, ..., size-1
    // Útil para ver el "mejor caso".
    std::vector<int> generateSorted(size_t size){
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 0); // Llena el vector incrementalmente
        return data;
    }

    // Genera una secuencia ordenada inversamente: size-1, size-2, ..., 0
    // Útil para ver el "peor caso" clásico de ordenamiento.
    std::vector<int> generateReverseSorted(size_t size){
        std::vector<int> data = generateSorted(size);
        std::reverse(data.begin(), data.end());
        return data;
    }

    // ====================
    // 2. DATOS ALEATORIOS
    // ====================

    // Genera datos con distribución uniforme (todos los números tienen igual probabilidad)
    std::vector<int> generateUniformRandom(size_t size, int min_val, int max_val){
        std::vector<int> data(size);
        std::random_device rd;  // Obtiene una semilla real del hardware/SO
        std::mt19937 gen(rd()); // Motor Mersenne Twister
        std::uniform_int_distribution<int> dist(min_val, max_val);

        for(size_t i = 0; i < size; ++i){
            data[i] = dist(gen);
        }
        return data;
    }

    // Genera datos con Distribución Normal
    // La mayoría de los datos se agrupan cerca de la 'media'.
    std::vector<int> generateNormalRandom(size_t size, double mean, double stddev){
        std::vector<int> data(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(mean, stddev);

        for(size_t i = 0; i < size; ++i){
            // Casteamos a entero ya que la distribución normal genera doubles
            data[i] = static_cast<int>(dist(gen)); 
        }
        return data;
    }

    // =====================================
    // 3. DATOS REALES (Trazas de Archivos)
    // =====================================

    // Lee un dataset real desde un archivo CSV o TXT.
    // Asume que hay un número por línea (formato típico de datasets simples o trazas limpias).
    std::vector<int> loadFromFile(const std::string& filename) {
        std::vector<int> data;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            throw std::runtime_error("Error: No se pudo abrir el archivo " + filename);
        }

        while (std::getline(file, line)) {
            if (line.empty()) continue; // Ignorar líneas en blanco
            try {
                data.push_back(std::stoi(line)); // Convierte string a entero
            } catch (const std::invalid_argument& e) {
                // Si la línea tiene texto (como un encabezado CSV), la ignora y continúa
                continue; 
            } catch (const std::out_of_range& e) {
                // Previene caídas si el número en el dataset es más grande que un int
                std::cerr << "Advertencia: Número fuera de rango ignorado.\n";
            }
        }
        return data;
    }
}