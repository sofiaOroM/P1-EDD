#ifndef GESTOR_DE_CATALOGO_PRODUCTO_H
#define GESTOR_DE_CATALOGO_PRODUCTO_H
#include <string>
using namespace std;

struct Producto
{
    string name;
    string barcode;
    string category;
    string expiry_date;
    string brand;
    double price;
    int stock;

    Producto();
    Producto(string name, string barcode, string category,
             string expiry_date, string brand, double price, int stock);

    void mostrarProducto() const;
    int getStock() const { return stock; }
};


#endif //GESTOR_DE_CATALOGO_PRODUCTO_H