#ifndef GESTOR_DE_CATALOGO_VENTANADESPACHO_H
#define GESTOR_DE_CATALOGO_VENTANADESPACHO_H

#include <QDialog>
#include <ui_ventanadespacho.h>

#include "../../Sucursales/Sucursal.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class VentanaDespacho;
}

QT_END_NAMESPACE

class VentanaDespacho : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaDespacho(Sucursal* sucursal, QWidget *parent = nullptr);
    ~VentanaDespacho();

    vector<std::pair<Producto*, int>> obtenerSeleccion();

private slots:
    void procesarBusqueda();
    void actualizarVisibilidadBusqueda(const QString& criterio);
    void alConfirmar();

private:
    void agregarProductoATabla(Producto* p);
    Ui::ventanaDespacho* ui;
    Sucursal* sucursalOrigen;
    vector<pair<Producto*, int>> listaFinal;
};

#endif //GESTOR_DE_CATALOGO_VENTANADESPACHO_H