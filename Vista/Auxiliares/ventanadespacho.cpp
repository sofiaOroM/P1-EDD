#include "ventanadespacho.h"
#include "ui_ventanadespacho.h"
#include <QMessageBox>
#include <QPushButton>

VentanaDespacho::VentanaDespacho(Sucursal* sucursal, QWidget* parent) :
    QDialog(parent), ui(new Ui::ventanaDespacho), sucursalOrigen(sucursal)
{
    ui->setupUi(this);

    ui->tablaInventario->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablaInventario->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablaInventario->setAlternatingRowColors(true);
    ui->tablaInventario->setStyleSheet("alternate-background-color: #f2f2f2; background-color: white;");

    ui->fechaInicio->setVisible(false);
    ui->fechaFin->setVisible(false);

    procesarBusqueda();

    connect(ui->txtBuscador, &QLineEdit::textChanged, this, &VentanaDespacho::procesarBusqueda);
    connect(ui->comboBuscarPor, &QComboBox::currentTextChanged, this, &VentanaDespacho::actualizarVisibilidadBusqueda);
    connect(ui->fechaInicio, &QDateEdit::dateChanged, this, &VentanaDespacho::procesarBusqueda);
    connect(ui->fechaFin, &QDateEdit::dateChanged, this, &VentanaDespacho::procesarBusqueda);
    connect(ui->btnConfirmar, &QPushButton::clicked, this, &VentanaDespacho::alConfirmar);
}

void VentanaDespacho::actualizarVisibilidadBusqueda(const QString& criterio)
{
    bool esFecha = (criterio == "Fecha de Caducidad");
    ui->fechaInicio->setVisible(esFecha);
    ui->fechaFin->setVisible(esFecha);
    ui->txtBuscador->setVisible(!esFecha);
    procesarBusqueda();
}

void VentanaDespacho::procesarBusqueda()
{
    QString texto = ui->txtBuscador->text();
    string busqueda = texto.toStdString();
    QString criterio = ui->comboBuscarPor->currentText();

    ui->tablaInventario->setRowCount(0);
    if (texto.isEmpty() && criterio == "Nombre")
    {
        vector<Producto*> todoElInventario = sucursalOrigen->arbolAvl.obtenerTodoOrdenado();

        for (Producto* p : todoElInventario) {
            agregarProductoATabla(p);
        }
        return;
    }
    if (criterio == "Nombre")
    {
        ListaNoOrdenada resultados = sucursalOrigen->arbolAvl.buscarCoincidencias(busqueda);
        Nodo* actual = resultados.obtenerPrimero();
        while (actual)
        {
            agregarProductoATabla(actual->producto);
            actual = actual->siguiente;
        }
    }
    else if (criterio == "Código de Barras")
    {
        vector<Producto*> resultados = sucursalOrigen->hash.buscarCoincidenciasBarcode(busqueda);
        for (Producto* p : resultados) agregarProductoATabla(p);
    }
    else if (criterio == "Categoría")
    {
        vector<Producto*> resultados = sucursalOrigen->arbolBM.buscarCoincidenciasCategoria(busqueda);
        for (Producto* p : resultados) agregarProductoATabla(p);
    }
    else if (criterio == "Fecha de Caducidad")
    {
        string inicio = ui->fechaInicio->date().toString("yyyy-MM-dd").toStdString();
        string fin = ui->fechaFin->date().toString("yyyy-MM-dd").toStdString();
        vector<Producto*> resultados = sucursalOrigen->arbolB.buscarPorRangoFechas(inicio, fin);
        for (Producto* p : resultados) agregarProductoATabla(p);
    }
}

void VentanaDespacho::agregarProductoATabla(Producto* p)
{
    if (!p) return;
    int fila = ui->tablaInventario->rowCount();
    ui->tablaInventario->insertRow(fila);

    ui->tablaInventario->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p->name)));
    ui->tablaInventario->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->barcode)));
    ui->tablaInventario->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->category)));
    ui->tablaInventario->setItem(fila, 3, new QTableWidgetItem(QString::number(p->getStock())));

    QTableWidgetItem* itemCant = new QTableWidgetItem("0");
    itemCant->setBackground(QBrush(QColor("#e8f4fd")));
    ui->tablaInventario->setItem(fila, 4, itemCant);
}

void VentanaDespacho::alConfirmar()
{
    listaFinal.clear();
    for (int i = 0; i < ui->tablaInventario->rowCount(); ++i)
    {
        int cantidad = ui->tablaInventario->item(i, 4)->text().toInt();
        int stockDisponible = ui->tablaInventario->item(i, 3)->text().toInt();

        if (cantidad > 0)
        {
            if (cantidad > stockDisponible)
            {
                QMessageBox::warning(this, "Error",
                                     "Stock insuficiente para: " + ui->tablaInventario->item(i, 0)->text());
                return;
            }
            string barcode = ui->tablaInventario->item(i, 1)->text().toStdString();
            Producto* p = sucursalOrigen->hash.buscar(barcode);
            listaFinal.push_back({p, cantidad});
        }
    }

    if (listaFinal.empty())
    {
        QMessageBox::information(this, "Aviso", "Seleccione al menos un producto.");
    }
    else
    {
        accept();
    }
}

vector<std::pair<Producto*, int>> VentanaDespacho::obtenerSeleccion()
{
    return listaFinal;
}

VentanaDespacho::~VentanaDespacho()
{
    delete ui;
}
