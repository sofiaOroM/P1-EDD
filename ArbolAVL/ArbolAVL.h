#ifndef GESTOR_DE_CATALOGO_ARBOLAVL_H
#define GESTOR_DE_CATALOGO_ARBOLAVL_H
#include "NodoAVL.h"
#include "../ListasEnlazada/NoOrdenada/ListaNoOrdenada.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


class ArbolAVL
{
private:
    NodoAVL* inicio;
    void destruirRecursivo(NodoAVL* inicio);
    void buscarCoincidenciasRec(NodoAVL* nodo, string subcadena, ListaNoOrdenada& resultados);
    void recorridoInorden(NodoAVL* nodo, vector<Producto*>& lista);

public:
    ArbolAVL();
    ~ArbolAVL();
    int obtenerAltura(NodoAVL* n);
    int obtenerFE(NodoAVL* n);
    int max(int a, int b);
    void actualizarAltura(NodoAVL* n);

    bool insertarPorNombre(Producto* producto);
    NodoAVL* insertarRecursivo(NodoAVL* nodo, Producto* producto);
    Producto* buscarPorNombre(string nombre);
    void vaciar();
    NodoAVL* rotarII(NodoAVL* nodo);
    NodoAVL* rotarDD(NodoAVL* nodo);
    NodoAVL* rotarID(NodoAVL* nodo);
    NodoAVL* rotarDI(NodoAVL* nodo);
    void generarReporteGrafico(std::string nombreArchivo);
    void escribirDot(NodoAVL* nodo, std::ofstream& archivo);
    void eliminarPorNombre(string nombreEliminar);
    NodoAVL* eliminarRecursivo(NodoAVL* nodo, Producto* p);
    ListaNoOrdenada buscarCoincidencias(string subcadena);
    vector<Producto*> obtenerTodoOrdenado();
};


#endif //GESTOR_DE_CATALOGO_ARBOLAVL_H
