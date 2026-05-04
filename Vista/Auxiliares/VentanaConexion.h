#ifndef GESTOR_DE_CATALOGO_VENTANACONEXION_H
#define GESTOR_DE_CATALOGO_VENTANACONEXION_H


#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QPushButton>

class VentanaConexion : public QDialog {
public:
    QComboBox *cbOrigen, *cbDestino;
    QSpinBox *sbTiempo;
    QDoubleSpinBox *sbCosto;

    VentanaConexion(QWidget* parent, const QMap<int, QString>& sucursales) : QDialog(parent) {
        setWindowTitle("Gestionar Conexión");
        QFormLayout* layout = new QFormLayout(this);

        cbOrigen = new QComboBox();
        cbDestino = new QComboBox();
        for(int id : sucursales.keys()) {
            cbOrigen->addItem(sucursales[id], id);
            cbDestino->addItem(sucursales[id], id);
        }

        sbTiempo = new QSpinBox(); sbTiempo->setRange(1, 1000);
        sbCosto = new QDoubleSpinBox(); sbCosto->setRange(0.1, 5000.0);

        layout->addRow("Origen:", cbOrigen);
        layout->addRow("Destino:", cbDestino);
        layout->addRow("Tiempo (min):", sbTiempo);
        layout->addRow("Costo (Q):", sbCosto);

        QPushButton* btn = new QPushButton("Guardar Conexión");
        connect(btn, &QPushButton::clicked, this, &QDialog::accept);
        layout->addRow(btn);
    }
};

#endif //GESTOR_DE_CATALOGO_VENTANACONEXION_H