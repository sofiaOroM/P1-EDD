#ifndef GESTOR_DE_CATALOGO_GRAFO_H
#define GESTOR_DE_CATALOGO_GRAFO_H

#include <string>
#include <vector>

using namespace std;

struct Conexion {
    int tiempo;
    double costo;
    bool activa;

    Conexion() : tiempo(0), costo(0.0), activa(false) {}
};

class Grafo {
private:
    int tamanio;           // Capacidad máxima de la matriz
    int totalVertices;     // Cantidad de vértices registrados
    Conexion** matrizAdyacencia;
    string* datosVertice;  // Nombres de las sucursales asociados al índice

public:
    Grafo() : tamanio(0), totalVertices(0), matrizAdyacencia(nullptr), datosVertice(nullptr) {}
    Grafo(int maxNodos);
    ~Grafo();

    void agregarDatoVertice(int vertice, const string& dato);
    void agregarCamino(int origen, int destino, int tiempo, double costo, bool bidireccional = false);

    vector<int> obtenerRutaOptima(int idOrigen, int idDestino, bool porTiempo);
    bool existeConexion(int u, int v) const;

    string obtenerNombreSucursal(int id);
    int obtenerTamanio() const;
    double obtenerCostoEntre(int u, int v);
    int obtenerTiempoEntre(int u, int v);
    void eliminarCaminosDeNodo(int id);
    void redimensionar(int nuevoMaximo);
};

#endif //GESTOR_DE_CATALOGO_GRAFO_H