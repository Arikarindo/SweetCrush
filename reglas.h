#ifndef REGLAS_H
#define REGLAS_H

void set_candy(unsigned char* tablero,
               int fila,
               int columna,
               int columnas,
               unsigned char valor);

void poblar_aleatorio(unsigned char* tablero,
                      int filas,
                      int columnas);

bool eliminar_ficha_usuario(unsigned char* tablero,
                            int fila,
                            int columna,
                            int filas,
                            int columnas);

int detectar_y_marcar_combinaciones(unsigned char* tablero,
                                     int filas,
                                     int columnas,
                                     int& fichasMarcadas);

int eliminar_fichas_marcadas(unsigned char* tablero,
                             int filas,
                             int columnas);

void aplicar_gravedad_y_relleno(unsigned char* tablero,
                                int filas,
                                int columnas);

bool procesar_un_paso_cascada(unsigned char* tablero,
                              int filas,
                              int columnas,
                              int& combinacionesTotales,
                              int& fichasEliminadasTotales,
                              int& cascadasActuales,
                              int& puntuacion);

bool agregar_columna(unsigned char*& tablero,
                     int filas,
                     int& columnas,
                     int posicion,
                     int& bytesReservados);

bool eliminar_columna(unsigned char*& tablero,
                      int filas,
                      int& columnas,
                      int posicion,
                      int& bytesReservados);

#endif
