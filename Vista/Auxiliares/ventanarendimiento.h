#ifndef GESTOR_DE_CATALOGO_VENTANARENDIMIENTO_H
#define GESTOR_DE_CATALOGO_VENTANARENDIMIENTO_H

#include <QWidget>
#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "../../Sucursales/ListaSucursales.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class VentanaRendimiento;
}

QT_END_NAMESPACE

class VentanaRendimiento : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaRendimiento(ListaSucursales* red,QWidget* parent = nullptr);

    ~VentanaRendimiento() override;
private slots:
    void on_btnEjecutarPrueba_clicked();
    void autocompletarPorCodigo();
    void autocompletarPorNombre();
private:
    Ui::VentanaRendimiento *ui;
    ListaSucursales* miRed;
    void inicializarGrafica(long long tHash, long long tAVL, long long tListaO, long long tListaN);

};
#endif //GESTOR_DE_CATALOGO_VENTANARENDIMIENTO_H