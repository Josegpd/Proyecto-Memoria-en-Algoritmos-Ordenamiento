# 1. Variables de Configuración
CXX = g++
# Añadimos las carpetas de las librerías para que el compilador encuentre los headers
INCLUDES = -ITrackedArray -IcacheSimulator -IDatasetGenerator
# Usamos el estándar C++20, advertencias básicas y las rutas de inclusión
CXXFLAGS = -std=c++20 -Wall $(INCLUDES)

# Nombre del archivo ejecutable que se generará
TARGET = simulador_cache

# 2. Archivos del proyecto
# Usamos 'wildcard' para incluir automáticamente todos los .cpp de cada carpeta
SRCS = main.cpp \
       $(wildcard TrackedArray/*.cpp) \
       $(wildcard cacheSimulator/*.cpp) \
       $(wildcard DatasetGenerator/*.cpp)

# Transformamos la lista de .cpp en una lista de .o (archivos objeto)
OBJS = $(SRCS:.cpp=.o)

# 3. Regla principal
all: run

# 4. Regla para enlazar los archivos objeto y crear el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 5. Regla genérica para compilar cada .cpp en su respectivo .o
# Make respeta las rutas relativas de las carpetas al crear los .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para ejecutar el programa automáticamente una vez compilado
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar el proyecto (eliminar compilados viejos)
clean:
	rm -f $(OBJS) $(TARGET)