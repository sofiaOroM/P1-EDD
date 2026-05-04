#include "Grafo.h"
#include <climits>
#include <algorithm>

Grafo::Grafo(int maxNodos) : tamanio(maxNodos), totalVertices(0) {
    matrizAdyacencia = new Conexion*[tamanio];
    for (int i = 0; i < tamanio; i++) {
        matrizAdyacencia[i] = new Conexion[tamanio];
    }
    datosVertice = new string[tamanio];
}

Grafo::~Grafo() {
    for (int i = 0; i < tamanio; i++) {
        delete[] matrizAdyacencia[i];
    }
    delete[] matrizAdyacencia;
    delete[] datosVertice;
}

void Grafo::agregarDatoVertice(int vertice, const string& dato) {
    if (vertice >= 0 && vertice < tamanio) {
        datosVertice[vertice] = dato;
        if (vertice >= totalVertices) totalVertices = vertice + 1;
    }
}

void Grafo::agregarCamino(int origen, int destino, int tiempo, double costo, bool bidireccional) {
    if (origen >= 0 && origen < tamanio && destino >= 0 && destino < tamanio) {
        matrizAdyacencia[origen][destino].tiempo = tiempo;
        matrizAdyacencia[origen][destino].costo = costo;
        matrizAdyacencia[origen][destino].activa = true;

        if (bidireccional) {
            matrizAdyacencia[destino][origen].tiempo = tiempo;
            matrizAdyacencia[destino][origen].costo = costo;
            matrizAdyacencia[destino][origen].activa = true;
        }
    }
}

string Grafo::obtenerNombreSucursal(int id) {
    if (id >= 0 && id < tamanio) return datosVertice[id];
    return "";
}

vector<int> Grafo::obtenerRutaOptima(int idOrigen, int idDestino, bool porTiempo) {
    // Validar existencia de los nodos
    if (idOrigen < 0 || idOrigen >= tamanio || idDestino < 0 || idDestino >= tamanio) return {};

    vector<int> distancias(tamanio, INT_MAX);
    vector<int> padres(tamanio, -1);
    vector<bool> visitado(tamanio, false);

    distancias[idOrigen] = 0;

    for (int iteracion = 0; iteracion < tamanio; iteracion++) {

        // Seleccionar el vértice no visitado con menor distancia conocida
        int u = -1;
        int distanciaMinima = INT_MAX;

        for (int i = 0; i < tamanio; i++) {
            if (!visitado[i] && distancias[i] < distanciaMinima) {
                distanciaMinima = distancias[i];
                u = i;
            }
        }

        // Si no hay más nodos alcanzables o llegamos al destino termina
        if (u == -1 || u == idDestino) break;
        visitado[u] = true;

        // Intentar mejorar distancias de los vecinos de u
        for (int v = 0; v < tamanio; v++) {
            bool existeArista = matrizAdyacencia[u][v].activa;

            if (existeArista && !visitado[v]) {
                // Elegimos el peso según el criterio del usuario
                int pesoActual = porTiempo ? matrizAdyacencia[u][v].tiempo : (int)matrizAdyacencia[u][v].costo;

                // Verificamos desbordamiento antes de sumar
                if (distancias[u] != INT_MAX && distancias[u] + pesoActual < distancias[v]) {
                    distancias[v] = distancias[u] + pesoActual;
                    padres[v] = u; // Guardamos el predecesor para reconstruir la ruta
                }
            }
        }
    }

    // Reconstrucción del camino de regreso desde el destino hasta el origen
    vector<int> ruta;
    if (distancias[idDestino] == INT_MAX) return {}; // No hay ruta posible

    for (int v = idDestino; v != -1; v = padres[v]) {
        ruta.push_back(v);
    }

    // Invertir el vector para que vaya de Origen -> Destino
    reverse(ruta.begin(), ruta.end());

    return ruta;
}

bool Grafo::existeConexion(int u, int v)  const {
    if (matrizAdyacencia && u < tamanio && v < tamanio) {
        return matrizAdyacencia[u][v].activa;
    }
    return false;
}

int Grafo::obtenerTamanio() const
{
    return tamanio;
}

double Grafo::obtenerCostoEntre(int u, int v) {
    if (u >= 0 && u < tamanio && v >= 0 && v < tamanio) {
        if (matrizAdyacencia[u][v].activa) {
            return matrizAdyacencia[u][v].costo;
        }
    }
    return 0.0;
}

int Grafo::obtenerTiempoEntre(int u, int v) {
    if (u >= 0 && u < tamanio && v >= 0 && v < tamanio) {
        if (matrizAdyacencia[u][v].activa) {
            return matrizAdyacencia[u][v].tiempo;
        }
    }
    return 0;
}

void Grafo::eliminarCaminosDeNodo(int id) {
    if (id >= 0 && id < tamanio) {
        for (int i = 0; i < tamanio; i++) {
            // Eliminar rutas de salida
            matrizAdyacencia[id][i].activa = false;
            matrizAdyacencia[id][i].tiempo = 0;
            matrizAdyacencia[id][i].costo = 0.0;

            // Eliminar rutas de entrada
            matrizAdyacencia[i][id].activa = false;
            matrizAdyacencia[i][id].tiempo = 0;
            matrizAdyacencia[i][id].costo = 0.0;
        }
        datosVertice[id] = ""; // Limpiar el nombre almacenado
    }
}

void Grafo::redimensionar(int nuevoMaximo) {
    if (nuevoMaximo <= tamanio) return;

    Conexion** nuevaMatriz = new Conexion*[nuevoMaximo];
    for (int i = 0; i < nuevoMaximo; i++) {
        nuevaMatriz[i] = new Conexion[nuevoMaximo];
    }
    string* nuevosDatos = new string[nuevoMaximo];

    for (int i = 0; i < tamanio; i++) {
        nuevosDatos[i] = datosVertice[i];
        for (int j = 0; j < tamanio; j++) {
            nuevaMatriz[i][j] = matrizAdyacencia[i][j];
        }
    }

    for (int i = 0; i < tamanio; i++) {
        delete[] matrizAdyacencia[i];
    }
    delete[] matrizAdyacencia;
    delete[] datosVertice;

    // 4. Actualizar punteros y tamaño
    matrizAdyacencia = nuevaMatriz;
    datosVertice = nuevosDatos;
    tamanio = nuevoMaximo;
}