#include "cliente.h"
#include<string>

using namespace std;


    string ci,nombre,prov,mncpio,dir;

    Cliente::Cliente(string ci,string nombre,string prov,string mncpio,string dir): ci(ci),nombre(nombre),prov(prov),mncpio(mncpio),dir(dir) {}

    void Cliente::setCi(string ci){this->ci=ci;}
    string Cliente::getCi(){return ci;}

    void Cliente::setNombre(string nombre){this->nombre=nombre;}
    string Cliente::getNombre(){return nombre;}

    void Cliente::setProv(string prov){this->prov=prov;}
    string Cliente::getProv(){return prov;}

    void Cliente::setMncpio(string mncpio){this->mncpio=mncpio;}
    string Cliente::getMncpio(){return mncpio;}

    void Cliente::setDir(string dir){this->dir=dir;}
    string Cliente::getDir(){return dir;}
