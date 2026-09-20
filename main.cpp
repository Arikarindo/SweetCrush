#include <iostream>

#include "matriz.h"
#include "reglas.h"
#include "constantes.h"

using namespace std;


// MOSTRAR TABLERO EN CARACTERES


void mostrar_tablero(const unsigned char* tablero,
                     int filas,
                     int columnas)
{
    cout << "\nTABLERO ACTUAL\n\n";

    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnas; columna++)
        {
            int indice = fila * columnas + columna;
            unsigned char ficha = obtenerFicha(tablero, indice);

            if (ficha < TOTALFICHAS)
            {
                cout << SIMBOLOS_FICHA[ficha] << "  ";
            }
            else
            {
                cout << "?  ";
            }
        }
        cout << '\n';
    }
    cout << '\n';
}


// MOSTRAR REPRESENTACION BINARIA


void mostrar_binario(const unsigned char* tablero,
                     int filas,
                     int columnas)
{
    int totalFichas = filas * columnas;
    int totalBits = totalFichas * BITSPORFICHA;

    cout << "\nSECUENCIA DE BITS\n\n";

    for (int bitGlobal = 0; bitGlobal < totalBits; bitGlobal++)
    {
        int byte = bitGlobal / 8;
        int bit = bitGlobal % 8;

        unsigned char valor = static_cast<unsigned char>((tablero[byte] >> bit) & 1u);

        cout << static_cast<int>(valor);

        if ((bitGlobal + 1) % BITSPORFICHA == 0)
        {
            cout << ' ';
        }
    }

    cout << "\n\n";
}


// MOSTRAR ESTADISTICAS

void mostrar_estadisticas(int filas,
                          int columnas,
                          int bytesReservados,
                          int eliminacionesUsuario,
                          int fichasEliminadas,
                          int combinaciones,
                          int cascadas,
                          int puntuacion)
{
    cout << "\nESTADISTICAS\n\n";
    cout << "Filas: " << filas << '\n';
    cout << "Columnas: " << columnas << '\n';
    cout << "Bytes reservados: " << bytesReservados << '\n';
    cout << "Eliminaciones del usuario: " << eliminacionesUsuario << '\n';
    cout << "Fichas eliminadas en total: " << fichasEliminadas << '\n';
    cout << "Combinaciones detectadas: " << combinaciones << '\n';
    cout << "Cascadas del ultimo movimiento: " << cascadas << '\n';
    cout << "Puntuacion: " << puntuacion << "\n\n";
}


