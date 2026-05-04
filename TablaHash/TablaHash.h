#ifndef GESTOR_DE_CATALOGO_TABLAHASH_H
#define GESTOR_DE_CATALOGO_TABLAHASH_H

#include "../ListasEnlazada/Nodo.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class TablaHash
{
private:
    static const int TAMANIO_INICIAL = 101;
    Nodo** tabla;
    int capacidad;
    int cantidadElementos;

    // Función Hash: Transforma el string del barcode en un índice
    int funcionHash(string llave);

public:
    TablaHash(int tamaño = TAMANIO_INICIAL);
    ~TablaHash();

    bool insertar(Producto* producto);
    Producto* buscar(string barcode);
    vector<Producto*> buscarCoincidenciasBarcode(string subcadena);
    bool eliminar(string barcode);

    void vaciar();
    double obtenerFactorCarga();
    int contarColisiones();
    void generarDot(string ruta);
    void generarDotParcial(string ruta);
};


#endif //GESTOR_DE_CATALOGO_TABLAHASH_H