#include "CargadorCSV.h"
#include "Producto.h"
#include <iostream>
using namespace std;

void CargadorCSV::cargar(string ruta, ListaNoOrdenada& listaN, ListaOrdenada& listaO, TablaHash& hash, ArbolAVL& arbolAvl, ArbolB& arbolB, ArbolBM& arbolBM) {
    ifstream archivo(ruta);
    ofstream logErrores("errors.log", std::ios::app);

    if (!archivo.is_open()) {
        cout << "[!] Error: No se pudo abrir el archivo: " << ruta << endl;
        return;
    }
    //Por si viene vacio
    archivo.seekg(0, ios::end); // Ir al final
    if (archivo.tellg() == 0) { // Si la posición final es 0
        cout << "[!] Advertencia: El archivo '" << ruta << "' esta totalmente vacio." << endl;
        archivo.close();
        return;
    }
    archivo.seekg(0, ios::beg);

    string linea;
    // Por si solo tiene encabezado
    if (!getline(archivo, linea)) {
        cout << "[!] Error: El archivo esta vacio o corrupto." << endl;
        archivo.close();
        return;
    }

    cout << "[...] Procesando archivo: " << ruta << endl;

    int numeroLinea = 1;
    int cargados = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        stringstream ss(linea);
        string nombre, barcode, categoria, fecha, marca, precioStr, stockStr;

        if (getline(ss, nombre, ',') && getline(ss, barcode, ',') &&
            getline(ss, categoria, ',') && getline(ss, fecha, ',') &&
            getline(ss, marca, ',') && getline(ss, precioStr, ',') &&
            getline(ss, stockStr, ','))
        {
            try {
                string fechaNorm = normalizarFecha(fecha);
                if (!esFechaValida(fechaNorm)) throw runtime_error("Fecha invalida");

                Producto* nuevo = new Producto(nombre, barcode, categoria, fechaNorm, marca, stod(precioStr), stoi(stockStr));

                // 1. Intentar insertar en Hash (valida duplicados)
                if (hash.insertar(nuevo)) {
                    // 2. Intentar insertar en las demás (Atomicidad)
                    bool listo = true;
                    listo &= listaN.insertar(nuevo);
                    listo &= listaO.insertar(nuevo);
                    listo &= arbolAvl.insertarPorNombre(nuevo);
                    listo &= arbolB.insertar(nuevo);
                    listo &= arbolBM.insertar(nuevo);

                    if (listo) {
                        cargados++;
                    } else {
                        // --- ROLLBACK ---
                        hash.eliminar(barcode);
                        listaN.eliminar(barcode);
                        listaO.eliminar(barcode);
                        arbolAvl.eliminarPorNombre(nombre);
                        arbolB.eliminar(nuevo);
                        arbolBM.eliminar(categoria, nuevo);
                        delete nuevo;
                        logErrores << "Error estructural en linea " << numeroLinea << endl;
                    }
                } else {
                    logErrores << "Barcode duplicado: " << barcode << " (Linea " << numeroLinea << ")" << endl;
                    delete nuevo;
                }
            } catch (...) {
                logErrores << "Error de formato o datos en linea " << numeroLinea << ": " << linea << endl;
            }
        }
    }
    if (cargados == 0) {
        cout << "[!] Aviso: El archivo se leyo pero no se cargo ningun producto valido (revise errors.log)." << endl;
    } else {
        cout << " Carga exitosa. Productos nuevos: " << cargados <<"/" << numeroLinea-1 << endl;
    }
    archivo.close();
    logErrores.close();
}

string CargadorCSV::normalizarFecha(string fechaOriginal) {
    // Si la fecha ya viene YYYY-MM-DD (tiene '-' en la pos 4), la dejamos igual
    if (fechaOriginal.length() >= 5 && fechaOriginal[4] == '-') {
        return fechaOriginal;
    }

    // Si viene como DD/MM/YYYY
    // Ejemplo: 15/05/2026 -> 2026-05-15
    if (fechaOriginal.find('/') != string::npos) {
        string dia = fechaOriginal.substr(0, 2);
        string mes = fechaOriginal.substr(3, 2);
        string anio = fechaOriginal.substr(6, 4);
        return anio + "-" + mes + "-" + dia;
    }

    return fechaOriginal; // Retorno por defecto
}

bool CargadorCSV::esFechaValida(string& fecha) {
    // Formato esperado: YYYY-MM-DD (10 caracteres)
    if (fecha.length() != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(fecha[i])) return false;
    }
    return true;
}