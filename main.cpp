#include <iostream>
#include <chrono>
#include "./ListasEnlazada/NoOrdenada/ListaNoOrdenada.h"
#include "./ListasEnlazada/Ordenada/ListaOrdenada.h"
#include "./TablaHash/TablaHash.h"
#include "./ArbolAVL/ArbolAVL.h"
#include "CargadorCSV.h"
#include "Producto.h"
#include "ArbolB/ArbolB.h"
#include "./ArbolBM/ArbolBM.h"

using namespace std;
using namespace std::chrono;

int main() {
    // --- 1. INSTANCIACIÓN DE ESTRUCTURAS DE DATOS ---
    // Usamos diferentes estructuras para comparar eficiencia según el requerimiento
    ListaNoOrdenada listaN;
    ListaOrdenada listaO;
    TablaHash miHash(1301); // Tamaño primo para reducir colisiones
    ArbolAVL arbolAvl;
    ArbolB arbolB;          // Orden 5 para búsqueda por fechas
    ArbolBM arbolBM;        // B+ para agrupación por categorías

    // --- 2. CARGA MASIVA Y VALIDACIÓN ---
    /*cout << "   SISTEMA DE GESTION DE CATALOGO   " << endl;
    cout << "Cargando Productos.csv..." << endl;

    // El cargador implementa normalización de fechas y rollback atómico
    CargadorCSV::cargar("Productos.csv", listaN, listaO, miHash, arbolAvl, arbolB, arbolBM);
*/
    int opcion = 0;

    // --- 3. MENU DE INTERACCIÓN PRINCIPAL ---
    while (opcion != 10) {
        cout << "\n           MENU PRINCIPAL          " << endl;
        cout << "------------------------------------" << endl;
        cout << "0. Cargar otro archivo CSV" << endl;
        cout << "1. Buscar Producto (Comparar Tiempos)" << endl;
        cout << "2. Eliminar Producto (Sincronizado)" << endl;
        cout << "3. Ver Reporte de Errores (Log)" << endl;
        cout << "4. Generar Reporte DOT (AVL)" << endl;
        cout << "5. Generar Reporte DOT (Arbol B)" << endl;
        cout << "6. Generar Reporte DOT (Arbol B+)" << endl;
        cout << "7. Listado Alfabetico (In-Order)" << endl;
        cout << "8. Consultar por Categoria" << endl;
        cout << "9. Consultar por Rango de Fechas" << endl;
        cout << "10. Salir del Sistema" << endl;
        cout << "------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) { // Validación por si ingresan letras
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();


        switch (opcion) {
            case 0: {
                    string nombreArchivo;
                    cout << "Ingrese el nombre o ruta del archivo (.csv): ";
                    getline(cin, nombreArchivo);
                    string rutaCompleta = "Archivos de Prueba/" + nombreArchivo;

                    // cargador con la ruta que el usuario escribió
                    CargadorCSV::cargar(rutaCompleta, listaN, listaO, miHash, arbolAvl, arbolB, arbolBM);
                    break;
            }
            case 1: {
                // COMPARACIÓN DE RENDIMIENTO: Lista vs AVL
                string nombreBusqueda;
                cout << "Ingrese el NOMBRE del producto: ";
                getline(cin, nombreBusqueda);

                auto startL = high_resolution_clock::now();
                Producto* pLista = listaN.buscarPorNombre(nombreBusqueda);
                auto endL = high_resolution_clock::now();

                auto startLO = high_resolution_clock::now();
                Producto* oLista = listaO.buscarPorNombre(nombreBusqueda);
                auto endLO = high_resolution_clock::now();

                auto startA = high_resolution_clock::now();
                Producto* pAvl = arbolAvl.buscarPorNombre(nombreBusqueda);
                auto endA = high_resolution_clock::now();

                if (pAvl != nullptr) {
                    cout << "\nProducto encontrado: " << pAvl->name << endl;
                    cout << "Precio: Q" << pAvl->price << " | Stock: " << pAvl->stock << endl;
                    cout << "------------------------------------" << endl;
                    cout << "Tiempo Lista No Ordenada (O(n)): " << duration_cast<nanoseconds>(endL - startL).count() << " ns" << endl;
                    cout << "Tiempo Lista Ordenada (O(n)): " << duration_cast<nanoseconds>(endLO - startLO).count() << " ns" << endl;
                    cout << "Tiempo AVL (O(log n)): " << duration_cast<nanoseconds>(endA - startA).count() << " ns" << endl;
                } else {
                    cout << "Tiempo Lista No Ordenada (O(n)): " << duration_cast<nanoseconds>(endL - startL).count() << " ns" << endl;
                    cout << "Tiempo Lista Ordenada (O(n)): " << duration_cast<nanoseconds>(endLO - startLO).count() << " ns" << endl;
                    cout << "Tiempo AVL (O(log n)): " << duration_cast<nanoseconds>(endA - startA).count() << " ns" << endl;
                    cout << "[!] El producto no existe en el catalogo." << endl;
                }
                break;
            }

            case 2: {
                // ELIMINACIÓN COORDINADA: Primero buscamos en Hash (O(1))
                string barcodeEliminar;
                cout << "Ingrese el BARCODE para eliminar: ";
                getline(cin, barcodeEliminar);

                auto startH = high_resolution_clock::now();
                Producto* p = miHash.buscar(barcodeEliminar);
                auto endH = high_resolution_clock::now();

                if (p != nullptr) {
                    cout << "Eliminando: " << p->name << " de todas las estructuras..." << endl;

                    // Capturamos datos antes de borrar de la Hash
                    string nombre = p->name;
                    string cat = p->category;

                    // Medimos eliminación en cada estructura para el reporte
                    auto s1 = high_resolution_clock::now();
                    arbolAvl.eliminarPorNombre(nombre);
                    auto e1 = high_resolution_clock::now();

                    auto s2 = high_resolution_clock::now();
                    arbolB.eliminar(p);
                    auto e2 = high_resolution_clock::now();

                    auto s3 = high_resolution_clock::now();
                    arbolBM.eliminar(cat, p);
                    auto e3 = high_resolution_clock::now();

                    auto s4 = high_resolution_clock::now();
                    miHash.eliminar(barcodeEliminar);
                    auto e4 = high_resolution_clock::now();

                    auto s5 = high_resolution_clock::now();
                    listaN.eliminar(barcodeEliminar);
                    auto e5 = high_resolution_clock::now();

                    auto s6 = high_resolution_clock::now();
                    listaO.eliminar(barcodeEliminar);
                    auto e6 = high_resolution_clock::now();

                    cout << "\n>>> TIEMPOS DE ELIMINACION <<<" << endl;
                    cout << "AVL: " << duration_cast<nanoseconds>(e1 - s1).count() << " ns" << endl;
                    cout << "B:   " << duration_cast<nanoseconds>(e2 - s2).count() << " ns" << endl;
                    cout << "B+:  " << duration_cast<nanoseconds>(e3 - s3).count() << " ns" << endl;
                    cout << "HASH: " << duration_cast<nanoseconds>(e4 - s4).count() << " ns" << endl;
                    cout << "LN: " << duration_cast<nanoseconds>(e5 - s5).count() << " ns" << endl;
                    cout << "LO: " << duration_cast<nanoseconds>(e6 - s6).count() << " ns" << endl;

                    delete p; // Único punto de liberación de memoria del objeto
                    cout << " Producto eliminado correctamente." << endl;
                } else {
                    cout << "[ERROR] Barcode no encontrado." << endl;
                }
                break;
            }

            case 3:
                cout << "Abriendo log de errores..." << endl;
                system("notepad errors.log");
                break;

            case 4:
                arbolAvl.generarReporteGrafico("reporte_avl.dot");
                cout << "[DOT] Archivo 'reporte_avl.dot' generado." << endl;
                break;

            case 5:
                arbolB.generarDotB("reporte_b.dot");
                cout << "[DOT] Archivo 'reporte_b.dot' generado." << endl;
                break;

            case 6:
                arbolBM.generarDotBM("reporte_b+.dot");
                cout << "[DOT] Archivo 'reporte_b+.dot' generado." << endl;
                break;

            case 7:
                // Uso del recorrido In-Order para el reporte alfabético
                arbolAvl.mostrarCatalogoAlfabetico();
                break;

            case 8: {
                string catFiltro;
                cout << "Ingrese categoria a filtrar: ";
                getline(cin, catFiltro);
                arbolBM.mostrarProductosPorCategoria(catFiltro);
                break;
            }
            case 9: {
                        string fechaInicio, fechaFin;
                        cout << "\n--- BUSQUEDA POR RANGO DE FECHAS ---" << endl;
                        cout << "Formato esperado: YYYY-MM-DD" << endl;
                        cout << "Ingrese fecha de inicio: ";
                        getline(cin, fechaInicio);
                        cout << "Ingrese fecha de fin: ";
                        getline(cin, fechaFin);

                        // Medimos el tiempo de respuesta del Arbol B
                        auto startB = high_resolution_clock::now();
                        vector<Producto*> resultados = arbolB.buscarPorRangoFechas(fechaInicio, fechaFin);
                        auto endB = high_resolution_clock::now();

                        if (!resultados.empty()) {
                            cout << "\nSe encontraron " << resultados.size() << " productos en ese rango:" << endl;
                            cout << "--------------------------------------------------------" << endl;
                            for (Producto* p : resultados) {
                                cout << "[" << p->expiry_date << "] - " << p->name
                                     << " (Barcode: " << p->barcode << ")" << endl;
                            }
                            cout << "--------------------------------------------------------" << endl;
                        } else {
                            cout << "[!] No se encontraron productos entre " << fechaInicio << " y " << fechaFin << endl;
                        }

                        cout << "Tiempo de busqueda en Arbol B: "
                             << duration_cast<nanoseconds>(endB - startB).count() << " ns" << endl;
                        break;
            }

            case 10:
                cout << "Liberando recursos y saliendo..." << endl;
                listaO.vaciar();
                listaN.vaciar();
                arbolAvl.vaciar();
                //arbolB.vaciar();
                //arbolBM.vaciar();
                break;
            default:
                cout << "Opcion invalida, intente de nuevo." << endl;
        }
    }
    return 0;
}