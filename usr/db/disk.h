#ifndef DISK_H
#define DISK_H

#include <iostream>
#include <string>
#include <sstream>

/* Manejo de Disco / espacio en disco*/
class Disk {
private:
    int nroPlatos;
    int nroSuperficies;
    int nroPistas;
    int nroSectores;
    int bytesxSector;
    int sectoresxBloque;
    std::string rutaBase;

public:
    Disk();
    Disk(int, int, int, int, int);
    void printTamañoTotal();
    //long int getTamañoSobrante(long int);

    // * Operaciones get sobre información del disco
    int getCantidadPlatos();
    int getCantidadPistas();
    int getCantidadSectores();
    int getBytesxSector();
    int getCantidadBloques();    
    int getCantidadSectoresxBloque();

    int getTamañoBloque();
    int getTotalSectores();

    // OTRO METODOS
    
    

    
};

struct Registro {
    bool partition; //Indica si pertenece a uno o más registros
    int space; //Indica si el registro apun cuenta con espacio
};

#endif
