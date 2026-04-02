#ifndef GESTOR_DE_CATALOGO_ARBOLB_H
#define GESTOR_DE_CATALOGO_ARBOLB_H

#include "NodoB.h"
#include <string>
#include <vector>
using namespace std;

class ArbolB {
private:
    NodoB* raiz;
    int orden;

    void dividirHijo(NodoB* padre, int i, NodoB* hijo);
    void insertarNoLleno(NodoB* nodo, Producto* producto);
    void buscarRangoRecursivo(NodoB* nodo, string inicio, string fin, vector<Producto*>& resultados);

public:
    ArbolB();
    ~ArbolB();
    void vaciar(NodoB* nodo);
    bool insertar(Producto* producto);
    vector<Producto*> buscarPorRangoFechas(string fechaInicio, string fechaFin);
    void generarDotB(string nombreArchivo);
    void escribirDotB(NodoB* nodo, ofstream& archivo, int& contador);
    void eliminar(Producto* p);
    void eliminarRecursivo(NodoB* nodo, Producto* p);
    void rebalancear(NodoB* padre, int idx);
    void fusionar(NodoB* padre, int idx);
    void prestarDelAnterior(NodoB* padre, int idx);
    void prestarDelSiguiente(NodoB* padre, int idx);
};

#endif //GESTOR_DE_CATALOGO_ARBOLB_H