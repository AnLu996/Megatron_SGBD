#include "diskManager.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

DiskManager::DiskManager() : disco(0, 0, 0, 0, 0), freeSpaceMap(nullptr) {


}


DiskManager::DiskManager(bool tipo, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque, int tamRegistro) : disco(nroPlatos, nroPistas, nroSectores, bytesxSector, sectoresxBloque), tipoLongitud(tipo), longitudRegistro(tamRegistro) {
    this->platoAct = 1;
    this->superfAct = 'A';
    this->pistaAct = 1;
    this->sectorAct = 1;
    this->bloqueAct = 1;

    if (!tipo) {
        this->registrosxSector = disco.getBytesxSector() / this->longitudRegistro;
    }    

    freeSpaceMap = new int[sectoresxBloque];
    for (int i = 0; i < disco.getCantidadBloques(); i++) {
        freeSpaceMap[i] = disco.getTamañoBloque();
    }
}

void DiskManager::getInformacionBloque() {
    std::cout << "------------------------------------------------------" << std::endl;
    disco.printTamañoTotal();
    std::cout << "La cantidad de bloques es: " << disco.getCantidadBloques() << std::endl;
    std::cout << "Tamanio del bloque: " << disco.getTamañoBloque() << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
}

void DiskManager::setLongitudRegistro(int longitud) {
    this->longitudRegistro = longitud;
}

void DiskManager::crearEstructura() {

    int plato = 1;
    char superf = 'A';
    int pista = 1;
    int sector = 1;
    int bloque = 1;
    int temp = 1;

    // * Crear file sectores
    std::string carpetaSectores = RUTA_BASE + "\\Sectores"; 
    fs::remove_all(carpetaSectores); // Remueve carpetas anteriores con el mismo nombre    
    fs::create_directories(carpetaSectores); // Crea la carpeta directorios

    for (int i = 0; i < disco.getTotalSectores(); i++) {
        std::string archivoSector = carpetaSectores + "\\" + std::to_string(plato) + "-" + superf + "-" + std::to_string(pista) + "-" + std::to_string(sector) + ".txt";
        std::ofstream archivo(archivoSector);

        sector++;
        temp ++;
        
        if (temp > disco.getCantidadSectores()) {
            temp = 1;
            pista++;
            if (pista > disco.getCantidadPistas()) {
                pista = 1;
                superf = (superf == 'A') ? 'B' : 'A';
                if (superf == 'A') {
                    plato++;
                    if (plato > disco.getCantidadPlatos())
                        break;
                }
            }
        }
    }

    sector = 1;

    // *Crear los files de bloques
    std::string carpetaBloques = RUTA_BASE + "\\Bloques" ;
    fs::remove_all(carpetaBloques);
    fs::create_directories(carpetaBloques);

    for (int bloque = 1; bloque <= disco.getCantidadBloques(); bloque++)
    {
        std::string archivoBloque = carpetaBloques + "\\Bloque" + std::to_string(bloque) + ".txt";
        std::ofstream archivo(archivoBloque);
        if (!archivo.is_open()) {
            std::cerr << "Error al crear el archivo " << archivoBloque << '\n';
        }
        
        std::string contenido = std::to_string(plato) + "#" + std::to_string(sector) + ";" + std::to_string(sector + disco.getCantidadSectores());

        archivo << contenido << std::endl;

        sector += disco.getCantidadSectores();
        temp++;

        archivo.close();
    }

    std::cout << "La estructura del disco ha sido creada exitosamente." << std::endl;
}

/*void DiskManager::configurarDirectorio() {
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
}*/



void DiskManager::usarLongitudFija(std::string lineaArchivo) {
    /**
    Verificar si el bloque tiene espacio
        - Crear archivo de arreglo
    Verificar que el sector actual se encuentre entre el rango de la cabecera de bloque
    No es así: Actualiza el bloque (bloqueact++;)
    Llena el bloque con la transformación corresponfiente a formato deseado
    * ? llena el sector correspondiente
    */

    int espacioLibre = getEspacioLibreBloque(this->bloqueAct);
    if (espacioLibre <= this->longitudRegistro) {
        actualizarBloque();

    } else {
        
        std::string atributo;
        std::string registro;
        bool is_string = false;

        std::string archivoBloque = RUTA_BASE + "\\Bloques\\Bloque" + std::to_string(this->bloqueAct) + ".txt";
        std::ofstream archivoWriteBloque(archivoBloque, std::ios::app);

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
        archivoWriteBloque << registro << std::endl;  
        
        llenarLongitudFija(registro);
        
        archivoWriteBloque.close(); 
    }

    actualizarBloque();

    this->sectorAct++;
}

void DiskManager::llenarLongitudFija(const std::string& lineaArchivo) {
    bool sectorActualizado = false;

    do {
        std::string archivoSector = RUTA_BASE + "Sectores\\" + std::to_string(this->platoAct) + "-" + this->superfAct + "-" + std::to_string(this->pistaAct) + "-" + std::to_string(this->sectorAct) + ".txt";
        std::ifstream archivoReadSector(archivoSector);

        if (!archivoReadSector) {
            std::cerr << "Error al abrir el archivo: " << archivoSector << std::endl;
            return;
        }

        std::string linea;
        int espacioDisponible = 0;
        int numRegistro = 0;
        std::string espaciosLibres;

        std::getline(archivoReadSector, linea);
        std::stringstream ss(linea);
        std::string token;

        std::getline(ss, token, '#'); // Ignorar el identificador
        std::getline(ss, token, '#');
        espacioDisponible = std::stoi(token);
        std::cout << "espacioDisponible: " << espacioDisponible << std::endl;

        if (espacioDisponible >= this->longitudRegistro) {
            std::getline(ss, token, '#');
            if (!token.empty()) {
                espaciosLibres = removerPrimerElemento(token);
                std::stringstream ssToken(token);
                std::getline(ssToken, token, ',');
                numRegistro = std::stoi(token) + 1;
            } else {
                numRegistro = 0;
            }
            std::cout << "numRegistro: " << numRegistro << std::endl;
        } else {
            archivoReadSector.close();
            actualizarSector();  //! OJO
            sectorActualizado = true;
            continue;
        }

        archivoReadSector.close();

        if (numRegistro == 0) {
            std::ofstream archivoWriteSector(archivoSector, std::ios::app);
            if (!archivoWriteSector) {
                std::cerr << "Error al abrir el archivo para escritura: " << archivoSector << std::endl;
                return;
            }

            archivoWriteSector << lineaArchivo << std::endl;
            archivoWriteSector.close();
        } else {
            actualizarLineaLongitudFija(archivoSector, lineaArchivo, numRegistro);           
        }

        espacioDisponible -= this->longitudRegistro;

        std::cout << "espacioDisponible DESPUES DE RESTA: " << espacioDisponible << std::endl;

        CabeceraSector nuevaCabecera;
        nuevaCabecera.identificador = this->sectorAct;
        nuevaCabecera.espacioDisponible = espacioDisponible;
        nuevaCabecera.espaciosLibres = espaciosLibres;
        nuevaCabecera.referenciaSector = 0;
        reemplazarCabecera(archivoSector, nuevaCabecera);

        sectorActualizado = false;

    } while (sectorActualizado);
}

void DiskManager::actualizarLineaLongitudFija(const std::string& archivoSector, const std::string& nuevaLinea, int numRegistro) {
    std::ifstream archivoLeer(archivoSector);
    std::ofstream archivoTemporal("temp.txt");

    if (!archivoLeer) {
        std::cerr << "Error al abrir el archivo para lectura: " << archivoSector << std::endl;
        return;
    }
    if (!archivoTemporal) {
        std::cerr << "Error al crear archivo temporal" << std::endl;
        archivoLeer.close();
        return;
    }

    //std::cout << "numRegistrodentro: " << numRegistro << std::endl;

    std::string linea;
    int numeroLinea = 1;

    while (std::getline(archivoLeer, linea)) {
        std::cout << numeroLinea << " -- ";
        std::cout << linea << std::endl;
        if (numeroLinea == numRegistro) {
            archivoTemporal << nuevaLinea << std::endl;
        } else {
            archivoTemporal << linea << std::endl;
        }
        numeroLinea++;
    }

    archivoLeer.close();
    archivoTemporal.close();

    if (std::remove(archivoSector.c_str()) != 0) {
        std::cerr << "Error al eliminar el archivo original: " << archivoSector << std::endl;
    }
    if (std::rename("temp.txt", archivoSector.c_str()) != 0) {
        std::cerr << "Error al renombrar archivo temporal a original: " << archivoSector << std::endl;
    }
}

//!Por mejorar

/*
VERIFICAR LA CAPACIDAD DEL BLOQUE USANDO EL FREE SPACE MAP

Verificar si el bloque tiene espacio
si NO tiene espacio
    bloque ++
    lee cabecera y se asigna el primer sector señalado

bloque=- this->longitudRegistro;
sector++;
si sector cambia
    bloque = longitud de sectores sobrantes


*/
void DiskManager::actualizarBloque() {
    if(getEspacioLibreBloque(this->bloqueAct)) {
        this->bloqueAct++;

        std::string archivoBloque = RUTA_BASE + "\\Bloques\\Bloque" + std::to_string(this->bloqueAct) + ".txt";
        std::ifstream file(archivoBloque);

        if (!file) {
            std::cerr << "Error al abrir el archivo: " << archivoBloque << std::endl;
            return;
        }

        std::string linea;
        std::string token;
        std::stringstream ss(linea);
        std::getline(file, linea);
        std::getline(ss, token, '#');
        std::getline(ss, token, ',');
        this->sectorAct = std::stoi(token);
    } else {
        this->freeSpaceMap[this->bloqueAct] =- this->longitudRegistro;
    }
}

void DiskManager::actualizarSector() {
    this->sectorAct++;
    std::cout << "Actualizando al siguiente sector: " << sectorAct << std::endl;
}

std::string removerPrimerElemento(const std::string& cadena) {
    std::cout << cadena << std::endl;
    size_t pos = cadena.find(',');
    if (pos == std::string::npos) {
        return "";
    }
    return cadena.substr(pos + 1);
}


















int DiskManager::getEspacioLibreBloque(int bloque) {
    return freeSpaceMap[bloque-1];
}


void DiskManager::updateEspacioLibreBloque(int bloque, int espacioUtilizado) {
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

DiskManager::~DiskManager() {
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


// *OPERACIONES CON EL STRUCT
std::string cabeceraToString(const CabeceraSector& cabecera) {
    std::stringstream ss;
    ss << cabecera.identificador << "#" << cabecera.espacioDisponible << "#" << cabecera.espaciosLibres << "#" << cabecera.referenciaSector << "#";
    return ss.str();
    
}

void eliminarLinea(const std::string& archivoEntrada, const std::string& archivoSalida, int numLineaEliminar) {
    std::ifstream archivoLectura(archivoEntrada);
    if (!archivoLectura) {
        std::cerr << "No se pudo abrir el archivo para leer: " << archivoEntrada << std::endl;
        return;
    }

    std::ofstream archivoEscritura(archivoSalida);
    if (!archivoEscritura) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << archivoSalida << std::endl;
        return;
    }

    std::string linea;
    int numeroLinea = 0;
    while (std::getline(archivoLectura, linea)) {
        numeroLinea++;
        if (numeroLinea != numLineaEliminar) {
            archivoEscritura << linea << std::endl;
        }
    }

    archivoLectura.close();
    archivoEscritura.close();

    // Renombrar el archivo de salida al nombre del archivo original
    std::rename(archivoSalida.c_str(), archivoEntrada.c_str());
}

void reemplazarCabecera(const std::string& archivo, const CabeceraSector& nuevaCabecera) {
    std::string archivoTemporal = "temporal.txt";
    std::ifstream archivoRead(archivo);
    if (!archivoRead) {
        std::cerr << "No se pudo abrir el archivo para leer: " << archivo << std::endl;
        return;
    }

    std::ofstream archivoTempWrite(archivoTemporal);
    if (!archivoTempWrite) {
        std::cerr << "No se pudo abrir el archivo temporal para escribir: " << archivoTemporal << std::endl;
        return;
    }

    std::string linea;
    if (std::getline(archivoRead, linea)) { } // Cabecera original leída y descartada

    // Leer el contenido original del archivo y escribirlo al archivo temporal
    while (std::getline(archivoRead, linea)) {
        archivoTempWrite << linea << std::endl;
    }

    archivoRead.close();
    archivoTempWrite.close();

    // Abrir el archivo original en modo de escritura para reemplazar la cabecera
    std::ofstream archivoWrite(archivo);
    if (!archivoWrite) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << archivo << std::endl;
        return;
    }

    // Escribir la nueva cabecera en el archivo original
    archivoWrite << cabeceraToString(nuevaCabecera) << std::endl;

    // Leer el contenido del archivo temporal y escribirlo al archivo original
    std::ifstream archivoTempRead(archivoTemporal);
    if (!archivoTempRead) {
        std::cerr << "No se pudo abrir el archivo temporal para leer: " << archivoTemporal << std::endl;
        return;
    }

    while (std::getline(archivoTempRead, linea)) {
        archivoWrite << linea << std::endl;
    }

    archivoTempRead.close();
    archivoWrite.close();

    std::remove(archivoTemporal.c_str());
}