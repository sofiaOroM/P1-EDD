#ifndef GESTOR_DE_CATALOGO_LISTAORDENADA_H
#define GESTOR_DE_CATALOGO_LISTAORDENADA_H

#include "../Nodo.h"
#include <string>
using namespace std;

class ListaOrdenada
{
private:
    Nodo* inicio;

public:
    ListaOrdenada();
    ~ListaOrdenada();

    bool insertar(Producto* producto);
    Producto* buscarPorNombre(string nombre);
    bool eliminar(string barcode);
    void vaciar();
};


#endif //GESTOR_DE_CATALOGO_LISTAORDENADA_H