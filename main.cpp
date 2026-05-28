#include <iostream>

#include <estructuras.h>

// =================================================================
// DEFINICIÓN DE VARIABLES GLOBALES
// Aquí se reserva la memoria real para las variables que 
// marcamos como 'extern' en estructuras.h
// =================================================================
struct PartidaContinental miPartida;
struct PartidaChinchon miPartidaChinchon; 
struct PartidaTute miPartidaTute; 
int paginaActual = 0; 
bool mostrandoErrorContinental = false;
bool mostrandoErrorChinchon = false;
bool tuteEditandoManos = false;
bool tuteEditandoPuntos = false;
bool tuteVolverMenu = false;
int tuteNumEquipoEditando = 1;

// =================================================================
// FUNCIÓN PRINCIPAL
// =================================================================
int main() {
    iniciarModoGrafico();
    
    int opcionMenu = 0;
    do {
        dibujarMenu();
        opcionMenu = controlarMenu();
        if (opcionMenu != 6) {
            ejecutarJuego(opcionMenu);
        }
    } while (opcionMenu != 6);
    
    closegraph();
    return 0;
}

// =================================================================
// INICIALIZACIÓN GRÁFICA
// =================================================================
void iniciarModoGrafico() {
    initwindow(1280, 720, (char*)"Baraja Espanola", 0, 0, false, true);
    setactivepage(0);
    setvisualpage(0);
}
