//#include "megatron.h"
//#include "esquema.h"
//#include "relacion.h"

#include <iostream>
#include <string>

int main() {
    int resp;

    std::cout<<"---------MENU----------\n 1. Crear esquema\n 2.Crear relación\n 3. Realizar consultas"<<std::endl;
    std::cin>>resp;

    //Megatron dataBase;
    //dataBase.Megatron();

    switch (resp)
    {
    case 1:
    {
        //Esquema esquema;
        break;
    }
    case 2:
    {
        std::cout<<"1. Leer desde .CSV\n 2.Ingresar datos manuamente"<<std::endl;
        std::cin>>resp;

        //Relacion relacion;

        if (resp == 1) {

        }
        else if ( resp == 2) {

        }
        else {
            break;
        }  
    }  
    default:
        break;
    } (resp == 1);   

    return 0;
}
