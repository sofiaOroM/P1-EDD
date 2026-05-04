#ifndef GESTOR_DE_CATALOGO_NODO_H
#define GESTOR_DE_CATALOGO_NODO_H

struct Producto;

class Nodo
{
public:
    Producto* producto;
    Nodo* siguiente;

    Nodo(Producto* producto);
};


#endif //GESTOR_DE_CATALOGO_NODO_H