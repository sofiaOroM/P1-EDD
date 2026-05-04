#include "ArbolB.h"
#include <fstream>
#include <iostream>

ArbolB::ArbolB() {
    raiz = nullptr;
    orden = 5;
}
ArbolB::~ArbolB() {
    vaciar(raiz);
    raiz = nullptr;
}

void ArbolB::vaciar(NodoB* nodo) {
    if (nodo != nullptr) {
        if (!nodo->hoja) {
            for (int i = 0; i <= nodo->cuenta; i++) {
                vaciar(nodo->hijos[i]);
            }
        }
        delete nodo;
    }
}
bool ArbolB::insertar(Producto* producto) {
    if (producto == nullptr) return false;

    try {
        if (raiz == nullptr) {
            raiz = new NodoB(true);
            raiz->claves[0] = producto;
            raiz->cuenta = 1;
        } else {
            if (raiz->cuenta == 4) {
                NodoB* s = new NodoB(false);
                s->hijos[0] = raiz;
                dividirHijo(s, 0, raiz);
                int i = 0;
                if (producto->expiry_date > s->claves[0]->expiry_date) i++;
                insertarNoLleno(s->hijos[i], producto);
                raiz = s;
            } else {
                insertarNoLleno(raiz, producto);
            }
        }
        return true;
    } catch (...) {
        return false; // Si hubo un error (ej. falta de memoria), retorna false
    }
}

void ArbolB::dividirHijo(NodoB* padre, int i, NodoB* hijo) {
    NodoB* z = new NodoB(hijo->hoja);
    z->cuenta = 2; // (M-1)/2 para orden 5

    // Mover las últimas 2 claves de 'hijo' a 'z'
    for (int j = 0; j < 2; j++) {
        z->claves[j] = hijo->claves[j + 2];
    }

    // Mover los hijos si no es hoja
    if (!hijo->hoja) {
        for (int j = 0; j < 3; j++) {
            z->hijos[j] = hijo->hijos[j + 2];
        }
    }

    hijo->cuenta = 1; // Se queda con una clave (la del medio sube)

    // Desplazar hijos del padre para hacer espacio a 'z'
    for (int j = padre->cuenta; j >= i + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = z;

    // Desplazar claves del padre para subir la clave media
    for (int j = padre->cuenta - 1; j >= i; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }
    padre->claves[i] = hijo->claves[1];
    padre->cuenta++;
}

void ArbolB::insertarNoLleno(NodoB* nodo, Producto* producto) {
    int i = nodo->cuenta - 1;

    if (nodo->hoja) {
        while (i >= 0 && producto->expiry_date < nodo->claves[i]->expiry_date) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = producto;
        nodo->cuenta++;
    } else {
        while (i >= 0 && producto->expiry_date < nodo->claves[i]->expiry_date) {
            i--;
        }
        i++;
        if (nodo->hijos[i]->cuenta == 4) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (producto->expiry_date > nodo->claves[i]->expiry_date) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], producto);
    }
}

// BUSQUEDA POR FECHA
vector<Producto*> ArbolB::buscarPorRangoFechas(string inicio, string fin) {
    vector<Producto*> resultados;
    buscarRangoRecursivo(raiz, inicio, fin, resultados);
    return resultados;
}

void ArbolB::buscarRangoRecursivo(NodoB* nodo, string inicio, string fin, vector<Producto*>& res) {
    if (nodo == nullptr) return;

    int i = 0;
    // Avanzar hasta la primera clave que podría estar en el rango
    while (i < nodo->cuenta && inicio > nodo->claves[i]->expiry_date) {
        i++;
    }

    // Explorar hijos y claves
    for (; i < nodo->cuenta; i++) {
        buscarRangoRecursivo(nodo->hijos[i], inicio, fin, res);

        if (nodo->claves[i]->expiry_date >= inicio && nodo->claves[i]->expiry_date <= fin) {
            res.push_back(nodo->claves[i]);
        } else if (nodo->claves[i]->expiry_date > fin) {
            return; // Ya nos pasamos del rango
        }
    }
    // Por el último hijo derecho
    buscarRangoRecursivo(nodo->hijos[i], inicio, fin, res);
}

void ArbolB::generarDotB(string nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    archivo << "digraph G {" << endl;
    archivo << "  node [shape=record, height=.1, style=filled, fillcolor=lemonchiffon];" << endl;
    archivo << "  label=\"Estructura Arbol B (Orden 5) - Filtro por Fecha\";" << endl;

    if (raiz != nullptr) {
        int contador = 0;
        escribirDotB(raiz, archivo, contador);
    }

    archivo << "}" << endl;
    archivo.close();
    cout << "Reporte Arbol B generado: " << nombreArchivo << endl;
}

void ArbolB::escribirDotB(NodoB* nodo, ofstream& archivo, int& contador) {
    // Dirección de memoria como ID para que Graphviz no se confunda
    archivo << "  \"node" << nodo << "\" [label=\"";

    for (int i = 0; i < nodo->cuenta; i++) {
        // Mostramos la fecha del producto en cada celda del nodo
        archivo << "<f" << i << "> | " << nodo->claves[i]->expiry_date << " | ";
    }
    archivo << "<f" << nodo->cuenta << ">\"];" << endl;

    if (!nodo->hoja) {
        for (int i = 0; i <= nodo->cuenta; i++) {
            if (nodo->hijos[i] != nullptr) {
                // Conectamos el puerto <fi> del padre al nodo hijo
                archivo << "  \"node" << nodo << "\":f" << i << " -> \"node" << nodo->hijos[i] << "\";" << endl;
                escribirDotB(nodo->hijos[i], archivo, contador);
            }
        }
    }
}

void ArbolB::eliminar(Producto* p) {
    if (raiz == nullptr || p == nullptr) return;

    eliminarRecursivo(raiz, p);

    // Si la raíz se queda vacía después de una fusión
    if (raiz->cuenta == 0) {
        NodoB* aux = raiz;
        if (raiz->hoja) raiz = nullptr;
        else raiz = raiz->hijos[0];
        delete aux;
    }
}

void ArbolB::eliminarRecursivo(NodoB* nodo, Producto* p) {
    int idx = 0;
    // Navegamos usando la fecha
    while (idx < nodo->cuenta && nodo->claves[idx]->expiry_date < p->expiry_date) {
        idx++;
    }

    // Si se encuentra la fecha
    if (idx < nodo->cuenta && nodo->claves[idx]->expiry_date == p->expiry_date) {

        // Si el producto en esta clave no es el que queremos (porque hay fechas repetidas)
        // y el nodo no es hoja, se sigue buscando en los hijos.
        if (nodo->claves[idx] != p) {
            if (nodo->hoja) return; // No estaba el producto específico

            // Si no es el puntero exacto, tratamos el caso como si no lo hubiéramos encontrado
            // para que baje al hijo correspondiente (donde podría estar el duplicado)
            if (nodo->hijos[idx]->cuenta < 2) rebalancear(nodo, idx);
            eliminarRecursivo(nodo->hijos[idx], p);
            return;
        }

        // Si llegamos aquí, nodo->claves[idx] == p (es el producto exacto)
        if (nodo->hoja) {
            for (int i = idx + 1; i < nodo->cuenta; ++i)
                nodo->claves[i - 1] = nodo->claves[i];
            nodo->cuenta--;
        } else {
            // Reemplazar por sucesor o predecesor que sea el mismo u otro
            NodoB* predecesor = nodo->hijos[idx];
            while (!predecesor->hoja) predecesor = predecesor->hijos[predecesor->cuenta];

            nodo->claves[idx] = predecesor->claves[predecesor->cuenta - 1];
            eliminarRecursivo(nodo->hijos[idx], nodo->claves[idx]);
        }
    }
    else {
        if (nodo->hoja) return;
        bool esUltimo = (idx == nodo->cuenta);
        if (nodo->hijos[idx]->cuenta < 2) rebalancear(nodo, idx);

        if (esUltimo && idx > nodo->cuenta)
            eliminarRecursivo(nodo->hijos[idx - 1], p);
        else
            eliminarRecursivo(nodo->hijos[idx], p);
    }
}

void ArbolB::rebalancear(NodoB* padre, int idx) {
    // Intentar pedir prestado al hermano izquierdo
    if (idx != 0 && padre->hijos[idx - 1]->cuenta >= 2) {
        prestarDelAnterior(padre, idx);
    }
    // Intentar pedir prestado al hermano derecho
    else if (idx != padre->cuenta && padre->hijos[idx + 1]->cuenta >= 2) {
        prestarDelSiguiente(padre, idx);
    }
    // Si no se puede, fusionar
    else {
        if (idx != padre->cuenta) fusionar(padre, idx);
        else fusionar(padre, idx - 1);
    }
}

void ArbolB::fusionar(NodoB* padre, int idx) {
    NodoB* hijo = padre->hijos[idx];
    NodoB* hermano = padre->hijos[idx + 1];

    // Bajar la clave del padre al hijo
    hijo->claves[1] = padre->claves[idx];

    // Copiar claves del hermano al hijo
    for (int i = 0; i < hermano->cuenta; ++i)
        hijo->claves[i + 2] = hermano->claves[i];

    if (!hijo->hoja) {
        for (int i = 0; i <= hermano->cuenta; ++i)
            hijo->hijos[i + 2] = hermano->hijos[i];
    }

    // Desplazar claves en el padre
    for (int i = idx + 1; i < padre->cuenta; ++i)
        padre->claves[i - 1] = padre->claves[i];
    for (int i = idx + 2; i <= padre->cuenta; ++i)
        padre->hijos[i - 1] = padre->hijos[i];

    hijo->cuenta += hermano->cuenta + 1;
    padre->cuenta--;
    delete hermano;
}

void ArbolB::prestarDelAnterior(NodoB* padre, int idx) {
    NodoB* hijo = padre->hijos[idx];
    NodoB* hermano = padre->hijos[idx - 1];

    // Desplazar claves del hijo para hacer espacio
    for (int i = hijo->cuenta - 1; i >= 0; --i)
        hijo->claves[i + 1] = hijo->claves[i];

    if (!hijo->hoja) {
        for (int i = hijo->cuenta; i >= 0; --i)
            hijo->hijos[i + 1] = hijo->hijos[i];
    }

    // El hijo recibe la clave del padre
    hijo->claves[0] = padre->claves[idx - 1];
    if (!hijo->hoja) hijo->hijos[0] = hermano->hijos[hermano->cuenta];

    // El padre recibe la última clave del hermano
    padre->claves[idx - 1] = hermano->claves[hermano->cuenta - 1];

    hijo->cuenta++;
    hermano->cuenta--;
}

void ArbolB::prestarDelSiguiente(NodoB* padre, int idx) {
    NodoB* hijo = padre->hijos[idx];
    NodoB* hermano = padre->hijos[idx + 1];

    // El hijo recibe la clave del padre
    hijo->claves[hijo->cuenta] = padre->claves[idx];
    if (!hijo->hoja) hijo->hijos[hijo->cuenta + 1] = hermano->hijos[0];

    // El padre recibe la primera clave del hermano
    padre->claves[idx] = hermano->claves[0];

    // Desplazar claves y hijos del hermano hacia la izquierda
    for (int i = 1; i < hermano->cuenta; ++i)
        hermano->claves[i - 1] = hermano->claves[i];
    if (!hermano->hoja) {
        for (int i = 1; i <= hermano->cuenta; ++i)
            hermano->hijos[i - 1] = hermano->hijos[i];
    }

    hijo->cuenta++;
    hermano->cuenta--;
}