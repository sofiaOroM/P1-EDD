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
    Producto(std::string name, std::string barcode, std::string category,
             std::string expiry_date, std::string brand, double price, int stock);

    void mostrarProducto() const;
};


#endif //GESTOR_DE_CATALOGO_PRODUCTO_H