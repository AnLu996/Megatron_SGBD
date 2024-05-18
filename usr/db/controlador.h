#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "disco.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <tuple>

const std::string RUTA_BASE = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\";

class Controlador {
    private:
        int sectores;
        int bloqueAct;
        int* freeSpaceMap;
        std::string rutaBase;

//---------------------------------------------------------
        Disco disco;
        bool tipo; //TRUE si es longitud variable, FALSE si es fija
        int longitudRegistro;
        int cantSectoresxBloque;
        int cantBloques;
        int totalSectores;

    public:
        Controlador();
        
        Controlador(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque);

        void getInformacion();

        void setLongitudRegistro(int); //Indica la longitud de registro en caso se considere un disco de LONGITUD FIJA

        void crearEsquema(); //Crea la carpte con la cantidad de sectores establecido y bloques

        void configurarDirectorio(); //Llenar el directorio con información de bloques

        //void guardarInfoEnSectores(); //Los registros se guardan en los sectores

        //void agregarBloquesEsquema(std::string);

        int getEspacioLibreBloque(int);

        void updateEspacioLibreBloque(int, int);


        //------------------------------------------

        /*void asignarEspacio(int, int, int, int, int);
        void crearBloque();
        void modificarBitMap();
        
        Registro recuperarRegistro(int);*/
        ~Controlador();
};

#endif