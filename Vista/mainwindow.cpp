#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "Auxiliares/ventanadespacho.h"
#include "Auxiliares/ventanadots.h"
#include "Auxiliares/ventanaRendimiento.h"
#include <QtMath>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    grafo(nullptr),
    esSimulacionEnProgreso(false)
{
    ui->setupUi(this);
    escena = new QGraphicsScene(this);
    if (ui->graphicsView)
    {
        ui->graphicsView->setScene(escena);
    }

    ui->tableProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableProductos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProductos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableProductos->setAlternatingRowColors(true);
    ui->tableProductos->setStyleSheet("alternate-background-color: #f2f2f2; background-color: white;");
    ui->fechaInicio->setVisible(false);
    ui->fechaFin->setVisible(false);

    timerAnimacion = new QTimer(this);
    timerGlobalSimulacion = new QTimer(this);
    connect(ui->txtBuscador, &QLineEdit::textChanged, this, &MainWindow::procesarBusqueda);
    connect(ui->comboBuscarPor, &QComboBox::currentIndexChanged, this, &MainWindow::procesarBusqueda);
    connect(ui->fechaInicio, &QDateEdit::dateChanged, this, &MainWindow::procesarBusqueda);
    connect(ui->fechaFin, &QDateEdit::dateChanged, this, &MainWindow::procesarBusqueda);
    connect(timerAnimacion, &QTimer::timeout, this, &MainWindow::dibujarSiguienteTramo);
    connect(timerGlobalSimulacion, &QTimer::timeout, this, &MainWindow::actualizarSimulacionGlobal);
    connect(ui->btnRealizarEnvio, &QPushButton::clicked, this, &MainWindow::on_btnConfirmarEnvio_clicked);
    connect(ui->comboSucursales, &QComboBox::currentTextChanged, this, &MainWindow::procesarBusqueda);
    ui->tableSucursales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSucursales->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableSucursales->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableSucursales->setAlternatingRowColors(true);
    ui->tableSucursales->setStyleSheet("alternate-background-color: #f2f2f2; background-color: white;");
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 1) {
            cargarSucursalesEnTabla();
            mostrarFormularioSucursal(false);
        }
    });
    connect(ui->btnCarga, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->btnInventario, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(ui->btnMapa, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(4);
    });

    connect(ui->btnSimulacion, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btnSucursales, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->comboBuscarPor, &QComboBox::currentTextChanged,
            this, &MainWindow::actualizarVisibilidadBusqueda);
}

MainWindow::~MainWindow()
{
    if (grafo) delete grafo;
    delete ui;
}

void MainWindow::on_btnCargarCSV_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Cargar Archivo", "", "CSV (*.csv)");
    if (filePath.isEmpty()) return;
    ui->txtLogErrores->clear();
    ui->txtVistaPrevia->clear();

    mostrarArchivoEnPantalla(filePath);

    string ruta = filePath.toStdString();
    int tipo = CargadorCSV::identificarTipoArchivo(ruta);

    ui->txtLogErrores->append("<b>Iniciando nueva carga...</b>");
    ui->txtLogErrores->append("Archivo: " + filePath);
    ui->txtLogErrores->append("------------------------------------------");

    switch (tipo)
    {
    case 1:
        ui->txtLogErrores->append("<b>[SISTEMA]</b> Detectado: Archivo de Sucursales");
        ui->txtLogErrores->append("<font color='blue'>Cargando Sucursales...</font>");
        CargadorCSV::cargarSucursales(ruta, this->red, ui->txtLogErrores);
        if (grafo) delete grafo;
        grafo = new Grafo(red.obtenerTamano() + 5);
        actualizarComboSucursales();
        break;
    case 2:
        if (grafo)
        {
            ui->txtLogErrores->append("<b>[SISTEMA]</b> Detectado: Archivo de Conexiones");
            ui->txtLogErrores->append("<font color='blue'>Cargando Conexiones...</font>");
            CargadorCSV::cargarConexiones(ruta, *this->grafo, this->red, ui->txtLogErrores);
            dibujarGrafoCircular();
        }
        else
        {
            ui->txtLogErrores->append("Error: Cargue las sucursales antes que las conexiones.");
        }
        break;
    case 3:
        ui->txtLogErrores->append("<b>[SISTEMA]</b> Detectado: Archivo de Productos");
        ui->txtLogErrores->append("<font color='blue'>Cargando Catálogo de Productos...</font>");
        CargadorCSV::cargarProductos(ruta, this->red, ui->txtLogErrores);
        break;
    default:
        ui->txtLogErrores->append("<font color='red'><b>[ERROR]</b> Formato de encabezado no reconocido.</font>");
        break;
    }

    actualizarComboSucursales();
    ui->txtLogErrores->append("------------------------------------------");
    ui->txtLogErrores->append("<b>Proceso finalizado.</b>");
}

void MainWindow::on_actionCargarSucursales_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Seleccionar CSV de Sucursales", "", "CSV Files (*.csv)");

    if (!filePath.isEmpty())
    {
        CargadorCSV::cargarSucursales(filePath.toStdString(), this->red, ui->txtLogErrores);
        ui->txtLogErrores->append("Sucursales cargadas. Total: " + QString::number(red.obtenerTamano()));

        actualizarComboSucursales();
    }
}

void MainWindow::on_actionCargarConexiones_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Seleccionar CSV de Conexiones", "", "CSV Files (*.csv)");

    if (!filePath.isEmpty())
    {
        CargadorCSV::cargarConexiones(filePath.toStdString(), *this->grafo, this->red, ui->txtLogErrores);
        ui->txtLogErrores->append("Grafo de conexiones actualizado.");
    }
}

void MainWindow::on_actionCargarProductos_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Seleccionar CSV de Productos", "", "CSV Files (*.csv)");

    if (!filePath.isEmpty())
    {
        CargadorCSV::cargarProductos(filePath.toStdString(), this->red, ui->txtLogErrores);
        ui->txtLogErrores->append("Carga de productos finalizada. Revisa errors.log si hubo fallos.");
    }
}
void MainWindow::on_btnConfirmarEnvio_clicked()
{
    int idOrigen = ui->comboOrigen->currentData().toInt();
    int idDestino = ui->comboDestino->currentData().toInt();
    Sucursal* sucursalOrigen = red.obtenerSucursal(idOrigen);

    if (!sucursalOrigen) return;

    VentanaDespacho dialogo(sucursalOrigen, this);

    if (dialogo.exec() == QDialog::Accepted)
    {
        auto productosAEnviar = dialogo.obtenerSeleccion();
        if(productosAEnviar.empty()) return;

        vector<int> rutaCalculada = grafo->obtenerRutaOptima(idOrigen, idDestino, ui->radioButton_tiempo->isChecked());
        if(rutaCalculada.empty()) {
            QMessageBox::warning(this, "Error", "No hay ruta disponible.");
            return;
        }
        // 3. Crear el objeto de control del envío
        ControlEnvio* nuevoEnvio = new ControlEnvio();
        nuevoEnvio->idEnvio = ++contadorEnvios;
        nuevoEnvio->productos = new Cola<pair<Producto*, int>>();
        nuevoEnvio->cantidadTotal = 0;
        nuevoEnvio->ruta = rutaCalculada;
        nuevoEnvio->idSedeActual = idOrigen;

        // 4. Descontar stock y preparar datos para el hilo
        for (auto& p : productosAEnviar)
        {
            p.first->stock -= p.second;
            nuevoEnvio->productos->encolar(p);
            nuevoEnvio->cantidadTotal += p.second;
        }
        QColor colorAzar = QColor(rand()%256, rand()%256, rand()%256);
        nuevoEnvio->puntoMovil = escena->addEllipse(-8, -8, 16, 16,
                                 QPen(Qt::black), QBrush(colorAzar));

        // Validar que las posiciones de los nodos existan
        if(posicionesNodos.contains(idOrigen)) {
            nuevoEnvio->puntoMovil->setPos(posicionesNodos[idOrigen]);
        }
        nuevoEnvio->puntoMovil->setZValue(10);

        // 5. Asignar el resto de datos
        nuevoEnvio->colorEnvio = colorAzar; // Ya tenemos el color seguro
        nuevoEnvio->nombreOrigen = ui->comboOrigen->currentText();
        nuevoEnvio->nombreDestino = ui->comboDestino->currentText();

        // 6. Registrar y ejecutar
        enviosActivos.append(nuevoEnvio);
        ejecutarTraspasoConHilos(nuevoEnvio);
    }
}

void MainWindow::on_btnVerGrafico_clicked() {
    QString nombreSucursal = ui->comboSucursales->currentText();
    Sucursal* sucursal = red.buscarPorNombre(nombreSucursal.toStdString());

    if (sucursal) {
        VentanaDots ventana(sucursal, this);
        ventana.exec();
    } else {
        QMessageBox::warning(this, "Error", "Debe seleccionar una sucursal válida.");
    }
}

void MainWindow::on_btnCalcularRuta_clicked()
{
    limpiarRutaVisual();
    if (!grafo) return;
    esSimulacionEnProgreso = false;

    int idOrigen = ui->comboOrigen->currentData().toInt();
    int idDestino = ui->comboDestino->currentData().toInt();
    bool porTiempo = ui->radioButton_tiempo->isChecked();

    // Detener animación previa si existe
    timerAnimacion->stop();
    rutaParaAnimar = grafo->obtenerRutaOptima(idOrigen, idDestino, porTiempo);

    if (rutaParaAnimar.empty())
    {
        ui->textCalcularRuta->append(
            "<font color='red'><b>[Error]</b> No existe conexión entre los puntos seleccionados.</font>");
        return;
    }

    double costoAcumulado = 0;
    int tiempoAcumulado = 0;

    // Recorremos la ruta de dos en dos para sumar los pesos de la matriz
    for (size_t i = 0; i < rutaParaAnimar.size() - 1; ++i)
    {
        int u = rutaParaAnimar[i];
        int v = rutaParaAnimar[i + 1];

        costoAcumulado += grafo->obtenerCostoEntre(u, v);
        tiempoAcumulado += grafo->obtenerTiempoEntre(u, v);
    }

    ui->textCalcularRuta->clear();
    ui->textCalcularRuta->append(
        "<b>Ruta encontrada:</b> de " + ui->comboOrigen->currentText() + " a " + ui->comboDestino->currentText());

    if (porTiempo)
    {
        ui->textCalcularRuta->append(
            "<font color='#2ecc71'><b>Tiempo Total:</b> " + QString::number(tiempoAcumulado) + " minutos</font>");
        ui->textCalcularRuta->append("Costo asociado: Q" + QString::number(costoAcumulado, 'f', 2));
    }
    else
    {
        ui->textCalcularRuta->append(
            "<font color='#2ecc71'><b>Costo Total:</b> Q" + QString::number(costoAcumulado, 'f', 2) + "</font>");
        ui->textCalcularRuta->append("Tiempo estimado: " + QString::number(tiempoAcumulado) + " minutos");
    }

    dibujarGrafoCircular();
    precalcularPosiciones();
    indiceTramoActual = 0;
    timerAnimacion->start(600);
}
void MainWindow::on_btnCrearSucursal_clicked() {
    esEdicion = false;
    limpiarFormularioSucursal();
    mostrarFormularioSucursal(true);
    ui->btnConfirmarSucursal->setText("Registrar Nueva Sucursal");
}


void MainWindow::on_btnActualizarSucursal_clicked() {
    int fila = ui->tableSucursales->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección", "Por favor, seleccione una sucursal de la tabla.");
        return;
    }

    esEdicion = true;
    mostrarFormularioSucursal(true);
    ui->btnConfirmarSucursal->setText("Guardar Cambios");

    // Cargar datos actuales
    ui->txtIdSucursal->setText(ui->tableSucursales->item(fila, 0)->text());
    ui->txtIdSucursal->setEnabled(false); // El ID no debe cambiarse
    ui->txtNombreSucursal->setText(ui->tableSucursales->item(fila, 1)->text());
    ui->txtUbicacionSucursal->setText(ui->tableSucursales->item(fila, 2)->text());
    ui->spinIngreso->setValue(ui->tableSucursales->item(fila, 3)->text().toInt());
    ui->spinDespacho->setValue(ui->tableSucursales->item(fila, 4)->text().toInt());
    ui->spinTraspaso->setValue(ui->tableSucursales->item(fila, 5)->text().toInt());
}

void MainWindow::on_btnEliminarSucursal_clicked() {
    int fila = ui->tableSucursales->currentRow();
    if (fila < 0) return;

    int id = ui->tableSucursales->item(fila, 0)->text().toInt();

    auto resp = QMessageBox::question(this, "Eliminar", "¿Está seguro de eliminar esta sucursal? Se borrarán sus conexiones y datos.");
    if (resp == QMessageBox::Yes) {
        // Eliminar del grafo primero (limpia la matriz de adyacencia)
        if (grafo) {
            grafo->eliminarCaminosDeNodo(id);
        }

        // Eliminar de la lista de sucursales (red)
        if (red.eliminar(id)) {
            // Refrescar la interfaz
            cargarSucursalesEnTabla();
            actualizarComboSucursales();
            escena->clear();
            dibujarGrafoCircular(); // Redibuja el mapa sin la sucursal ni sus líneas
            mostrarFormularioSucursal(false);

            QMessageBox::information(this, "Éxito", "Sucursal y conexiones eliminadas correctamente.");
        }
    }
}

void MainWindow::on_btnEliminar_clicked()
{
    QTableWidgetItem* itemActual = ui->tableProductos->currentItem();
    if (!itemActual) {
        QMessageBox::warning(this, "Eliminar", "Por favor, seleccione un producto de la tabla.");
        return;
    }

    int fila = ui->tableProductos->currentRow();
    string barcode = ui->tableProductos->item(fila, 1)->text().toStdString();
    string nombre = ui->tableProductos->item(fila, 0)->text().toStdString();
    string categoria = ui->tableProductos->item(fila, 2)->text().toStdString();

    //Obtener la sucursal actual para saber dónde buscar
    int idSucursal = ui->comboSucursales->currentData().toInt();
    Sucursal* suc = red.obtenerSucursal(idSucursal);
    Producto* producto = suc->hash.buscar(barcode);

    if (!suc) return;

    auto respuesta = QMessageBox::question(this, "Confirmar",
                        "¿Está seguro de eliminar el producto: " + QString::fromStdString(nombre) + "?",
                        QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        // Eliminacion de todas las estructuras
        bool hashOk = suc->hash.eliminar(barcode);
        suc->arbolAvl.eliminarPorNombre(nombre);
        suc->arbolB.eliminar(producto);
        suc->arbolBM.eliminar(categoria, producto);
        suc->listaN.eliminar(barcode);
        suc->listaO.eliminar(barcode);

        if (hashOk) {
            QMessageBox::information(this, "Éxito", "Producto eliminado de la sucursal.");
            procesarBusqueda();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo encontrar el producto para eliminar.");
        }
    }
}

void MainWindow::on_btnConfirmarSucursal_clicked() {
    int id = ui->txtIdSucursal->text().toInt();
    string nombre = ui->txtNombreSucursal->text().toStdString();

    if (esEdicion) {
        Sucursal* s = red.obtenerSucursal(id);
        if (s) {
            s->nombre = nombre;
            s->t_ingreso = ui->spinIngreso->value();
            s->t_despacho = ui->spinDespacho->value();
            s->t_traspaso = ui->spinTraspaso->value();
            if(grafo) grafo->agregarDatoVertice(id, nombre);
            QMessageBox::information(this, "Actualización", "Datos actualizados correctamente.");
        }
    } else {
        if (red.obtenerSucursal(id)) {
            QMessageBox::warning(this, "Error", "El ID ya pertenece a otra sucursal.");
            return;
        }
        if (grafo && id >= grafo->obtenerTamanio()) {
            grafo->redimensionar(id + 1);
        }

        string ubicacion = ui->txtUbicacionSucursal->text().toStdString();
        Sucursal* nueva = new Sucursal(id, nombre, ubicacion,
                                       ui->spinIngreso->value(),
                                       ui->spinDespacho->value(),
                                       ui->spinTraspaso->value());
        red.insertarFinal(nueva);
        if(grafo) grafo->agregarDatoVertice(id, nombre);
    }
    cargarSucursalesEnTabla();
    actualizarComboSucursales();
    mostrarFormularioSucursal(false);
}

void MainWindow::on_btnGestionarRutas_clicked() {
    if (!grafo) {
        QMessageBox::warning(this, "Error", "Debe cargar sucursales primero.");
        return;
    }

    QMap<int, QString> sucursales;
    NodoSucursal* actual = red.obtenerPrimero();
    while(actual) {
        sucursales[actual->dato->id] = QString::fromStdString(actual->dato->nombre);
        actual = actual->siguiente;
    }

    VentanaConexion diag(this, sucursales);
    if (diag.exec() == QDialog::Accepted) {
        int u = diag.cbOrigen->currentData().toInt();
        int v = diag.cbDestino->currentData().toInt();
        if (u == v) return;

        grafo->agregarCamino(u, v, diag.sbTiempo->value(), diag.sbCosto->value(), true);
        dibujarGrafoCircular();
        QMessageBox::information(this, "Éxito", "Conexión establecida.");
    }
}

void MainWindow::on_btnVentanaRendimiento_clicked()
{
    VentanaRendimiento *ventana = new VentanaRendimiento(&this->red, this);
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ventana->show();
}
void MainWindow::mostrarInventario()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::mostrarGrafo()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::actualizarComboSucursales()
{
    ui->comboSucursales->clear();
    ui->comboSucursalesProcesos->clear();
    ui->comboOrigen->clear();
    ui->comboDestino->clear();

    NodoSucursal* actual = red.obtenerPrimero();
    ui->comboSucursalesProcesos->addItem("Ver Todas");
    while (actual != nullptr)
    {
        QString nombre = QString::fromStdString(actual->dato->nombre);
        int id = actual->dato->id;

        ui->comboOrigen->addItem(nombre, id);
        ui->comboDestino->addItem(nombre, id);
        ui->comboSucursales->addItem(nombre, id);
        ui->comboSucursalesProcesos->addItem(nombre, id);
        actual = actual->siguiente;
    }
}

void MainWindow::mostrarArchivoEnPantalla(QString ruta)
{
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->txtLogErrores->append("No se pudo abrir el visor para: " + ruta);
        return;
    }

    QTextStream in(&archivo);
    int contador = 1;

    QString contenidoNumerado = "";

    while (!in.atEnd())
    {
        QString linea = in.readLine();
        QString num = QString::number(contador).rightJustified(3, '0');
        contenidoNumerado += "<font color='#888888'>" + num + " | </font> " + linea + "\n";
        contador++;
    }

    ui->txtVistaPrevia->appendHtml("<pre>" + contenidoNumerado + "</pre>");
    archivo.close();
}

void MainWindow::procesarBusqueda()
{
    QString texto = ui->txtBuscador->text();
    string busqueda = texto.toStdString();
    QString criterio = ui->comboBuscarPor->currentText();
    QString nombreSucursal = ui->comboSucursales->currentText();

    Sucursal* sucursal = red.buscarPorNombre(nombreSucursal.toStdString());
    if (!sucursal) return;

    ui->tableProductos->setRowCount(0);

    // Reiniciar etiquetas de tiempo antes de buscar
    ui->lblTiempoAVL->setText("-");
    ui->lblTiempoHash->setText("-");
    ui->lblTiempoB->setText("-");
    ui->lblTiempoBPlus->setText("-");

    if (texto.isEmpty() && criterio == "Nombre")
    {
        vector<Producto*> todoElInventario = sucursal->arbolAvl.obtenerTodoOrdenado();
        for (Producto* p : todoElInventario) agregarProductoATabla(p);
        return;
    }

    vector<Producto*> resultados;

    // --- Búsqueda por Categoría (B+) ---
    if (criterio == "Categoría") {
        auto start = std::chrono::high_resolution_clock::now();
        resultados = sucursal->arbolBM.buscarCoincidenciasCategoria(busqueda);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        ui->lblTiempoBPlus->setText(QString::number(duration) + " µs");
    }
    // --- Búsqueda por Nombre (AVL) ---
    else if (criterio == "Nombre") {
        auto start = std::chrono::high_resolution_clock::now();
        ListaNoOrdenada listaAvl = sucursal->arbolAvl.buscarCoincidencias(busqueda);
        Nodo* actual = listaAvl.obtenerPrimero();
        while(actual) {
            resultados.push_back(actual->producto);
            actual = actual->siguiente;
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        ui->lblTiempoAVL->setText(QString::number(duration) + " µs");
    }
    // --- Búsqueda por Código de Barras (Hash) ---
    else if (criterio == "Código de Barras") {
        auto start = std::chrono::high_resolution_clock::now();
        resultados = sucursal->hash.buscarCoincidenciasBarcode(busqueda);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        ui->lblTiempoHash->setText(QString::number(duration) + " µs");
    }
    // --- Búsqueda por Fecha (Árbol B) ---
    else if (criterio == "Fecha de Caducidad") {
        string inicio = ui->fechaInicio->date().toString("yyyy-MM-dd").toStdString();
        string fin = ui->fechaFin->date().toString("yyyy-MM-dd").toStdString();

        auto start = std::chrono::high_resolution_clock::now();
        resultados = sucursal->arbolB.buscarPorRangoFechas(inicio, fin);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        ui->lblTiempoB->setText(QString::number(duration) + " µs");
    }

    // --- PROCESAMIENTO DE RESULTADOS ---
    if (resultados.empty()) {
        // Caso A: No existe el producto con ese nombre/categoría en esta sucursal
        ui->txtInventario->setText("No se encontraron coincidencias locales.");
    } else {
        ui->tableProductos->setRowCount(0);
        bool hayStockLocal = false;
        QString ultimoBarcode;

        for (Producto* p : resultados) {
            agregarProductoATabla(p);
            if (p->getStock() > 0) hayStockLocal = true;
            ultimoBarcode = QString::fromStdString(p->barcode);
        }

        // Caso B: El producto existe aquí, pero no hay stock
        if (!hayStockLocal && !ultimoBarcode.isEmpty()) {
            ofrecerAlternativas(ultimoBarcode);
        } else {
            ui->txtInventario->clear();
            ui->txtInventario->append("<b style='color: #2ecc71;'>[INFO]</b> Stock disponible localmente.");
        }
    }
}

void MainWindow::ofrecerAlternativas(QString barcode)
{
    ui->txtInventario->clear();
    ui->txtInventario->append("<b style='color: #e67e22;'>[AVISO]</b> Producto no encontrado en esta sede.");
    ui->txtInventario->append("<b>Buscando disponibilidad en la red de sucursales...</b>");
    ui->txtInventario->append("--------------------------------------------------");

    bool encontradoEnAlguna = false;
    NodoSucursal* actual = red.obtenerPrimero();

    while (actual != nullptr)
    {
        // Buscar en la Tabla Hash de cada sucursal
        Producto* p = actual->dato->hash.buscar(barcode.toStdString());

        if (p && p->getStock() > 0)
        {
            encontradoEnAlguna = true;
            QString nombreSuc = QString::fromStdString(actual->dato->nombre);
            QString nombreProd = QString::fromStdString(p->name);

            ui->txtInventario->append(QString("<b>%1</b>")
                .arg(nombreSuc));
            ui->txtInventario->append(QString("   - Producto: %1")
                .arg(nombreProd));
            ui->txtInventario->append(QString("   - Stock disponible: <font color='blue'>%1 unidades</font>")
                .arg(p->getStock()));
            ui->txtInventario->append("");
        }
        actual = actual->siguiente;
    }

    if (!encontradoEnAlguna)
    {
        ui->txtInventario->append("<font color='red'><b>Resultado:</b> El producto no existe en ninguna sucursal del sistema.</font>");
    }
}

void MainWindow::agregarProductoATabla(Producto* p)
{
    if (!p) return;
    int fila = ui->tableProductos->rowCount();
    ui->tableProductos->insertRow(fila);
    QTableWidgetItem* itemNombre = new QTableWidgetItem(QString::fromStdString(p->name));
    QTableWidgetItem* itemPrecio = new QTableWidgetItem("Q" + QString::number(p->price, 'f', 2));

    itemPrecio->setTextAlignment(Qt::AlignCenter);
    ui->tableProductos->setItem(fila, 0, itemNombre);
    ui->tableProductos->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->barcode)));
    ui->tableProductos->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->category)));
    ui->tableProductos->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p->expiry_date)));
    ui->tableProductos->setItem(fila, 4, new QTableWidgetItem(QString::number(p->getStock())));
    ui->tableProductos->setItem(fila, 5, itemPrecio);
}

void MainWindow::actualizarVisibilidadBusqueda(const QString& criterio)
{
    bool esBusquedaPorFecha = (criterio == "Fecha de Caducidad");

    ui->fechaInicio->setVisible(esBusquedaPorFecha);
    ui->fechaFin->setVisible(esBusquedaPorFecha);
    ui->txtBuscador->setVisible(!esBusquedaPorFecha);
}

void MainWindow::dibujarGrafoCircular()
{
    if (!grafo || red.obtenerTamano() == 0) return;
    //escena->clear();

    int total = red.obtenerTamano();
    double radioMapa = 200;
    double centroX = 300;
    double centroY = 300;

    // Mapa para guardar posiciones visuales temporalmente
    QMap<int, QPointF> puntos;
    double anguloPaso = 2.0 * M_PI / total;
    int i = 0;

    //Dibujar Nodos (Sucursales)
    NodoSucursal* actual = red.obtenerPrimero();
    while (actual != nullptr)
    {
        double angulo = i * anguloPaso;
        double px = centroX + radioMapa * qCos(angulo);
        double py = centroY + radioMapa * qSin(angulo);

        puntos[actual->dato->id] = QPointF(px, py);

        escena->addEllipse(px - 15, py - 15, 30, 30, QPen(Qt::blue), QBrush(Qt::white));
        auto* texto = escena->addText(QString::fromStdString(actual->dato->nombre));
        texto->setPos(px - 20, py + 15);

        actual = actual->siguiente;
        i++;
    }

    // Dibujar Aristas (Conexiones)
    QPen penArista(Qt::lightGray, 1);
    // Iteración sobre los IDs conocidos en el mapa de puntos
    for (int u : puntos.keys())
    {
        for (int v : puntos.keys())
        {
            if (grafo->existeConexion(u, v))
            {
                escena->addLine(QLineF(puntos[u], puntos[v]), penArista);
            }
        }
    }
}

void MainWindow::precalcularPosiciones()
{
    posicionesNodos.clear();
    int total = red.obtenerTamano();
    double anguloPaso = 2.0 * M_PI / total;
    int i = 0;

    NodoSucursal* actual = red.obtenerPrimero();
    while (actual)
    {
        double angulo = i * anguloPaso;
        posicionesNodos[actual->dato->id] = QPointF(300 + 200 * qCos(angulo), 300 + 200 * qSin(angulo));
        actual = actual->siguiente;
        i++;
    }
}

void MainWindow::dibujarSiguienteTramo()
{
    if (indiceTramoActual >= (int)rutaParaAnimar.size() - 1)
    {
        timerAnimacion->stop();
        return;
    }

    int u = rutaParaAnimar[indiceTramoActual];
    int v = rutaParaAnimar[indiceTramoActual + 1];

    if (posicionesNodos.contains(u) && posicionesNodos.contains(v))
    {
        // Verde si es envío, Azul si es cálculo
        QColor color = esSimulacionEnProgreso ? QColor("#2ecc71") : QColor("#3498db");
        QPen pen(color, 5);
        auto* linea = escena->addLine(QLineF(posicionesNodos[u], posicionesNodos[v]), pen);
        if (!esSimulacionEnProgreso) {
            lineasRutaCalculada.append(linea);
        }
        if (esSimulacionEnProgreso)
        {
            escena->addEllipse(posicionesNodos[v].x() - 6, posicionesNodos[v].y() - 6, 12, 12, QPen(Qt::black),
                               QBrush(QColor("#e67e22")));
        }
    }
    indiceTramoActual++;
}

void MainWindow::ejecutarTraspasoConHilos(ControlEnvio* nuevoEnvio) {
    QThread* thread = new QThread();
    Traspaso* worker = new Traspaso(nuevoEnvio, &this->red);

    worker->moveToThread(thread);

    // Conexiones para actualizar la UI desde el hilo
    connect(thread, &QThread::started, worker, &Traspaso::procesar);
    connect(worker, &Traspaso::cambioPosicion, this, &MainWindow::actualizarPosicionGrafica);
    connect(worker, &Traspaso::actualizarColaVisual, this, &MainWindow::actualizarColaVisual);
    // Limpieza automática de memoria
    connect(worker, &Traspaso::finalizarEnvio, this, &MainWindow::finalizarEnvioEspecifico);
    connect(worker, &Traspaso::finalizarEnvio, thread, &QThread::quit);
    connect(worker, &Traspaso::finalizarEnvio, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}

void MainWindow::actualizarSimulacionGlobal() {
    for (int i = 0; i < enviosActivos.size(); ++i) {
        ControlEnvio* envio = enviosActivos[i];

        if (envio->indiceActual < (int)envio->ruta.size() - 1) {
            int u = envio->ruta[envio->indiceActual];
            int v = envio->ruta[envio->indiceActual + 1];
            Sucursal* sSiguiente = red.obtenerSucursal(v);

            // Calcular tiempos según el tipo de sucursal
            int tiempoTramo = grafo->obtenerTiempoEntre(u, v);
            int tiempoInterno = (v == envio->ruta.back()) ? sSiguiente->t_ingreso : sSiguiente->t_traspaso;
            int tiempoDespachoTotal = sSiguiente->t_despacho * envio->productos->obtenerTamano();

            envio->tiempoAcumulado += (tiempoTramo + tiempoInterno + tiempoDespachoTotal);

            // Mover visualmente el punto
            envio->puntoMovil->setPos(posicionesNodos[v]);

            // Log de seguimiento individual
            ui->txtEnvios->append(QString("Envío #%1: Llegó a %2 (ETA: %3 min)")
                .arg(envio->idEnvio).arg(QString::fromStdString(sSiguiente->nombre)).arg(envio->tiempoAcumulado));

            envio->indiceActual++;
        } else {
            // El envío ha llegado a su destino final
            finalizarEnvioEspecifico(envio);
            enviosActivos.removeAt(i);
            --i; // Ajustar índice por la remoción
        }
    }

    if (enviosActivos.isEmpty()) timerGlobalSimulacion->stop();
}

void MainWindow::finalizarEnvioEspecifico(ControlEnvio* envio) {
    int idDestino = envio->ruta.back();
    Sucursal* sDestino = red.obtenerSucursal(idDestino);

    while (!envio->productos->estaVacia()) {
        auto p = envio->productos->desencolar();
        Producto* exist = sDestino->hash.buscar(p.first->barcode);

        if (exist) {
            exist->stock += p.second;
        } else {
            // Insertar en las 4 estructuras del destino (AVL, Hash, B, B+)
            Producto* nuevo = new Producto(p.first->name, p.first->barcode, p.first->category,
                                         p.first->expiry_date, p.first->brand, p.first->price, p.second);
            sDestino->hash.insertar(nuevo);
            sDestino->arbolAvl.insertarPorNombre(nuevo);
            sDestino->arbolB.insertar(nuevo);
            sDestino->arbolBM.insertar(nuevo);
        }
    }

    ui->txtEnvios->append(QString("<font color='#2ecc71'><b>[EXITO]</b> Envío #%1 entregado en %2.</font>")
        .arg(envio->idEnvio)
        .arg(ui->comboDestino->currentText()));

    escena->removeItem(envio->puntoMovil);
    delete envio->puntoMovil;
    delete envio->productos;
    delete envio;

    procesarBusqueda();
}

void MainWindow::cargarSucursalesEnTabla() {
    ui->tableSucursales->setRowCount(0);
    NodoSucursal* actual = red.obtenerPrimero();
    int total = red.obtenerTamano();
    for(int i = 0; i < total; ++i) {
        int fila = ui->tableSucursales->rowCount();
        ui->tableSucursales->insertRow(fila);
        ui->tableSucursales->setItem(fila, 0, new QTableWidgetItem(QString::number(actual->dato->id)));
        ui->tableSucursales->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(actual->dato->nombre)));
        ui->tableSucursales->setItem(fila,2, new QTableWidgetItem(QString::fromStdString(actual->dato->ubicacion)));
        ui->tableSucursales->setItem(fila, 3, new QTableWidgetItem(QString::number(actual->dato->t_ingreso)));
        ui->tableSucursales->setItem(fila, 4, new QTableWidgetItem(QString::number(actual->dato->t_despacho)));
        ui->tableSucursales->setItem(fila, 5, new QTableWidgetItem(QString::number(actual->dato->t_traspaso)));
        actual = actual->siguiente;
    }
}

void MainWindow::mostrarFormularioSucursal(bool visible) {
    ui->label_15->setVisible(visible); ui->txtIdSucursal->setVisible(visible);
    ui->label_16->setVisible(visible); ui->txtNombreSucursal->setVisible(visible);
    ui->label_20->setVisible(visible); ui->txtUbicacionSucursal->setVisible(visible);
    ui->label_17->setVisible(visible); ui->spinIngreso->setVisible(visible);
    ui->label_18->setVisible(visible); ui->spinDespacho->setVisible(visible);
    ui->label_19->setVisible(visible); ui->spinTraspaso->setVisible(visible);

    ui->btnConfirmarSucursal->setVisible(visible);
    if (esEdicion)
    {
        ui->btnGestionarRutas->setVisible(visible);
    }
}

void MainWindow::limpiarFormularioSucursal() {
    ui->txtIdSucursal->clear();
    ui->txtIdSucursal->setEnabled(true);
    ui->txtNombreSucursal->clear();
    ui->txtUbicacionSucursal->clear();
    ui->spinIngreso->setValue(0);
    ui->spinDespacho->setValue(0);
    ui->spinTraspaso->setValue(0);
}

void MainWindow::actualizarPosicionGrafica(int idEnvio, int idSiguienteNodo) {
    //Buscar el envío en la lista de activos
    for(auto envio : enviosActivos) {
        if(envio->idEnvio == idEnvio) {
            // Mover el círculo a la nueva posición del nodo
            if(posicionesNodos.contains(idSiguienteNodo)) {
                QPointF nuevaPos = posicionesNodos[idSiguienteNodo];
                envio->puntoMovil->setPos(nuevaPos);
            }
            break;
        }
    }
}

void MainWindow::actualizarColaVisual(int idEnvio, int tipoCola, bool insertando) {
    ControlEnvio* env = nullptr;
    for(auto e : enviosActivos) {
        if(e->idEnvio == idEnvio) {
            env = e;
            break;
        }
    }
    if (!env) return;

    //Filtrado por Sucursal
    QString filtro = ui->comboSucursalesProcesos->currentText();
    Sucursal* sActual = red.obtenerSucursal(env->idSedeActual);
    QString nombreSedeActual = sActual ? QString::fromStdString(sActual->nombre) : "";

    bool debeMostrarse = (filtro == "Ver Todas" || filtro == nombreSedeActual);

    //Seleccionar la lista según el tipo
    QListWidget* lista = nullptr;
    if (tipoCola == 1) lista = ui->listWidget_colaTraspaso;
    else if (tipoCola == 2) lista = ui->listWidget_colaDespacho;
    else if (tipoCola == 3) lista = ui->listWidget_colaIngreso;

    if (!lista) return;

    //Ejecutar Acción
    QString identificador = "Envío #" + QString::number(idEnvio);

    if (insertando && debeMostrarse) {
        // Formatear texto: "Envío #1 [Origen -> Destino]"
        QString info = identificador + " [" + env->nombreOrigen + " -> " + env->nombreDestino + "]";
        QListWidgetItem* item = new QListWidgetItem(info);

        // Aplicar el color del punto del grafo al texto
        item->setForeground(env->colorEnvio);
        item->setFont(QFont("Segoe UI", 9, QFont::Bold));

        lista->addItem(item);
    } else {
        // Remover el item (buscamos por el inicio del texto "Envío #X")
        for (int i = 0; i < lista->count(); ++i) {
            if (lista->item(i)->text().startsWith(identificador)) {
                delete lista->takeItem(i);
                break;
            }
        }
    }
}

void MainWindow::on_comboSucursalesProcesos_currentIndexChanged(int index) {
    repoblarListasProcesos();
}

void MainWindow::repoblarListasProcesos() {
    ui->listWidget_colaTraspaso->clear();
    ui->listWidget_colaDespacho->clear();
    ui->listWidget_colaIngreso->clear();

    for (ControlEnvio* env : enviosActivos) {
        if (env->faseActual > 0) {
            actualizarColaVisual(env->idEnvio, env->faseActual, true);
        }
    }
}

void MainWindow::limpiarRutaVisual() {
    for (auto linea : lineasRutaCalculada) {
        escena->removeItem(linea);
        delete linea;
    }
    lineasRutaCalculada.clear();
}