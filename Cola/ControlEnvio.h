#ifndef GESTOR_DE_CATALOGO_CONTROLENVIO_H
#define GESTOR_DE_CATALOGO_CONTROLENVIO_H

#include <vector>
#include <QGraphicsEllipseItem>
#include "Cola.h"
#include "../Producto.h"
using namespace std;

struct ControlEnvio
{
    int idEnvio;
    vector<int> ruta;
    int indiceActual;
    int idSedeActual;
    int cantidadTotal;
    int faseActual;
    QString nombreOrigen;
    QString nombreDestino;
    QColor colorEnvio;
    Cola<pair<Producto*, int>>* productos;
    int tiempoAcumulado;
    QGraphicsEllipseItem* puntoMovil;
};
#endif //GESTOR_DE_CATALOGO_CONTROLENVIO_H