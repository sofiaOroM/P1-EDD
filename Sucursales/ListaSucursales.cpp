#include "ListaSucursales.h"

ListaSucursales::ListaSucursales() : primero(nullptr), tamano(0) {}

ListaSucursales::~ListaSucursales() {
    NodoSucursal* actual = primero;
    while (actual != nullptr) {
        NodoSucursal* siguiente = actual->siguiente;
        delete actual->dato;
        delete actual;
        actual = siguiente;
    }
}

void ListaSucursales::insertarFinal(Sucursal* s) {
    NodoSucursal* nuevo = new NodoSucursal(s);
    if (primero == nullptr) {
        primero = nuevo;
    } else {
        NodoSucursal* aux = primero;
        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
    tamano++;
}

Sucursal* ListaSucursales::buscarPorId(int id) {
    NodoSucursal* aux = primero;
    while (aux != nullptr) {
        if (aux->dato->obtenerId() == id) {
            return aux->dato;
        }
        aux = aux->siguiente;
    }
    return nullptr;
}

Sucursal* ListaSucursales::buscarPorNombre(string nombre) {
    NodoSucursal* actual = primero;
    while (actual != nullptr) {
        if (actual->dato->obtenerNombre() == nombre) {
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}
int ListaSucursales::obtenerTamano() const {
    return tamano;
}

NodoSucursal* ListaSucursales::obtenerPrimero() const {
    return primero;
}

Sucursal* ListaSucursales::obtenerSucursal(int id) {
    NodoSucursal* actual = primero;
    while (actual != nullptr) {
        if (actual->dato->id == id) {
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaSucursales::eliminar(int id) {
    if (primero == nullptr) return false;

    NodoSucursal* actual = primero;
    NodoSucursal* anterior = nullptr;

    // Buscar el nodo
    while (actual != nullptr && actual->dato->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encontró el ID
    if (actual == nullptr) return false;

    // Caso 1: Es el primer elemento
    if (anterior == nullptr) {
        primero = actual->siguiente;
    }
    // Caso 2: Está en medio o al final
    else {
        anterior->siguiente = actual->siguiente;
    }

    // Liberar memoria
    delete actual->dato; // Borra la instancia de Sucursal
    delete actual;       // Borra el contenedor Nodo
    tamano--;
    return true;
}