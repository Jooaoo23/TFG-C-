#include <stdio.h>    // Librería estándar para entrada/salida (printf, scanf)
#include <conio.h>    // Librería para manejo del teclado (kbhit, getch)
#include <string.h>   // Librería para cadenas de texto (strcpy, sprintf)
#include <graphics.h> // Librería gráfica para la interfaz visual

// =================================================================
// ESTRUCTURAS DE DATOS PARA EL JUEGO CONTINENTAL
// =================================================================
struct Jugador {
    char nombre[30];
    int puntosPorMano[7];
    int puntosTotales;
    bool yaHaPuntuadoManoActual;
};

struct PartidaContinental {
    struct Jugador listaJugadores[7];
    int cantidadJugadores;
    int manoActual;
};

// =================================================================
// ESTRUCTURAS DE DATOS PARA EL JUEGO TUTE (ACTUALIZADO)
// =================================================================
struct JugadorTute {
    char nombre[30];
};

struct PartidaTute {
    int cantidadManos;
    float precioPunto;
    struct JugadorTute equipo1[2]; // Dos jugadores por equipo
    struct JugadorTute equipo2[2];
    int puntosEq1[20];             // Puntos acumulados por mano (máx 20 manos)
    int puntosEq2[20];
    int manoActual;                // Contador de la mano en curso
};

// =================================================================
// PROTOTIPOS DE LAS FUNCIONES
// =================================================================
void iniciarModoGrafico();
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);

// Modos de juego generales
void jugarContinental();
void jugarChinchon();
void jugarTute();
void jugarEscoba();

// Subpantallas del modo Continental
void prepararPantallaJuego(const char* nombreDelJuego); 
void pantallaConfigurarJugadores(struct PartidaContinental *partida);
void pantallaMesaJuego(struct PartidaContinental *partida);

// Subpantallas del modo TUTE (NUEVO)
void pantallaConfigurarTute(struct PartidaTute *tute);
void pantallaEditarEquipo(struct JugadorTute equipo[2], int numEquipo);
void pantallaMesaTute(struct PartidaTute *tute); // <--- NUEVA FUNCIÓN SOLICITADA

// Variables globales
struct PartidaContinental miPartida;
struct PartidaTute miPartidaTute; // Instancia para el Tute

// =================================================================
// FUNCIÓN PRINCIPAL (MAIN)
// =================================================================
int main() {
    int opcionMenu = 0;
    iniciarModoGrafico();
    
    do {
        dibujarMenu();
        opcionMenu = controlarMenu();
        if (opcionMenu != 5) { 
            ejecutarJuego(opcionMenu);
        }
    } while (opcionMenu != 5); 
    
    closegraph();
    return 0;
}

// =================================================================
// IMPLEMENTACIÓN DEL MENÚ PRINCIPAL
// =================================================================

void iniciarModoGrafico() {
    int anchoPantalla = getmaxwidth();  
    int altoPantalla = getmaxheight(); 
    initwindow(anchoPantalla, altoPantalla, (char*)"TFG - Baraja Espanola", 0, 0, false, true);
    
    if (graphresult() != grOk) {
        printf("Error al iniciar el entorno grafico.");
        getch();
        exit(1);
    }
}

void dibujarMenu() {
    cleardevice(); 
    int mitadX = getmaxx() / 2;
    int mitadY = getmaxy() / 2;
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); 
    outtextxy(mitadX, mitadY - 180, (char*)"BARAJA ESPANOLA");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); 
    int medioAnchoBoton = 170; 
    int medioAltoBoton = 25;   
    
    rectangle(mitadX - medioAnchoBoton, mitadY - 100 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY - 100 + medioAltoBoton);
    outtextxy(mitadX, mitadY - 100, (char*)"CONTINENTAL");
    
    rectangle(mitadX - medioAnchoBoton, mitadY - 30 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY - 30 + medioAltoBoton);
    outtextxy(mitadX, mitadY - 30, (char*)"CHINCHON");
    
    rectangle(mitadX - medioAnchoBoton, mitadY + 40 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY + 40 + medioAltoBoton);
    outtextxy(mitadX, mitadY + 40, (char*)"TUTE");
    
    rectangle(mitadX - medioAnchoBoton, mitadY + 110 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY + 110 + medioAltoBoton);
    outtextxy(mitadX, mitadY + 110, (char*)"ESCOBA");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); 
    outtextxy(mitadX, mitadY + 200, (char*)"Presiona ESC para salir o haz clic en un juego");
}

int controlarMenu() {
    int clickX, clickY;
    int mitadX = getmaxx() / 2;
    int mitadY = getmaxy() / 2;
    int medioAnchoBoton = 170;
    int medioAltoBoton = 25;

    while(1) {
        if (kbhit()) {
            if (getch() == 27) return 5;
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= (mitadX - medioAnchoBoton) && clickX <= (mitadX + medioAnchoBoton)) {
                if (clickY >= (mitadY - 100 - medioAltoBoton) && clickY <= (mitadY - 100 + medioAltoBoton)) return 1;
                if (clickY >= (mitadY - 30 - medioAltoBoton) && clickY <= (mitadY - 30 + medioAltoBoton)) return 2;
                if (clickY >= (mitadY + 40 - medioAltoBoton) && clickY <= (mitadY + 40 + medioAltoBoton)) return 3;
                if (clickY >= (mitadY + 110 - medioAltoBoton) && clickY <= (mitadY + 110 + medioAltoBoton)) return 4;
            }
        }
    }
}

void ejecutarJuego(int opcionSeleccionada) {
    switch(opcionSeleccionada) {
        case 1: jugarContinental(); break;
        case 2: jugarChinchon();    break;
        case 3: jugarTute();        break;
        case 4: jugarEscoba();      break;
    }
}

void prepararPantallaJuego(const char* nombreDelJuego) {
    cleardevice();
    int mitadX = getmaxx() / 2;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(mitadX, 50, (char*)nombreDelJuego);
}

// =================================================================
// LÓGICA DEL MODO DE JUEGO: TUTE
// =================================================================

void jugarTute() {
    // Inicialización de la partida
    miPartidaTute.cantidadManos = 10;
    miPartidaTute.precioPunto = 0.10;
    miPartidaTute.manoActual = 0; // Empezamos en la mano 1 (índice 0)
    
    // Limpiar arrays de puntos
    for(int i=0; i<20; i++) { miPartidaTute.puntosEq1[i] = 0; miPartidaTute.puntosEq2[i] = 0; }

    strcpy(miPartidaTute.equipo1[0].nombre, "ALEJANDRO");
    strcpy(miPartidaTute.equipo1[1].nombre, "JOAO");
    strcpy(miPartidaTute.equipo2[0].nombre, "JUGADOR 3");
    strcpy(miPartidaTute.equipo2[1].nombre, "JUGADOR 4");

    pantallaConfigurarTute(&miPartidaTute);
    
    // Al pulsar JUGAR en la config, entramos a la mesa
    pantallaMesaTute(&miPartidaTute);
}

void pantallaConfigurarTute(struct PartidaTute *tute) {
    bool salirConfig = false;
    int clickX, clickY;

    while (!salirConfig) {
        cleardevice();
        settextjustify(LEFT_TEXT, TOP_TEXT);
        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
        outtextxy(100, 50, (char*)"TUTE");

        setcolor(RED); settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(100, 120, (char*)"IMPORTANTE PARA SABER CUANDO ACABA Y CALCULAR LOS PUNTOS");

        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(100, 180, (char*)"CANTIDAD DE MANOS:");
        rectangle(350, 175, 420, 215);
        char bufM[5]; sprintf(bufM, "%d", tute->cantidadManos); outtextxy(370, 185, bufM);

        outtextxy(100, 300, (char*)"EQUIPO 1 EDITAR"); rectangle(350, 290, 500, 330);
        outtextxy(100, 380, (char*)"EQUIPO 2 EDITAR"); rectangle(350, 370, 500, 410);

        rectangle(150, 550, 400, 620); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(275, 585, (char*)"JUGAR");

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= 350 && clickX <= 500 && clickY >= 290 && clickY <= 330) pantallaEditarEquipo(tute->equipo1, 1);
            if (clickX >= 350 && clickX <= 500 && clickY >= 370 && clickY <= 410) pantallaEditarEquipo(tute->equipo2, 2);
            if (clickX >= 150 && clickX <= 400 && clickY >= 550 && clickY <= 620) salirConfig = true;
            if (clickX >= 350 && clickX <= 420 && clickY >= 175 && clickY <= 215) {
                printf("CANTIDAD DE MANOS: "); scanf("%d", &tute->cantidadManos);
            }
        }
        delay(20);
    }
}

// NUEVA FUNCIÓN: Mesa de juego con tabla de puntos
void pantallaMesaTute(struct PartidaTute *tute) {
    bool finPartida = false;
    int clickX, clickY;
    int totalEq1 = 0, totalEq2 = 0;

    while (!finPartida) {
        cleardevice();
        int mX = getmaxx() / 2;
        
        // Cabecera e info de mano
        settextjustify(CENTER_TEXT, TOP_TEXT);
        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(mX, 30, (char*)"TUTE");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        char bufM[30]; sprintf(bufM, "MANO %d", tute->manoActual + 1);
        outtextxy(100, 80, bufM);

        // Botón SIGUIENTE MANO
        rectangle(mX + 150, 20, mX + 450, 70);
        outtextxy(mX + 300, 35, (char*)"SIGUIENTE MANO");

        // Formato de nombres de equipo
        char nEq1[60], nEq2[60];
        sprintf(nEq1, "%s Y %s", tute->equipo1[0].nombre, tute->equipo1[1].nombre);
        sprintf(nEq2, "%s Y %s", tute->equipo2[0].nombre, tute->equipo2[1].nombre);
        
        outtextxy(mX - 250, 150, nEq1);
        outtextxy(mX + 250, 150, nEq2);

        // Tabla: PUNTOS y CANTOS (Separador central)
        line(mX, 180, mX, 600); 
        outtextxy(mX - 350, 200, (char*)"PUNTOS"); outtextxy(mX - 150, 200, (char*)"CANTOS");
        outtextxy(mX + 150, 200, (char*)"PUNTOS"); outtextxy(mX + 350, 200, (char*)"CANTOS");
        line(50, 230, getmaxx() - 50, 230);

        // Dibujar puntos de cada mano acumulados
        totalEq1 = 0; totalEq2 = 0;
        for(int i=0; i <= tute->manoActual; i++) {
            int yFila = 250 + (i * 30);
            if(tute->puntosEq1[i] > 0 || tute->puntosEq2[i] > 0) {
                char p1[10], p2[10];
                sprintf(p1, "%d", tute->puntosEq1[i]); sprintf(p2, "%d", tute->puntosEq2[i]);
                outtextxy(mX - 350, yFila, p1);
                outtextxy(mX + 150, yFila, p2);
                totalEq1 += tute->puntosEq1[i];
                totalEq2 += tute->puntosEq2[i];
            }
        }

        // Botón AÑADIR PUNTOS
        rectangle(mX - 120, 620, mX + 120, 680);
        outtextxy(mX, 640, (char*)"ANADIR PUNTOS");

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            // Lógica Añadir Puntos
            if (clickX >= mX - 120 && clickX <= mX + 120 && clickY >= 620 && clickY <= 680) {
                printf("\n--- PUNTOS MANO %d ---\n", tute->manoActual + 1);
                printf("Puntos Equipo 1: "); scanf("%d", &tute->puntosEq1[tute->manoActual]);
                printf("Puntos Equipo 2: "); scanf("%d", &tute->puntosEq2[tute->manoActual]);
            }

            // Lógica Siguiente Mano y Fin de Partida
            if (clickX >= mX + 150 && clickX <= mX + 450 && clickY >= 20 && clickY <= 70) {
                tute->manoActual++;
                if (tute->manoActual >= tute->cantidadManos) {
                    cleardevice();
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
                    outtextxy(mX, 200, (char*)"FIN DE PARTIDA");
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    char res[100];
                    if (totalEq1 > totalEq2) sprintf(res, "GANADOR: EQUIPO 1 (%d pts)", totalEq1);
                    else if (totalEq2 > totalEq1) sprintf(res, "GANADOR: EQUIPO 2 (%d pts)", totalEq2);
                    else strcpy(res, "EMPATE");
                    outtextxy(mX, 350, res);
                    outtextxy(mX, 500, (char*)"Pulsa para salir...");
                    getch(); finPartida = true;
                }
            }
        }
        delay(20);
    }
}

void pantallaEditarEquipo(struct JugadorTute equipo[2], int numEquipo) {
    bool volver = false; int x, y;
    while (!volver) {
        cleardevice();
        settextjustify(CENTER_TEXT, CENTER_TEXT); setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        char t[30]; sprintf(t, "EQUIPO %d", numEquipo); outtextxy(getmaxx()/2, 100, t);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(getmaxx()/2 - 100, 250, equipo[0].nombre); rectangle(getmaxx()/2 + 50, 225, getmaxx()/2 + 200, 275);
        outtextxy(getmaxx()/2 - 100, 350, equipo[1].nombre); rectangle(getmaxx()/2 + 50, 325, getmaxx()/2 + 200, 375);
        rectangle(getmaxx()/2 - 100, 500, getmaxx()/2 + 100, 550); outtextxy(getmaxx()/2, 525, (char*)"VOLVER");
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= getmaxx()/2+50 && x <= getmaxx()/2+200 && y >= 225 && y <= 275) { printf("Nombre: "); scanf("%s", equipo[0].nombre); }
            if (x >= getmaxx()/2+50 && x <= getmaxx()/2+200 && y >= 325 && y <= 375) { printf("Nombre: "); scanf("%s", equipo[1].nombre); }
            if (x >= getmaxx()/2-100 && x <= getmaxx()/2+100 && y >= 500 && y <= 550) volver = true;
        }
        delay(20);
    }
}

// =================================================================
// LÓGICA RESTANTE (NO MODIFICADA)
// =================================================================
void jugarContinental() {
    miPartida.cantidadJugadores = 0;
    miPartida.manoActual = 1;      
    pantallaConfigurarJugadores(&miPartida);
    if (miPartida.cantidadJugadores > 0) {
        pantallaMesaJuego(&miPartida);
    }
}

void pantallaConfigurarJugadores(struct PartidaContinental *partida) {
    bool salirAjustes = false;
    int clickX, clickY;
    while (!salirAjustes) {
        cleardevice(); 
        prepararPantallaJuego("CONTINENTAL");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        rectangle(100, 120, 350, 180); 
        outtextxy(225, 150, (char*)"ANADIR JUGADOR");
        rectangle(getmaxx() - 350, getmaxy() - 120, getmaxx() - 100, getmaxy() - 60);
        outtextxy(getmaxx() - 225, getmaxy() - 90, (char*)"JUGAR");
        outtextxy(200, getmaxy() - 90, (char*)"MAX: 7 JUGADORES");

        for (int i = 0; i < partida->cantidadJugadores; i++) {
            int filaY = 220 + (i * 60); 
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            char bufferNombre[50]; sprintf(bufferNombre, "%s:", partida->listaJugadores[i].nombre);
            outtextxy(100, filaY + 20, bufferNombre); 
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            rectangle(350, filaY, 480, filaY + 40);
            outtextxy(415, filaY + 20, (char*)"EDITAR");
            rectangle(510, filaY, 660, filaY + 40);
            outtextxy(585, filaY + 20, (char*)"ELIMINAR");
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= 100 && clickX <= 350 && clickY >= 120 && clickY <= 180) {
                if (partida->cantidadJugadores < 7) {
                    char nuevoNombre[30];
                    printf("INTRODUCE NOMBRE JUGADOR %d: ", partida->cantidadJugadores + 1);
                    scanf("%29s", nuevoNombre);
                    strcpy(partida->listaJugadores[partida->cantidadJugadores].nombre, nuevoNombre);
                    partida->cantidadJugadores++; 
                }
            }
            if (clickX >= (getmaxx() - 350) && clickX <= (getmaxx() - 100) && clickY >= (getmaxy() - 120) && clickY <= (getmaxy() - 60)) {
                if (partida->cantidadJugadores > 0) salirAjustes = true;
            }
        }
        delay(30);
    }
}

void pantallaMesaJuego(struct PartidaContinental *partida) {
    cleardevice();
    prepararPantallaJuego("MESA DE JUEGO");
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"Partida Iniciada!");
    getch(); 
}

void jugarChinchon() { prepararPantallaJuego("MESA DE CHINCHON"); getch(); }
void jugarEscoba()   { prepararPantallaJuego("MESA DE LA ESCOBA"); getch(); }
