#ifndef GESTOR_DE_CATALOGO_NODOB_H
#define GESTOR_DE_CATALOGO_NODOB_H

#include "../Producto.h"

class NodoB {
public:
    Producto* claves[4];    // Máximo M-1 (4)
    NodoB* hijos[5];       // Máximo M (5)
    int cuenta;             // Claves actuales
    bool hoja;

    NodoB(bool esHoja);
};

#endif //GESTOR_DE_CATALOGO_NODOB_H