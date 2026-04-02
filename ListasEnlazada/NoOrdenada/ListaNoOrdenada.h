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
    Producto* buscarPorNombre(string nombre); // Para comparación de tiempos
    bool eliminar(string barcode);
    void vaciar();
    bool estaVacia() const;
};


#endif //GESTOR_DE_CATALOGO_LISTANOORDENADA_H