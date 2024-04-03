#ifndef CLIENTE_H
#define CLIENTE_H
#include<string>


using namespace std;


class Cliente{
    string ci,nombre,prov,mncpio,dir;

public:
    Cliente(string ci,string nombre,string prov,string mncpio,string dir);

    void setCi(string ci);
    string getCi();

    void setNombre(string nombre);
    string getNombre();

    void setProv(string prov);
    string getProv();

    void setMncpio(string mncpio);
    string getMncpio();

    void setDir(string dir);
    string getDir();
};

#endif // CLIENTE_H
