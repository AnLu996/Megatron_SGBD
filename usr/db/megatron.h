#ifndef MEGATRON_H
#define MEGATRON_H

#include "diskManager.h"
#include "BufferManager.h"

#include <iostream>
#include <string>
#include <sstream>

#define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"

class Megatron {
    private:
        DiskManager controladorDisco;
        BufferManager gestor;

        std::string esquemaActual; //Nombre del esquema actual
        



        int nroEncabezados; //contador de encabezados ATRIBUTOS
        
        std::string ruta; // Nombre del txt
        std::string nombreFile; //Nombre del archivo que se leerá 
        std::string nEsquema; //Nombre del archivo en el que se guardan las relaciones
        std::string esquema; //

    public:
        Megatron(); //constructor por defecto
        Megatron(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque, int frames); //constructor
        
        // Sets ----------------------------------------------------------------------------
        void setEsquema(const std::string);
        void setRelacion(const std::string);

        // Esquema -------------------------------------------------------------------------
        void crearEsquemaDesdeCsv(const std::string, int); //Crea ESQUEMA y RELACIÓN desde un .CSV
        void agregarEsquemaManual(); //Agregar esquema de forma MANUAL

        //void eliminarEsquema(); //POR IMPLEMENTARSE

        // Relaciones ----------------------------------------------------------------------
        void ingresarRelacionManual(bool); //Ingresa el registro manual para el esquema
        void ingresarDesdeArchivoCsv(const std::string, int); //Ingresa registro desde un csv


        //Extras ----------------------------------------------------------------------------
        std::string crearArchivoRelacion(std::string); //Crea el archivo de la relación R1, R2, ....  

        //bool eliminarRegistro(std::string);

        void menuDisco();
        void menuBuffer(); 

        
        //~Megatron();
};

#endif