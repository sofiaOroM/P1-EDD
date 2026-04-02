cmake_minimum_required(VERSION 4.1)
project(Gestor_de_Catalogo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Gestor_de_Catalogo main.cpp
        ListasEnlazada/Nodo.cpp
        ListasEnlazada/Nodo.h
        Producto.cpp
        Producto.h
        ListasEnlazada/NoOrdenada/ListaNoOrdenada.cpp
        ListasEnlazada/NoOrdenada/ListaNoOrdenada.h
        ListasEnlazada/Ordenada/ListaOrdenada.cpp
        ListasEnlazada/Ordenada/ListaOrdenada.h
        TablaHash/TablaHash.cpp
        TablaHash/TablaHash.h
        CargadorCSV.cpp
        CargadorCSV.h
        ArbolAVL/NodoAVL.cpp
        ArbolAVL/NodoAVL.h
        ArbolAVL/ArbolAVL.cpp
        ArbolAVL/ArbolAVL.h
        ArbolB/NodoB.cpp
        ArbolB/NodoB.h
        ArbolB/ArbolB.cpp
        ArbolB/ArbolB.h
        ArbolBM/NodoBM.cpp
        ArbolBM/NodoBM.h
        ArbolBM/ArbolBM.cpp
        ArbolBM/ArbolBM.h
)
