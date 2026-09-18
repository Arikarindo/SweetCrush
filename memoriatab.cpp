#include "matriz.h"
#include "constantes.h"

unsigned char* crearTablero(int filas, int columnas, int &bytesReservados) {
    int totalBits = filas * columnas * BITSPORFICHA;
    bytesReservados = (totalBits + 7) / 8; 

    unsigned char* tablero = new unsigned char[bytesReservados]();
    return tablero;
}

unsigned char obtenerFicha(const unsigned char* tablero, int indiceFicha) {
    int bitInicio = indiceFicha * BITSPORFICHA;
    int byteIdx = bitInicio / 8;
    int bitOffset = bitInicio % 8;

    unsigned short bloque = tablero[byteIdx];
    
    if (bitOffset > 5) { 
        bloque |= (tablero[byteIdx + 1] << 8);
    }

    return (bloque >> bitOffset) & 0x07;
}

void guardarFicha(unsigned char* tablero, int indiceFicha, unsigned char valor) {
    int bitInicio = indiceFicha * BITSPORFICHA;
    int byteIdx = bitInicio / 8;
    int bitOffset = bitInicio % 8;

    valor &= 0x07;

    if (bitOffset <= 5) {
        tablero[byteIdx] &= ~(0x07 << bitOffset);
        tablero[byteIdx] |= (valor << bitOffset);
    }
    else {
        int bitsPrimerByte = 8 - bitOffset;
        
        tablero[byteIdx] &= ~(0x07 << bitOffset);
        tablero[byteIdx] |= (valor << bitOffset);

        int bitsSegundoByte = BITSPORFICHA - bitsPrimerByte;
        unsigned char mascaraSegundoByte = (1 << bitsSegundoByte) - 1;

        tablero[byteIdx + 1] &= ~mascaraSegundoByte;
        tablero[byteIdx + 1] |= (valor >> bitsPrimerByte);
    }
}

unsigned char* evaluarYRedimensionar(unsigned char* tableroViejo, int fichasActivas, int &bytesReservados) {
    int totalBitsNecesarios = fichasActivas * BITSPORFICHA;
    int bytesUtilizados = (totalBitsNecesarios + 7) / 8;

    float porcentajeUso = (float)bytesUtilizados / bytesReservados;

    if (porcentajeUso < REDUCCION && bytesReservados > 1) {
        int nuevosBytesReservados = bytesUtilizados;
        unsigned char* tableroNuevo = new unsigned char[nuevosBytesReservados]();

        for (int i = 0; i < nuevosBytesReservados; ++i) {
            tableroNuevo[i] = tableroViejo[i];
        }

        delete[] tableroViejo;

        bytesReservados = nuevosBytesReservados;
        return tableroNuevo;
    }

    return tableroViejo;
}
