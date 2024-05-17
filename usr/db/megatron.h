#ifndef MEGATRON_H
#define MEGATRON_H

#include <iostream>
#include <string>
#include <sstream>

class Megatron {
    private:
        int nroEncabezados; //contador de encabezados ATRIBUTOS
        std::string esquemaActual;
        std::string rutaBase; //Ruta donde se alacenarán los archivos
        std::string ruta; // Nombre del txt
        std::string nombreFile; //Nombre del archivo que se leerá 
        std::string nEsquema; //Nombre del archivo en el que se guardan las relaciones
        std::string esquema; //

    public:
        Megatron(); //constructor
        //Sets
        void setEsquema(const std::string);
        void setRelacion(const std::string);

        //Esquema
        void crearEsquemaDesdeCsv(const std::string, int); //Crea ESQUEMA y RELACIÓN desde un .CSV
        void agregarEsquemaManual(); //Agregar esquema de forma MANUAL
        //void eliminarEsquema(); //POR IMPLEMENTARSE

        //Relaciones
        void ingresarRelacionManual(bool); //Ingresa el registro manual para el esquema
        void ingresarDesdeArchivoCsv(const std::string, int); //Ingresa registro desde un csv


        //Extras
        std::string crearArchivoRelacion(std::string); //crea el archivo de la relación R1, R2, ....  
        //bool eliminarRegistro(std::string);
        //void Megatron::trim(std::string&);

        
        //~Megatron();
};

#endif