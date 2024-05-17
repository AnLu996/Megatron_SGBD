#include "megatron.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#define MAX_ENCABEZADOS 15

/*void trim(std::string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    str = str.substr(first, (last - first + 1));
}*/

Megatron::Megatron() {
    this->rutaBase = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\";
    this->nEsquema = "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\esquema.txt";

    std::ifstream archivo(nEsquema);
    
    if (!archivo.good()) {
        std::ofstream nuevoArchivo;
        nuevoArchivo.open(nEsquema);
        if (nuevoArchivo.is_open()) {
            nuevoArchivo.close();
            std::cout << "El archivo ha sido creado." << std::endl;
        } else {
            std::cout << "Error al crear el archivo." << std::endl;
        }
    }
    archivo.close();
}

void Megatron::setEsquema(const std::string nombreEsquema){
    this->esquemaActual = nombreEsquema;
}

void Megatron::crearEsquemaDesdeCsv(const std::string file, int cant){
    std::cout << "Ruta completa: " << rutaBase + file + ".csv" << std::endl;
    std::ifstream readFile(rutaBase + file + ".csv");
    std::ofstream writeEsquema(this->nEsquema, std::ios::app);
    std::string esquema;
    int i = 0;

    if (readFile.is_open()) {
        
        //CREAR ESQUEMA CON CABECERA
        std::string lineaArchivo;
        std::string cabeza;
        std::string tipo;

        esquema += file;

        std::getline(readFile, lineaArchivo);
        
        while (lineaArchivo[i] != '\0') {
            if (lineaArchivo[i] == ',') {
                std::cout << "\tIndica el tipo de dato para '" << cabeza << "': " ;
                std::cin >> tipo;   
                esquema +=  + "#" + cabeza + "#" + tipo;
                cabeza.clear();
            }
            else {
                cabeza.push_back(lineaArchivo[i]);
            }
            i++;
        }
        writeEsquema << esquema << std::endl;
        writeEsquema.close();

        //CREAR RELACIÓN CON CONTENIDO
        std::string relacionArchivo;
        relacionArchivo = crearArchivoRelacion(file);
        std::ofstream writeRelacion(relacionArchivo);

        if (cant == 0) {
            while (std::getline(readFile, lineaArchivo)) {
                std::string atributo;
                std::string registro;
                bool is_string = false;

                for (char c : lineaArchivo) {
                    if (c == '"') {
                        is_string = !is_string;
                    } else if (c == ',') {
                        if (is_string) {
                            atributo.push_back(c);
                        } else {
                            if (!atributo.empty()) {
                                registro += atributo + "#";
                                atributo.clear();
                            } else {
                                registro += "NULL#"; 
                            }
                        }
                    } else {
                        atributo.push_back(c);
                    }
                }
                registro += atributo;
                writeRelacion << registro << std::endl;         
            }
        } else {
            for (int j = 0; j < cant; j++) {  // Cambiado el nombre de la variable i a j
                std::getline(readFile, lineaArchivo);
                std::string atributo;
                std::string registro;
                bool is_string = false;

                for (char c : lineaArchivo) {
                    if (c == '"') {
                        is_string = !is_string;
                    } else if (c == ',') {
                        if (is_string) {
                            atributo.push_back(c);
                        } else {
                            if (!atributo.empty()) {
                                registro += atributo + "#";
                                atributo.clear();
                            } else {
                                registro += "NULL#"; 
                            }
                        }
                    } else {
                        atributo.push_back(c);
                    }
                }
                registro += atributo;
                writeRelacion << registro << std::endl; 
            }
        }

        writeRelacion.close();
        readFile.close();

        std::cout << "Esquema agregado y relación creada exitosamente\n";
    }
    else {
        std::cerr << "Error al abrir el archivo." << std::endl;
    }
}

void Megatron::agregarEsquemaManual() {
    std::ofstream archivo(this->nEsquema);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    std::string esquema;
    std::string respuesta;
    int contador = 1;

    std::cout << "Ingrese el NOMBRE del esquema: ";
    std::getline(std::cin, respuesta);
    esquema += respuesta + "#";

    std::cout << "Ingrese el NOMBRE del atributo " << contador << ": ";
    std::getline(std::cin, respuesta);
    esquema += respuesta + "#";
    contador++;

    std::cout << "Ingrese el TIPO del atributo '" << respuesta << "': ";
    std::cin >> respuesta;
    esquema += respuesta + "#";

    while (true) {
        std::cout << "Ingrese el NOMBRE del atributo " << contador << " (escriba '0' si desea salir): ";
        std::getline(std::cin, respuesta);
        esquema += respuesta + "#";
        contador++;

        if(respuesta == "0") break;
        std::cout << "Ingrese el TIPO del atributo '" << respuesta << "': ";
        std::cin >> respuesta;
        esquema += respuesta + "#";
    }
    archivo << esquema << std::endl;
    archivo.close();
}

/*void Megatron::eliminarEsquema(std::string lineaActual) {
    std::ofstream archivo(this->nombreArchivo);
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
}*/

void Megatron::ingresarRelacionManual(bool tipo) {
    char rpta = 'S';
    std::string lineaEsquema;
    std::string encabezado;
    std::string tipoDato;
    std::string dato;

    /*if (!tipo) {
        this -> esquemaActual = crearArchivoRelacion(this->rutaBase + this->esquemaActual + ".txt");
    }*/

    std::ofstream writeFile(rutaBase + this->esquemaActual + ".txt", std::ios::app);
    if (!writeFile.is_open()) {
        std::cerr << "Error al abrir el archivo de relación\n";
        return;
    }
    std::cout << nEsquema << std::endl;
    std::ifstream readEsquema(this->nEsquema);
    if (!readEsquema.is_open()) {
        std::cerr << "Error al abrir el archivo de esquema\n";
        writeFile.close(); 
        return;
    }
    std::getline(readEsquema, lineaEsquema);
    std::cout << lineaEsquema << std::endl;

    while (std::getline(readEsquema, lineaEsquema)) {
        std::istringstream esquema_ss(lineaEsquema);
        std::getline(esquema_ss, encabezado, '#');
        

        std::cout << "Encabezado:" << encabezado << std::endl;
        std::cout << "Encabezado:" << this->esquemaActual << std::endl;
        if (encabezado == this->esquemaActual) {
            bool esPrimeraEntrada = true;
            while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
                std::cout << "Ingrese el dato para '" << encabezado << "': ";
                std::getline(std::cin, dato);
                if (esPrimeraEntrada) {
                    writeFile << dato;
                    esPrimeraEntrada = false;
                    std::getline(std::cin >> std::ws, dato);
                } else {
                    writeFile << "#" + dato;
                }
                

                std::cout << "Archivo llenado exitosamente\n";
            }
            writeFile << std::endl;
            break;
        }
    }
    readEsquema.close();
    writeFile.close();
}

void Megatron::ingresarDesdeArchivoCsv(const std::string file, int cant) {
    std::string lineaEsquema;
    std::string lineaArchivo;
    int i = 0;
    

    std::ofstream writeRelacion(this->rutaBase + this->esquemaActual, std::ios::app);
    std::ifstream readFile(this->rutaBase + this->nEsquema);
    std::ifstream archivo(this->rutaBase + file);

    if (cant == 0) {
        while(std::getline(readFile, lineaArchivo)) {
            std::string atributo;
            std::string registro;
            bool is_string = false;
            i = 0;
            
            for (char c : lineaArchivo) {
                if (c == '"') {
                    is_string = !is_string;
                } else if (c == ',') {
                    if (is_string) {
                        atributo.push_back(c);
                    } else {
                        if (!atributo.empty()) {
                            registro += atributo + "#";
                            atributo.clear();
                        } else {
                            registro += "NULL#"; 
                        }
                    }
                } else {
                    atributo.push_back(c);
                }
            }
            registro += atributo;
            writeRelacion << registro << std::endl;         
        }
    }
    else {
        for(int i = 0; i < cant; i++){
            std::getline(readFile, lineaArchivo);
            std::string atributo;
            std::string registro;
            bool is_string = false;
            i = 0;
            
            for (char c : lineaArchivo) {
                if (c == '"') {
                    is_string = !is_string;
                } else if (c == ',') {
                    if (is_string) {
                        atributo.push_back(c);
                    } else {
                        if (!atributo.empty()) {
                            registro += atributo + "#";
                            atributo.clear();
                        } else {
                            registro += "NULL#"; 
                        }
                    }
                } else {
                    atributo.push_back(c);
                }
            }
            registro += atributo;
            writeRelacion << registro << std::endl; 
        }
    }
    writeRelacion.close();
    readFile.close();
}

std::string Megatron::crearArchivoRelacion(std::string Esquema) {
    std::cout << "Creando relacion #" << Esquema << "...." << std::endl;

    std::string nombreRelacion = rutaBase + Esquema + ".txt";
    std::ofstream archivo(nombreRelacion);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreRelacion << std::endl;
        return "";
    }

    archivo.close();
    return nombreRelacion;
}
        
/*bool Megatron::eliminarRegistro(std::string query) {
    std::string nombreArchivo = this->esquemaActual;
    std::ifstream archivoIn(this->rutaBase + this->esquemaActual + ".txt");
    if (!archivoIn.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return false;
    }

    std::ofstream archivoTemp("temp.txt");
    if (!archivoTemp.is_open()) {
        std::cerr << "Error al crear el archivo temporal." << std::endl;
        archivoIn.close();
        return false;
    }

    std::string linea;
    bool lineaEliminada = false;
    while (std::getline(archivoIn, linea)) {
        trim(linea);

        if (linea.find(query) != std::string::npos) {
            std::cout << "Se elimino la línea: " << linea << std::endl;
            lineaEliminada = true;
        } else {
            archivoTemp << linea << std::endl; // Escribir la línea en el archivo temporal
        }
    }

    // Cerrar archivos
    archivoIn.close();
    archivoTemp.close();

    // Eliminar el archivo original y renombrar el temporal si se realizó alguna eliminación
    if (lineaEliminada) {
        if (std::remove(nombreArchivo.c_str()) != 0) {
            std::cerr << "Error al eliminar el archivo original." << std::endl;
            return false;
        }
        if (std::rename("temp.txt", nombreArchivo.c_str()) != 0) {
            std::cerr << "Error al renombrar el archivo temporal." << std::endl;
            return false;
        }
    } else {
        std::cout << "No se encontró ninguna línea para eliminar." << std::endl;
        std::remove("temp.txt"); 
    }

    std::cout << "Operación completada correctamente." << std::endl;
    return true;    
}*/


/*~Megatron() {
    delete[] arreglo_encabezados;
    delete[] arreglo_tipoDato;
}*/


