#include "relacion.h"

#include <iostream>
#include <string>
#include <fstream>


Relacion::Relacion () {
    this->num = 0;
}

std::string Relacion::crearArchivo() {
    std::string ruta = "F:/UNSA/2024-A/Base de Datos II/Megatron/usr/db/rel" + std::to_string(this->num) + ".txt";
    std::ofstream archivo(ruta);    
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo\n";
        return "";
    }
    this->num++;
    archivo.close();
    return ruta;
}



void Relacion::crearRelacion() {
    this->ruta = crearArchivo();
    std::cout << "Creando relación # " << this->num << std::endl;
    std::ofstream archivo(ruta); 

    
}


void Relacion::llenarDesdeArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo\n";
            return;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Procesar cada línea del archivo para crear relaciones
            std::cout << "Creando relación desde archivo: " << linea << std::endl;
            // Aquí puedes implementar la lógica para crear las relaciones según la línea del archivo
        }

        archivo.close();
}

void Relacion::llenarManual();
        
void Reacion::eliminarRelacion();
    
