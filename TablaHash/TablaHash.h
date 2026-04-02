#ifndef GESTOR_DE_CATALOGO_TABLAHASH_H
#define GESTOR_DE_CATALOGO_TABLAHASH_H

#include "../ListasEnlazada/Nodo.h"
#include <string>

class TablaHash
{
private:
    static const int TAMANIO_INICIAL = 101;
    Nodo** tabla;
    int capacidad;
    int cantidadElementos;

    // Función Hash: Transforma el string del barcode en un índice
    int funcionHash(std::string llave);

public:
    TablaHash(int tamaño = TAMANIO_INICIAL);
    ~TablaHash();

    bool insertar(Producto* producto);
    Producto* buscar(std::string barcode);
    bool eliminar(std::string barcode);

    void vaciar();
    double obtenerFactorCarga();
};


#endif //GESTOR_DE_CATALOGO_TABLAHASH_H