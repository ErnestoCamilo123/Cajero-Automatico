#ifndef CUENTAMLC_H
#define CUENTAMLC_H
#include"cuentas.h"
#include<vector>
#include<string>

using namespace std;

class CuentaMLC: public Cuenta{
    //al entrar al cajero una tarjeta de este tipo se le aplicara el cambio vigente
    double cambio;
public:
    CuentaMLC(double cambio,Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper);

         double getCambio();

};

#endif // CUENTAMLC_H
