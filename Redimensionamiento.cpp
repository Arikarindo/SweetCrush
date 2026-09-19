#include "matriz.h"
#include "reglas.h"
#include "constantes.h"

bool agregar_columna(unsigned char*& tablero,
                     int filas,
                     int& columnas,
                     int posicion,
                     int& bytesReservados)
{
    if (tablero == nullptr || filas <= 0 || columnas <= 0 || posicion < 0 || posicion > columnas)
    {
        return false;
    }

    int columnasViejas = columnas;
    int columnasNuevas = columnas + 1;
    int totalNuevo = filas * columnasNuevas;

    int bytesNecesarios = calcular_bytes_necesarios(totalNuevo);

    // CASO A: Los Bytes reservados actualmente alcanzan para la nueva columna (No hay realloc)
    if (bytesNecesarios <= bytesReservados)
    {
        // Recorremos la matriz desde el final hacia el principio para no sobrescribir datos
        for (int fila = filas - 1; fila >= 0; fila--)
        {
            for (int columna = columnasViejas - 1; columna >= 0; columna--)
            {
                int viejoIndice = fila * columnasViejas + columna;
                unsigned char ficha = obtenerFicha(tablero, viejoIndice);

                int nuevaColumna = columna;
                if (columna >= posicion)
                {
                    nuevaColumna++; // Desplaza las columnas a la derecha del punto de inserción
                }

                int nuevoIndice = fila * columnasNuevas + nuevaColumna;
            
                guardarFicha(tablero, nuevoIndice, ficha);
            }
        }
    }
    // CASO B: El bloque de memoria actual es insuficiente. Requiere reasignación en el Heap.
    else
    {
        // Allocate de un nuevo bloque en el Heap
        unsigned char* tableroNuevo = new unsigned char[bytesNecesarios]();

        for (int fila = 0; fila < filas; fila++)
        {
            for (int columna = 0; columna < columnasViejas; columna++)
            {
                int viejoIndice = fila * columnasViejas + columna;
                unsigned char ficha = obtenerFicha(tablero, viejoIndice);

                int nuevaColumna = columna;
                if (columna >= posicion)
                {
                    nuevaColumna++;
                }

                int nuevoIndice = fila * columnasNuevas + nuevaColumna;
                guardarFicha(tableroNuevo, nuevoIndice, ficha);
            }
        }

        delete[] tablero; // Libera el bloque antiguo en la memoria RAM

        tablero = tableroNuevo; // Redirecciona el puntero al nuevo bloque
        bytesReservados = bytesNecesarios;
    }

    // 3. Rellena la columna recién insertada con fichas aleatorias
    for (int fila = 0; fila < filas; fila++)
    {
        int indice = fila * columnasNuevas + posicion;
        guardarFicha(tablero, indice, generar_ficha_aleatoria());
    }

    columnas = columnasNuevas;

    // Limpia bits huérfanos que puedan haber quedado en el último Byte
    limpiar_bits_no_usados(tablero, filas * columnas, bytesReservados);

    return true;
}

bool eliminar_columna(unsigned char*& tablero,
                      int filas,
                      int& columnas,
                      int posicion,
                      int& bytesReservados)
{
    if (tablero == nullptr || filas <= 0 || columnas <= 1 || posicion < 0 || posicion >= columnas)
    {
        return false;
    }

    int columnasViejas = columnas;
    int columnasNuevas = columnas - 1;

    // Copia omitiendo las fichas pertenecientes a la columna que se va a eliminar
    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnasViejas; columna++)
        {
            if (columna == posicion)
            {
                continue; // Salta la columna eliminada
            }

            int viejoIndice = fila * columnasViejas + columna;
            unsigned char ficha = obtenerFicha(tablero, viejoIndice);

            int nuevaColumna = columna;
            if (columna > posicion)
            {
                nuevaColumna--; // Desplaza columnas hacia la izquierda para llenar el vacío
            }

            int nuevoIndice = fila * columnasNuevas + nuevaColumna;
            guardarFicha(tablero, nuevoIndice, ficha);
        }
    }

    columnas = columnasNuevas;
    int totalNuevo = filas * columnas;

    // Limpia los bits no utilizados
    limpiar_bits_no_usados(tablero, totalNuevo, bytesReservados);

    // Revisa si es necesario liberar memoria en el Heap si el tamaño del tablero disminuyó bastante
    tablero = evaluarYRedimensionar(tablero, totalNuevo, bytesReservados);

    return true;
}
