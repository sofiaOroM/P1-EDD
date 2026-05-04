#include "TablaHash.h"
#include "../Producto.h"
#include <iostream>
#include <fstream>
#include <vector>

TablaHash::TablaHash(int tamaño) : capacidad(tamaño), cantidadElementos(0) {
    tabla = new Nodo*[capacidad];
    for (int i = 0; i < capacidad; ++i) {
        tabla[i] = nullptr;
    }
}

TablaHash::~TablaHash() {
    vaciar();
    delete[] tabla;
}

// Algoritmo de hashing para strings
int TablaHash::funcionHash(std::string llave) {
    unsigned long hash = 5381;
    for (char c : llave) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacidad;
}

// Inserción con validación de duplicados
bool TablaHash::insertar(Producto* producto) {
    if (buscar(producto->barcode) != nullptr) {
        return false; // Error: El código de barras ya existe
    }

    int indice = funcionHash(producto->barcode);
    Nodo* nuevo = new Nodo(producto);

    // Insertar al inicio de la lista
    nuevo->siguiente = tabla[indice];
    tabla[indice] = nuevo;
    cantidadElementos++;
    return true;
}

// Búsqueda por código de barra: O(1) promedio
Producto* TablaHash::buscar(std::string barcode) {
    int indice = funcionHash(barcode);
    Nodo* actual = tabla[indice];

    while (actual != nullptr) {
        if (actual->producto->barcode == barcode) {
            return actual->producto;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

vector<Producto*> TablaHash::buscarCoincidenciasBarcode(string subcadena) {
    vector<Producto*> resultados;
    for (int i = 0; i < capacidad; i++) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            if (actual->producto->barcode.rfind(subcadena, 0) == 0) {
                resultados.push_back(actual->producto);
            }
            actual = actual->siguiente;
        }
    }
    return resultados;
}

bool TablaHash::eliminar(std::string barcode) {
    int indice = funcionHash(barcode);
    Nodo* actual = tabla[indice];
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->producto->barcode == barcode) {
            if (anterior == nullptr) {
                tabla[indice] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cantidadElementos--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

void TablaHash::vaciar() {
    for (int i = 0; i < capacidad; ++i) {
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        tabla[i] = nullptr;
    }
    cantidadElementos = 0;
}

double TablaHash::obtenerFactorCarga() {
    return (double)cantidadElementos / capacidad;
}

int TablaHash::contarColisiones() {
    int colisiones = 0;
    for (int i = 0; i < capacidad; ++i) {
        Nodo* actual = tabla[i];
        // Si hay más de un nodo en el mismo índice, hay colisión
        if (actual != nullptr && actual->siguiente != nullptr) {
            int nodosEnIndice = 0;
            while (actual != nullptr) {
                nodosEnIndice++;
                actual = actual->siguiente;
            }
            colisiones += (nodosEnIndice - 1);
        }
    }
    return colisiones;
}

void TablaHash::generarDot(string ruta) {
    ofstream archivo(ruta);
    archivo << "digraph G {" << endl;
    archivo << "  nodesep=.05;" << endl;
    archivo << "  rankdir=LR;" << endl; // De izquierda a derecha
    archivo << "  node [shape=record,width=.1,height=.1];" << endl;

    // Nodo principal de la tabla (el arreglo)
    archivo << "  tabla [label = \"";
    for (int i = 0; i < capacidad; i++) {
        archivo << "<f" << i << "> " << i;
        if (i < capacidad - 1) archivo << " | ";
    }
    archivo << "\", height=2.5];" << endl;

    archivo << "  node [shape=box];" << endl;

    // Factor de carga como una etiqueta flotante
    archivo << "  label = \"Factor de Carga: " << obtenerFactorCarga() << "\";" << endl;

    // Generar las listas de colisiones
    for (int i = 0; i < capacidad; i++) {
        Nodo* actual = tabla[i];
        int j = 0;
        while (actual != nullptr) {
            // Crear nodo para el producto
            archivo << "  node" << i << "_" << j << " [label=\"" << actual->producto->name << "\\n(" << actual->producto->barcode << ")\"];" << endl;

            // Conectar el índice de la tabla al primer nodo o nodos entre sí
            if (j == 0) {
                archivo << "  tabla:f" << i << " -> node" << i << "_" << j << ";" << endl;
            } else {
                archivo << "  node" << i << "_" << j-1 << " -> node" << i << "_" << j << ";" << endl;
            }
            actual = actual->siguiente;
            j++;
        }
    }
    archivo << "}" << endl;
    archivo.close();
}