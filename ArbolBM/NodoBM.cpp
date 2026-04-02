#include "NodoBM.h"

NodoBM::NodoBM(bool hoja) {
    esHoja = hoja;
    claves = new std::string[ORDEN_BP];
    hijos = new NodoBM*[ORDEN_BP + 1];
    productos = hoja ? new ListaProductos*[ORDEN_BP] : nullptr;
    siguiente = nullptr;
    cuenta = 0;
    for (int i = 0; i < ORDEN_BP + 1; i++) hijos[i] = nullptr;
    if (hoja) {
        for (int i = 0; i < ORDEN_BP; i++) productos[i] = nullptr;
    }
}