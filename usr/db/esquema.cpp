#include "esquema.h"

#include <iostream>
#include <string>
#include <fstream>

Esquema::Esquema () {
    this->nombreArchivo = "esquema.txt";

    std::ifstream archivo(nombreArchivo);
    
    if (archivo.good()) {
        std::cout << "El archivo '" << nombreArchivo << "' ya existe." << std::endl;
    } else {
        std::ofstream nuevoArchivo(nombreArchivo);
        if (nuevoArchivo.is_open()) {
            nuevoArchivo << "Este es un nuevo archivo creado." << std::endl;
            nuevoArchivo.close();
            std::cout << "El archivo ha sido creado." << std::endl;
        } else {
            std::cerr << "Error al crear el archivo." << std::endl;
        }
    }
    archivo.close();
}

void Esquema::agregarEsquema() {
    std::fstream archivo(this->nombreArchivo, std::ios::in | std::ios::out);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string esquema;
    std::string respuesta;
    int n;

    cout << "Ingrese el NOMBRE del esquema: ";
    cin >> respuesta;
    esquema += respuesta + "#";

    cout << "Ingrese el NOMBRE del atributo: " << n;
        cin >> respuesta;
        esquema += respuesta + "#";

        cout << "Ingrese el TIPO del atributo:";
        cin >> respuesta;
        esquema += respuesta + "#";

    while (respuesta != '0') {
        cout << "Ingrese el NOMBRE del atributo" << n << "(escriba 0 si desea salir):";
        cin >> respuesta;
        esquema += respuesta + "#";

        cout << "Ingrese el TIPO del atributo " << respuesta << ":";
        cin >> respuesta;
        esquema += respuesta + "#";
    }

    archivo << esquema << std::endl;

    archivo.close();
}

void Esquema::validarEsquema() {
    int i = 0;
    int j = 0;
    
    std::ifstream archivo("esquema.txt");
    if (!archivo) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    std::string linea;
    while (std::getline(archivo, linea)) { 
        while (linea[i] != '#') {
            j = 0;
            if (linea[i] != ' ') {
                texto.push_back(linea[i]);
                j++;
            }
            else if (linea[i] == '#') {  /*CORREGIR LOS QUE ESTA DENTRO, PUESTO QUE DEBE VALIDAR*/
                if (std::regex_match(texto, regexSimbolos)) {
                    std::cout << "simbolo = " << texto << std::endl;
                }
                else if (std::regex_match(texto, regexNumeros)) {
                    std::cout << "numero = " << texto << std::endl;
                }
                else if (std::regex_match(texto, regexAlfanumericos)) {
                    std::cout << "palabra = " << texto << std::endl;
                }
                texto.clear();
            }
            else {
                std::cout << "Ocurrió un error con el esquema, no tiene la sintaxis adecuada. \n";
                archivoSalida << '' << std::endl;

            }

            i++;
        }
        i = 0;
        //std::cout << std::endl;
}

void Esquema::eliminarEsquema(std::string lineaActual) {
    std::fstream archivo(this->nombreArchivo, std::ios::in | std::ios::out);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    std::getline(archivo, lineaActual);
    while (std::getline(archivoEntrada, lineaActual)) {
        if (lineaActual != lineaAEliminar) {
            archivoSalida << lineaActual << std::endl; 
        }
    }

    archivo.close();
}


