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
// MAIN


int main()
{
    int filas;
    int columnas;

    cout << "_______________________________________SWEET CRUSH_________________________________________________________\n";

    cout << "Numero de filas: ";
    cin >> filas;

    cout << "Numero de columnas: ";
    cin >> columnas;

    while (columnas > 35)
    {
        cout << "Excede el limite visible de pantalla.\n"
             << "Por favor reduzca el numero (Max: 26): ";
        cin >> columnas;
    }

    if (filas <= 0 || columnas <= 0)
    {
        cout << "Dimensiones invalidas.\n";
        return 0;
    }

    int bytesReservados = 0;
    unsigned char* tablero = crearTablero(filas, columnas, bytesReservados);

    poblar_aleatorio(tablero, filas, columnas);

    int eliminacionesUsuario = 0;
    int fichasEliminadasTotales = 0;
    int combinacionesTotales = 0;
    int cascadasActuales = 0;
    int puntuacion = 0;

    // ESTABILIZAR TABLERO INICIAL
    cout << "\nEstabilizando tablero inicial...\n";
    // El bucle de cascadas ahora es responsabilidad del main
    while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
    {
        // Se resuelve todo en silencio al inicio para no generar spam visual.
    }
    
    // Reseteamos estadisticas despues de la inicializacion
    combinacionesTotales = 0;
    fichasEliminadasTotales = 0;
    cascadasActuales = 0;
    puntuacion = 0;

    int opcion = -1;

    while (opcion != 0)
    {
        cout << "\n=============================\n";
        cout << "          MENU\n";
        cout << "=============================\n";
        cout << "1. Mostrar tablero\n";
        cout << "2. Eliminar ficha\n";
        cout << "3. Agregar columna\n";
        cout << "4. Eliminar columna\n";
        cout << "5. Agregar fila\n";
        cout << "6. Eliminar fila\n";
        cout << "7. Mostrar bits\n";
        cout << "8. Mostrar estadisticas\n";
        cout << "0. Salir\n\n";
        cout << "Opcion: ";

        cin >> opcion;

        if (opcion == 1)
        {
            mostrar_tablero(tablero, filas, columnas);
        }
        else if (opcion == 2)
        {
            int fila;
            int columna;

            mostrar_tablero(tablero, filas, columnas);

            cout << "Fila (1-" << filas << "): ";
            cin >> fila;
            cout << "Columna (1-" << columnas << "): ";
            cin >> columna;

            fila--;
            columna--;

            bool eliminado = eliminar_ficha(tablero, fila, columna, filas, columnas);

            if (eliminado)
            {
                eliminacionesUsuario++;
                fichasEliminadasTotales++;

                cout << "\nMovimiento realizado. Aplicando gravedad...\n";
                
                // IMPORTANTE: Al eliminar una ficha manual queda un VACIO. 
                // Debemos hacer que caigan las fichas y rellenar ANTES de buscar cascadas.
                aplicar_gravedad_y_relleno(tablero, filas, columnas);
                mostrar_tablero(tablero, filas, columnas);

                cascadasActuales = 0; 
                
                // ORQUESTACION DE CASCADAS EN EL MAIN:
                while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
                {
                    cout << "\n--- Cascada #" << cascadasActuales << " terminada! ---\n";
                    mostrar_tablero(tablero, filas, columnas);
                }
                
                cout << "Tablero estable.\n";
            }
            else
            {
                cout << "\nPosicion invalida.\n";
            }
        }
        else if (opcion == 3)
        {
            if (columnas >= 36)
            {
                cout << "\nNo se pueden agregar mas columnas.\n";
                continue;
            }

            int posicion;
            cout << "Posicion de la nueva columna (1-" << columnas + 1 << "): ";
            cin >> posicion;
            posicion--;

            bool resultado = agregar_columna(tablero, filas, columnas, posicion, bytesReservados);

            if (resultado)
            {
                cout << "\nColumna agregada. Estabilizando...\n";
                cascadasActuales = 0;
                while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
                {
                    mostrar_tablero(tablero, filas, columnas);
                }
                mostrar_tablero(tablero, filas, columnas);
            }
            else
            {
                cout << "\nError al agregar la columna.\n";
            }
        }
        else if (opcion == 4)
        {
            int posicion;
            cout << "Columna a eliminar (1-" << columnas << "): ";
            cin >> posicion;
            posicion--;

            bool resultado = eliminar_columna(tablero, filas, columnas, posicion, bytesReservados);

            if (resultado)
            {
                cout << "\nColumna eliminada. Estabilizando...\n";
                cascadasActuales = 0;
                while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
                {
                    mostrar_tablero(tablero, filas, columnas);
                }
                mostrar_tablero(tablero, filas, columnas);
            }
            else
            {
                cout << "\nError al eliminar la columna.\n";
            }
        }
        else if (opcion == 5)
        {
            int posicion;
            cout << "Posicion de la nueva fila (1-" << filas + 1 << "): ";
            cin >> posicion;
            posicion--;

            bool resultado = agregar_fila(tablero, filas, columnas, posicion, bytesReservados);

            if (resultado)
            {
                cout << "\nFila agregada. Estabilizando...\n";
                cascadasActuales = 0;
                while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
                {
                    mostrar_tablero(tablero, filas, columnas);
                }
                mostrar_tablero(tablero, filas, columnas);
            }
            else
            {
                cout << "\nError al agregar la fila.\n";
            }
        }
        else if (opcion == 6)
        {
            int posicion;
            cout << "Fila a eliminar (1-" << filas << "): ";
            cin >> posicion;
            posicion--;

            bool resultado = eliminar_fila(tablero, filas, columnas, posicion, bytesReservados);

            if (resultado)
            {
                cout << "\nFila eliminada. Estabilizando...\n";
                cascadasActuales = 0;
                while (procesar_un_paso_cascada(tablero, filas, columnas, combinacionesTotales, fichasEliminadasTotales, cascadasActuales, puntuacion))
                {
                    mostrar_tablero(tablero, filas, columnas);
                }
                mostrar_tablero(tablero, filas, columnas);
            }
            else
            {
                cout << "\nError al eliminar la fila.\n";
            }
        }
        else if (opcion == 7)
        {
            mostrar_binario(tablero, filas, columnas);
        }
        else if (opcion == 8)
        {
            mostrar_estadisticas(filas, columnas, bytesReservados, eliminacionesUsuario, fichasEliminadasTotales, combinacionesTotales, cascadasActuales, puntuacion);
        }
        else if (opcion == 0)
        {
            cout << "\nFinalizando Sweet Crush...\n";
        }
        else
        {
            cout << "\nOpcion invalida.\n";
        }
    }

    liberar_tablero(tablero);
    return 0;
}


