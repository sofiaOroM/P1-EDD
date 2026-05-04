#include "Nodo.h"

Nodo::Nodo(Producto* producto) {
    this->producto = producto;
    this->siguiente = nullptr;
}