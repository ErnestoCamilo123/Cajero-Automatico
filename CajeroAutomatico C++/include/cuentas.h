#ifndef CUENTAS_H
#define CUENTAS_H
#include<vector>
#include<string>
#include"cliente.h"

using namespace std;

class Cuenta{
protected:
    Cliente prop;
    string banco, no_cuenta, clave;
    double saldo;
    vector<string> oper;

public:
    Cuenta(Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper);

    Cliente getProp();

    string getBanco();

    void setClave(string clave);
    string getClave();

    string getNo_cuenta();

    void setSaldo(double saldo);
    double getSaldo();

    vector<string> getOper();

    //sobrecarga del operador -
    //se usa este operador para indicar un debito
    //se guarda la operacion realizada(fecha de la operacion y monto con dos lugares despues del punto decimal)
    void operator-(const double debito);

    //sobrecarga del operador +
    //se usa este operador para indicar un credito
    //se guarda la operacion realizada(fecha de la operacion y monto con dos lugares despues del punto decimal)
    void operator+(const double credito);

    //metod para tranferir dinero a una segunda cuenta
    //se llama a la sobrecarga de operadores + y -
    void transferencia(Cuenta* d, const double t);
};
#endif // CUENTAS_H
