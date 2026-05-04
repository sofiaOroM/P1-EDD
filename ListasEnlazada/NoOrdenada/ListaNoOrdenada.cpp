#include "ListaNoOrdenada.h"
#include "../../Producto.h"

ListaNoOrdenada::ListaNoOrdenada() : inicio(nullptr) {}

ListaNoOrdenada::~ListaNoOrdenada() {
    vaciar();
}

bool ListaNoOrdenada::estaVacia() const {
    return inicio == nullptr;
}

Nodo* ListaNoOrdenada::obtenerPrimero() const
{
    return inicio;
}

// Inserción al frente: O(1)
bool ListaNoOrdenada::insertar(Producto* producto) {
    if (producto == nullptr) return false;
    Nodo* nuevo = new Nodo(producto);
    if (nuevo == nullptr) return false; //fallo en la memoria
    nuevo->siguiente = inicio;
    inicio = nuevo;
    return true;
}

// Búsqueda secuencial: O(n)
Producto* ListaNoOrdenada::buscarPorNombre(std::string nombre) {
    Nodo* actual = inicio;
    while (actual != nullptr) {
        if (actual->producto->name == nombre) return actual->producto;
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaNoOrdenada::eliminar(string barcode) {
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

void ListaNoOrdenada::vaciar() {
    while (inicio != nullptr) {
        Nodo* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }
}