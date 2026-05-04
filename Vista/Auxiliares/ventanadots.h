#ifndef GESTOR_DE_CATALOGO_VENTANADOTS_H
#define GESTOR_DE_CATALOGO_VENTANADOTS_H

#include <QWidget>
#include <QDialog>
#include <QProcess>
#include <QPixmap>
#include "../../Sucursales/Sucursal.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class VentanaDots;
}

QT_END_NAMESPACE

class VentanaDots : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaDots(Sucursal* sucursal, QWidget* parent = nullptr);
    ~VentanaDots() override;

private slots:
    void actualizarGrafico();
    void on_btnExportarArbol_clicked();

private:
    Ui::VentanaDots* ui;
    Sucursal* sucursalActual;
    void generarImagen(QString dotPath, QString imgPath);
};


#endif //GESTOR_DE_CATALOGO_VENTANADOTS_H
