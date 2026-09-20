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

void poblar_aleatorio(unsigned char* tablero, int filas, int columnas)
{
    int totalFichas = filas * columnas;
    for (int i = 0; i < totalFichas; i++)
    {
        guardarFicha(tablero, i, generar_ficha_aleatoria());
    }
}

bool eliminar_ficha(unsigned char* tablero, int fila, int columna, int filas, int columnas)
{
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas)
    {
        return false; // Coordenadas inválidas
    }

    int indice = fila * columnas + columna;
    guardarFicha(tablero, indice, VACIO);
    return true;
}
void marcar_posicion(unsigned char* marcas, int indice) //No sabe de reglas ni de fichas.
{
    // MATEMATICA: Divide el indice entre 8 para ubicar exactamente en que Byte de la memoria se almacena la marca.
    int byte = indice / 8; 
    
    // MATEMATICA: Usa el modulo (%) 8 para hallar el desplazamiento de bit exacto (0-7) dentro de ese Byte.
    int bit = indice % 8;   

    // LOGICA: Operacion OR (|) combinada con desplazamiento a la izquierda (<<).
    // Desplaza un '1' a la posicion calculada y hace OR. Resuelve encender ese bit especifico (marcar la ficha) sin borrar las demas marcas del byte.
    marcas[byte] |= static_cast<unsigned char>(1u << bit);
}

bool posicion_marcada(const unsigned char* marcas, int indice) //Solo consulta si ese bit específico está en 1 o en 0 
{
    // MATEMATICA: División entera para ubicar Byte, Módulo para ubicar bit dentro de la bandera de marcas  
    int byte = indice / 8;
    int bit = indice % 8;

    // LOGICA: Se desplaza el 1 a la posicion del bit y se aplica una operacion AND (&) con el byte.
    // Resuelve aislar el bit especifico; si el resultado no es cero (!= 0), significa que el bit estaba en 1 (estaba marcado).
    return (marcas[byte] & static_cast<unsigned char>(1u << bit)) != 0;
}

int detectar_y_marcar_combinaciones(unsigned char* tablero, int filas,int columnas, int& fichasMarcadas)
{
    fichasMarcadas = 0;

    if (tablero == nullptr || filas <= 0 || columnas <= 0)
    {
        return 0;
    }

    int totalFichas = filas * columnas;
    // Asignación compacta: 1 bit por ficha en el Heap (división techo)
    int bytesMarcas = (totalFichas + 7) / 8;
    unsigned char* marcas = new unsigned char[bytesMarcas](); // Inicializado en 0x00

    int combinaciones = 0;

    // 1. ESCANEO HORIZONTAL (Fila por fila)
    for (int fila = 0; fila < filas; fila++)
    {
        int columna = 0;
        while (columna < columnas)
        {
            unsigned char ficha = leer_ficha(tablero, fila, columna, columnas);

            // Se ignoran celdas vacías o con estados no jugables
            if (ficha >= FICHASJUGABLES)
            {
                columna++;
                continue;
            }

            int inicio = columna;
            int longitud = 1;
            columna++;

            // Cuenta fichas idénticas consecutivas hacia la derecha
            while (columna < columnas && leer_ficha(tablero, fila, columna, columnas) == ficha)
            {
                longitud++;
                columna++;
            }

            // Si hay 3 o más consecutivas, se registra la combinación horizontal
            if (longitud >= 3)
            {
                combinaciones++;
                for (int c = inicio; c < inicio + longitud; c++)
                {
                    int indice = calcular_indice(fila, c, columnas);
                    marcar_posicion(marcas, indice); // Enciende el bit correspondiente
                }
            }
        }
    }

    // 2. ESCANEO VERTICAL (Columna por columna)
    for (int columna = 0; columna < columnas; columna++)
    {
        int fila = 0;
        while (fila < filas)
        {
            unsigned char ficha = leer_ficha(tablero, fila, columna, columnas);

            if (ficha >= FICHASJUGABLES)
            {
                fila++;
                continue;
            }

            int inicio = fila;
            int longitud = 1;
            fila++;

            // Cuenta fichas idénticas consecutivas hacia abajo
            while (fila < filas && leer_ficha(tablero, fila, columna, columnas) == ficha)
            {
                longitud++;
                fila++;
            }

            // Si hay 3 o más consecutivas, se registra la combinación vertical
            if (longitud >= 3)
            {
                combinaciones++;
                for (int f = inicio; f < inicio + longitud; f++)
                {
                    int indice = calcular_indice(f, columna, columnas);
                    marcar_posicion(marcas, indice); // Enciende el bit correspondiente
                }
            }
        }
    }

    // 3. ESTAMPAR MARCAS EN EL TABLERO ORIGINAL
    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnas; columna++)
        {
            int indice = calcular_indice(fila, columna, columnas);

            if (posicion_marcada(marcas, indice))
            {
                guardarFicha(tablero, indice, MARCADO);
                fichasMarcadas++;
            }
        }
    }

    // Liberación estricta del arreglo auxiliar para evitar memory leaks
    delete[] marcas;
    marcas = nullptr;

    return combinaciones;
}

int eliminar_fichas_marcadas(unsigned char* tablero, int filas, int columnas)
{
    if (tablero == nullptr)
    {
        return 0;
    }

    int eliminadas = 0;
    int totalFichas = filas * columnas;

    for (int indice = 0; indice < totalFichas; indice++)
    {
        // Se llama directamente a la lectura por índice en memoria
        unsigned char ficha = obtenerFicha(tablero, indice);

        if (ficha == MARCADO)
        {
            // Se escribe directamente en el índice
            guardarFicha(tablero, indice, VACIO);
            eliminadas++;
        }
    }

    return eliminadas;
}


void aplicar_gravedad_y_relleno(unsigned char* tablero, int filas, int columnas)
{
    if (tablero == nullptr || filas <= 0 || columnas <= 0)
    {
        return;
    }

    // Iteramos columna por columna (necesario porque la gravedad funciona en vertical)
    for (int columna = 0; columna < columnas; columna++)
    {
        // En lugar de usar filas y hacer fila*columnas, calculamos el índice del fondo una sola vez:
        int indiceDestino = (filas - 1) * columnas + columna;
        int indiceLectura = indiceDestino;

        // 1. GRAVEDAD: Mueve fichas hacia el fondo si hay espacios vacios abajo
        for (int fila = filas - 1; fila >= 0; fila--)
        {
            unsigned char ficha = obtenerFicha(tablero, indiceLectura);

            // Si es una ficha válida (no está VACIO ni MARCADO)
            if (ficha < FICHASJUGABLES)
            {
                if (indiceLectura != indiceDestino)
                {
                    // Movemos la ficha al destino
                    guardarFicha(tablero, indiceDestino, ficha);
                    
                    //no hay necesidad de vaciar indice lectura por que de todas maneras se va a sobreescribir
                }
                
                indiceDestino -= columnas; // Subimos el puntero de escritura 1 casilla real
            }
            indiceLectura -= columnas;     // Subimos el puntero de lectura 1 casilla real
        }

        // 2. RELLENO: Genera fichas aleatorias para las casillas superiores que quedaron vacías
        // Aprovechamos que indiceDestino quedó apuntando a la primera celda vacía
        while (indiceDestino >= 0)
        {
            guardarFicha(tablero, indiceDestino, generar_ficha_aleatoria());
            indiceDestino -= columnas;     // Subimos hasta llegar al inicio del tablero
        }
    }
}

// Procesa UNA sola cascada: detecta, elimina y rellena.
// Retorna true si encontró combinaciones y hubo reacción; false si no hubo nada.
bool procesar_un_paso_cascada(unsigned char* tablero,
                              int filas,
                              int columnas,
                              int& combinacionesTotales,
                              int& fichasEliminadasTotales,
                              int& cascadasActuales,
                              int& puntuacion)
{
    if (tablero == nullptr) return false;

    int fichasMarcadas = 0;
    // La regla busca los tríos (el main no sabe nada de esto)
    int combinaciones = detectar_y_marcar_combinaciones(tablero, filas, columnas, fichasMarcadas);

    if (combinaciones == 0)
    {
        return false; // Tablero estable: no hay más cascadas
    }

    cascadasActuales++;
    combinacionesTotales += combinaciones;

    int eliminadas = eliminar_fichas_marcadas(tablero, filas, columnas);
    fichasEliminadasTotales += eliminadas;

    puntuacion += combinaciones * 10 * cascadasActuales;

    aplicar_gravedad_y_relleno(tablero, filas, columnas);
    return true; // Hubo una cascada resuelta con éxito
}

