#ifndef GESTOR_DE_CATALOGO_SUCURSAL_H
#define GESTOR_DE_CATALOGO_SUCURSAL_H
#include <string>
#include <iostream>
#include "../ListasEnlazada/NoOrdenada/ListaNoOrdenada.h"
#include "../ListasEnlazada/Ordenada/ListaOrdenada.h"
#include "../TablaHash/TablaHash.h"
#include "../ArbolB/ArbolB.h"
#include "../ArbolAVL/ArbolAVL.h"
#include "../ArbolBM/ArbolBM.h"
#include "../Cola/Cola.h"

class Sucursal
{
public:
    int id;
    string nombre, ubicacion;
    int t_ingreso, t_traspaso, t_despacho;

    ListaNoOrdenada listaN;
    ListaOrdenada listaO;
    TablaHash hash;
    ArbolAVL arbolAvl;
    ArbolB arbolB;
    ArbolBM arbolBM;

    Cola<Producto*> colaIngreso;
    Cola<Producto*> colaTraspaso;
    Cola<Producto*> colaSalida;

    Sucursal(int idSucursal, string nombreSucursal, string ubicacion, int tIngreso, int tTraspaso, int tDespacho);
    int obtenerId();
    string obtenerNombre();
};


#endif //GESTOR_DE_CATALOGO_SUCURSAL_H