#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>

// ==========================================
// PROTOTIPOS DE LAS FUNCIONES DEL MENÚ
// ==========================================
void iniciarModoGrafico();
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcion);

// ==========================================
// PROTOTIPOS DE LOS CUATRO JUEGOS
// ==========================================
void jugarContinental();
void jugarChinchon();
void jugarTute();
void jugarEscoba();
void prepararPantallaJuego(char* nombreJuego); 

// ==========================================
// FUNCIÓN PRINCIPAL (MAIN)
// ==========================================
int main() {
    int opcion = 0;
    
    iniciarModoGrafico();
    
    do {
        dibujarMenu();
        opcion = controlarMenu();
        if (opcion != 5) { // 5 es la opción para salir
            ejecutarJuego(opcion);
        }
    } while (opcion != 5);
    
    closegraph(); 
    return 0;
}

// ==========================================
// IMPLEMENTACIÓN DE LAS FUNCIONES DEL MENÚ
// ==========================================

void iniciarModoGrafico() {
    int anchoMonitor = getmaxwidth();
    int altoMonitor = getmaxheight();
    
    initwindow(anchoMonitor, altoMonitor, (char*)"Mi TFG - Baraja Espanola", 0, 0, false, true);
    
    if (graphresult() != grOk) {
        printf("Error al iniciar el entorno grafico.");
        getch();
        exit(1);
    }
}

void dibujarMenu() {
    cleardevice(); 
    
    int centroX = getmaxx() / 2;
    int centroY = getmaxy() / 2;
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    // --- TÍTULO PRINCIPAL ---
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); 
    outtextxy(centroX, centroY - 180, (char*)"BARAJA ESPANOLA");
    
    // --- DISEÑO DE LOS BOTONES ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); 
    int anchoBoton = 170; 
    int altoBoton = 25;   
    
    // Botón 1: CONTINENTAL
    rectangle(centroX - anchoBoton, centroY - 100 - altoBoton, centroX + anchoBoton, centroY - 100 + altoBoton);
    outtextxy(centroX, centroY - 100, (char*)"CONTINENTAL");
    
    // Botón 2: CHINCHON
    rectangle(centroX - anchoBoton, centroY - 30 - altoBoton, centroX + anchoBoton, centroY - 30 + altoBoton);
    outtextxy(centroX, centroY - 30, (char*)"CHINCHON");
    
    // Botón 3: TUTE
    rectangle(centroX - anchoBoton, centroY + 40 - altoBoton, centroX + 40 + altoBoton, centroY + 40 + altoBoton); // Ajuste simétrico
    // Para asegurar precisión matemática exacta con las dimensiones:
    // Caja Tute:
    rectangle(centroX - anchoBoton, centroY + 40 - altoBoton, centroX + anchoBoton, centroY + 40 + altoBoton);
    outtextxy(centroX, centroY + 40, (char*)"TUTE");
    
    // Botón 4: ESCOBA
    rectangle(centroX - anchoBoton, centroY + 110 - altoBoton, centroX + anchoBoton, centroY + 110 + altoBoton);
    outtextxy(centroX, centroY + 110, (char*)"ESCOBA");
    
    // --- PIE DE PÁGINA ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); 
    outtextxy(centroX, centroY + 200, (char*)"Presiona ESC para salir o haz clic en los botones");
}

int controlarMenu() {
    int mouseX, mouseY;
    int centroX = getmaxx() / 2;
    int centroY = getmaxy() / 2;
    int anchoBoton = 170;
    int altoBoton = 25;

    // Bucle para capturar eventos de ratón o teclado
    while(1) {
        // Opción alternativa por si acaso: si pulsan ESC, salimos
        if (kbhit()) {
            if (getch() == 27) return 5; 
        }

        // CONTROL POR RATÓN: ¿Se ha hecho clic izquierdo?
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY); // Guarda los píxeles X e Y del clic

            // Validar si el clic está dentro del rango horizontal (X) de TODOS los botones
            if (mouseX >= (centroX - anchoBoton) && mouseX <= (centroX + anchoBoton)) {
                
                // Clic en Botón 1: CONTINENTAL
                if (mouseY >= (centroY - 100 - altoBoton) && mouseY <= (centroY - 100 + altoBoton)) {
                    return 1;
                }
                // Clic en Botón 2: CHINCHON
                if (mouseY >= (centroY - 30 - altoBoton) && mouseY <= (centroY - 30 + altoBoton)) {
                    return 2;
                }
                // Clic en Botón 3: TUTE
                if (mouseY >= (centroY + 40 - altoBoton) && mouseY <= (centroY + 40 + altoBoton)) {
                    return 3;
                }
                // Clic en Botón 4: ESCOBA
                if (mouseY >= (centroY + 110 - altoBoton) && mouseY <= (centroY + 110 + altoBoton)) {
                    return 4;
                }
            }
        }
    }
}

void ejecutarJuego(int opcion) {
    switch(opcion) {
        case 1: jugarContinental(); break;
        case 2: jugarChinchon();    break;
        case 3: jugarTute();        break;
        case 4: jugarEscoba();      break;
    }
}

// ==========================================
// ZONA DE JUEGO: AQUÍ PROGRAMAS CADA MODO
// ==========================================

void prepararPantallaJuego(char* nombreJuego) {
    cleardevice();
    int centroX = getmaxx() / 2;
    int centroY = getmaxy() / 2;
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(centroX, centroY - 100, nombreJuego);
}

void jugarContinental() {
    prepararPantallaJuego((char*)"MESA DE CONTINENTAL");
    
    // -------------------------------------------------------------
    // [¡TU TURNO!] AQUÍ VA TU LÓGICA DEL JUEGO CONTINENTAL
    // Puedes pintar las cartas, pedir datos, etc.
    // -------------------------------------------------------------
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"Espacio reservado para el codigo del Continental...");
    
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para salir al menu.");
    getch(); 
}

void jugarChinchon() {
    prepararPantallaJuego((char*)"MESA DE CHINCHON");
    
    // -------------------------------------------------------------
    // [¡TU TURNO!] AQUÍ VA TU LÓGICA DEL JUEGO CHINCHÓN
    // -------------------------------------------------------------
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"Espacio reservado para el codigo del Chinchon...");
    
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para salir al menu.");
    getch();
}

void jugarTute() {
    prepararPantallaJuego((char*)"MESA DE TUTE");
    
    // -------------------------------------------------------------
    // [¡TU TURNO!] AQUÍ VA TU LÓGICA DEL JUEGO TUTE
    // -------------------------------------------------------------
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"Espacio reservado para el codigo del Tute...");
    
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para salir al menu.");
    getch();
}

void jugarEscoba() {
    prepararPantallaJuego((char*)"MESA DE LA ESCOBA");
    
    // -------------------------------------------------------------
    // [¡TU TURNO!] AQUÍ VA TU LÓGICA DEL JUEGO ESCOBA
    // -------------------------------------------------------------
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"Espacio reservado para el codigo de la Escoba...");
    
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para salir al menu.");
    getch();
}
