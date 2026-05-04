#ifndef GESTOR_DE_CATALOGO_CARGADORCSV_H
#define GESTOR_DE_CATALOGO_CARGADORCSV_H
#include <string>
#include <QTextEdit>
#include "./TablaHash/TablaHash.h"
#include "Grafo/Grafo.h"
#include "Sucursales/ListaSucursales.h"

using namespace std;

class CargadorCSV
{
public:
    static int identificarTipoArchivo(string ruta);
    static vector<string> dividirLineaCSV(string linea);
    static string limpiarToken(string token);
    static void cargarSucursales(string ruta, ListaSucursales& red, QTextEdit* logVisual);
    static void cargarConexiones(string ruta, Grafo& grafo, ListaSucursales& red, QTextEdit* logVisual);
    static void cargarProductos(string ruta, ListaSucursales& red, QTextEdit* logVisual);
private:
    static string normalizarFecha(string fechaOriginal);
    static bool esFechaValida(string& fecha);
    static void registrarError(string mensaje, QTextEdit* logVisual = nullptr);
};


#endif //GESTOR_DE_CATALOGO_CARGADORCSV_H