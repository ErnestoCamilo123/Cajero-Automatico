#include "cuentas.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;



    Cuenta::Cuenta(Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper):prop(prop),banco(banco),no_cuenta(no_cuenta),clave(clave),saldo(saldo),oper(oper){}

    Cliente Cuenta::getProp(){return prop;}

    string Cuenta::getBanco(){return banco;}

    void Cuenta::setClave(string clave){this->clave=clave;}
    string Cuenta::getClave(){return clave;}

    string Cuenta::getNo_cuenta(){return no_cuenta;}

    void Cuenta::setSaldo(double saldo){this->saldo=saldo;}
    double Cuenta::getSaldo(){return saldo;}

    vector<string> Cuenta::getOper(){return oper;}

    //sobrecarga del operador -
    //se usa este operador para indicar un debito
    //se guarda la operacion realizada(fecha de la operacion y monto con dos lugares despues del punto decimal)
    void Cuenta::operator-(const double debito){
        saldo-=debito;
        time_t tiempoActual = time(nullptr);
        tm* f = localtime(&tiempoActual);
        string fecha = to_string(f->tm_mday) + "/" + to_string(f->tm_mon + 1) + "/"+to_string(f->tm_year - 100);
        stringstream ss;
        ss<<fixed<<setprecision(2)<<debito;
        string s = "Fecha: " + fecha + "  DB:  " + ss.str() + " CUP";
        oper.push_back(s);
    }

    //sobrecarga del operador +
    //se usa este operador para indicar un credito
    //se guarda la operacion realizada(fecha de la operacion y monto con dos lugares despues del punto decimal)
    void Cuenta::operator+(const double credito){
        saldo+=credito;
        time_t tiempoActual = time(nullptr);
        tm* f = localtime(&tiempoActual);
        string fecha = to_string(f->tm_mday) + "/" + to_string(f->tm_mon + 1) + "/" +to_string(f->tm_year - 100);
        stringstream ss;
        ss<<fixed<<setprecision(2)<<credito;
        string s = "Fecha: " + fecha + "  CR:  " + ss.str() + " CUP";
        oper.push_back(s);
    }

    //metod para tranferir dinero a una segunda cuenta
    //se llama a la sobrecarga de operadores + y -
    void Cuenta::transferencia(Cuenta* d, const double t){
        *this-t;
        *d+t;
    }


