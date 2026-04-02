#ifndef GESTOR_DE_CATALOGO_ARBOLAVL_H
#define GESTOR_DE_CATALOGO_ARBOLAVL_H
#include "NodoAVL.h"
#include <iostream>
#include <fstream>

using namespace std;


class ArbolAVL
{
private:
    NodoAVL* inicio;
    void destruirRecursivo(NodoAVL* inicio);

public:
    ArbolAVL();
    int obtenerAltura(NodoAVL* n);
    int obtenerFE(NodoAVL* n);
    ~ArbolAVL();

    int max(int a, int b);
    void actualizarAltura(NodoAVL* n);
    bool insertarPorNombre(Producto* producto);
    NodoAVL* insertarRecursivo(NodoAVL* nodo, Producto* producto);
    Producto* buscarPorNombre(std::string nombre);
    void mostrarCatalogoAlfabetico();
    void ListarInorden(NodoAVL* nodo);
    void vaciar();
    NodoAVL* rotarII(NodoAVL* nodo);
    NodoAVL* rotarDD(NodoAVL* nodo);
    NodoAVL* rotarID(NodoAVL* nodo);
    NodoAVL* rotarDI(NodoAVL* nodo);
    void generarReporteGrafico(std::string nombreArchivo);
    void escribirDot(NodoAVL* nodo, std::ofstream& archivo);
    void eliminarPorNombre(std::string nombreEliminar);
    NodoAVL* eliminarRecursivo(NodoAVL* nodo, Producto* p);
};


#endif //GESTOR_DE_CATALOGO_ARBOLAVL_H
