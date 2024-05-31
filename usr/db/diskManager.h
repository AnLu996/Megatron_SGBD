#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "disco.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <tuple>

const std::string RUTA_BASE = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\";

struct CabeceraSector {
    int identificador;
    int espacioDisponible;
    std::string espaciosLibres;
    int referenciaSector;
};

std::string removerPrimerElemento(const std::string&);


class DiskManager {
    private:
        int sectores;
        //int bloqueAct;
        int* freeSpaceMap;
        std::string rutaBase;

//---------------------------------------------------------

        
        Disco disco;
        bool tipoLongitud; //TRUE si es longitud variable, FALSE si es fija
        int longitudRegistro;
        int registrosxSector;

        int platoAct;
        char superfAct;
        int pistaAct;
        int sectorAct;
        int bloqueAct;

        int* freeSpaceMap;


        // * MÉTODOS 
        void llenarLongitudFija(const std::string&);
        void llenarSectorLongitudFija(std::string);
        void actualizarLineaLongitudFija(const std::string&, const std::string&, int);
        void actualizarBloque();
        void actualizarSector();
        int getEspacioLibreBloque(int);
        void updateEspacioLibreBloque(int, int);




    public:
        DiskManager();
        
        DiskManager(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque, int tamRegistro);

        void getInformacionBloque(); //Imprime la cantidad de bloque y la longitud del mismo

        void setLongitudRegistro(int); //Indica la longitud de registro en caso se considere un disco de LONGITUD FIJA

        void crearEstructura(); //Crea la carpetas con la cantidad de sectores y bloques establecido

        void configurarDirectorio(); //Llenar el directorio con información de bloques
        //todo: PROBABLEMENTE CAMBIE

        //void guardarInfoEnSectores(); //Los registros se guardan en los sectores ------ 
        //!CONVERSARLO - Agregar arreglo en txt de bloques libres y no disponibles

        void usarLongitudFija(const std::string);





        //void agregarBloquesEsquema(std::string); //? ni idea



        //------------------------------------------

        /*void asignarEspacio(int, int, int, int, int);
        void crearBloque();
        void modificarBitMap();
        
        Registro recuperarRegistro(int);*/
        ~DiskManager();
};

#endif