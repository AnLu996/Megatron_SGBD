#ifndef RELACION_H
#define RELACION_H

#include <iostream>
#include <string>
//#include

class Relacion {
    private:
        int num; //número de relación
        std::string ruta;

    public:
        Relacion();
        void crearRelacion();
        std::string crearArchivo();
        void llenarDesdeArchivo(const std::string);
        void llenarManual();
        
        void eliminarRelacion();
};

#endif