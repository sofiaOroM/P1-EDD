#include "TablaHash.h"
#include "../Producto.h"
#include <iostream>

TablaHash::TablaHash(int tamaño) : capacidad(tamaño), cantidadElementos(0) {
    tabla = new Nodo*[capacidad];
    for (int i = 0; i < capacidad; ++i) {
        tabla[i] = nullptr;
    }
}

TablaHash::~TablaHash() {
    vaciar();
    delete[] tabla;
}

// Algoritmo de hashing para strings
int TablaHash::funcionHash(std::string llave) {
    unsigned long hash = 5381;
    for (char c : llave) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacidad;
}

// Inserción con validación de duplicados [cite: 48]
bool TablaHash::insertar(Producto* producto) {
    if (buscar(producto->barcode) != nullptr) {
        return false; // Error: El código de barras ya existe [cite: 48]
    }

    int indice = funcionHash(producto->barcode);
    Nodo* nuevo = new Nodo(producto);

    // Insertar al inicio de la lista en ese balde (Chaining)
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
    cantidadElementos++;
    return true;
}

// Búsqueda por código de barra: O(1) promedio
Producto* TablaHash::buscar(std::string barcode) {
    int indice = funcionHash(barcode);
    Nodo* actual = tabla[indice];

    while (actual != nullptr) {
        if (actual->producto->barcode == barcode) {
            return actual->producto;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHash::eliminar(std::string barcode) {
    int indice = funcionHash(barcode);
    Nodo* actual = tabla[indice];
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->producto->barcode == barcode) {
            if (anterior == nullptr) {
                tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cantidadElementos--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

void TablaHash::vaciar() {
    for (int i = 0; i < capacidad; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        tabla[i] = nullptr;
    }
    cantidadElementos = 0;
}

double TablaHash::obtenerFactorCarga() {
    return (double)cantidadElementos / capacidad;
}