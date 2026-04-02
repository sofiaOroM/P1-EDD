#ifndef GESTOR_DE_CATALOGO_ARBOLBM_H
#define GESTOR_DE_CATALOGO_ARBOLBM_H


#include "NodoBM.h"
#include <iostream>
#include <fstream>
using namespace std;


class ArbolBM
{
private:
    NodoBM* raiz;
    void dividirHoja(NodoBM* izquierda, NodoBM* padre, Producto* p);
    void insertarInterno(string clave, NodoBM* actual, NodoBM* hijo);
    void eliminarInterno(string clave, NodoBM* actual, NodoBM* hijo);

public:
    ArbolBM();
    bool insertar(Producto* p);
    NodoBM* buscarPadre(NodoBM* actual, NodoBM* hijo);
    void eliminar(string categoria, Producto* p);
    void generarDotBM(string nombreArchivo);
    void mostrarProductosPorCategoria(std::string categoria);
};


#endif //GESTOR_DE_CATALOGO_ARBOLBM_H