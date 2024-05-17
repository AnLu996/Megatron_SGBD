#include "disco.h"

#include <iostream>
#include <fstream>
#include <string>

Disco::Disco(int platos, int pistas, int sectores, int tamañoSector, int tamañoBloque) {
    this->nroPlatos = platos;
    this->nroSuperficies = 2;
    this->nroPistas = pistas;
    this->nroSectores = sectores;
    this->bytesxSector = tamañoSector;
    this->sectoresxBloque = tamañoBloque;
    this->rutaBase = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"; 
}

void Disco::getTamañoTotal() {
    std::cout << "Nro. Platos: " << this->nroPlatos << std::endl;
    std::cout << "Nro. Superficies: " << this->nroSuperficies << std::endl;
    std::cout << "Nro. Pistas: " << this->nroPistas << std::endl;
    std::cout << "Nro. Sectores: " << this->nroSectores << std::endl;
    std::cout << "Bytes x Sector: " << this->bytesxSector << std::endl;
    std::cout << "Sectores x Bloque: " << this->sectoresxBloque << std::endl;

    long long int total = static_cast<long long int>(this->nroPlatos) *
                          this->nroSuperficies *
                          this->nroPistas *
                          this->nroSectores *
                          this->bytesxSector;

    std::cout << "Tamanio Total: " << total << std::endl;
}

/*long int Disco::getTamañoSobrante(long int espacioOcupado) const {
    return getTamañoTotal() - espacioOcupado;
}*/

int Disco::getCantidadBloques() {
    int cantidadBloques = nroPlatos * nroPistas * nroSectores * (bytesxSector / sectoresxBloque);
    return cantidadBloques;
}

int Disco::getCantidadSectores() {
    int cantidadSectores = this->nroSectores;
    return cantidadSectores;
}

int Disco::getCantidadSectoresBloque() {
    int cantidadSectoresxBloque = this->sectoresxBloque;
    return cantidadSectoresxBloque;
}

int Disco::getTamañoBloque() {
    int tamaño = this->bytesxSector * this->sectoresxBloque;
    return tamaño;
}