#ifndef GESTOR_DE_CATALOGO_TRASPASO_H
#define GESTOR_DE_CATALOGO_TRASPASO_H

#include <QObject>
#include <QThread>
#include <vector>
#include <QColor>
#include "Sucursales/Sucursal.h"
#include "Cola/ControlEnvio.h"
#include "Sucursales/ListaSucursales.h"

class Traspaso : public QObject {
    Q_OBJECT

public:
    explicit Traspaso(ControlEnvio* envio, ListaSucursales* red);
public slots:
    void procesar();

signals:
    // Avisa a la UI para mover el punto en el grafo
    void cambioPosicion(int idEnvio, int idSedeActual);
    // Avisa para actualizar las etiquetas de "Cola de Ingreso/Salida"
    void actualizarColaVisual(int idEnvio, int tipoCola, bool insertando);
    // Notifica que el envío llegó a su destino final
    void finalizarEnvio(ControlEnvio* envio);

private:
    ControlEnvio* envio;
    ListaSucursales* red; // Puntero a la Red de Sucursales para consultar tiempos
};
#endif //GESTOR_DE_CATALOGO_TRASPASO_H