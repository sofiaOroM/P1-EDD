#include "Traspaso.h"
#include "Sucursales/ListaSucursales.h"

Traspaso::Traspaso(ControlEnvio* envio, ListaSucursales* red)
    : envio(envio), red(red) {}

void Traspaso::procesar() {
    // 1 minuto de tiempo simulado equivale a 50ms reales para una animación fluida
    const int FACTOR_VELOCIDAD = 50;

    // Recorrer la ruta paso a paso
    for (size_t i = 0; i < envio->ruta.size() - 1; ++i) {
        int idProximaSede = envio->ruta[i + 1];

        // Actualizar el estado interno del envío antes de procesar
        envio->idSedeActual = idProximaSede;
        envio->indiceActual = i + 1;

        Sucursal* suc = red->obtenerSucursal(idProximaSede);

        if (suc) {
            // Mover el punto visualmente al llegar a la sucursal actual de la ruta
            emit cambioPosicion(envio->idEnvio, idProximaSede);

            // --- FASE 1: TRASPASO (Llegada y descarga) ---
            // 1. Registro en la cola real de la sucursal
            envio->faseActual = 1;
            suc->colaTraspaso.encolar(nullptr); // Se puede pasar el puntero al envío si tu cola lo soporta

            // 2. Notificar a la UI para que aparezca en el ListWidget
            emit actualizarColaVisual(envio->idEnvio, 1, true);

            // 3. Simulación del tiempo de espera
            QThread::msleep(suc->t_traspaso * FACTOR_VELOCIDAD);

            // 4. Salida de la cola real y de la UI
            suc->colaTraspaso.desencolar();
            emit actualizarColaVisual(envio->idEnvio, 1, false);

            // --- FASE 2: DESPACHO (Procesamiento de carga) ---
            // 1. Registro en cola de despacho
            envio->faseActual = 2;
            suc->colaSalida.encolar(nullptr);
            emit actualizarColaVisual(envio->idEnvio, 2, true);

            // 2. El tiempo de despacho depende de la cantidad de productos del envío
            int tiempoDespachoTotal = suc->t_despacho * envio->cantidadTotal;
            QThread::msleep(tiempoDespachoTotal * FACTOR_VELOCIDAD);

            suc->colaSalida.desencolar();
            emit actualizarColaVisual(envio->idEnvio, 2, false);

            // --- FASE 3: INGRESO (Solo si es la última parada) ---
            if (idProximaSede == envio->ruta.back()) {
                envio->faseActual = 3;
                suc->colaIngreso.encolar(nullptr);
                emit actualizarColaVisual(envio->idEnvio, 3, true);

                QThread::msleep(suc->t_ingreso * FACTOR_VELOCIDAD);

                suc->colaIngreso.desencolar();
                emit actualizarColaVisual(envio->idEnvio, 3, false);
            }
            envio->faseActual = 0;
        }
    }

    // Al salir del bucle, el envío ha terminado su recorrido
    emit finalizarEnvio(envio);
}