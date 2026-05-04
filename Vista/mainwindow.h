#ifndef GESTOR_DE_CATALOGO_MAINWINDOW_H
#define GESTOR_DE_CATALOGO_MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "../CargadorCSV.h"
#include "../Sucursales/ListaSucursales.h"
#include "../Grafo/Grafo.h"
#include "../Cola/ControlEnvio.h"
#include "Auxiliares/VentanaConexion.h"
#include  "../Traspaso.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    QList<QGraphicsLineItem*> lineasRutaCalculada;
    void actualizarComboSucursales();
    void mostrarArchivoEnPantalla(QString ruta);
    void procesarBusqueda();
    void ofrecerAlternativas(QString barcode);
    void agregarProductoATabla(Producto* p);
    void actualizarVisibilidadBusqueda(const QString& criterio);
    void precalcularPosiciones();
    void dibujarSiguienteTramo();
    void ejecutarTraspasoConHilos(ControlEnvio* nuevoEnvio);

private slots:
    void on_actionCargarSucursales_triggered();
    void on_actionCargarProductos_triggered();
    void on_btnConfirmarEnvio_clicked();
    void on_btnCalcularRuta_clicked();
    void on_btnCrearSucursal_clicked();
    void on_btnActualizarSucursal_clicked();
    void on_btnEliminarSucursal_clicked();
    void on_btnEliminar_clicked();
    void on_btnConfirmarSucursal_clicked();
    void on_btnGestionarRutas_clicked();
    void on_btnVentanaRendimiento_clicked();
    void on_actionCargarConexiones_triggered();
    void on_btnCargarCSV_clicked();
    void on_btnVerGrafico_clicked();
    void on_comboSucursalesProcesos_currentIndexChanged(int index);
    void repoblarListasProcesos();
    void limpiarRutaVisual();
    // Navegación
    void mostrarInventario();
    void mostrarGrafo();
    void actualizarSimulacionGlobal();


private:
    Ui::MainWindow* ui;
    bool esSimulacionEnProgreso;
    ListaSucursales red;
    QGraphicsScene *escena;
    Grafo* grafo;
    void dibujarGrafoCircular();
    void finalizarEnvioEspecifico(ControlEnvio* envio);
    void cargarSucursalesEnTabla();
    QList<ControlEnvio*> enviosActivos;
    int contadorEnvios = 0;
    QTimer* timerGlobalSimulacion;
    QTimer* timerAnimacion;
    vector<int> rutaParaAnimar;
    int indiceTramoActual;
    QMap<int, QPointF> posicionesNodos;
    bool esEdicion = false;
    void mostrarFormularioSucursal(bool visible);
    void limpiarFormularioSucursal();
    void actualizarPosicionGrafica(int idEnvio, int idSiguienteNodo);
    void actualizarColaVisual(int idEnvio, int tipoCola, bool insertando);
};


#endif //GESTOR_DE_CATALOGO_MAINWINDOW_H