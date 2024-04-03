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
#include"cuentas.h"
#include"cuentamlc.h"
#include"cajero.h"
#include"cliente.h"


using namespace std;

/*
class Cliente{
    string ci,nombre,prov,mncpio,dir;

public:
    Cliente(string ci,string nombre,string prov,string mncpio,string dir): ci(ci),nombre(nombre),prov(prov),mncpio(mncpio),dir(dir) {}

    void setCi(string ci){this->ci=ci;}
    string getCi(){return ci;}

    void setNombre(string nombre){this->nombre=nombre;}
    string getNombre(){return nombre;}

    void setProv(string prov){this->prov=prov;}
    string getProv(){return prov;}

    void setMncpio(string mncpio){this->mncpio=mncpio;}
    string getMncpio(){return mncpio;}

    void setDir(string dir){this->dir=dir;}
    string getDir(){return dir;}
};

class Cuenta{
protected:
    Cliente prop;
    string banco, no_cuenta, clave;
    double saldo;
    vector<string> oper;

public:
    Cuenta(Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper):prop(prop),banco(banco),no_cuenta(no_cuenta),clave(clave),saldo(saldo),oper(oper){}

    Cliente getProp(){return prop;}

    string getBanco(){return banco;}

    void setClave(string clave){this->clave=clave;}
    string getClave(){return clave;}

    string getNo_cuenta(){return no_cuenta;}

    void setSaldo(double saldo){this->saldo=saldo;}
    double getSaldo(){return saldo;}

    vector<string> getOper(){return oper;}

    //sobrecarga del operador -
    //se usa este operador para indicar un debito
    //se guarda la operacion realizada(fecha de la operacion y monto con dos lugares despues del punto decimal)
    void operator-(const double debito){
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
    void operator+(const double credito){
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
    void transferencia(Cuenta* d, const double t){
        *this-t;
        *d+t;
    }
};

class CuentaMLC: public Cuenta{
    //al entrar al cajero una tarjeta de este tipo se le aplicara el cambio vigente
    double cambio;
public:
    CuentaMLC(double cambio,Cliente prop,string banco,string no_cuenta,string clave,double saldo,vector<string> oper):
         cambio(cambio), Cuenta(prop,banco,no_cuenta,clave,saldo,oper){}

         double getCambio(){return cambio;}

};

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
    Cajero(map<string,Cuenta*> mapa): mapa(mapa){
        ifstream ifs("efectivo.txt");
        string linea;
        getline(ifs,linea);
        efectivo = stod(linea);
        ifs.close();
        ifstream ifs2("cambio.txt");
        getline(ifs2,linea);
        cambio = stod(linea);
        ifs2.close();
    }

    //Recibe un numero de tarjeta que va a buscar
    void mostrar_interfaz_inicial() {
        string no_cuenta;
        cout<<"----------------------------------------------"<<endl;
        cout<<"BANCO DE CREDITO Y COMERCIO"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<"               POR FAVOR"<<endl;
        cout<<"               INSERTE SU"<<endl;
        cout<<"                TARJETA"<<endl;
        cout<<""<<endl;
        cin>>no_cuenta;
        interfaz_espera(1);
        leer_tarjeta(no_cuenta);
    }
    //Busca si la tarjeta existe o no
    //En caso de existir, si la tarjeta es de MLC se muestra la interfaz de alerta
    //Posteriormente se inserta la clave
    void leer_tarjeta(string numero) {
        cout<<"----------------------------------------------"<<endl;
        auto iter = mapa.find(numero);
        string clave;
        if (iter == mapa.end()) {
            cout<<"SU CUENTA NO EXISTE"<<endl;
            mostrar_interfaz_inicial();
        } else {
            Cuenta* c = iter->second;
            if(numero.substr(0,4)=="9225"){
                interfaz_mlc(c);
            }
            cout<<"              POR FAVOR"<<endl;
            cout<<"               INGRESE"<<endl;
            cout<<"              SU NUMERO"<<endl;
            cout<<"                  DE "<<endl;
            cout<<"            IDENTIFICACION"<<endl;
            cout<<"                 XXXX"<<endl;
            cin>>clave;
            leer_clave(c, clave);
        }
    }
    //interfaz de alerta para terjetas MLC
    void interfaz_mlc(Cuenta* c){
        cout<<"----------------------------------------------"<<endl;
        cout<<"             ALERTA"<<endl;
        cout<<""<<endl;
        cout<<"         TARJETA EN MLC"<<endl;
        cout<<"    DEPENDIENDO DE SU OPERACION SE"<<endl;
        cout<<"   APLICA EL TIPO DE CAMBIO VIGENTE"<<endl;
        cout<<""<<endl;
        cout<<"     SEGURO QUE DESEA CONTINUAR?"<<endl;
        cout<<"     1-CONTINUAR"<<endl;
        cout<<"     2-CANCELAR"<<endl;
        int opcion;
        cin>>opcion;
        switch (opcion){
        case 1:
            c->setSaldo(c->getSaldo()*cambio);
            return;
            break;
        case 2:
            cout<<"----------------------------------------------"<<endl;
            cout<<"   SU TRANSACCION HA SIDO CANCELADA"<<endl;
            cout<<"          TOME SU TARJETA"<<endl;
            this_thread::sleep_for(chrono::seconds(5));
            mostrar_interfaz_inicial();
            break;
        default:
            cout<<"----------------------------------------------"<<endl;
            cout<<"    OPCION INCORRECTA"<<endl;
            cout<<"   ELIJA NUEVAMENTE LA"<<endl;
            cout<<"      OPCION DESEADA"<<endl;
            interfaz_mlc(c);
        }
    }
    //Determina si la clave es o no correcta
    void leer_clave(Cuenta* c, string clave) {
        interfaz_espera(1);
        if (clave != c->getClave()) {
            interfaz_clave_incorrecta(c);
        } else {
            interfaz_informativa(c);
        }
    }

    void interfaz_clave_incorrecta(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"             SU CLAVE ES INCORRECTA"<<endl;
        cout<<"              TECLEE NUEVAMENTE SU "<<endl;
        cout<<"             CLAVE DE IDENTIFICACION"<<endl;
        cout<<"                        XXXX"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        string clave;
        cin>>clave;
        leer_clave(c, clave);
    }

    void interfaz_informativa(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"ESTIMADO CLIENTE"<<endl;
        cout<<""<<endl;
        cout<<"    LAS PANTALLAS DEL MENU"<<endl;
        cout<<"    PUEDEN MODIFICARSE PARA"<<endl;
        cout<<"   BRINDAR UN MEJOR SERVICIO"<<endl;
        cout<<""<<endl;
        cout<<"    LEA CUIDADOSAMENTE CADA"<<endl;
        cout<<"   OPCION ANTES DE PRESIONAR"<<endl;
        cout<<"         ALGUNA TECLA"<<endl;
        cout<<""<<endl;
        cout<<"        MUCHAS GRACIAS"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
        string entrada;
        getline(cin,entrada);
        getline(cin,entrada);
        menu_principal(c);
    }


    void menu_principal(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"         MENU PRINCIPAL"<<endl;
        cout<<"   SELECCIONE EL SERVICIO DESEADO "<<endl;
        cout<<""<<endl;
        cout<<"1-CONSULTAR SALDO"<<endl;
        cout<<"2-ULTIMAS OPERACIONES"<<endl;
        cout<<"3-EXTRAER DINERO"<<endl;
        cout<<"4-TRANSFERIR DINERO"<<endl;
        cout<<"5-CAMBIO DE CLAVE"<<endl;
        int opcion;
        cin>>opcion;
        interfaz_espera(1);
        switch (opcion) {
            case 1:
                interfaz_consultar_saldo(c);
                break;
            case 2:
                interfaz_ultimas_operaciones(c);
                break;
            case 3:
                interfaz_extraer_dinero(c);
                break;
            case 4:
                interfaz_transferir_dinero(c);
                break;
            case 5:
                interfaz_cambiar_clave(c);
                break;
            default:
                cout<<"----------------------------------------------"<<endl;
                cout<<"    OPCION INCORRECTA"<<endl;
                cout<<"   ELIJA NUEVAMENTE LA"<<endl;
                cout<<"      OPCION DESEADA"<<endl;
                menu_principal(c);
        }
    }

    void interfaz_consultar_saldo(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"         CONSULTAS"<<endl;
        cout<<"      CONSULTA INTEGRADA"<<endl;
        string  numero = c->getNo_cuenta();
        cout<<"CUENTA:   "<<numero.substr(0,4)<<"XXXXXXXX"<<numero.substr(12,16)<<endl;
        cout<<"  SALDO CONTABLE:  CRED     "<<c->getSaldo()<<endl;
        cout<<"SALDO DISPONIBLE:  CRED     "<<c->getSaldo()<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
        string entrada;
        getline(cin,entrada);
        getline(cin,entrada);
        otra_operacion(c);
    }

    void interfaz_ultimas_operaciones(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<""<<endl;
        cout<<"1-ULTIMA OPERACION"<<endl;
        cout<<"2-ULTIMAS DOS OPERACIONES"<<endl;
        cout<<"3-ULTIMAS TRES OPERACIONES"<<endl;
        int opcion;
        cin>>opcion;
        interfaz_espera(1);
        cout<<"----------------------------------------------"<<endl;
        vector<string> lista = c->getOper();
        switch (opcion) {
            case 1:
                cout<<"        ULTIMA OPERACION"<<endl;
                break;
            case 2:
                cout<<"     ULTIMAS DOS OPERACIONES"<<endl;
                break;
            case 3:
                cout<<"     ULTIMAS TRES OPERACIONES"<<endl;
                break;
            default:
                cout<<"    OPCION INCORRECTA"<<endl;
                cout<<"   ELIJA NUEVAMENTE LA"<<endl;
                cout<<"      OPCION DESEADA"<<endl;
                interfaz_ultimas_operaciones(c);
        }
        if (opcion > 0 && opcion < 4) {
            while (opcion) {
                if (lista.size() >= opcion) {
                    cout<<lista[lista.size() - opcion]<<endl;
                }
                opcion--;
            }
        }
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
        string entrada;
        getline(cin,entrada);
        getline(cin,entrada);
        otra_operacion(c);
    }

    void interfaz_extraer_dinero(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"         EXTRAER DINERO"<<endl;
        cout<<"   SELECCIONE LA CANTIDA DESEADA"<<endl;
        cout<<""<<endl;
        cout<<"1- 200"<<endl;
        cout<<"2- 500"<<endl;
        cout<<"3- 1000"<<endl;
        cout<<"4- 2000"<<endl;
        cout<<"5- 3000"<<endl;
        cout<<"6- 5000"<<endl;
        cout<<"7- 10000"<<endl;
        cout<<"8- OTRA CANTIDAD"<<endl;
        int opcion;
        cin>>opcion;
        int db;
        switch (opcion) {
            case 1:
                db = 200;
                interfaz_espera(3);
            break;
            case 2:
                db = 500;
                interfaz_espera(3);
            break;
            case 3:
                db = 1000;
                interfaz_espera(4);
            break;
            case 4:
                db = 2000;
                interfaz_espera(4);
            break;
            case 5:
                db = 3000;
                interfaz_espera(5);
            break;
            case 6:
                db = 5000;
                interfaz_espera(5);
            break;
            case 7:
                db = 10000;
                interfaz_espera(6);
            break;
            case 8:
                db = interfaz_otra_cantidad();
                interfaz_espera(7);
                break;
            default:
                interfaz_espera(1);
                cout<<"----------------------------------------------"<<endl;
                cout<<"    OPCION INCORRECTA"<<endl;
                cout<<"   ELIJA NUEVAMENTE LA"<<endl;
                cout<<"     CANTIDAD DESEADA"<<endl;
                interfaz_extraer_dinero(c);
        }
        if(db<=c->getSaldo()){
            if(efectivo>db){
                //Si el efectivo es mayor que la cantidad deseada se guarda el efectivo restante en el fichero
                efectivo-=db;
                ofstream ofs("efectivo.txt");
                ofs<<efectivo<<endl;
                ofs.close();

                //Se utiliza la sobrecarga de operadores
                *c-db;
                cout<<"----------------------------------------------"<<endl;
                cout<<"     TOME SU DINERO"<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
                string entrada;
                getline(cin,entrada);
                getline(cin,entrada);
            }else{
                interfaz_no_disponibilidad(c);
            }
            otra_operacion(c);
        } else {
            interfaz_saldo_insuficiente(c);
        }
    }

    double interfaz_otra_cantidad() {
        cout<<"----------------------------------------------"<<endl;
        cout<<"    TECLEE LA CANTIDAD DESEADA"<<endl;
        cout<<""<<endl;
        double cantidad;
        cin>>cantidad;
        return cantidad;
    }

    void interfaz_saldo_insuficiente(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"    EL SALDO DE SU TARJETA"<<endl;
        cout<<"       ES INSUFICIENTE"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
        string entrada;
        getline(cin,entrada);
        getline(cin,entrada);
        otra_operacion(c);
    }

    void interfaz_transferir_dinero(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"     INTRODUZCA EL NUMERO"<<endl;
        cout<<"       DE LA TARJETA DEL"<<endl;
        cout<<"         DESTINATARIO"<<endl;
        string numero;
        cin>>numero;
        auto iter = mapa.find(numero);
        if (iter == mapa.end()) {
            cout<<"LA CUENTA NO EXISTE"<<endl;
            interfaz_transferir_dinero(c);
        } else {
            cout<<"              POR FAVOR"<<endl;
            cout<<"           INGRESE EL MONTO"<<endl;
            cout<<"             A TRANSFERIR"<<endl;
            cout<<""<<endl;
            cout<<""<<endl;
            Cuenta* d = iter->second;
            double monto;
            cin>>monto;
            interfaz_espera(5);
            if(monto>c->getSaldo()){
                interfaz_saldo_insuficiente(c);
            } else{
                c->transferencia(d,monto);
                cout<<"----------------------------------------------"<<endl;
                cout<<"          LA TRANSFERENCIA FUE"<<endl;
                cout<<"           REALIZADA CON EXITO"<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
                string entrada;
                getline(cin,entrada);
                getline(cin,entrada);
                otra_operacion(c);
            }
        }
    }

    void interfaz_cambiar_clave(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"    INTRODUZCA SU"<<endl;
        cout<<"     NUEVA CLAVE"<<endl;
        cout<<""<<endl;
        string clave;
        cin>>clave;
        //Se comprueba que la clave introducida es una clave valida
        int indicador = 1;
        if(clave.size()==4){
                int i = 0;
            while(indicador && i<4){
                if(isdigit(clave[i])){
                    i++;
                } else{
                    indicador = 0;
                }
            }
        } else {
            indicador = 0;
        }
        if (indicador) {
            interfaz_espera(1);
            cout<<"----------------------------------------------"<<endl;
            cout<<"           SU CLAVE FUE"<<endl;
            cout<<"       CORRECTAMENTE GUARDADA"<<endl;
            cout<<""<<endl;
            c->setClave(clave);
            otra_operacion(c);
        } else {
            cout<<"----------------------------------------------"<<endl;
            cout<<"      EL FORMATO DE"<<endl;
            cout<<"      LA NUEVA CLAVE"<<endl;
            cout<<"      ES INCORRECTO"<<endl;
            interfaz_cambiar_clave(c);
        }
    }

    void interfaz_espera(int seg) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"                POR FAVOR"<<endl;
        cout<<"                  ESPERE"<<endl;
        cout<<"                UN MOMENTO"<<endl;
        this_thread::sleep_for(chrono::seconds(seg));
    }
    void otra_operacion(Cuenta* c) {
        cout<<"----------------------------------------------"<<endl;
        cout<<"   DESEA HACER OTRA OPERACION?"<<endl;
        cout<<"1-SI                          2-NO"<<endl;
        int opcion;
        cin>>opcion;
        switch (opcion) {
            case 1:
                menu_principal(c);
                break;
            case 2:
                if(c->getNo_cuenta().substr(0,4)=="9225"){
                    c->setSaldo(c->getSaldo()/cambio);
                }
                escribir();
                //Una vez sacada una tarjeta se sobreescribe todo el fichero
                cout<<"----------------------------------------------"<<endl;
                cout<<"    TOME SU TARJETA"<<endl;
                this_thread::sleep_for(chrono::seconds(5));
                mostrar_interfaz_inicial();
            break;
            default:
                cout<<"----------------------------------------------"<<endl;
                cout<<"    OPCION INCORRECTA"<<endl;
                cout<<"   ELIJA NUEVAMENTE LA"<<endl;
                cout<<"     OPCION DESEADA"<<endl;
                otra_operacion(c);
        }
    }

    void interfaz_no_disponibilidad(Cuenta* c){
        cout<<"----------------------------------------------"<<endl;
                cout<<"          ESTE CAJERO NO DISPENSA"<<endl;
                cout<<"           LA MONEDA SOLICITADA"<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<""<<endl;
                cout<<"                   PRESIONE ENTER PARA CONTINUAR"<<endl;
                string entrada;
                getline(cin,entrada);
                getline(cin,entrada);
                otra_operacion(c);
    }

    void leer(){
        ifstream ifs("cuentas.txt");
        int cantidad;
        string linea;
        getline(ifs,linea);
        cantidad = stoi(linea);
        for(int i = 0; i<cantidad; i++) {
            string indicador;
            getline(ifs,indicador);
            string nombre;
            getline(ifs,nombre);
            string ci;
            getline(ifs,ci);
            string prov;
            getline(ifs,prov);
            string mncpio;
            getline(ifs,mncpio);
            string dir;
            getline(ifs,dir);
            string banco;
            getline(ifs,banco);
            string noCuenta;
            getline(ifs,noCuenta);
            string clave;
            getline(ifs,clave);
            double saldo;
            getline(ifs,linea);
            saldo = stod(linea);
            int cantOper;
            getline(ifs,linea);
            cantOper = stoi(linea);
            vector<string> oper;
            for (int i = 0; i < cantOper; i++) {
                    getline(ifs,linea);
                oper.push_back(linea);
            }
            Cliente cliente(ci, nombre, prov, mncpio, dir);
            if (indicador!="N") {
                Cuenta* m = new CuentaMLC(cambio,cliente, banco, noCuenta, clave, saldo, oper);
                mapa.insert({m->getNo_cuenta(),m});
            } else {
                Cuenta* c = new Cuenta(cliente, banco, noCuenta, clave, saldo, oper);
                mapa.insert({c->getNo_cuenta(), c});
            }
        }
        ifs.close();
    }

    void escribir() {
        ofstream ofs("cuentas.txt");
        ofs<<mapa.size()<<endl;
        for (auto const& par : mapa){
            Cuenta* c = par.second;
            string noCuenta = par.first;
            if (noCuenta.substr(0,4)=="9225") {
                ofs<<"MLC"<<endl;
            } else {
                ofs<<"N"<<endl;
            }
            ofs<<c->getProp().getNombre()<<endl;
            ofs<<c->getProp().getCi()<<endl;
            ofs<<c->getProp().getProv()<<endl;
            ofs<<c->getProp().getMncpio()<<endl;
            ofs<<c->getProp().getDir()<<endl;
            ofs<<c->getBanco()<<endl;
            ofs<<c->getNo_cuenta()<<endl;
            ofs<<c->getClave()<<endl;
            ofs<<fixed<<setprecision(2)<<c->getSaldo()<<endl;
            vector<string> oper = c->getOper();
            int cant = oper.size();
            ofs<<cant<<endl;
            for (int i = 0; i<cant; i++) {
                ofs<<oper[i]<<endl;
            }
        }
        ofs.close();
    }
};
*/

int main(){
    map<string,Cuenta*> mapa;
    Cajero d(mapa);
    d.leer();
    //Se leen todas la cuentas
    d.mostrar_interfaz_inicial();
    return 0;
}
