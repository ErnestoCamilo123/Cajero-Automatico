#include "cuentamlc.h"
#include<vector>
#include<string>

using namespace std;

    //al entrar al cajero una tarjeta de este tipo se le aplicara el cambio vigente
    CuentaMLC::CuentaMLC(double cambio,Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper):
         cambio(cambio), Cuenta(prop,banco,no_cuenta,clave,saldo,oper){}

         double CuentaMLC::getCambio(){return cambio;}


