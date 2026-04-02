# Sistema de Gestion de Catalogo - Estructuras de Datos

Este proyecto es un sistema de gestion de inventarios desarrollado en C++ para el curso de Estructura de Datos. El objetivo principal es demostrar y comparar el rendimiento de diversas estructuras en la gestion de un catalogo masivo de productos.



## Estructuras Implementadas

El sistema organiza y procesa los datos simultaneamente en las siguientes estructuras:
* Listas Enlazadas: Variantes Ordenadas y No Ordenadas.
* Tabla Hash: Gestion de colisiones y acceso optimizado.
* Arbol AVL: Arbol binario de busqueda con balanceo por altura.
* Arbol B: Estructura multirrama para manejo de grandes volumenes.
* Arbol B+ (B-M): Optimizacion del Arbol B para busquedas secuenciales y por rango.

## Requisitos del Sistema
* Compilador: g++ compatible con el estandar C++20.
* IDE recomendado: CLion 2025.3 o superior.
* Entorno: MinGW-w64.
* Archivo de Datos: Debe existir un archivo llamado Productos.csv en el directorio raiz.


## Instrucciones de Compilacion
### Desde la Terminal (g++)
Para compilar el proyecto incluyendo todos los subdirectorios y archivos fuente, utilice el siguiente comando:

* #### Bash
```bash 
g++ -std=c++20 -o gestor\_catalogo main.cpp Producto.cpp CargadorCSV.cpp ListasEnlazada/\*.cpp ListasEnlazada/NoOrdenada/\*.cpp ListasEnlazada/Ordenada/\*.cpp TablaHash/\*.cpp ArbolAVL/\*.cpp ArbolB/\*.cpp ArbolBM/\*.cpp -I.
``` 
* #### Desde CLion
  * Abrir la carpeta raiz del proyecto en el IDE.
  * Cargar el archivo CMakeLists.txt.
  * Seleccionar el perfil de Debug o Release.
  * Presionar el icono del Martillo (Build) para generar el ejecutable.

## Instrucciones de Ejecucion

Verifique que el archivo Productos.csv este en la misma carpeta que el ejecutable generado.

Ejecute el binario desde la consola:

* En Windows:
  * Bash
```bash 
gestor\_catalogo.exe
```

## Organizacion del Proyecto
El codigo fuente se encuentra segmentado en modulos especificos:
* /ArbolAVL: Implementacion del arbol balanceado y sus rotaciones.
* /ArbolB y /ArbolBM: Implementaciones de estructuras de grado superior.
* /ListasEnlazada: Contiene las carpetas /Ordenada y /NoOrdenada.
* /TablaHash: Logica de funciones hash y manejo de memoria.
* /CargadorCSV: Modulo encargado de la lectura y parseo del archivo de datos.

## Funcionalidades Principales
* Carga Masiva: Importacion de 1,000 registros de forma eficiente.
* Analisis de Rendimiento: Medicion de tiempos de respuesta en microsegundos.
* Busqueda por Barcode: Comparativa de eficiencia entre Arboles.
* Manejo de Memoria: Implementacion de destructores para evitar fugas de memoria.

