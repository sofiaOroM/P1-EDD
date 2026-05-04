#include "Sucursal.h"

Sucursal::Sucursal(int idSucursal, string nombreSucursal, string ubicacion, int tIngreso, int tTraspaso, int tDespacho)
        : id(idSucursal), nombre(nombreSucursal), ubicacion(ubicacion),
          t_ingreso(tIngreso), t_traspaso(tTraspaso), t_despacho(tDespacho), hash(1301) {}

int Sucursal::obtenerId()
{
    return id;
}

string Sucursal::obtenerNombre()
{
    return nombre;
}