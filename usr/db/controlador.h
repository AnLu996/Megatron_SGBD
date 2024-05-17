#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "disco.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <tuple>

class Controlador {
    private:
        Disco disco;
        int sectores;
        int cantBloques;
        int bloqueAct;
        int* freeSpaceMap;
        bool longitud; //TRUE si es longitud variable, FALSE si es fija
        std::string rutaBase;

    public:
        Controlador(int, bool);
        void ingresarLongitudEsquema(std::string, int);
        void agregarBloquesEsquema(std::string);
        int getEspacioLibreBloque(int);
        void actualizarEspacioLibreBloque(int, int);
        void generarBloque();
        void crearCarpeta(const std::string&);
        



        //------------------------------------------

        /*void asignarEspacio(int, int, int, int, int);
        void crearBloque();
        void modificarBitMap();
        
        Registro recuperarRegistro(int);*/
        ~Controlador();
};

#endif