#include "CargadorCSV.h"
#include "Producto.h"
#include <QTextEdit>

bool esLineaVaciaOBasura(const string& linea) {
    if (linea.empty()) return true;
    return linea.find_first_not_of(", \t\r\n;") == string::npos;
}

void CargadorCSV::cargarProductos(string ruta, ListaSucursales& red, QTextEdit* logVisual) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        registrarError("No se pudo abrir el archivo de productos: " + ruta, logVisual);
        return;
    }

    string linea;
    int numeroLinea = 0;
    bool datosIniciados = false;

    while (getline(archivo, linea))
    {
        numeroLinea++;

        // 1. Manejo de líneas vacías
        if (esLineaVaciaOBasura(linea)) {
            registrarError("Línea " + to_string(numeroLinea) + ": [Aviso] Línea vacía omitida.", logVisual);
            continue;
        }

        string temp = linea;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (!datosIniciados && temp.find("codigobarra") != string::npos) {
            datosIniciados = true;
            continue;
        }

        vector<string> t = dividirLineaCSV(linea);

        if (t.size() < 8) {
            registrarError("Línea " + to_string(numeroLinea) + ": Columnas insuficientes.", logVisual);
            continue;
        }

        try {
            int idSucursal = stoi(t[0]);
            Sucursal* s = red.buscarPorId(idSucursal);

            if (s == nullptr) {
                registrarError("Línea " + to_string(numeroLinea) + ": Sucursal " + t[0] + " no existe.", logVisual);
                continue;
            }

            string barcode = t[2];

            if (barcode.length() != 10) {
                registrarError("Línea " + to_string(numeroLinea) + ": Código de barras inválido (" + barcode + "). Debe tener 10 caracteres.", logVisual);
                continue;
            }

            if (s->hash.buscar(barcode) != nullptr) {
                registrarError("Línea " + to_string(numeroLinea) + ": Código duplicado (" + barcode + ")", logVisual);
                continue;
            }

            string fechaNorm = normalizarFecha(t[4]);
            if (!esFechaValida(fechaNorm)) {
                registrarError("Línea " + to_string(numeroLinea) + ": Fecha inválida.", logVisual);
                continue;
            }

            Producto* nuevo = new Producto(t[1], barcode, t[3], fechaNorm, t[5], stod(t[6]), stoi(t[7]));

            if (s->hash.insertar(nuevo)) {
                bool ok = s->arbolAvl.insertarPorNombre(nuevo) &&
                          s->arbolB.insertar(nuevo) &&
                          s->arbolBM.insertar(nuevo);

                if (!ok) {
                    s->hash.eliminar(barcode);
                    s->arbolAvl.eliminarPorNombre(nuevo->name);
                    s->arbolB.eliminar(nuevo);
                    s->arbolBM.eliminar(nuevo->category,nuevo);
                    delete nuevo;
                    registrarError("Línea " + to_string(numeroLinea) + ": Error de balanceo en estructuras.", logVisual);
                }
            }
        } catch (const exception& e) {
            registrarError("Línea " + to_string(numeroLinea) + ": Error de formato (Dato no numérico).", logVisual);
        }
    }
    archivo.close();
}

void CargadorCSV::cargarSucursales(string ruta, ListaSucursales& red, QTextEdit* logVisual) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        registrarError("No se pudo abrir sucursales: " + ruta, logVisual);
        return;
    }

    string linea;
    int numeroLinea = 0;
    bool datosIniciados = false;

    while (getline(archivo, linea)) {
        numeroLinea++;

        if (esLineaVaciaOBasura(linea)) {
            registrarError("Línea " + to_string(numeroLinea) + ": [Aviso] Línea vacía omitida.", logVisual);
            continue;
        }

        string temp = linea;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (!datosIniciados && temp.find("t_ingreso") != string::npos) {
            datosIniciados = true;
            continue;
        }

        vector<string> t = dividirLineaCSV(linea);
        if (t.size() < 6) {
            registrarError("Línea " + to_string(numeroLinea) + ": Columnas insuficientes en sucursal.", logVisual);
            continue;
        }

        try {
            int id = stoi(t[0]);
            if (red.buscarPorId(id) == nullptr) {
                // t[0]=id, t[1]=nombre, t[2]=ubicacion, t[3]=tI, t[4]=tT, t[5]=tD
                Sucursal* nueva = new Sucursal(id, t[1], t[2], stoi(t[3]), stoi(t[4]), stoi(t[5]));
                red.insertarFinal(nueva);
            } else {
                registrarError("Línea " + to_string(numeroLinea) + ": ID de sucursal duplicado (" + t[0] + ")", logVisual);
            }
        } catch (...) {
            registrarError("Línea " + to_string(numeroLinea) + ": Datos numéricos inválidos en sucursal.", logVisual);
        }
    }
    archivo.close();
}

void CargadorCSV::cargarConexiones(string ruta, Grafo& grafo, ListaSucursales& red, QTextEdit* logVisual) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        registrarError("No se pudo abrir conexiones: " + ruta, logVisual);
        return;
    }

    string linea;
    int numeroLinea = 0;
    bool datosIniciados = false;

    while (getline(archivo, linea)) {
        numeroLinea++;

        if (esLineaVaciaOBasura(linea)) {
            registrarError("Línea " + to_string(numeroLinea) + ": [Aviso] Línea vacía omitida.", logVisual);
            continue;
        }

        string temp = linea;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (!datosIniciados && temp.find("origenid") != string::npos) {
            datosIniciados = true;
            continue;
        }

        vector<string> t = dividirLineaCSV(linea);
        if (t.size() < 4) {
            registrarError("Línea " + to_string(numeroLinea) + ": Columnas insuficientes en conexión.", logVisual);
            continue;
        }

        try {
            int o = stoi(t[0]);
            int d = stoi(t[1]);
            int tiempo = stoi(t[2]);
            double costo = stod(t[3]);

            if (red.buscarPorId(o) != nullptr && red.buscarPorId(d) != nullptr) {
                grafo.agregarCamino(o, d, tiempo, costo);
            } else {
                registrarError("Línea " + to_string(numeroLinea) + ": Origen (" + t[0] + ") o Destino (" + t[1] + ") no existen.", logVisual);
            }
        } catch (...) {
            registrarError("Línea " + to_string(numeroLinea) + ": Formato numérico inválido en conexión.", logVisual);
        }
    }
    archivo.close();
}

string CargadorCSV::normalizarFecha(string fechaOriginal)
{
    // Si la fecha ya viene YYYY-MM-DD (tiene '-' en la pos 4), se queda igual
    if (fechaOriginal.length() >= 5 && fechaOriginal[4] == '-')
    {
        return fechaOriginal;
    }

    // Si viene como DD/MM/YYYY
    // Ejemplo: 15/05/2026 -> 2026-05-15
    if (fechaOriginal.find('/') != string::npos)
    {
        string dia = fechaOriginal.substr(0, 2);
        string mes = fechaOriginal.substr(3, 2);
        string anio = fechaOriginal.substr(6, 4);
        return anio + "-" + mes + "-" + dia;
    }

    return fechaOriginal;
}

bool CargadorCSV::esFechaValida(string& fecha)
{
    // Formato esperado: YYYY-MM-DD (10 caracteres)
    if (fecha.length() != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7) continue;
        if (!isdigit(fecha[i])) return false;
    }
    return true;
}

void CargadorCSV::registrarError(string mensaje, QTextEdit* logVisual) {
    ofstream logErrores("errors.log", ios::app);
    if (logErrores.is_open()) {
        logErrores << mensaje << endl;
        logErrores.close();
    }

    if (logVisual) {
        logVisual->append(QString::fromStdString(mensaje));
    }
}

int CargadorCSV::identificarTipoArchivo(string ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return -1;

    string linea;
    int intentos = 0;
    while (getline(archivo, linea) && intentos < 5) {
        transform(linea.begin(), linea.end(), linea.begin(), ::tolower);

        if (linea.find("t_ingreso") != string::npos) return 1;
        if (linea.find("origenid") != string::npos) return 2;
        if (linea.find("codigobarra") != string::npos) return 3;

        intentos++;
    }
    return 0;
}

vector<string> CargadorCSV::dividirLineaCSV(string linea) {
    linea.erase(0, linea.find_first_not_of(" \t\r\n\""));
    size_t ultimo = linea.find_last_not_of(" \t\r\n;\"");
    if (ultimo != string::npos) {
        linea = linea.substr(0, ultimo + 1);
    }

    vector<string> tokens;
    string token;
    bool dentroComillas = false;

    for (size_t i = 0; i < linea.length(); i++) {
        char c = linea[i];
        if (c == '"') {
            if (i + 1 < linea.length() && linea[i+1] == '"') {
                token += '"';
                i++;
            } else {
                dentroComillas = !dentroComillas;
            }
        } else if (c == ',' && !dentroComillas) {
            tokens.push_back(limpiarToken(token));
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(limpiarToken(token));
    return tokens;
}

string CargadorCSV::limpiarToken(string token) {
    if (token.empty()) return "";

    token.erase(0, token.find_first_not_of(" \t\r\n"));
    size_t last = token.find_last_not_of(" \t\r\n;");
    if (last != string::npos) token = token.substr(0, last + 1);

    if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
        token = token.substr(1, token.size() - 2);
    }

    // Limpiar hasta que ya no haya comillas en los extremos
    while (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
        token = token.substr(1, token.size() - 2);
    }

    return token;
}