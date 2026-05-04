#include "ventanadots.h"
#include "ui_VentanaDots.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

VentanaDots::VentanaDots(Sucursal* sucursal, QWidget *parent) :
    QDialog(parent), ui(new Ui::VentanaDots), sucursalActual(sucursal) {
    ui->setupUi(this);
    ui->lblImagen->setScaledContents(true);
    ui->scrollArea->setWidgetResizable(true);

    ui->comboEstructura->addItems({"Árbol AVL",  "Árbol B", "Árbol B+", "Tabla Hash"});
    connect(ui->comboEstructura, &QComboBox::currentIndexChanged, this, &VentanaDots::actualizarGrafico);

    actualizarGrafico();
}

void VentanaDots::actualizarGrafico() {
    QString dotPath = "temp.dot";
    QString imgPath = "temp.png";
    int index = ui->comboEstructura->currentIndex();

    switch(index) {
    case 0: sucursalActual->arbolAvl.generarReporteGrafico(dotPath.toStdString()); break;
    case 1: sucursalActual->arbolB.generarDotB(dotPath.toStdString()); break;
    case 2: sucursalActual->arbolBM.generarDotBM(dotPath.toStdString()); break;
    case 3: sucursalActual->hash.generarDot(dotPath.toStdString()); break;
    }

    generarImagen(dotPath, imgPath);
}

void VentanaDots::generarImagen(QString dotPath, QString imgPath)
{
    QProcess process;

    QString program = "C:/Program Files/Graphviz/bin/dot.exe";

    QStringList arguments;
    arguments << "-Tpng" << dotPath << "-o" << imgPath;

    process.start(program, arguments);

    if (!process.waitForFinished()) {
        qDebug() << "Fallo al ejecutar Graphviz:" << process.errorString();
        return;
    }

    QByteArray salida = process.readAll();
    if (!salida.isEmpty()) {
        qDebug() << "Salida de Graphviz:" << salida;
    }

    QPixmap pix(imgPath);
    if(pix.isNull()) {
        qDebug() << "Error: La imagen no se pudo cargar. Ruta intentada:" << QDir::current().absoluteFilePath(imgPath);
    } else {
        ui->lblImagen->setPixmap(pix);
        ui->lblImagen->setScaledContents(false);
        ui->lblImagen->resize(pix.size());
        ui->scrollAreaWidgetContents->resize(pix.size());
    }
}

void VentanaDots::on_btnExportarArbol_clicked() {
    QString rutaTemporal = "temp.png";

    if (!QFile::exists(rutaTemporal)) {
        QMessageBox::warning(this, "Exportar", "No hay un gráfico generado para exportar.");
        return;
    }

    QString nombreArchivo = QFileDialog::getSaveFileName(this,
        "Exportar Árbol o Tabla como Imagen",
        QDir::homePath(),
        "Imágenes (*.png);;Todos los archivos (*.*)");

    if (nombreArchivo.isEmpty()) return;

    if (!nombreArchivo.endsWith(".png", Qt::CaseInsensitive)) {
        nombreArchivo += ".png";
    }

    if (QFile::exists(nombreArchivo)) QFile::remove(nombreArchivo);

    if (QFile::copy(rutaTemporal, nombreArchivo)) {
        QMessageBox::information(this, "Éxito", "Árbol exportado correctamente.");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo exportar el archivo.");
    }
}

VentanaDots::~VentanaDots()
{
    delete ui;
}

