#include "matriz.h"
#include "constantes.h"
#include <QRandomGenerator>


int calcular_indice(int fila, int columna, int columnas)
{
    // MATEMATICA: Fórmula de mapeo 2D -> 1D: (Fila_actual * Ancho_Total) + Columna_actual transforma 2 coordenadas conceptuales en un único índice lineal real
    return fila * columnas + columna;
}

unsigned char leer_ficha(const unsigned char* tablero, int fila, int columna, int columnas)
{
    int indice = calcular_indice(fila, columna, columnas);
   //Llamado de memoriatab.cpp para extraer los 3 bits de la ficha
    return obtenerFicha(tablero, indice); 
}

unsigned char generar_ficha_aleatoria()
{
    // Obtiene un número entero en el rango [0, FICHASJUGABLES - 1]
    return static_cast<unsigned char>(
        QRandomGenerator::global()->bounded(FICHASJUGABLES)
        );
}
void marcar_posicion(unsigned char* marcas, int indice)
{
    // MATEMATICA: Divide el indice entre 8 para ubicar exactamente en que Byte de la memoria se almacena la marca.
    int byte = indice / 8; 
    
    // MATEMATICA: Usa el modulo (%) 8 para hallar el desplazamiento de bit exacto (0-7) dentro de ese Byte.
    int bit = indice % 8;   

    // LOGICA: Operacion OR (|) combinada con desplazamiento a la izquierda (<<).
    // Desplaza un '1' a la posicion calculada y hace OR. Resuelve encender ese bit especifico (marcar la ficha) sin borrar las demas marcas del byte.
    marcas[byte] |= static_cast<unsigned char>(1u << bit);
}


