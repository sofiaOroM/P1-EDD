#ifndef GESTOR_DE_CATALOGO_LISTANOORDENADA_H
#define GESTOR_DE_CATALOGO_LISTANOORDENADA_H
#include "../Nodo.h"
#include <iostream>
using namespace std;

class ListaNoOrdenada
{
private:
    Nodo* inicio;

public:
    ListaNoOrdenada();
    ~ListaNoOrdenada();

    bool insertar(Producto* producto);
    Producto* buscarPorNombre(string nombre);
    bool eliminar(string barcode);
    void vaciar();
    bool estaVacia() const;
    Nodo* obtenerPrimero() const;

};


#endif //GESTOR_DE_CATALOGO_LISTANOORDENADA_H