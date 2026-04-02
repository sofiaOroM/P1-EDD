#ifndef GESTOR_DE_CATALOGO_NODOAVL_H
#define GESTOR_DE_CATALOGO_NODOAVL_H

struct Producto;

class NodoAVL
{
public:
    Producto* producto;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(Producto* producto);
};

#endif //GESTOR_DE_CATALOGO_NODOAVL_H
