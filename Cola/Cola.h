#ifndef GESTOR_DE_CATALOGO_COLA_H
#define GESTOR_DE_CATALOGO_COLA_H

#include <iostream>

template <typename T>
struct NodoCola {
    T dato;
    NodoCola<T>* siguiente;

    NodoCola(T d) : dato(d), siguiente(nullptr) {}
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* final;
    int tamano;

public:
    Cola();
    ~Cola();

    void encolar(T dato);
    T desencolar();
    T obtenerFrente() const;

    bool estaVacia() const;
    int obtenerTamano() const;
    void vaciar();
};

template <typename T>
Cola<T>::Cola() : frente(nullptr), final(nullptr), tamano(0) {}

template <typename T>
Cola<T>::~Cola() {
    vaciar();
}

template <typename T>
void Cola<T>::encolar(T dato) {
    NodoCola<T>* nuevo = new NodoCola<T>(dato);
    if (estaVacia()) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    tamano++;
}

template <typename T>
T Cola<T>::desencolar() {
    if (estaVacia()) return T();

    NodoCola<T>* aux = frente;
    T dato = aux->dato;
    frente = frente->siguiente;

    if (frente == nullptr) {
        final = nullptr;
    }

    delete aux;
    tamano--;
    return dato;
}

template <typename T>
T Cola<T>::obtenerFrente() const {
    if (estaVacia()) return nullptr;
    return frente->dato;
}

template <typename T>
bool Cola<T>::estaVacia() const {
    return frente == nullptr;
}

template <typename T>
int Cola<T>::obtenerTamano() const {
    return tamano;
}

template <typename T>
void Cola<T>::vaciar() {
    while (!estaVacia()) {
        desencolar();
    }
}

#endif //GESTOR_DE_CATALOGO_COLA_H