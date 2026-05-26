#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <winbgim.h>

// Prototipos de las funciones
void iniciarModoGrafico();
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcion);

int main() {
    int opcion = 0;
    
    iniciarModoGrafico();
    
    do {
        dibujarMenu();
        opcion = controlarMenu();
        if (opcion != 5) {
            ejecutarJuego(opcion);
        }
    } while (opcion != 5);
    
    closegraph();
    return 0;
}

void iniciarModoGrafico() {
    int anchoMonitor = getmaxwidth();
    int altoMonitor = getmaxheight();
    
    // Abrimos a pantalla completa
    initwindow(anchoMonitor, altoMonitor, (char*)"Mi TFG - Baraja Espanola", 0, 0, false, true);
    
    if (graphresult() != grOk) {
        printf("Error al iniciar el entorno grafico.");
        getch();
        exit(1);
    }
}

void dibujarMenu() {
    cleardevice(); 
    
    // 1. OBTENER LAS DIMENSIONES REALES DE LA PANTALLA ACTUAL
    int centroX = getmaxx() / 2;
    int centroY = getmaxy() / 2;
    
    // 2. CONFIGURAR LA ALINEACIÓN DEL TEXTO (¡Clave para que outtextxy centre de verdad!)
    // CENTER_TEXT alinea horizontalmente al medio, y TOP_TEXT o BOTTOM_TEXT verticalmente
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    // --- TÍTULO ---
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); // Un poco más grande para pantallas altas
    outtextxy(centroX, centroY - 180, (char*)"BARAJA ESPANOLA");
    
    // --- CONFIGURACIÓN DE BOTONES ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    int anchoBoton = 170; // Mitad del ancho total del botón (340 píxeles en total)
    int altoBoton = 25;   // Mitad del alto total del botón (50 píxeles en total)
    
    // Botón 1: CONTINENTAL (Posicionado arriba del centro de la pantalla)
    rectangle(centroX - anchoBoton, centroY - 100 - altoBoton, centroX + anchoBoton, centroY - 100 + altoBoton);
    outtextxy(centroX, centroY - 100, (char*)"CONTINENTAL");
    
    // Botón 2: CHINCHON
    rectangle(centroX - anchoBoton, centroY - 30 - altoBoton, centroX + anchoBoton, centroY - 30 + altoBoton);
    outtextxy(centroX, centroY - 30, (char*)"CHINCHON");
    
    // Botón 3: TUTE
    rectangle(centroX - anchoBoton, centroY + 40 - altoBoton, centroX + anchoBoton, centroY + 40 + altoBoton);
    outtextxy(centroX, centroY + 40, (char*)"TUTE");
    
    // Botón 4: ESCOBA
    rectangle(centroX - anchoBoton, centroY + 110 - altoBoton, centroX + anchoBoton, centroY + 110 + altoBoton);
    outtextxy(centroX, centroY + 110, (char*)"ESCOBA");
    
    // --- NOTA DE PIE DE PÁGINA ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(centroX, centroY + 200, (char*)"Presiona ESC para salir del programa");
}

int controlarMenu() {
    char tecla;
    while(1) {
        if (kbhit()) {
            tecla = getch();
            if (tecla == '1') return 1;
            if (tecla == '2') return 2;
            if (tecla == '3') return 3;
            if (tecla == '4') return 4;
            if (tecla == 27)  return 5; 
        }
    }
}

void ejecutarJuego(int opcion) {
    cleardevice();
    
    int centroX = getmaxx() / 2;
    int centroY = getmaxy() / 2;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    switch(opcion) {
        case 1: outtextxy(centroX, centroY - 30, (char*)"Iniciando juego: CONTINENTAL"); break;
        case 2: outtextxy(centroX, centroY - 30, (char*)"Iniciando juego: CHINCHON");    break;
        case 3: outtextxy(centroX, centroY - 30, (char*)"Iniciando juego: TUTE");        break;
        case 4: outtextxy(centroX, centroY - 30, (char*)"Iniciando juego: ESCOBA");      break;
    }
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(centroX, centroY + 50, (char*)"Presiona cualquier tecla para volver al menu principal...");
    getch(); 
}
