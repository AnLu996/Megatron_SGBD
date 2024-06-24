#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>

class Frame {
private:
    int capacidad; 
    std::vector<std::string> data;
    bool isPinned;
    bool dirtyFlag;
    int pinCount;
    std::queue<char> countPin;
    int lastUsed; //LRU
    bool refBit; //CLOCK
public:
    /*
        @author Michael Ticona
    */
    std::vector<std::string> GetData();
    
    /*
        @author Michael Ticona
    */
    bool GetDirty();
    
    /*
        @author Michael Ticona
    */
    int GetPinCount();

    /*
        @author Michael Ticona
    */
    bool GetIsPinned();

    /*
        @author Michael Ticona
    */
    int GetLastUsed();

    /*
        @author Michael Ticona
    */
    bool GetRefBit();

    /*
        @author Michael Ticona
    */
    void SetData(std::string);

    /*
        @author Michael Ticona
    */
    void ViewData();

    /*
        @author Michael Ticona
    */
    void SetPinCount(char);

    /*
        @author Michael Ticona
    */
    void ReleasePinCount();

    /*
        @author Michael Ticona
    */
    void PrintPinCount();

    /*
        @author Michael Ticona
    */
    Frame(int);
    /*
    Constructor de la clase Frame. Inicializa un marco con valores por defecto: el ID de página se establece en -1,
    el marco no está "pinneado" (no está en uso), la bandera de suciedad está desactivada, el contador de "pines" 
    se establece en 0, y el tiempo de "último uso" se establece en 0.
    */

    /*
        @author Michael Ticona
    */
    void pin();
    /*
    Esta función marca el marco como "pinneado", lo que indica que está en uso. Incrementa el contador de "pines" 
    para este marco.
    */

    /*
        @author Michael Ticona
    */
    void IncrementCount();

    /*
        @author Michael Ticona
    */
    void DecrementCount();

    /*
        @author Michael Ticona
    */
    void unpin();
    /*
    Esta función reduce el contador de "pines" para el marco. Si el contador de "pines" llega a cero, marca el marco 
    como no "pinneado".
    */

    /*
        @author Michael Ticona
    */
    void setDirty();
    /*
    Establece la bandera en verdadero indicando que se a modificado el frame
    */

    /*
        @author Michael Ticona
    */
    void reset();
    /*
    Esta función restablece todos los atributos del marco a sus valores por defecto.
    */

    /*
        @author Michael Ticona
    */
    void increment();

    /*
        @author Michael Ticona
    */
    void refOn();

    /*
        @author Michael Ticona
    */
    void refOff();

    /*
        @author Michael Ticona
    */
    int getLastUsed();

    /*
        @author Michael Ticona
    */
    void used();
};