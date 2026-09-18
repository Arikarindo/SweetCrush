#ifndef REGLAS_TABLERO_H
#define REGLAS_TABLERO_H

// Inicialización y juego
void poblar_tablero(unsigned char* tab, int filas, int cols);
void poner_ficha(unsigned char* tab, int fil, int col, int cols, unsigned char val);
bool borrar_ficha_usr(unsigned char* tab, int fil, int col, int filas, int cols);

// Detección y cascadas
int detectar_marcas(unsigned char* tab, int filas, int cols, int &marcadas);
int limpiar_marcadas(unsigned char* tab, int filas, int cols);
void aplicar_gravedad(unsigned char* tab, int filas, int cols);
void procesar_cascadas(unsigned char* tab, int filas, int cols, int &combos, int &eliminadas, int &pts);

// Geometría del tablero
bool insertar_col(unsigned char* &tab, int filas, int &cols, int pos, int &bytes_res);
bool quitar_col(unsigned char* &tab, int filas, int &cols, int pos, int &bytes_res);

#endif 