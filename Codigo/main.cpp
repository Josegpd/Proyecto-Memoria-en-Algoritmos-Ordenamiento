#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <exception>
#include "cacheSimulator.hpp"
#include "TrackedArray.hpp"
#include "DatasetGenerator.hpp"

// Configuración de la arquitectura de cache
const size_t CACHE_SIZE = 32768; // 32 KB (Tamaño típico de una L1)
const size_t BLOCK_SIZE = 64;    // 64 Bytes por linea de cache

void runEvaluation(const std::string& testName, const std::vector<int>& originalData);
int partition(TrackedArray<int>& arr, int low, int high);
void quickSort(TrackedArray<int>& arr, int low, int high);
void merge(TrackedArray<int>& arr, int left, int mid, int right, TrackedArray<int>& temp);
void mergeSort(TrackedArray<int>& arr, int left, int right, TrackedArray<int>& temp);

int main() {
    // Tamaño estandar para los datasets generados por codigo
    const size_t DATA_SIZE = 10000; 

    std::cout << "=================================================\n";
    std::cout << " CASOS DE PRUEBA: IMPACTO DE CACHE EN ORDENAMIENTO\n";
    std::cout << " Cache simulada: " << CACHE_SIZE / 1024 << " KB | Bloque: " << BLOCK_SIZE << " Bytes\n";
    std::cout << "=================================================\n\n";

    // ==========================================
    // CASO 1: DATOS SINTETICOS
    // ==========================================
    
    std::vector<int> dataSorted = DatasetGenerator::generateSorted(DATA_SIZE);
    runEvaluation("Datos sinteticos - Secuencia ordenada (Mejor caso)", dataSorted);

    std::vector<int> dataReverse = DatasetGenerator::generateReverseSorted(DATA_SIZE);
    runEvaluation("Datos sinteticos - Secuencia inversa (Peor caso)", dataReverse);


    // ==========================================
    // CASO 2: DATOS ALEATORIOS
    // ==========================================
    
    std::vector<int> dataUniform = DatasetGenerator::generateUniformRandom(DATA_SIZE);
    runEvaluation("Datos aleatorios - Distribución uniforme", dataUniform);

    std::vector<int> dataNormal = DatasetGenerator::generateNormalRandom(DATA_SIZE);
    runEvaluation("Datos aleatorios - Distribución normal", dataNormal);


    // ==========================================
    // CASO 3: DATOS REALES
    // ==========================================
    
    std::string filename = "datos_reales.csv";
    std::vector<int> dataReal;
    
    try{
        // Intentamos cargar el archivo. Si no existe, el bloque catch captura el error
        // sin detener abruptamente todo el programa.
        dataReal = DatasetGenerator::loadFromFile(filename);
        runEvaluation("Datos reales - Extraidos de archivo CSV/TXT", dataReal);
        
    }catch (const std::exception& e){
        std::cerr << "[!] Aviso: No se pudo ejecutar la prueba de datos reales.\n";
        std::cerr << "    Detalle: " << e.what() << "\n";
        std::cerr << "    Asegurate de crear '" << filename << "' en el directorio.\n\n";
    }

    std::cout << "=================================================\n";
    std::cout << "                 FIN DE LAS PRUEBAS\n";
    std::cout << "=================================================\n";

    return 0;
}

// ==========================================
// QUICKSORT
// ==========================================

// Función auxiliar para particionar el arreglo
int partition(TrackedArray<int>& arr, int low, int high){
    int pivot = arr[high]; // Se registra 1 lectura
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Se registra 1 lectura en arr[j]
        if (arr[j] < pivot) {
            i++;
            // Intercambio (Swap): Registra 2 lecturas y 2 escrituras
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Colocar el pivote en su posición final
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

void quickSort(TrackedArray<int>& arr, int low, int high){
    if (low < high) {
        int pivote = partition(arr, low, high);
            
        // Llamadas recursivas para las dos mitades
        quickSort(arr, low, pivote - 1);
        quickSort(arr, pivote + 1, high);
    }
}

// ==========================================
// MERGESORT
// ==========================================

// Función auxiliar para fusionar dos mitades ordenadas
void merge(TrackedArray<int>& arr, int left, int mid, int right, TrackedArray<int>& temp){
    int i = left;
    int j = mid + 1;
    int k = left;

    // Comparamos e insertamos en el arreglo temporal
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++]; // Lectura en arr, Escritura en temp
        } else {
            temp[k++] = arr[j++]; // Lectura en arr, Escritura en temp
        }
    }

    // Copiamos los elementos restantes de la primera mitad (si hay)
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // Copiamos los elementos restantes de la segunda mitad (si hay)
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // Copiamos el arreglo temporal de vuelta al arreglo original
    for (int p = left; p <= right; p++) {
        arr[p] = temp[p]; // Lectura en temp, Escritura en arr
    }
}

void mergeSort(TrackedArray<int>& arr, int left, int right, TrackedArray<int>& temp){
    if(left < right){
        // Calcular la mitad del arreglo
        int mid = left + (right - left) / 2;

        // Dividir
        mergeSort(arr, left, mid, temp);
        mergeSort(arr, mid + 1, right, temp);

        // Fusionar
        merge(arr, left, mid, right, temp);
    }
}

// =========================================================================
// FUNCIÓN EVALUADORA
// Ejecuta QuickSort y MergeSort sobre un dataset específico y mide la caché
// =========================================================================
void runEvaluation(const std::string& testName, const std::vector<int>& originalData){
    std::cout << "-------------------------------------------------\n";
    std::cout << "[ PRUEBA ]: " << testName << " (" << originalData.size() << " elementos)\n";
    std::cout << "-------------------------------------------------\n";

    if (originalData.empty()) {
        std::cout << "Dataset vacío. Saltando prueba...\n\n";
        return;
    }

    // --- 1. Evaluación de QuickSort ---
    std::cout << ">> Ejecutando QuickSort...\n";
    std::vector<int> dataQS = originalData; 
    CacheSimulator cacheQS(CACHE_SIZE, BLOCK_SIZE);
    
    // El arreglo de QS inicia en la dirección ficticia 0x1000
    TrackedArray<int> arrQS(dataQS, cacheQS, 0x1000); 

    quickSort(arrQS, 0, arrQS.size() - 1);
    cacheQS.printStats();

    // --- 2. Evaluación de MergeSort ---
    std::cout << "\n>> Ejecutando MergeSort...\n";
    std::vector<int> dataMS = originalData;
    CacheSimulator cacheMS(CACHE_SIZE, BLOCK_SIZE);
    
    // El arreglo principal inicia en 0x1000
    TrackedArray<int> arrMS(dataMS, cacheMS, 0x1000);
    
    // El arreglo temporal requerido por MergeSort inicia en 0x80000
    std::vector<int> tempData(originalData.size(), 0);
    TrackedArray<int> tempMS(tempData, cacheMS, 0x80000); 

    mergeSort(arrMS, 0, arrMS.size() - 1, tempMS);
    cacheMS.printStats();
    std::cout << "\n";
}