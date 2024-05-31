#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

/*
    * Detecta el tipo de dato y retorna su tamanio
*/
int scaner(std::string str) {
	int c;
	int i = 0;  

    if (str.empty()) return 0; //Si la cadena está vacía 

    c = str[i++];

	if (std::isalpha(c) || c == '"') {
        i = 0;

        if (c == '"') c = str[i++];  // Si es una comilla, avanzar al siguiente carácter

        do {
            c = str[i++];
        } while (std::isalnum(c) || c == '_');

        // No es necesario agregar el carácter nulo '\0' al final de la cadena en C++

        if (i > 0) return i;
        else if (i == 1) return 1;
    }

	if(isdigit(c))   //regla del NUM
	{
		do{
		   c = str[i++];
		}while(isdigit(c));

		if ( i > 1) return 4 * i;
        else if (i == 1) return 1;
	}

    return 0;
}

/*
    * Calcula el tamaño de un resgitro (LONGITUD FIJA)
*/
int calcularTamanioRegistro(const std::string& registro) {
    std::ifstream file(registro);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << registro << std::endl;
        return -1;
    }
    
    std::string linea;
    int tamanioLinea;
    int tamanioRegistro = 0;
    std::getline(file, linea);
    
    // Leer el archivo línea por línea
    while (std::getline(file, linea)) {
        std::istringstream ss(linea);
        std::string atributo;

        tamanioLinea = 0;        

        while (std::getline(ss, atributo, ',')) {
            // Elimina comillas al inicio y al final, si existen
            tamanioLinea = tamanioLinea + scaner(atributo);            
        }


        if (tamanioLinea > tamanioRegistro) {
            tamanioRegistro = tamanioLinea;
        }
    }
    return tamanioRegistro;
}

