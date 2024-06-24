#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "disk.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <tuple>
#include <list>
#include <optional>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

#define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"

struct Nodo {
    int numeroBloque;
    int espacioLibre;
    std::unordered_map<int, std::tuple<int, char, int, bool>> sectores; // <idSector, <plato, superficie, pista, espacioSector>>

    Nodo* prev;
    Nodo* next;

    Nodo(int numBloque, int espLibre)
        : numeroBloque(numBloque), espacioLibre(espLibre), prev(nullptr), next(nullptr) {
            std::cout << "Insertando bloque a nodo --> Numero de bloque: " << numeroBloque << ", Espacio libre: " << espacioLibre << std::endl;
        }
    
    void agregarSector(int idSector, int plato, char superficie, int pista) {
        sectores[idSector] = std::make_tuple(plato, superficie, pista, false);
    }
};

struct CabeceraSector {
    std::optional<int> identificador;
    std::optional<int> espacioDisponible;
    std::optional<std::string> espaciosLibres;
    std::optional<int> numRegistros;

    // Función para convertir la cabecera a una cadena
    std::string toString() const {
        std::string cabecera;
        if (identificador.has_value()) {
            cabecera += std::to_string(identificador.value()) + "#";
        }
        if (espacioDisponible.has_value()) {
            cabecera += std::to_string(espacioDisponible.value()) + "#";
        }
        if (espaciosLibres.has_value()) {
            cabecera += espaciosLibres.value() + "#";
        }
        if (numRegistros.has_value()) {
            cabecera += std::to_string(numRegistros.value()) + "#";
        }
        return cabecera;
    }
};

std::string removerPrimerElemento(const std::string&);


class DiskManager {
    private:        
        Disk disco; // Declaración de clase disco
        bool tipoLongitud; //TRUE si es longitud variable, FALSE si es fija
        int longitudRegistro; //Variable que guarda la longitud de registro en caso sea de LONGITUD FIJA

        int platoAct;
        char superfAct;
        int pistaAct;
        int sectorAct;
        int bloqueAct;

        //HeapFile -- FreeSpace
        Nodo* freeSpaceInicial; //Puntero hacia el nodo inicial de la linked list
        Nodo* freeSpaceFinal; //Puntero hacia el nodo final de la linked list

        //HeapFile -- FullSpace
        Nodo* fullSpaceInicial; //Puntero hacia el nodo inicial de la linked list
        Nodo* fullSpaceFinal; //Puntero hacia el nodo final de la linked list


        int* tipoCampo; //Puntero hacia un arreglo de tipos de campo del esquema seleccionado


        // * MÉTODOS




    public:
        DiskManager();
        DiskManager(bool longitud, int nroPlatos, int nroPistas, int nroSectores, int bytesxSector, int sectoresxBloque);

        // ================ GENERAL =========================

        void showInformation(); //Imprime la información del disco

        void getBlockInformation(); //Imprime la cantidad de bloque y la longitud del mismo

        void validarUbicacionActual(); //Hace el incremento de valores Act en caso lo necesite. Valida ubicaciones

        void createStructureDisk(); //Crea la carpetas con la cantidad de sectores y bloques establecido

        void setCurrentScheme(std::string nameScheme);

        std::string getScheme();

        
        //void agregarBloquesEsquema(std::string); //? ni idea





        void showBlockContent(int); //Imprime el contenido de un bloque
        void showSectorContent(int, char, int, int); //Imprime el contenido de un sector

        std::vector<std::string> readBlockToVector(int);


        // ================= HEAP FILE ==========
        void insertBlocktoFreeHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int, bool>>&);
        void insertBlocktoFullHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int, bool>>&);
        void showFullHeapFile();

        void printBlockInformation(Nodo*);
        void printBlockHeapFile(int);
        Nodo* searchFreeSpace(int);
        Nodo* searchFullSpace(int);
        Nodo* searchBlockHeapFile(int);
        void decreaseSpaceofBlock(int);
        void increaseSpaceofBlock(int);
        void deleteBlockHeapFile(Nodo*&, Nodo*&, Nodo*);
        void moveBlockFreeToFull(Nodo*);
        void moveBlockFullToFree(Nodo*);

        void vaciarHeapFile(Nodo*&, Nodo*&); //Sobrecarga de funciones
        void vaciarHeapFile();

        // !OJO - NO ASEGURA BUEN FUNCIONAMIENTO
        void saveHeapFile();
        void saveInformationInFile(Nodo*, std::ofstream&);
        void recoverInformationFromHeapFile();


        // ================= LONGITUD FIJA ==================

        void setSizeScheme(const std::string&);
        void setLongitudRegistro(int); //Indica la longitud de registro en caso se considere un disco de LONGITUD FIJA

        void useLongitudFija(const std::string);
        void sectorFillLongitudFija(const std::string&, int, Nodo*&);
        void actualizarLineaLongitudFija(const std::string&, const std::string&, int);


        void actualizarSector();






        // ================= LONGITUD VARIABLE ==============


        // ================= DE USO GENERAL - ARCHIVOS ======



        ~DiskManager();
};

#endif