#include "ListaOrdenada.h"
#include "../../Producto.h"

ListaOrdenada::ListaOrdenada() : inicio(nullptr) {}

ListaOrdenada::~ListaOrdenada() {
    vaciar();
}

bool ListaOrdenada::insertar(Producto* producto) {
    if (producto == nullptr) return false;
    Nodo* nuevo = new Nodo(producto);
    if (nuevo == nullptr) return false;

    if (inicio == nullptr || producto->name < inicio->producto->name) {
        nuevo->siguiente = inicio;
        inicio = nuevo;
    } else {
        Nodo* actual = inicio;
        while (actual->siguiente != nullptr && actual->siguiente->producto->name < producto->name) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
    return true;
}

Producto* ListaOrdenada::buscarPorNombre(std::string nombre) {
    Nodo* actual = inicio;
    while (actual != nullptr) {
        if (actual->producto->name == nombre) return actual->producto;
        if (actual->producto->name > nombre) break;
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaOrdenada::eliminar(string barcode) {
    Nodo* actual = inicio;
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->producto->barcode == barcode) {
            if (anterior == nullptr) inicio = actual->siguiente;
            else anterior->siguiente = actual->siguiente;
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

void ListaOrdenada::vaciar() {
    while (inicio != nullptr) {
        Nodo* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }
}