#ifndef DISCO_H
#define DISCO_H

#include <iostream>
#include <string>
#include <sstream>

class Disco {
private:
    int nroPlatos;
    int nroSuperficies;
    int nroPistas;
    int nroSectores;
    int bytesxSector;
    int sectoresxBloque;
    std::string rutaBase;

public:
    Disco();
    Disco(int, int, int, int, int);
    void getTamañoTotal();
    //long int getTamañoSobrante(long int);
    int getCantidadBloques();
    int getCantidadSectores();
    int getCantidadSectoresBloque();
    int getTamañoBloque();
};

struct Registro {
    bool partition; //Indica si pertenece a uno o más registros
    int space; //Indica si el registro apun cuenta con espacio
};

#endif
