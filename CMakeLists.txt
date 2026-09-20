cmake_minimum_required(VERSION 3.16)

project(SweetCrush VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar el modulo Core de Qt (necesario para QRandomGenerator)
find_package(Qt6 COMPONENTS Core REQUIRED)

# Incluir archivos de cabecera en el entorno de construccion
set(PROJECT_SOURCES
    main.cpp
    memoriatab.cpp
    Reglasinternas.cpp
    Redimensionamiento.cpp
    matriz.h
    reglas.h
    constantes.h
)

add_executable(SweetCrush ${PROJECT_SOURCES})

# Vincular la libreria de Qt Core al ejecutable
target_link_libraries(SweetCrush PRIVATE Qt6::Core)
