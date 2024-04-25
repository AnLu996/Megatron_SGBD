#ifndef ESQUEMA_H
#define ESQUEMA_H

#include <iostream>
#include <string>
#include <fstream>

class Esquema {
    protected:
        std::string nombreArchivo;
    public:
        Esquema();
        void agregarEsquema();
        void validarEsquema();
        void eliminarEsquema();
};

#endif