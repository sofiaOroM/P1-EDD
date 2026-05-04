#include "ArbolAVL.h"

#include "../Producto.h"
#include <fstream>
#include <algorithm>

ArbolAVL::ArbolAVL() : inicio(nullptr)
{
}

ArbolAVL::~ArbolAVL()
{
    vaciar();
}

void ArbolAVL::vaciar()
{
    destruirRecursivo(this->inicio);
    this->inicio = nullptr;
}

void ArbolAVL::destruirRecursivo(NodoAVL* nodo)
{
    if (nodo != nullptr)
    {
        destruirRecursivo(nodo->izquierdo);
        destruirRecursivo(nodo->derecho);
        delete nodo;
    }
}

int ArbolAVL::obtenerAltura(NodoAVL* n)
{
    if (n == nullptr) return 0;
    return n->altura;
}

int ArbolAVL::obtenerFE(NodoAVL* n)
{
    if (n == nullptr) return 0;
    return obtenerAltura(n->izquierdo) - obtenerAltura(n->derecho);
}

int ArbolAVL::max(int a, int b)
{
    return (a > b) ? a : b;
}

void ArbolAVL::actualizarAltura(NodoAVL* n)
{
    if (n != nullptr)
    {
        n->altura = 1 + max(obtenerAltura(n->izquierdo), obtenerAltura(n->derecho));
    }
}

bool ArbolAVL::insertarPorNombre(Producto* producto)
{
    try
    {
        this->inicio = insertarRecursivo(this->inicio, producto);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

NodoAVL* ArbolAVL::insertarRecursivo(NodoAVL* nodo, Producto* producto)
{
    if (nodo == nullptr) return new NodoAVL(producto);

    // Si el nombre es menor, va a la izquierda
    if (producto->name < nodo->producto->name)
    {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, producto);
    }
    // Si es mayor o igual, va a la derecha (Permitiendo duplicados)
    else
    {
        nodo->derecho = insertarRecursivo(nodo->derecho, producto);
    }

    actualizarAltura(nodo);
    int fe = obtenerFE(nodo);

    // Casos de rotación
    if (fe > 1 && producto->name < nodo->izquierdo->producto->name)
        return rotarII(nodo);
    if (fe < -1 && producto->name >= nodo->derecho->producto->name)
        return rotarDD(nodo);
    if (fe > 1 && producto->name >= nodo->izquierdo->producto->name)
        return rotarID(nodo);
    if (fe < -1 && producto->name < nodo->derecho->producto->name)
        return rotarDI(nodo);

    return nodo;
}

Producto* ArbolAVL::buscarPorNombre(string nombre)
{
    NodoAVL* actual = this->inicio;
    while (actual != nullptr)
    {
        if (nombre == actual->producto->name)
        {
            return actual->producto;
        }
        if (nombre < actual->producto->name)
        {
            actual = actual->izquierdo;
        }
        else
        {
            actual = actual->derecho;
        }
    }
    return nullptr;
}

NodoAVL* ArbolAVL::rotarII(NodoAVL* nodo)
{
    if (nodo == nullptr || nodo->izquierdo == nullptr) return nodo;
    NodoAVL* actual = nodo; //nodo
    NodoAVL* nodoIzquierdo = actual->izquierdo; //nodo1

    actual->izquierdo = nodoIzquierdo->derecho;
    nodoIzquierdo->derecho = actual;

    actualizarAltura(actual);
    actualizarAltura(nodoIzquierdo);

    return nodoIzquierdo;
}

NodoAVL* ArbolAVL::rotarDD(NodoAVL* nodo)
{
    if (nodo == nullptr || nodo->derecho == nullptr) return nodo;
    NodoAVL* actual = nodo; //nodo
    NodoAVL* nodoDerecho = actual->derecho; //nodo1

    actual->derecho = nodoDerecho->izquierdo;
    nodoDerecho->izquierdo = actual;

    actualizarAltura(actual);
    actualizarAltura(nodoDerecho);

    return nodoDerecho;
}

NodoAVL* ArbolAVL::rotarID(NodoAVL* nodo)
{
    if (nodo == nullptr || nodo->izquierdo == nullptr || nodo->izquierdo->derecho == nullptr)
    {
        return nodo;
    }
    nodo->izquierdo = rotarDD(nodo->izquierdo);
    return rotarII(nodo);
}

NodoAVL* ArbolAVL::rotarDI(NodoAVL* nodo)
{
    if (nodo == nullptr || nodo->derecho == nullptr || nodo->derecho->izquierdo == nullptr)
    {
        return nodo;
    }
    nodo->derecho = rotarII(nodo->derecho);
    return rotarDD(nodo);
}

void ArbolAVL::generarReporteGrafico(std::string nombreArchivo)
{
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        std::cerr << "Error al crear el archivo .dot" << std::endl;
        return;
    }

    archivo << "digraph G {" << std::endl;
    archivo << "    node [shape=record, style=filled, fillcolor=lightblue];" << std::endl;
    archivo << "    label=\"Arbol AVL de Productos (por Nombre)\";" << std::endl;

    if (this->inicio != nullptr)
    {
        escribirDot(this->inicio, archivo);
    }
    else
    {
        archivo << "    \"Arbol Vacio\";" << std::endl;
    }

    archivo << "}" << std::endl;
    archivo.close();
    std::cout << "Reporte DOT generado: " << nombreArchivo << std::endl;
}

void ArbolAVL::escribirDot(NodoAVL* nodo, std::ofstream& archivo)
{
    if (nodo == nullptr) return;

    // Dirección de memoria como ID único para evitar confusiones con nombres iguales
    archivo << "    \"node" << nodo << "\" [label=\"{Nombre: " << nodo->producto->name
        << " | FE: " << obtenerFE(nodo) << "}\"];" << std::endl;

    if (nodo->izquierdo != nullptr)
    {
        archivo << "    \"node" << nodo << "\" -> \"node" << nodo->izquierdo << "\" [label=\"L\"];" << std::endl;
        escribirDot(nodo->izquierdo, archivo);
    }

    if (nodo->derecho != nullptr)
    {
        archivo << "    \"node" << nodo << "\" -> \"node" << nodo->derecho << "\" [label=\"R\"];" << std::endl;
        escribirDot(nodo->derecho, archivo);
    }
}

void ArbolAVL::eliminarPorNombre(std::string nombre)
{
    Producto* producto = buscarPorNombre(nombre);
    this->inicio = eliminarRecursivo(this->inicio, producto);
}

NodoAVL* ArbolAVL::eliminarRecursivo(NodoAVL* nodo, Producto* p)
{
    if (nodo == nullptr) return nullptr;

    if (p->name < nodo->producto->name)
    {
        nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, p);
    }
    else if (p->name > nodo->producto->name)
    {
        nodo->derecho = eliminarRecursivo(nodo->derecho, p);
    }
    else
    {
        // ENCONTRAMOS EL NOMBRE
        if (nodo->producto != p)
        {
            // Si no es el mismo objeto, seguimos buscando en el subárbol derecho
            nodo->derecho = eliminarRecursivo(nodo->derecho, p);
        }
        else
        {
            // Es el producto exacto, procedemos a borrar el nodo
            if (nodo->izquierdo == nullptr || nodo->derecho == nullptr)
            {
                NodoAVL* temp = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;
                if (temp == nullptr)
                {
                    temp = nodo;
                    nodo = nullptr;
                }
                else
                {
                    *nodo = *temp;
                }
                delete temp;
            }
            else
            {
                NodoAVL* temp = nodo->derecho;
                while (temp->izquierdo != nullptr) temp = temp->izquierdo;
                nodo->producto = temp->producto;
                // Eliminar por objeto, no solo por nombre
                nodo->derecho = eliminarRecursivo(nodo->derecho, temp->producto);
            }
        }
    }

    if (nodo == nullptr) return nullptr;

    actualizarAltura(nodo);
    int fe = obtenerFE(nodo);

    // Re-balanceo tras eliminar
    if (fe > 1 && obtenerFE(nodo->izquierdo) >= 0) return rotarII(nodo);
    if (fe > 1 && obtenerFE(nodo->izquierdo) < 0) return rotarID(nodo);
    if (fe < -1 && obtenerFE(nodo->derecho) <= 0) return rotarDD(nodo);
    if (fe < -1 && obtenerFE(nodo->derecho) > 0) return rotarDI(nodo);

    return nodo;
}

ListaNoOrdenada ArbolAVL::buscarCoincidencias(string subcadena) {
    ListaNoOrdenada resultados;
    if (subcadena.empty()) return resultados;

    string busqueda = subcadena;
    transform(busqueda.begin(), busqueda.end(), busqueda.begin(), ::tolower);

    buscarCoincidenciasRec(this->inicio, busqueda, resultados);
    return resultados;
}

void ArbolAVL::buscarCoincidenciasRec(NodoAVL* nodo, string subcadena, ListaNoOrdenada& resultados) {
    if (nodo == nullptr) return;

    string nombreProd = nodo->producto->name;
    transform(nombreProd.begin(), nombreProd.end(), nombreProd.begin(), ::tolower);

    if (nodo->izquierdo != nullptr) {
        buscarCoincidenciasRec(nodo->izquierdo, subcadena, resultados);
    }

    if (nombreProd.rfind(subcadena, 0) == 0) {
        resultados.insertar(nodo->producto);
    }

    if (nodo->derecho != nullptr) {
        buscarCoincidenciasRec(nodo->derecho, subcadena, resultados);
    }
}

vector<Producto*> ArbolAVL::obtenerTodoOrdenado() {
    vector<Producto*> lista;
    recorridoInorden(inicio, lista);
    return lista;
}

void ArbolAVL::recorridoInorden(NodoAVL* nodo, vector<Producto*>& lista) {
    if (nodo != nullptr) {
        recorridoInorden(nodo->izquierdo, lista);
        lista.push_back(nodo->producto);
        recorridoInorden(nodo->derecho, lista);
    }
}