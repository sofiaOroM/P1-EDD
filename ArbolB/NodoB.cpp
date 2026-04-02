#include "NodoB.h"

NodoB::NodoB(bool esHoja) {
    this->hoja = esHoja;
    this->cuenta = 0;
    for (int i = 0; i < 5; i++) {
        hijos[i] = nullptr;
    }
}