#ifndef REGLAS_TABLERO_H
#define REGLAS_TABLERO_H

// Memoria de tablero
unsigned char obtenerFicha(const unsigned char* tablero, int indiceFicha);
void guardarFicha(unsigned char* tablero, int indiceFicha, unsigned char valor);
int calcular_bytes_necesarios(int totalFichas);
unsigned char* evaluarYRedimensionar(unsigned char* tableroViejo, int fichasActivas, int &bytesReservados);
void limpiar_bits_no_usados(unsigned char* tablero, int totalFichas, int bytesReservados);

// Inicialización y limpieza de tablero
unsigned char* crearTablero(int filas, int columnas, int &bytesReservados);
void liberar_tablero(unsigned char* &tablero);

#endif 