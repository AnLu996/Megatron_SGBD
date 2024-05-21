#include "megatron.h"
#include "disco.h"
#include "discoManager.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <limits>

void trim(std::string &str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

bool esNumero(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false; // Si encuentra un carácter que no es un dígito, devuelve falso
        }
    }
    return true; // Si todos los caracteres son dígitos, devuelve verdadero
}

size_t encontrarSimbolo(const std::string& cadena, char simbolo) {
    return cadena.find_first_of(simbolo);
}

bool consulta(std::string& query) {
	std::ifstream esquema("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\esquema.txt");
    if (!esquema.is_open()) {
        std::cerr << "No se pudo abrir el archivo Esquema " << std::endl;
        return false;
    }

    std::ofstream fileConsulta("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\Consulta.txt");
	if (!fileConsulta.is_open()) {
        std::cerr << "No se pudo abrir el archivo Consulta.txt " << std::endl;
        esquema.close();
        return false;
    }

	if(query.size() >= 12 && query.substr(0,6) == "SELECT") {
		std::string texto;
		std::string linea;
		std::string lineaEsquema;
		std::string encabezadoEsquema;
		std::string encabezado;
		std::string tipoDato;
		int i= 0;

		if (query.substr(7, 1) == "*" && query.substr(9, 4) == "FROM") {
        	auto wherePos = std::find(query.begin() + 15, query.end(), 'W');
			auto barraPos = std::find(query.begin() + 18, query.end(), '|');
			if (barraPos != query.end() && std::string(wherePos, wherePos + 5) == "WHERE" && std::string(barraPos, barraPos + 1) == "|") {
				// SELECT * FROM tabla WHERE condicion | NombreArchivo
				std::cout << "La consulta contiene 'SELECT * FROM tabla WHERE condicion | Archivo'." << std::endl;

				std::string campo;
				std::string tipoDatoCampo;
				char signo;
				std::string comparar;
				int i = 0;

				size_t posHasta = query.find("WHERE");
				std::string tabla = query.substr(14, posHasta - 14);

				std::string condicion = query.substr(std::distance(query.begin(), wherePos) + 6, std::distance(wherePos, barraPos));
				std::cout << "Condición: " << condicion << std::endl;
				
				std::string file = query.substr(barraPos - query.begin() + 2);
				trim(file);

				std::ofstream outputFile("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + file + ".txt");
				if (!outputFile.is_open()) {
					std::cerr << "No se pudo abrir el archivo: " << file << std::endl;
					esquema.close();
					fileConsulta.close();
					return false;
				}

				while (condicion[i] != '<' && condicion[i] != '=' && condicion[i] != '>') {
					campo.push_back(condicion[i]);
					i++;
				}  
				std::cout << condicion[i] << std::endl;
				if (condicion[i] == 60 || condicion[i] == 61 || condicion[i] == 62) {
					signo = condicion[i];
					i++;
				}
				while (condicion[i] != '|') {
					comparar.push_back(condicion[i]);
					i++;
				}
				trim(comparar);

				int columna = -1; 
				int j = 0; 

				while (std::getline(esquema, lineaEsquema)) {
					size_t pos = lineaEsquema.find('#');
					encabezadoEsquema = lineaEsquema.substr(0, pos);
					std::istringstream esquema_ss(lineaEsquema);
					std::string texto;
					std::getline(esquema_ss, texto, '#');
					trim(tabla);
					if (encabezadoEsquema == tabla) {
						while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
							if (encabezado == campo) {
								columna = j;
								tipoDatoCampo = tipoDato; 
							}
							fileConsulta << encabezado + ' ';
							outputFile << encabezado + '#';
							j++;							
						}
						fileConsulta << std::endl;
						outputFile << std::endl;
						break;
					}
				}

				if (columna == -1) {
					std::cerr << "Campo no encontrado en la tabla." << std::endl;
					esquema.close();
					fileConsulta.close();
					return false;
				}
				esquema.close();
				texto.clear(); 

				std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
				
				std::string registro;
				
				while (std::getline(lectura, linea)) {
					registro = linea; 

					std::istringstream ss(linea);
					std::string valor;
					std::string valorCampo;
					int k = 0; 

					while (std::getline(ss, valor, '#')) {
						if (k == columna) {
							valorCampo = valor;
							break;
						}
						k++;
					}
					trim(valorCampo);
					bool cumpleCondicion = false;

					if (tipoDatoCampo == "string") {
						// Comparación de cadenas de texto
						if (signo == '=' && valorCampo == comparar) {
							cumpleCondicion = true;
						}
					} else if (tipoDatoCampo == "int" || tipoDatoCampo == "double") {
						// Comparación de números (int o double)
						if (esNumero(valorCampo) && esNumero(comparar)) {
							// Convertir las cadenas a números
							double valorRegistro = std::stod(valorCampo);
							double valorComparar = std::stod(comparar);
							// Realizar la comparación de números
							//std::cout << "Signo:" << signo << ", Valor Registro:" << valorRegistro << ", Valor Comparar:" << valorComparar << std::endl;
							if ((signo == '<' && valorRegistro < valorComparar) ||
								(signo == '>' && valorRegistro > valorComparar) ||
								(signo == '=' && valorRegistro == valorComparar)) {
								cumpleCondicion = true;
							}
						}
					}

					if (cumpleCondicion) {
						outputFile << registro << std::endl;
						std::replace(registro.begin(), registro.end(), '#', ' ');
						fileConsulta << registro << std::endl;
					}
				}
				outputFile.close();
				lectura.close();
			}
			else if (wherePos != query.end() && std::string(wherePos, wherePos + 5) == "WHERE") {
				// SELECT * FROM tabla WHERE condicion
				std::cout << "La consulta contiene 'SELECT * FROM tabla WHERE condicion'." << std::endl;

				std::string campo;
				std::string tipoDatoCampo;
				char signo;
				std::string comparar;
				int i = 0;

				size_t posHasta = query.find("WHERE");
				std::string tabla = query.substr(14, posHasta - 14);

				std::string condicion = query.substr(wherePos - query.begin() + 6);
    			std::cout << "Condición: " << condicion << std::endl;

				while (condicion[i] != '<' && condicion[i] != '=' && condicion[i] != '>') {
					campo.push_back(condicion[i]);
					i++;
				}  
				std::cout << condicion[i] << std::endl;
				if (condicion[i] == 60 || condicion[i] == 61 || condicion[i] == 62) {
					signo = condicion[i];
					i++;
				}
				while (condicion[i] != '\0') {
					comparar.push_back(condicion[i]);
					i++;
				}
				trim(comparar);

				int columna = -1; 
				int j = 0; 

				while (std::getline(esquema, lineaEsquema)) {
					size_t pos = lineaEsquema.find('#');
					encabezadoEsquema = lineaEsquema.substr(0, pos);
					std::istringstream esquema_ss(lineaEsquema);
					std::string texto;
					std::getline(esquema_ss, texto, '#');
					trim(tabla);
					if (encabezadoEsquema == tabla) {
						while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
							if (encabezado == campo) {
								columna = j;
								tipoDatoCampo = tipoDato; 
							}
							fileConsulta << encabezado + ' ';
							j++;							
						}
						fileConsulta << std::endl;
						break;
					}
				}

				if (columna == -1) {
					std::cerr << "Campo no encontrado en la tabla." << std::endl;
					esquema.close();
					fileConsulta.close();
					return false;
				}
				esquema.close();
				texto.clear(); 

				std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
				
				std::string registro;
				
				while (std::getline(lectura, linea)) {
					registro = linea; 

					std::istringstream ss(linea);
					std::string valor;
					std::string valorCampo;
					int k = 0; 

					while (std::getline(ss, valor, '#')) {
						if (k == columna) {
							valorCampo = valor;
							break;
						}
						k++;
					}
					trim(valorCampo);
					bool cumpleCondicion = false;

					if (tipoDatoCampo == "string") {
						// Comparación de cadenas de texto
						if (signo == '=' && valorCampo == comparar) {
							cumpleCondicion = true;
						}
					} else if (tipoDatoCampo == "int" || tipoDatoCampo == "double") {
						// Comparación de números (int o double)
						if (esNumero(valorCampo) && esNumero(comparar)) {
							// Convertir las cadenas a números
							double valorRegistro = std::stod(valorCampo);
							double valorComparar = std::stod(comparar);
							// Realizar la comparación de números
							//std::cout << "Signo:" << signo << ", Valor Registro:" << valorRegistro << ", Valor Comparar:" << valorComparar << std::endl;
							if ((signo == '<' && valorRegistro < valorComparar) ||
								(signo == '>' && valorRegistro > valorComparar) ||
								(signo == '=' && valorRegistro == valorComparar)) {
								cumpleCondicion = true;
							}
						}
					}

					if (cumpleCondicion) {
						std::replace(registro.begin(), registro.end(), '#', ' ');
						fileConsulta << registro << std::endl;
					}
				}
				lectura.close();
			} 
			else if (wherePos == query.end()) {
				// SELECT * FROM tabla
				std::cout << "La consulta contiene 'SELECT * FROM tabla'." << std::endl;

				std::string tabla = query.substr(14, 10);
				trim(tabla);

				while (std::getline(esquema, lineaEsquema)){
					size_t pos = lineaEsquema.find('#');
					encabezadoEsquema = lineaEsquema.substr(0, pos);
					std::cout << encabezadoEsquema << std::endl;
					if (encabezadoEsquema == tabla) {
						std::istringstream esquema_ss(lineaEsquema);
						std::getline(esquema_ss, texto, '#'); 
						while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
							fileConsulta << encabezado + ' ';
						}
						fileConsulta << std::endl;
						break; 
					}					
				}
				esquema.close();
				texto.clear();

				std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
				if (!lectura.is_open()) {
					std::cerr << "No se pudo abrir el archivo: " << encabezadoEsquema << std::endl;
					esquema.close();
					fileConsulta.close();
					lectura.close();
					return false;
				}

				while (std::getline(lectura, linea)) {
					std::replace(linea.begin(), linea.end(), '#', ' '); 
					fileConsulta << linea << std::endl;
				}
				lectura.close();
			}
			else {
				std::cout << "La consulta no fue bien escrita." << std::endl;
				esquema.close();
				fileConsulta.close();
				return false;
			}
		} else {
			auto fromPos = std::find(query.begin() + 9, query.end(), 'F');
			auto barraPos = std::find(query.begin() + 18, query.end(), '|');
			if (fromPos != query.end() && std::string(fromPos, fromPos + 4) == "FROM" && std::string(barraPos, barraPos + 1) == "|") {
				auto wherePos = std::find(query.begin() + 15, query.end(), 'W');
				if (wherePos != query.end() && std::string(wherePos, wherePos + 5) == "WHERE") {
					// SELECT Campo FROM tabla WHERE condicion
					std::cout << "La consulta contiene 'SELECT campo FROM tabla WHERE condicion | file'." << std::endl;
					
					std::string tabla;
					std::string campo;
					std::string tipoDatoCampo;
					char signo;
					std::string comparar;
					int i = 0;

					size_t posFrom = query.find("FROM");
					size_t posWhere = query.find("WHERE");

					if (posFrom != std::string::npos && posWhere != std::string::npos) {
						campo = query.substr(7, posFrom-7);
						tabla = query.substr(posFrom + 4, posWhere - posFrom - 4);
						trim(campo);
						trim(tabla);
					} else {
						std::cerr << "La consulta no contiene FROM o WHERE." << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}

					std::string condicion = query.substr(std::distance(query.begin(), wherePos) + 6, std::distance(wherePos, barraPos));
					trim(condicion);

					std::string file = query.substr(barraPos - query.begin() + 2);
					trim(file);

					size_t posSimbolo = encontrarSimbolo(condicion, '<');
					if (posSimbolo == std::string::npos) {
						posSimbolo = encontrarSimbolo(condicion, '=');
						if (posSimbolo == std::string::npos) {
							posSimbolo = encontrarSimbolo(condicion, '>');
						}
					}
					if (condicion[posSimbolo] == 60 || condicion[posSimbolo] == 61 || condicion[posSimbolo] == 62) {
						signo = condicion[posSimbolo];
						posSimbolo++;
					}
					while (condicion[posSimbolo] != ' ') {
						comparar.push_back(condicion[posSimbolo]);
						posSimbolo++;
					}
					trim(comparar);

					std::ofstream outputFile("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + file + ".txt");
					if (!outputFile.is_open()) {
						std::cerr << "No se pudo abrir el archivo: " << file << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}

					int columna = -1;
					int j = 0;

					while (std::getline(esquema, lineaEsquema)) {
						size_t pos = lineaEsquema.find('#');
						encabezadoEsquema = lineaEsquema.substr(0, pos);
						std::istringstream esquema_ss(lineaEsquema);
						std::string texto;
						std::getline(esquema_ss, texto, '#');
						trim(tabla);
						if (encabezadoEsquema == tabla) {
							std::cout << "ENTRO " << std::endl;
							while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
								if (encabezado == campo) {
									columna = j; 
									tipoDatoCampo = tipoDato; 
									fileConsulta << encabezado + ' ';
									outputFile << encabezado + '#';
								}
								j++;							
							}
							fileConsulta << std::endl;
							outputFile << std::endl;
							break;
						}
					}

					if (columna == -1) {
						std::cerr << "Campo no encontrado en la tabla." << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}
					esquema.close();
					texto.clear(); 

					std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
					
					std::string registro;
					
					while (std::getline(lectura, linea)) {
						std::istringstream ss(linea);
						std::string valor;
						int k = 0;

						while (std::getline(ss, valor, '#')) {
							if (k == columna) {
								registro = valor; 
								break;
							}
							k++;
						}
						trim(registro);
						bool cumpleCondicion = false;

						if (tipoDatoCampo == "string") {
							// Comparación de cadenas de texto
							std::cout << "Signo:" << signo << ", Valor Registro:" << registro << ", Valor Comparar:" << comparar << std::endl;
							if (signo == '=' && registro == comparar) {
								cumpleCondicion = true;
							}
						} else if (tipoDatoCampo == "int" || tipoDatoCampo == "double") {
							// Comparación de números (int o double)
							if (esNumero(registro) && esNumero(comparar)) {
								// Convertir las cadenas a números
								double valorRegistro = std::stod(registro);
								double valorComparar = std::stod(comparar);
								// Realizar la comparación de números
								//std::cout << "Signo:" << signo << ", Valor Registro:" << valorRegistro << ", Valor Comparar:" << valorComparar << std::endl;
								if ((signo == '<' && valorRegistro < valorComparar) ||
									(signo == '>' && valorRegistro > valorComparar) ||
									(signo == '=' && valorRegistro == valorComparar)) {
									cumpleCondicion = true;
								}
							}
						}

						if (cumpleCondicion) {
							outputFile << registro << std::endl;
							std::replace(registro.begin(), registro.end(), '#', ' ');
							fileConsulta << registro << std::endl;
						}
					}
					lectura.close();					
				}
			}
			else if (fromPos != query.end() && std::string(fromPos, fromPos + 4) == "FROM") {
				auto wherePos = std::find(query.begin() + 15, query.end(), 'W');
				if (wherePos != query.end() && std::string(wherePos, wherePos + 5) == "WHERE") {
					// SELECT Campo FROM tabla WHERE condicion
					std::cout << "La consulta contiene 'SELECT campo FROM tabla WHERE condicion'." << std::endl;
					
					std::string tabla;
					std::string campo;
					std::string tipoDatoCampo;
					char signo;
					std::string comparar;
					int i = 0;

					size_t posFrom = query.find("FROM");
					size_t posWhere = query.find("WHERE");

					if (posFrom != std::string::npos && posWhere != std::string::npos) {
						campo = query.substr(7, posFrom-7);
						tabla = query.substr(posFrom + 4, posWhere - posFrom - 4);
						trim(campo);
						trim(tabla);
					} else {
						std::cerr << "La consulta no contiene FROM o WHERE." << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}

					std::string condicion = query.substr(wherePos - query.begin() + 6);

					size_t posSimbolo = encontrarSimbolo(condicion, '<');
					if (posSimbolo == std::string::npos) {
						posSimbolo = encontrarSimbolo(condicion, '=');
						if (posSimbolo == std::string::npos) {
							posSimbolo = encontrarSimbolo(condicion, '>');
						}
					}
					if (condicion[posSimbolo] == 60 || condicion[posSimbolo] == 61 || condicion[posSimbolo] == 62) {
						signo = condicion[posSimbolo];
						posSimbolo++;
					}
					while (condicion[posSimbolo] != '\0') {
						comparar.push_back(condicion[posSimbolo]);
						posSimbolo++;
					}
					trim(comparar);

					int columna = -1;
					int j = 0;

					while (std::getline(esquema, lineaEsquema)) {
						size_t pos = lineaEsquema.find('#');
						encabezadoEsquema = lineaEsquema.substr(0, pos);
						std::istringstream esquema_ss(lineaEsquema);
						std::string texto;
						std::getline(esquema_ss, texto, '#');
						trim(tabla);
						if (encabezadoEsquema == tabla) {
							std::cout << "ENTRO " << std::endl;
							while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
								if (encabezado == campo) {
									columna = j; 
									tipoDatoCampo = tipoDato; 
									fileConsulta << encabezado + ' ';
								}
								j++;							
							}
							fileConsulta << std::endl;
							break;
						}
					}

					if (columna == -1) {
						std::cerr << "Campo no encontrado en la tabla." << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}
					esquema.close();
					texto.clear(); 

					std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
					
					std::string registro;
					
					while (std::getline(lectura, linea)) {
						std::istringstream ss(linea);
						std::string valor;
						int k = 0;

						while (std::getline(ss, valor, '#')) {
							if (k == columna) {
								registro = valor; 
								break;
							}
							k++;
						}
						trim(registro);
						bool cumpleCondicion = false;

						if (tipoDatoCampo == "string") {
							// Comparación de cadenas de texto
							std::cout << "Signo:" << signo << ", Valor Registro:" << registro << ", Valor Comparar:" << comparar << std::endl;
							if (signo == '=' && registro == comparar) {
								cumpleCondicion = true;
							}
						} else if (tipoDatoCampo == "int" || tipoDatoCampo == "double") {
							// Comparación de números (int o double)
							if (esNumero(registro) && esNumero(comparar)) {
								// Convertir las cadenas a números
								double valorRegistro = std::stod(registro);
								double valorComparar = std::stod(comparar);
								// Realizar la comparación de números
								//std::cout << "Signo:" << signo << ", Valor Registro:" << valorRegistro << ", Valor Comparar:" << valorComparar << std::endl;
								if ((signo == '<' && valorRegistro < valorComparar) ||
									(signo == '>' && valorRegistro > valorComparar) ||
									(signo == '=' && valorRegistro == valorComparar)) {
									cumpleCondicion = true;
								}
							}
						}

						if (cumpleCondicion) {
							std::replace(registro.begin(), registro.end(), '#', ' ');
							fileConsulta << registro << std::endl;
						}
					}
					lectura.close();					
				} 
				else if (wherePos == query.end()) {
					// SELECT Campo FROM tabla
					std::cout << "La consulta contiene 'SELECT Campo FROM tabla'." << std::endl;

					std::string tabla = query.substr(14, 10);
					trim(tabla);
					std::string campo;

					size_t posFrom = query.find("FROM");

					if (posFrom != std::string::npos) {
						campo = query.substr(7, posFrom - 7);
						tabla = query.substr(posFrom + 4, query.length() - posFrom - 4);
						trim(campo);
						trim(tabla);
						std::cout << "Campo:" << campo << "yo" << std::endl;
					} else {
						std::cerr << "La consulta no contiene FROM o WHERE." << std::endl;
						esquema.close();
						fileConsulta.close();
						return false;
					}
					int columna = -1;
					int j = 0;

					while (std::getline(esquema, lineaEsquema)){
						size_t pos = lineaEsquema.find('#');
						encabezadoEsquema = lineaEsquema.substr(0, pos);
						std::cout << encabezadoEsquema << std::endl;
						if (encabezadoEsquema == tabla) {
							std::istringstream esquema_ss(lineaEsquema);
							std::getline(esquema_ss, texto, '#'); 
							while (std::getline(esquema_ss, encabezado, '#') && std::getline(esquema_ss, tipoDato, '#')) {
								if(campo == encabezado) {
									columna = j;
									fileConsulta << encabezado;
								}
								j++;
							}
							fileConsulta << std::endl;
							break; 
						}					
					}
					esquema.close();
					texto.clear();

					std::ifstream lectura("F:\\UNSA\\2024-A\\Base de Datos II\\Megatron\\usr\\db\\" + encabezadoEsquema + ".txt");
					if (!lectura.is_open()) {
						std::cerr << "No se pudo abrir el archivo: " << encabezadoEsquema << std::endl;
						esquema.close();
						fileConsulta.close();
						lectura.close();
						return false;
					}

					std::string registro;
					
					while (std::getline(lectura, linea)) {
						std::istringstream ss(linea);
						std::string valor;
						int k = 0;

						while (std::getline(ss, valor, '#')) {
							if (k == columna) {
								registro = valor;
								break;
							}
							k++;
						}
						trim(registro);
						
						fileConsulta << registro << std::endl;
					}
					lectura.close();					
				}
				else {
					std::cout << "La consulta no esta bien escrita." << std::endl;
					esquema.close();
					fileConsulta.close();
					return false;
				}				
			} 
			else {
				std::cout << "La consulta no cumple con los patrones conocidos." << std::endl;
				esquema.close();
				fileConsulta.close();
				return false;
			}
		}
	}

	else {
		std::cout<<"Consulta SQL no valida: No empezo con la instruccion con SELECT\n";
		esquema.close();
		fileConsulta.close();
		return false;
	}
	
	esquema.close();
	fileConsulta.close();
	return true;
}

int calcularTamanioRegistro(const std::string& registro) {
    std::istringstream ss(registro);
    std::string atributo;
    int tamanioRegistro = 0;

    while (std::getline(ss, atributo, ',')) {
        if (atributo.front() == '"' && atributo.back() == '"') {
            atributo = atributo.substr(1, atributo.size() - 2);
        }

        if (atributo == "char" || atributo == "bool") {
            tamanioRegistro += 1;
        } else if (atributo == "float" || atributo == "int") {
            tamanioRegistro += 4;
        } else if (atributo == "double") {
            tamanioRegistro += 8;
        } else if (atributo == "string") {
            tamanioRegistro += atributo.length();
        }
    }

    return tamanioRegistro;
}

int obtenerTamanioMaximo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    std::getline(archivo, linea);
    int tamanioMaximo = 0;

    while (std::getline(archivo, linea)) {
        int tamanioRegistro = calcularTamanioRegistro(linea);
        if (tamanioRegistro > tamanioMaximo) {
            tamanioMaximo = tamanioRegistro;
        }
    }

    return tamanioMaximo;
}

int main() {
    int resp;
    int option;
	bool continuar = true;
    std::string nombreFile;

    do {
        std::cout << "\n\n*********************************************************" << std::endl;
        std::cout << "--------- MENU SISTEMA GESTOR DE BASE DE DATOS ----------" << std::endl;
        std::cout << "1. Crear disco" << std::endl;
		std::cout << "2. Crear esquema" << std::endl;
        std::cout << "3. Crear relacion" << std::endl;
        std::cout << "4. Agregar registro a relacion" << std::endl;
        std::cout << "5. Realizar consultas" << std::endl;
        std::cout << "6. Salir" << std::endl;
        std::cout << "\tIngresa una opcion: ";
        std::cin>>resp;
		Megatron dataBase;

        switch (resp)
        {
            case 1:
			{
				std::cout << "\n*********************************************************" << std::endl;
				std::cout << "Desea crear disco por DEFAULT? (S/N)" << std::endl;
				char answer;
				std::cin >> answer;

				std::cin.ignore();
				if(answer == 'S' || answer == 's' || answer == 'N' || answer == 'n' ) {
					/*std::cout << "\nEscoja el tipo de resgistro:" << std::endl;
					std::cout << "0. Longitud Fija" << std::endl;
					std::cout << "1. Longitud Variable" << std::endl;
					std::cin >> option;*/
					option = false;

					//Controlador controlador;

					if(answer == 'S' || answer == 's') {
						DiscoManager controlador(option, 3, 6, 10, 300,20);
						controlador.crearEsquema();
						controlador.configurarDirectorio();
					}
					else if(answer == 'N' || answer == 'n') {
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
						
						DiscoManager controlador(option, nroPlatos, nroPistas, nroSectores, bytesxSector,sectoresxBloque);
						controlador.crearEsquema();
						controlador.configurarDirectorio();
					}
				}
				break;
			}
            case 2:
			{
				std::cout << "\n*********************************************************" << std::endl;
				std::cout << "1. Generar esquema a partir de archivo .CSV y crear relacion" << std::endl;
				std::cout << "2. Crear un esquema nuevo" << std::endl;                
				std::cout << "\tIngresa una opcion: ";
				std::cin >> option;

				if (option == 1) {
					std::cin.ignore(); // Ignora el carácter de nueva línea después de leer 'option'
					std::cout << "\n\tIndica el nombre del archivo a leer: ";
					std::getline(std::cin, nombreFile);
					std::cin.ignore();
					std::cout << "\n\tElige la cantidad de registros a llenar (0 = todo): ";
					int cant;
					std::cin >> cant;
					std::cin.ignore(); // Ignora el carácter de nueva línea después de leer 'cant'
					dataBase.crearEsquemaDesdeCsv(nombreFile, cant);
					break;
				}
				else if (option == 2) {
					std::cin.ignore(); // Ignora el carácter de nueva línea después de leer 'option'
					dataBase.agregarEsquemaManual();
					break;
				}
				break; 
			}
            case 3:
            {                
                std::string nombreEsquema;

                std::cout << "\n*********************************************************" << std::endl;
                std::cout << "1. Crear relacion usando el esquema creado previamente" << std::endl;
                std::cout << "2. Crear relacion de un esquema existente" << std::endl;                
                std::cout << "\tIngresa una opcion: ";
                std::cin >> option;
                if (option == 2) {
                    std::cout << "Indica el nombre del esquema: ";
                    std::cin >> nombreEsquema;
                    dataBase.setEsquema(nombreEsquema);
                }
                else if (option != 1) {
                    break;
                }
                
                std::cout << "\n*********************************************************" << std::endl;
                std::cout << "1. Leer desde .CSV" << std::endl;
                std::cout << "2. Ingresar datos manualmente" << std::endl;                
                std::cout << "\tIngresa una opcion: ";
                std::cin >> option;                  

                if (option == 1) {
                    
                    std::cout << "\n\tIndica el nombre del archivo a leer: ";
                    std::getline(std::cin, nombreFile);
					std::cin.ignore();
					std::cout << "\n\tElige la cantidad de registros a llenar (0 = todo): ";
					int cant;
                    std::cin >> cant;
                    //dataBase.ingresarDesdeArchivoCsv(nombreFile,cant);
                    break;
                }
                else if (option == 2) {
                    dataBase.ingresarRelacionManual(0);
                }
                else {
                    break;
                }
        } 
        case 4:
		{
			std::string nomEsquema;
			std::string nomRelacion;
			
			std::cout << "Indica el nombre del Esquema: ";
			std::cin >> nomEsquema;
			dataBase.setEsquema(nomEsquema);

			// Limpiar el búfer de entrada
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "\n*********************************************************" << std::endl;
			std::cout << "1. Leer desde .CSV" << std::endl;
			std::cout << "2. Ingresar datos manualmente" << std::endl;                
			std::cout << "\tIngresa una opcion: ";
			std::cin >> option;

			// Limpiar el búfer de entrada nuevamente
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (option == 1) {
				std::cout << "\n\tIndica el nombre del archivo a leer: ";
				std::getline(std::cin, nombreFile);
				//dataBase.llenarDesdeArchivo(1, nombreArchivo);
			}
			else if (option == 2) {
				dataBase.ingresarRelacionManual(1);
			}
			else {
				break;
			}

			break;
		} 

        case 5:
        {
            bool seguir = false;
            std::string query;
			std::cin.ignore();
			while (!seguir) {
				std::cout << "Realiza tu consulta de tipo SQL: ";
            	std::getline(std::cin, query);
            	seguir = consulta(query);
			}
            break;
        }
		case 6: {
			//controlador.~Controlador();
			continuar = false;

			break;
		}
        default:
            std::cout << "Opción no válida. Inténtalo de nuevo." << std::endl;
            break;
        };
    } while (continuar);

    return 0;
}
