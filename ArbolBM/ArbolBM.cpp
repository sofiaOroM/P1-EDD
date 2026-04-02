#include "ArbolBM.h"
#include <queue>

ArbolBM::ArbolBM() : raiz(nullptr) {}

void ArbolBM::dividirHoja(NodoBM* izquierda, NodoBM* padre, Producto* p) {
    NodoBM* derecha = new NodoBM(true);

    // Crear un buffer temporal para manejar m+1 elementos
    std::string tempClaves[ORDEN_BP + 1];
    NodoBM::ListaProductos* tempProd[ORDEN_BP + 1];

    int i = 0;
    while (i < ORDEN_BP && izquierda->claves[i] < p->category) {
        tempClaves[i] = izquierda->claves[i];
        tempProd[i] = izquierda->productos[i];
        i++;
    }
    tempClaves[i] = p->category;
    tempProd[i] = new NodoBM::ListaProductos{p, nullptr};

    while (i < ORDEN_BP) {
        tempClaves[i + 1] = izquierda->claves[i];
        tempProd[i + 1] = izquierda->productos[i];
        i++;
    }

    // Repartir: izquierda se queda con el mínimo, el resto a la derecha
    int division = (ORDEN_BP + 1) / 2;
    izquierda->cuenta = division;
    derecha->cuenta = (ORDEN_BP + 1) - division;

    for (int j = 0; j < izquierda->cuenta; j++) {
        izquierda->claves[j] = tempClaves[j];
        izquierda->productos[j] = tempProd[j];
    }
    for (int j = 0; j < derecha->cuenta; j++) {
        derecha->claves[j] = tempClaves[j + division];
        derecha->productos[j] = tempProd[j + division];
    }

    derecha->siguiente = izquierda->siguiente;
    izquierda->siguiente = derecha;

    // Subir la clave al padre
    if (izquierda == raiz) {
        NodoBM* nuevaRaiz = new NodoBM(false);
        nuevaRaiz->claves[0] = derecha->claves[0];
        nuevaRaiz->hijos[0] = izquierda;
        nuevaRaiz->hijos[1] = derecha;
        nuevaRaiz->cuenta = 1;
        raiz = nuevaRaiz;
    } else {
        insertarInterno(derecha->claves[0], padre, derecha);
    }
}
void ArbolBM::insertarInterno(std::string clave, NodoBM* actual, NodoBM* hijo) {
    // Caso 1: El nodo actual (interno) tiene espacio
    if (actual->cuenta < ORDEN_BP) {
        int i = actual->cuenta - 1;
        // Desplazar claves e hijos para insertar la nueva clave
        while (i >= 0 && actual->claves[i] > clave) {
            actual->claves[i + 1] = actual->claves[i];
            actual->hijos[i + 2] = actual->hijos[i + 1];
            i--;
        }
        actual->claves[i + 1] = clave;
        actual->hijos[i + 2] = hijo;
        actual->cuenta++;
    }
    // Caso 2: El nodo interno está lleno, hay que dividirlo
    else {
        NodoBM* nuevoInterno = new NodoBM(false);

        // Buffers temporales para manejar M claves y M+1 hijos
        std::string tempClaves[ORDEN_BP + 1];
        NodoBM* tempHijos[ORDEN_BP + 2];

        // Copiar datos actuales al buffer
        for (int i = 0; i < ORDEN_BP; i++) tempClaves[i] = actual->claves[i];
        for (int i = 0; i < ORDEN_BP + 1; i++) tempHijos[i] = actual->hijos[i];

        // Insertar la nueva clave e hijo en el buffer ordenadamente
        int i = ORDEN_BP - 1;
        while (i >= 0 && tempClaves[i] > clave) {
            tempClaves[i + 1] = tempClaves[i];
            tempHijos[i + 2] = tempHijos[i + 1];
            i--;
        }
        tempClaves[i + 1] = clave;
        tempHijos[i + 2] = hijo;

        // El nodo interno se divide: la clave del medio SUBE al padre
        // y NO se queda en los hijos (a diferencia de las hojas)
        int division = (ORDEN_BP + 1) / 2; // Índice de la clave que sube
        actual->cuenta = division;
        nuevoInterno->cuenta = ORDEN_BP - division;

        // Repartir claves e hijos entre el nodo actual y el nuevo
        for (int j = 0; j < actual->cuenta; j++) {
            actual->claves[j] = tempClaves[j];
        }
        for (int j = 0; j < actual->cuenta + 1; j++) {
            actual->hijos[j] = tempHijos[j];
        }

        for (int j = 0; j < nuevoInterno->cuenta; j++) {
            nuevoInterno->claves[j] = tempClaves[j + division + 1];
        }
        for (int j = 0; j < nuevoInterno->cuenta + 1; j++) {
            nuevoInterno->hijos[j] = tempHijos[j + division + 1];
        }

        // Propagar la clave del medio hacia un nivel superior
        if (actual == raiz) {
            // Si la raíz se dividió, creamos una nueva raíz
            NodoBM* nuevaRaiz = new NodoBM(false);
            nuevaRaiz->claves[0] = tempClaves[division];
            nuevaRaiz->hijos[0] = actual;
            nuevaRaiz->hijos[1] = nuevoInterno;
            nuevaRaiz->cuenta = 1;
            raiz = nuevaRaiz;
        } else {
            // Llamada recursiva: buscar al padre del nodo 'actual'
            // Nota: En una implementación ideal, puedes pasar el padre como parámetro
            // o mantener punteros al padre en cada nodo.
            insertarInterno(tempClaves[division], buscarPadre(raiz, actual), nuevoInterno);
        }
    }
}
bool ArbolBM::insertar(Producto* p) {
    if (p == nullptr) return false;

    try {
        if (raiz == nullptr) {
            raiz = new NodoBM(true);
            raiz->claves[0] = p->category;
            raiz->productos[0] = new NodoBM::ListaProductos{p, nullptr};
            raiz->cuenta = 1;
            return true;
        } else {
            NodoBM* cursor = raiz;
            NodoBM* padre = nullptr;

            while (!cursor->esHoja) {
                padre = cursor;
                int i = 0;
                while (i < cursor->cuenta && p->category >= cursor->claves[i]) i++;
                cursor = cursor->hijos[i];
            }

            for (int i = 0; i < cursor->cuenta; i++) {
                if (cursor->claves[i] == p->category) {
                    auto actual = cursor->productos[i];
                    while (actual->siguiente) actual = actual->siguiente;
                    actual->siguiente = new NodoBM::ListaProductos{p, nullptr};
                    return true; // Inserción exitosa en categoría existente
                }
            }

            if (cursor->cuenta < ORDEN_BP) {
                int i = cursor->cuenta - 1;
                while (i >= 0 && cursor->claves[i] > p->category) {
                    cursor->claves[i + 1] = cursor->claves[i];
                    cursor->productos[i + 1] = cursor->productos[i];
                    i--;
                }
                cursor->claves[i + 1] = p->category;
                cursor->productos[i + 1] = new NodoBM::ListaProductos{p, nullptr};
                cursor->cuenta++;
                return true;
            } else {
                dividirHoja(cursor, padre, p);
                return true;
            }
        }
    } catch (...) {
        return false;
    }
}
/*void ArbolBM::insertar(Producto* p) {
    if (raiz == nullptr) {
        raiz = new NodoBM(true);
        raiz->claves[0] = p->category;
        raiz->productos[0] = new NodoBM::ListaProductos{p, nullptr};
        raiz->cuenta = 1;
    } else {
        NodoBM* cursor = raiz;
        NodoBM* padre = nullptr;

        // 1. Bajar hasta la hoja
        while (!cursor->esHoja) {
            padre = cursor;
            int i = 0;
            while (i < cursor->cuenta && p->category >= cursor->claves[i]) i++;
            cursor = cursor->hijos[i];
        }

        // 2. Comprobar si la categoría ya existe en la hoja
        for (int i = 0; i < cursor->cuenta; i++) {
            if (cursor->claves[i] == p->category) {
                auto actual = cursor->productos[i];
                while (actual->siguiente) actual = actual->siguiente;
                actual->siguiente = new NodoBM::ListaProductos{p, nullptr};
                return;
            }
        }

        // 3. Si no existe y hay espacio, insertar ordenado
        if (cursor->cuenta < ORDEN_BP) {
            int i = cursor->cuenta - 1;
            while (i >= 0 && cursor->claves[i] > p->category) {
                cursor->claves[i + 1] = cursor->claves[i];
                cursor->productos[i + 1] = cursor->productos[i];
                i--;
            }
            cursor->claves[i + 1] = p->category;
            cursor->productos[i + 1] = new NodoBM::ListaProductos{p, nullptr};
            cursor->cuenta++;
        }
        // 4. Si está llena, dividir hoja
        else {
            dividirHoja(cursor, padre, p);
        }
    }
}*/

NodoBM* ArbolBM::buscarPadre(NodoBM* actual, NodoBM* hijo) {
    if (actual == nullptr || actual->esHoja) return nullptr;

    for (int i = 0; i <= actual->cuenta; i++) {
        if (actual->hijos[i] == hijo) return actual;
        NodoBM* resultado = buscarPadre(actual->hijos[i], hijo);
        if (resultado != nullptr) return resultado;
    }
    return nullptr;
}

void ArbolBM::eliminar(std::string categoria, Producto* p) {
    if (raiz == nullptr) return;

    NodoBM* cursor = raiz;
    NodoBM* padre = nullptr;
    int indiceEnPadre = -1;

    // 1. Localizar la hoja que contiene la categoría
    while (!cursor->esHoja) {
        padre = cursor;
        int i = 0;
        while (i < cursor->cuenta && categoria >= cursor->claves[i]) i++;
        indiceEnPadre = i;
        cursor = cursor->hijos[i];
    }

    // 2. Buscar la categoría en la hoja
    bool encontrada = false;
    for (int i = 0; i < cursor->cuenta; i++) {
        if (cursor->claves[i] == categoria) {
            encontrada = true;

            // Eliminar el producto específico de la lista enlazada de esa categoría
            auto actual = cursor->productos[i];
            NodoBM::ListaProductos* anterior = nullptr;

            while (actual != nullptr) {
                if (actual->dato == p) {
                    if (anterior == nullptr) cursor->productos[i] = actual->siguiente;
                    else anterior->siguiente = actual->siguiente;
                    delete actual;
                    break;
                }
                anterior = actual;
                actual = actual->siguiente;
            }

            // 3. Si la categoría ya no tiene productos, eliminar la llave de la hoja
            if (cursor->productos[i] == nullptr) {
                for (int j = i; j < cursor->cuenta - 1; j++) {
                    cursor->claves[j] = cursor->claves[j + 1];
                    cursor->productos[j] = cursor->productos[j + 1];
                }
                cursor->cuenta--;

                // 4. Verificar si la hoja necesita rebalanceo (Underflow)
                // Para orden 3, el mínimo es 1 llave. Si queda en 0, se requiere fusión.
                if (cursor->cuenta == 0 && cursor != raiz) {
                    // Aquí se implementaría la fusión/redistribución con hermanos
                    std::cout << "Aviso: Rebalanceo por eliminación requerido." << std::endl;
                }
            }
            break;
        }
    }
}

void ArbolBM::generarDotBM(std::string nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    archivo << "digraph G {" << std::endl;
    // Ajustes de visualización jerárquica
    archivo << "  rankdir=TB;" << std::endl; // Dirección: Arriba hacia Abajo
    archivo << "  nodesep=0.5;" << std::endl; // Separación entre nodos
    archivo << "  ranksep=1.0;" << std::endl; // Separación entre niveles
    archivo << "  node [shape=record, style=filled, fillcolor=lightblue];" << std::endl;
    archivo << "  label=\"Estructura Árbol B+ - Categorías\";" << std::endl;

    if (raiz == nullptr) {
        archivo << "  vacio [label=\"Árbol Vacío\"];" << std::endl;
        archivo << "}" << std::endl;
        return;
    }

    std::queue<NodoBM*> cola;
    cola.push(raiz);

    // Almacenaremos las hojas para forzarlas al mismo nivel al final
    std::vector<NodoBM*> hojas;

    while (!cola.empty()) {
        NodoBM* actual = cola.front();
        cola.pop();

        // 1. Definición del nodo
        archivo << "  \"node" << actual << "\" [label=\"";
        for (int i = 0; i < actual->cuenta; i++) {
            archivo << "<f" << i << "> | " << actual->claves[i] << " | ";
        }
        archivo << "<f" << actual->cuenta << ">\"];" << std::endl;

        if (!actual->esHoja) {
            // 2. Conexiones a hijos
            for (int i = 0; i <= actual->cuenta; i++) {
                if (actual->hijos[i] != nullptr) {
                    archivo << "  \"node" << actual << "\":f" << i << " -> \"node" << actual->hijos[i] << "\";" << std::endl;
                    cola.push(actual->hijos[i]);
                }
            }
        } else {
            hojas.push_back(actual);
            // 3. Enlace horizontal (next) - Se mantiene con constraint=false
            if (actual->siguiente != nullptr) {
                archivo << "  \"node" << actual << "\" -> \"node" << actual->siguiente << "\" [style=dashed, color=red, constraint=false, label=\"next\"];" << std::endl;
            }
        }
    }

    // 4. Forzar que todas las hojas estén en el mismo nivel horizontal
    if (!hojas.empty()) {
        archivo << "  { rank=same; ";
        for (NodoBM* h : hojas) {
            archivo << "\"node" << h << "\"; ";
        }
        archivo << "}" << std::endl;
    }

    archivo << "}" << std::endl;
    archivo.close();
}

void ArbolBM::mostrarProductosPorCategoria(std::string categoria) {
    if (raiz == nullptr) {
        std::cout << "El arbol esta vacio." << std::endl;
        return;
    }

    // 1. Bajar hasta encontrar la hoja que debería tener la categoría
    NodoBM* cursor = raiz;
    while (!cursor->esHoja) {
        int i = 0;
        while (i < cursor->cuenta && categoria >= cursor->claves[i]) i++;
        cursor = cursor->hijos[i];
    }

    // 2. Buscar la categoría exacta dentro de esa hoja
    bool encontrada = false;
    for (int i = 0; i < cursor->cuenta; i++) {
        if (cursor->claves[i] == categoria) {
            encontrada = true;
            std::cout << "\n--- CATEGORIA: " << categoria << " ---" << std::endl;

            // 3. Recorrer la lista enlazada interna de productos
            NodoBM::ListaProductos* actual = cursor->productos[i];
            int contador = 1;
            while (actual != nullptr) {
                std::cout << contador << ". " << actual->dato->name
                        << " [Precio: Q" << actual->dato->price << "]" << std::endl;
                actual = actual->siguiente;
                contador++;
            }
            break;
        }
    }

    if (!encontrada) {
        std::cout << "La categoria '" << categoria << "' no tiene productos registrados." << std::endl;
    }
}