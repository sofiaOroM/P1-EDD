#ifndef GESTOR_DE_CATALOGO_NODOBM_H
#define GESTOR_DE_CATALOGO_NODOBM_H
#include <string>
#include "../Producto.h"

#define ORDEN_BP 3
class NodoBM
{
    public:
    bool esHoja;
    std::string* claves;
    NodoBM** hijos;
    NodoBM* siguiente; // Enlace entre hojas para recorrido eficiente

    // Para manejar múltiples productos en una misma categoría
    struct ListaProductos {
        Producto* dato;
        ListaProductos* siguiente;
    };
    ListaProductos** productos;

    int cuenta;

    NodoBM(bool hoja);
};


#endif //GESTOR_DE_CATALOGO_NODOBM_H