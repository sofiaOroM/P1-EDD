#include <QApplication>
#include "Vista/mainwindow.h"
#include "Sucursales/ListaSucursales.h"
#include "Grafo/Grafo.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ListaSucursales red;
    Grafo grafo;
    MainWindow w;
    w.show();
    return a.exec();
}