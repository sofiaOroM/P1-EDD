#include "Producto.h"
#include <iostream>
#include <iomanip>

Producto::Producto()
{
}

Producto::Producto(string name, string barcode, string category,
                   string expiry_date, string brand, double price, int stock) {
    this->name = name;
    this->barcode = barcode;
    this->category = category;
    this->expiry_date = expiry_date;
    this->brand = brand;
    this->price = price;
    this->stock = stock;
}

void Producto::mostrarProducto() const {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Nombre:   " << name << std::endl;
    std::cout << "Código:   " << barcode << std::endl;
    std::cout << "Categoría: " << category << std::endl;
    std::cout << "Vence:     " << expiry_date << std::endl;
    std::cout << "Marca:     " << brand << std::endl;
    std::cout << "Precio:    Q" << std::fixed << std::setprecision(2) << price << std::endl;
    std::cout << "Stock:     " << stock << std::endl;
}