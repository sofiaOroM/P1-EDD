cmake_minimum_required(VERSION 3.20)
project(Gestor_de_Catalogo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

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
        Vista/mainwindow.cpp
        Vista/mainwindow.h
        Vista/mainwindow.ui
        Sucursales/Sucursal.cpp
        Sucursales/Sucursal.h
        Grafo/Grafo.cpp
        Grafo/Grafo.h
        Sucursales/ListaSucursales.cpp
        Sucursales/ListaSucursales.h
        Cola/Cola.h
        Vista/Auxiliares/ventanadespacho.cpp
        Vista/Auxiliares/ventanadespacho.h
        Vista/Auxiliares/ventanadespacho.ui
        Cola/ControlEnvio.h
        Vista/Auxiliares/ventanadots.cpp
        Vista/Auxiliares/ventanadots.h
        Vista/Auxiliares/ventanadots.ui
        Vista/Auxiliares/VentanaConexion.h
        Traspaso.cpp
        Traspaso.h
        Vista/Auxiliares/ventanarendimiento.cpp
        Vista/Auxiliares/ventanarendimiento.h
        Vista/Auxiliares/ventanarendimiento.ui
)
target_link_libraries(Gestor_de_Catalogo PRIVATE Qt6::Widgets Qt6::Charts)