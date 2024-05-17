#include "controlador.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;


Controlador::Controlador(int porDefecto, bool longitud) : longitud(longitud), bloqueAct(1) {
    Disco disco;
    this->bloqueAct = 1;
    if (porDefecto == 1) {
        disco = Disco();
    } else {
        std::cout << "Ingresa la cantidad de platos: ";
        int nroPlatos;
        std::cin >> nroPlatos;
        
        std::cout << "Ingresa la cantidad de pistas x plato: ";
        int nroPistas;
        std::cin >> nroPistas;

        std::cout << "Ingresa la cantidad de sectores x pista: ";
        int nroSectores;
        std::cin >> nroSectores;

        std::cout << "Ingresa la cantidad de bytes x sector: ";
        int bytesxSector;
        std::cin >> bytesxSector;

        std::cout << "Ingresa la cantidad de sectores x bloque: ";
        int sectoresxBloque;
        std::cin >> sectoresxBloque;
        
        disco = Disco(nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque);
    }
    
    this->rutaBase = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"; 
    std::cout << "------------------------------------------------------" << std::endl;
    disco.getTamañoTotal();
    std::cout << "La cantidad de bloques es: " << disco.getCantidadBloques() << std::endl;
    std::cout << "Tamanio del bloque: " << disco.getTamañoBloque() << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    std::ofstream diccionarioFile("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\directorio.txt");
    if (!diccionarioFile.is_open()) {
        std::cerr << "Error al abrir el archivo directorio.txt" << std::endl;
    } else {
        std::cout << "Archivo directorio.txt creado correctamente." <<2 std::endl;
        diccionarioFile.close();
    }

    cantBloques = disco.getCantidadBloques();

    freeSpaceMap = new int[cantBloques];
    for (int i = 0; i < cantBloques; i++) {
        freeSpaceMap[i] = disco.getTamañoBloque();
    }
}

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
}*/

/*void llenarDirectorioBloques(int bloque, )*/

void crearCarpeta(const std::string& carpeta) {
    if (!fs::exists(carpeta)) {
        fs::create_directory(carpeta);
    }
}

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


int Controlador::getEspacioLibreBloque(int bloque) {
    return freeSpaceMap[bloque-1];
}

void Controlador::actualizarEspacioLibreBloque(int bloque, int espacioUtilizado) {
    freeSpaceMap[bloque-1] -= espacioUtilizado;
}




//---------------------------------------------------------------------------



void asignarEspacio(int idRegistro, int plato, int superficie, int pista, int sector) {
    
    
    //diccionarioBloques[idRegistro] = std::make_tuple(plato, superficie, pista, sector);




    //disco.almacenarRegistro(idRegistro, plato, superficie, pista, sector);
    
}

/*Registro Controlador::recuperarRegistro(int idRegistro) {
    auto ubicacion = diccionarioBloques.find(idRegistro);
    if (ubicacion != diccionarioBloques.end()) {
        auto [plato, superficie, pista, sector] = ubicacion->second;
        //return disco.recuperarRegistro(plato, superficie, pista, sector);
    } else {
        std::cerr << "Registro no encontrado." << std::endl;
        return Registro{-1 };
    }
}*/

void Controlador::crearCarpeta(const std::string& carpeta) {
    if (!fs::exists(carpeta)) {
        fs::create_directory(carpeta);
    }
}

Controlador::~Controlador() {
    delete[] freeSpaceMap;
}