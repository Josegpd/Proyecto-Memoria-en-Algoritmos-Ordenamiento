#pragma once

#include <vector>
#include "cacheSimulator.hpp"

template <typename T>
class TrackedArray {
private:
    std::vector<T> data;  // Almacenamiento de los valores reales del array
    CacheSimulator& cache;
    uint64_t baseAddress; // Direccion ficticia donde comienza el arreglo en RAM

    // Clase para interceptar lecturas y escrituras
    class ProxyElement {
    private:
        T& ref_value;
        CacheSimulator& cache_ref;
        uint64_t physical_address;

    public:
        ProxyElement(T& val, CacheSimulator& c, uint64_t addr)
            : ref_value(val), cache_ref(c), physical_address(addr) {}

        // 1. Intercepta ESCRITURAS: arreglo[i] = valor;
        ProxyElement& operator=(const T& new_value) {
            cache_ref.accessMemory(physical_address); // Simulamos el acceso
            ref_value = new_value;                    // Guardamos el dato real
            return *this;
        }

        // 2. Intercepta ESCRITURAS desde otro dato en memoria: arreglo[i] = arreglo[j];
        ProxyElement& operator=(const ProxyElement& other) {
            // Se lee de 'other' (el operador de conversión de other se activará)
            // y luego se escribe aquí:
            cache_ref.accessMemory(physical_address);
            ref_value = other.ref_value;
            return *this;
        }

        // 3. Intercepta LECTURAS: int x = arreglo[i]; o comparaciones: if(arreglo[i] > 5)
        operator T() const { // Operador de conversion de tipo
            cache_ref.accessMemory(physical_address); // Simulamos el acceso
            return ref_value;                         // Devolvemos el dato real
        }
    };

public:
    // Constructor: Recibe los datos, una referencia al simulador y una dirección base inventada
    TrackedArray(const std::vector<T>& initial_data, CacheSimulator& simulator, uint64_t base_addr = 0x1000)
        : data(initial_data), cache(simulator), baseAddress(base_addr) {}

    // Sobrecarga del operador de acceso. Devuelve el Proxy, no el dato directo.
    ProxyElement operator[](size_t index) {
        // Calculamos la direccion de memoria simulada basada en el indice y el tamaño del tipo de dato
        uint64_t address = baseAddress + (index * sizeof(T));
        return ProxyElement(data[index], cache, address);
    }

    // Métodos de utilidad
    size_t size() const { return data.size(); }
    
    // Método para ver los datos finales sin alterar las estadisticas de cache
    void printData() const {
        for (const auto& val : data) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
};