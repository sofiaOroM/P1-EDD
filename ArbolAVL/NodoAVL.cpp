#include "NodoAVL.h"

NodoAVL::NodoAVL(Producto* producto)
{
    this->producto = producto;
    this->izquierdo = nullptr;
    this->derecho = nullptr;
    this->altura = 1;
}

