#ifndef REGLAS_TABLERO_H
#define REGLAS_TABLERO_H

// Memoria de tablero
unsigned char obtenerFicha(const unsigned char* tablero, int indiceFicha);
void guardarFicha(unsigned char* tablero, int indiceFicha, unsigned char valor);
int calcular_bytes_necesarios(int totalFichas);
unsigned char* evaluarYRedimensionar(unsigned char* tableroViejo, int fichasActivas, int &bytesReservados);
void limpiar_bits_no_usados(unsigned char* tablero, int totalFichas, int bytesReservados);

// Inicialización y juego
void poblar_tablero(unsigned char* tab, int filas, int cols);
void poner_ficha(unsigned char* tab, int fil, int col, int cols, unsigned char val);
bool borrar_ficha_usr(unsigned char* tab, int fil, int col, int filas, int cols);

// Detección y cascadas
int detectar_marcas(unsigned char* tab, int filas, int cols, int &marcadas);
int limpiar_marcadas(unsigned char* tab, int filas, int cols);
void aplicar_gravedad(unsigned char* tab, int filas, int cols);
bool procesar_un_paso_cascada(unsigned char* tab, int filas, int cols, int &combos, int &eliminadas, int &cascadasActuales, int &pts);

// Geometría del tablero
bool insertar_col(unsigned char* &tab, int filas, int &cols, int pos, int &bytes_res);
bool quitar_col(unsigned char* &tab, int filas, int &cols, int pos, int &bytes_res);

#endif 