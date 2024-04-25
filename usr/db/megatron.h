#ifndef MEGATRON_H
#define MEGATRON_H

#include <iostream>
#include <string>

class Megatron {
    private:
        int num; //número de relación
        std::string ruta;
        std::string nombreArchivo;

    public:
        Esquema();
        void agregarEsquema();
        void validarEsquema();
        void eliminarEsquema();
        Relacion();
        void crearRelacion();
        std::string crearArchivo();
        void llenarDesdeArchivo(const std::string);
        void llenarManual();
        
        void eliminarRelacion();
};

#endif