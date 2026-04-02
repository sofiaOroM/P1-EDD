#include "Nodo.h"
#include "../Producto.h"

Nodo::Nodo(Producto* producto) {
    this->producto = producto;
    this->siguiente = nullptr;
}