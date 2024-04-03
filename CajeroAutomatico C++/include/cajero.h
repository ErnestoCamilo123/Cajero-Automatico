#ifndef CAJERO_H
#define CAJERO_H
#include<map>
#include<string>
#include<vector>
#include"cuentas.h"
#include"cuentamlc.h"

using namespace std;

class Cajero{
   map<string,Cuenta*> mapa;
   double cambio;
   double efectivo;
   //Se utiliza una estructura de mapa para optimizar la insercion y busqueda de las cuentas
   //la variable efectivo guarda la cantidad de dinero disponible en el cajero
   //la variable cambio representa el cambio vigente para tarjetas MLC

public:
    //Se lee el cambio y el efectivo de dos ficheros
    //La valor de la moneda es variable por lo tanto cuando cambie solo es necesaio cambiar el fichero
    //Si el dinero se acaba solo se necesita cambir el valor en el fihero
    //El proceso de colocar dinero en el cajero es totalmente ajeno al cliente por eso se guarda esto en un fichero
    Cajero(map<string,Cuenta*> mapa);

    //Recibe un numero de tarjeta que va a buscar
    void mostrar_interfaz_inicial();
    //Busca si la tarjeta existe o no
    //En caso de existir, si la tarjeta es de MLC se muestra la interfaz de alerta
    //Posteriormente se inserta la clave
    void leer_tarjeta(string numero);

    //interfaz de alerta para terjetas MLC
    void interfaz_mlc(Cuenta* c);
    //Determina si la clave es o no correcta
    void leer_clave(Cuenta* c, string clave);

    void interfaz_clave_incorrecta(Cuenta* c);

    void interfaz_informativa(Cuenta* c);


    void menu_principal(Cuenta* c);

    void interfaz_consultar_saldo(Cuenta* c);

    void interfaz_ultimas_operaciones(Cuenta* c);

    void interfaz_extraer_dinero(Cuenta* c);

    double interfaz_otra_cantidad();

    void interfaz_saldo_insuficiente(Cuenta* c);

    void interfaz_transferir_dinero(Cuenta* c);

    void interfaz_cambiar_clave(Cuenta* c);

    void interfaz_espera(int seg);

    void otra_operacion(Cuenta* c);

    void interfaz_no_disponibilidad(Cuenta* c);

    void leer();

    void escribir();
};
#endif // CAJERO_H
