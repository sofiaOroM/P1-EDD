#include "VentanaRendimiento.h"
#include "ui_VentanaRendimiento.h"
#include <chrono>
#include <QDialog>
#include <QtCharts>

VentanaRendimiento::VentanaRendimiento(ListaSucursales* red, QWidget *parent) :
    QDialog(parent), ui(new Ui::VentanaRendimiento), miRed(red) {
    ui->setupUi(this);

    if (miRed) {
        NodoSucursal* actual = miRed->obtenerPrimero();
        while (actual != nullptr) {
            ui->comboSucursalesRendimiento->addItem(QString::fromStdString(actual->dato->nombre));
            actual = actual->siguiente;
        }
    }

    ui->tableRendimiento->setColumnCount(4);
    ui->tableRendimiento->setRowCount(0);
    ui->tableRendimiento->setHorizontalHeaderLabels({"Estructura", "Complejidad", "Promedio (ns)", "Eficiencia"});
    ui->tableRendimiento->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableRendimiento->verticalHeader()->setVisible(false); // Quita los números de fila extra
    connect(ui->txtBarcodeRendimiento, &QLineEdit::editingFinished, this, &VentanaRendimiento::autocompletarPorCodigo);
    connect(ui->txtNombreRendimiento, &QLineEdit::editingFinished, this, &VentanaRendimiento::autocompletarPorNombre);
}

void VentanaRendimiento::on_btnEjecutarPrueba_clicked() {
    string barcode = ui->txtBarcodeRendimiento->text().toStdString();
    string nombre = ui->txtNombreRendimiento->text().toStdString();

    if (barcode.empty() || nombre.empty()) return;

    Sucursal* suc = miRed->buscarPorNombre(ui->comboSucursalesRendimiento->currentText().toStdString());
    if (!suc) return;

    // Ajustamos N a 20 según instrucción del ingeniero
    const int N = 20;

    // --- Mediciones en NANOSEGUNDOS para evitar el 0 ---

    // 1. Tabla Hash
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++) suc->hash.buscar(barcode);
    auto end = std::chrono::high_resolution_clock::now();
    long long tHash = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N;

    // 2. Árbol AVL
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++) suc->arbolAvl.buscarPorNombre(nombre);
    end = std::chrono::high_resolution_clock::now();
    long long tAVL = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N;

    // 3. Lista NO Ordenada
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++) suc->listaN.buscarPorNombre(nombre);
    end = std::chrono::high_resolution_clock::now();
    long long tListaN = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N;

    // 4. Lista Ordenada
    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<N; i++) suc->listaO.buscarPorNombre(nombre);
    end = std::chrono::high_resolution_clock::now();
    long long tListaO = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / N;

    // Actualizar Tabla
    ui->tableRendimiento->setRowCount(4);
    auto setCelda = [&](int f, QString e, QString c, long long t) {
        ui->tableRendimiento->setItem(f, 0, new QTableWidgetItem(e));
        ui->tableRendimiento->setItem(f, 1, new QTableWidgetItem(c));
        ui->tableRendimiento->setItem(f, 2, new QTableWidgetItem(QString::number(t) + " ns"));

        // Cálculo de eficiencia simple para llenar la columna
        QString ef = (t < 500) ? "Excelente" : (t < 2000 ? "Buena" : "Regular");
        ui->tableRendimiento->setItem(f, 3, new QTableWidgetItem(ef));
    };

    setCelda(0, "Tabla Hash", "O(1)", tHash);
    setCelda(1, "Árbol AVL", "O(log n)", tAVL);
    setCelda(2, "Lista Ord.", "O(n)", tListaO);
    setCelda(3, "Lista No Ord.", "O(n)", tListaN);

    inicializarGrafica(tHash, tAVL, tListaO, tListaN);
}

void VentanaRendimiento::inicializarGrafica(long long tHash, long long tAVL, long long tListaO, long long tListaN) {
    QBarSet *set = new QBarSet("Latencia de Búsqueda");
    *set << tHash << tAVL << tListaO << tListaN;

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Comparativa de Estructuras (Escala: Nanosegundos)");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Hash" << "AVL" << "Lista Ord." << "Lista No Ord.";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Tiempo (ns)");
    axisY->applyNiceNumbers();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void VentanaRendimiento::autocompletarPorCodigo() {
    string barcode = ui->txtBarcodeRendimiento->text().toStdString();
    if (barcode.empty()) return;

    Sucursal* suc = miRed->buscarPorNombre(ui->comboSucursalesRendimiento->currentText().toStdString());
    if (!suc) return;

    // Buscamos en la Hash para obtener el nombre
    Producto* p = suc->hash.buscar(barcode);
    if (p) {
        // Bloqueamos señales temporalmente para no entrar en un bucle infinito
        ui->txtNombreRendimiento->blockSignals(true);
        ui->txtNombreRendimiento->setText(QString::fromStdString(p->name));
        ui->txtNombreRendimiento->blockSignals(false);
    }
}

void VentanaRendimiento::autocompletarPorNombre() {
    string nombre = ui->txtNombreRendimiento->text().toStdString();
    if (nombre.empty()) return;

    Sucursal* suc = miRed->buscarPorNombre(ui->comboSucursalesRendimiento->currentText().toStdString());
    if (!suc) return;

    // Buscamos en el AVL para obtener el código
    Producto* p = suc->arbolAvl.buscarPorNombre(nombre);
    if (p) {
        ui->txtBarcodeRendimiento->blockSignals(true);
        ui->txtBarcodeRendimiento->setText(QString::fromStdString(p->barcode));
        ui->txtBarcodeRendimiento->blockSignals(false);
    }
}

VentanaRendimiento::~VentanaRendimiento() {
    delete ui;
}