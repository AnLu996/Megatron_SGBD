#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "disk.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <tuple>
#include <list>

#define RUTA_BASE "F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\"

struct Nodo {
    int numeroBloque;
    int espacioLibre;
    std::unordered_map<int, std::tuple<int, char, int>> sectores; // <idSector, <plato, superficie, pista>>

    Nodo* prev;
    Nodo* next;

    Nodo(int numBloque, int espLibre)
        : numeroBloque(numBloque), espacioLibre(espLibre), prev(nullptr), next(nullptr) {}
};

struct CabeceraSector {
    int identificador;
    int espacioDisponible;
    std::string espaciosLibres;
    int referenciaSector;
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

        void createStructureDisk(); //Crea la carpetas con la cantidad de sectores y bloques establecido

        void setCurrentScheme(std::string nameScheme);

        std::string getScheme();

        
        //void agregarBloquesEsquema(std::string); //? ni idea





        void showSectorContent(int, char, int, int); //Imprime el contenido de un sector



        // ================= HEAP FILE ==========
        void insertBlocktoFreeHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int>>&);
        void insertBlocktoFullHeapFile(int, int, const std::unordered_map<int, std::tuple<int, char, int>>&);
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

        void getSizeScheme(const std::string&);

        void setLongitudRegistro(int); //Indica la longitud de registro en caso se considere un disco de LONGITUD FIJA

        void usarLongitudFija(const std::string);

        void llenarLongitudFija(const std::string&);
        void llenarSectorLongitudFija(std::string);
        void actualizarLineaLongitudFija(const std::string&, const std::string&, int);
        void actualizarBloque();
        void actualizarSector();
        int getEspacioLibreBloque(int);
        void updateEspacioLibreBloque(int, int);






        // ================= LONGITUD VARIABLE ==============



        ~DiskManager();
};

#endif