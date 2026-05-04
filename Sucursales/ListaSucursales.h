#ifndef GESTOR_DE_CATALOGO_LISTASUCURSALES_H
#define GESTOR_DE_CATALOGO_LISTASUCURSALES_H

#include "Sucursal.h"

// Nodo que envuelve a la sucursal para poder enlazarla
struct NodoSucursal {
    Sucursal* dato;
    NodoSucursal* siguiente;

    NodoSucursal(Sucursal* s) : dato(s), siguiente(nullptr) {}
};

class ListaSucursales {
private:
    NodoSucursal* primero;
    int tamano;

public:
    ListaSucursales();
    ~ListaSucursales();

    void insertarFinal(Sucursal* s);

    Sucursal* buscarPorId(int id);
    Sucursal* buscarPorNombre(string nombre);

    int obtenerTamano() const;
    NodoSucursal* obtenerPrimero() const;
    Sucursal* obtenerSucursal(int id);
    bool eliminar(int id);
};

#endif //GESTOR_DE_CATALOGO_LISTASUCURSALES_H