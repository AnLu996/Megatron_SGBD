#include "controlador.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

Controlador::Controlador() : disco(0, 0, 0, 0, 0), freeSpaceMap(nullptr) {


}


Controlador::Controlador (bool tipo, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque) : disco(nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque), tipo(tipo), cantSectoresxBloque(sectoresxBloque) {
    this->totalSectores = nroPlatos * nroPistas * nroSectores;
    this-> cantBloques = this->totalSectores / sectoresxBloque;

    freeSpaceMap = new int[sectoresxBloque];
    for (int i = 0; i < cantBloques; i++) {
        freeSpaceMap[i] = disco.getTamañoBloque();
    }
}

void Controlador::getInformacion() {
    std::cout << "------------------------------------------------------" << std::endl;
    disco.getTamañoTotal();
    std::cout << "La cantidad de bloques es: " << disco.getCantidadBloques() << std::endl;
    std::cout << "Tamanio del bloque: " << disco.getTamañoBloque() << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
}

void Controlador::setLongitudRegistro(int longitud) {
    this->longitudRegistro = longitud;
}

void Controlador::crearEsquema() {
    std::string carpetaSectores = RUTA_BASE + "\\Sectores" ;
    fs::remove_all(carpetaSectores);
    fs::create_directories(carpetaSectores);

    for (int sector = 1; sector <= this->totalSectores; sector++) {
        std::string archivoSector = carpetaSectores + "\\Sector" + std::to_string(sector) + ".txt";
        std::ofstream archivo(archivoSector);
        if (!archivo.is_open()) {
            std::cerr << "Error al crear el archivo " << archivoSector << '\n';
        }
        archivo.close();
    }

    std::string carpetaBloques = RUTA_BASE + "\\Bloques" ;
    fs::remove_all(carpetaBloques);
    fs::create_directories(carpetaBloques);

    for (int bloque = 1; bloque <= this->cantBloques; bloque++)
    {
        std::string archivoBloque = carpetaBloques + "\\Bloque" + std::to_string(bloque) + ".txt";
        std::ofstream archivo(archivoBloque);
        if (!archivo.is_open()) {
            std::cerr << "Error al crear el archivo " << archivoBloque << '\n';
        }
        archivo.close();
    }
    
    std::cout << "La estructura del disco ha sido creada exitosamente." << std::endl;
}

void Controlador::configurarDirectorio() {
    std::ofstream diccionarioFile(RUTA_BASE + "directorio.txt");
    if (!diccionarioFile.is_open()) {
        std::cerr << "Error al abrir el archivo directorio.txt" << std::endl;
        return;
    }
    
    int bloque = 1;
    int totalSectores = this->totalSectores;

    while (totalSectores > 0 && bloque <= totalSectores) { // Añadimos la condición bloque <= totalSectores
        diccionarioFile << std::to_string(bloque) + '#';
        for(int sector = 1; sector <= this->cantSectoresxBloque; sector++) { // Cambiamos sectores++ a sector++
            diccionarioFile << std::to_string(sector);
            if (sector < cantSectoresxBloque || totalSectores > 1) {
                diccionarioFile << ',';
            }
            totalSectores--;
        }
        diccionarioFile << std::endl;
        bloque++;
    }

    diccionarioFile.close();
}

int Controlador::getEspacioLibreBloque(int bloque) {
    return freeSpaceMap[bloque-1];
}


void Controlador::updateEspacioLibreBloque(int bloque, int espacioUtilizado) {
    freeSpaceMap[bloque-1] -= espacioUtilizado;
}






/*

void Controlador::ingresarLongitudEsquema(std::string esquema, int tamañoRegistro) {
    std::ifstream fileEsquema(this->rutaBase + "esquema.txt");
    std::ofstream outputFile(this->rutaBase + "esquema_temp.txt", std::ios::app); // Abre el archivo en modo de escritura al final

    if (!fileEsquema.is_open() || !outputFile.is_open()) {
        std::cerr << "Error al abrir los archivos." << std::endl;
        return;
    }

    std::string lineaEsquema;
    std::string encabezadoEsquema;

    while (std::getline(fileEsquema, lineaEsquema)) {
        size_t pos = lineaEsquema.find('#');
        encabezadoEsquema = lineaEsquema.substr(0, pos);
        std::istringstream esquema_ss(lineaEsquema);
        std::string texto;
        std::getline(esquema_ss, texto, '#');
        
        if (encabezadoEsquema == esquema) {
            // Agregar información adicional al final de la línea
            lineaEsquema += '/' + tamañoRegistro;
        }

        // Escribir la línea modificada en el archivo de salida
        outputFile << lineaEsquema << std::endl;
    }

    fileEsquema.close();
    outputFile.close();

    // Renombrar el archivo temporal al original
    std::rename((this->rutaBase + "esquema_temp.txt").c_str(), (this->rutaBase + "esquema.txt").c_str());
}

/*void Controlador::generarBloque() {
    std::string carpeta = "Bloques";
    fs::remove_all(carpeta);
    crearCarpeta(carpeta);
    this->sectores = disco.getCantidadSectores();
    int sectoresxBloque = disco.getCantidadSectoresBloque();
    int cont = 1;

    for (int i = 1; i <= this->cantBloques; ++i) {
        std::string nombreArchivo = this->rutaBase + carpeta + "/bloque" + std::to_string(i) + ".txt";
        std::ofstream archivo(nombreArchivo);

        if (archivo.is_open()) {
            // Estructura de ingreso a archivo
            archivo << this->bloqueAct << "#" << cont << "#" << std::endl;

            archivo.close();
        } else {
            std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        }
        cont += sectoresxBloque;
    }
}

void llenarDirectorioBloques(int bloque, )


void Controlador::generarBloque() {
    std::string carpeta = "Bloques";
    fs::remove_all(carpeta);
    crearCarpeta(carpeta);
    this->sectores = disco.getCantidadSectores();
    int sectoresxBloque = disco.getCantidadSectoresBloque();
    int cont = 1;

    for (int i = 1; i <= this->cantBloques; ++i) {
        std::string nombreArchivo = this->rutaBase +  carpeta + "\\bloque" + std::to_string(this->bloqueAct) + ".txt";
        std::ofstream archivo(nombreArchivo);
        

        if (archivo.is_open()) {
            archivo << this->bloqueAct << "#" << cont << "#" << std::endl;

            archivo.close();
        } else {
            std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        }
        cont = cont + sectoresxBloque;
    }
}




//---------------------------------------------------------------------------



void asignarEspacio(int idRegistro, int plato, int superficie, int pista, int sector) {
    
    
    //diccionarioBloques[idRegistro] = std::make_tuple(plato, superficie, pista, sector);




    //disco.almacenarRegistro(idRegistro, plato, superficie, pista, sector);
    
}

Registro Controlador::recuperarRegistro(int idRegistro) {
    auto ubicacion = diccionarioBloques.find(idRegistro);
    if (ubicacion != diccionarioBloques.end()) {
        auto [plato, superficie, pista, sector] = ubicacion->second;
        //return disco.recuperarRegistro(plato, superficie, pista, sector);
    } else {
        std::cerr << "Registro no encontrado." << std::endl;
        return Registro{-1 };
    }
}

void Controlador::crearCarpeta(const std::string& carpeta) {
    if (!fs::exists(carpeta)) {
        fs::create_directory(carpeta);
    }
}*/

Controlador::~Controlador() {
    delete[] freeSpaceMap;

    std::string pathBloque = RUTA_BASE + "\\Bloques";
    std::string pathSector = RUTA_BASE + "\\Sectores";

    /*try {
        // Verificar si la carpeta existe
        if (fs::exists(pathBloque) && fs::is_directory(pathBloque) && fs::exists(pathSector) && fs::is_directory(pathSector) ) {
            // Eliminar la carpeta y su contenido
            fs::remove_all(pathBloque);
            fs::remove_all(pathSector);
            std::cout << "Carpetas eliminada correctamente.\n";
        } else {
            std::cerr << "Las carpetas no existen o no es una carpeta válida.\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error al eliminar la carpeta: " << e.what() << '\n';
    }*/
}