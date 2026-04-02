#ifndef GESTOR_DE_CATALOGO_CARGADORCSV_H
#define GESTOR_DE_CATALOGO_CARGADORCSV_H
#include <string>
#include <fstream>
#include <sstream>
#include "./ListasEnlazada/NoOrdenada/ListaNoOrdenada.h"
#include "./ListasEnlazada/Ordenada/ListaOrdenada.h"
#include "./TablaHash/TablaHash.h"
#include "./ArbolAVL/ArbolAVL.h"
#include "ArbolB/ArbolB.h"
#include "ArbolBM/ArbolBM.h"

class CargadorCSV
{
public:
    static void cargar(std::string ruta,
                       ListaNoOrdenada& listaN,
                       ListaOrdenada& listaO,
                       TablaHash& hash,
                       ArbolAVL& arbolAvl,
                       ArbolB& arbolB,
                       ArbolBM& arbolBM);
    static string normalizarFecha(string fechaOriginal);
    static bool esFechaValida(string& fecha);
};


#endif //GESTOR_DE_CATALOGO_CARGADORCSV_H