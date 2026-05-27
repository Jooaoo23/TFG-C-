#include <stdio.h>    
#include <conio.h>    
#include <string.h>   
#include <graphics.h> 

// =================================================================
// STRUCTS SIMPLES 
// =================================================================

struct EntradaHistorial {
    char juego[30];
    char ganador[30];
};

struct Jugador {
    char nombre[30];
    int puntosPorMano[12];       
    int puntosAcumuladosEnMano[12]; 
    int puntosTotales;              
    bool yaAnoto;                   
};

struct PartidaContinental {
    struct Jugador listaJugadores[8]; 
    int cantidadJugadores;            
    int manoActual;                    
};

struct JugadorChinchon {
    char nombre[30];
    int puntosPorRonda[100];  
    int puntosAcumuladosEnRonda[100]; 
    int puntosTotales;                
    bool yaAnoto;                     
};

struct PartidaChinchon {
    struct JugadorChinchon listaJugadores[8]; 
    int cantidadJugadores;            
    int rondaActual;                  
};

// --- Estructuras para TUTE (Necesarias para el código incrustado) ---
struct JugadorTute {
    char nombre[30];
};

struct PartidaTute {
    int cantidadManos;
    float precioPunto;
    int puntosEq1[20];
    int puntosEq2[20];
    int manoActual;
    struct JugadorTute equipo1[2];
    struct JugadorTute equipo2[2];
};

// --- VARIABLES GLOBALES ---
struct PartidaContinental miPartida;
struct PartidaChinchon miPartidaChinchon;
struct PartidaTute miPartidaTute; 

// --- PROTOTIPOS DE LAS FUNCIONES ---
void iniciarModoGrafico();
void actualizarPantalla(); 
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);
void leerTextoGrafico(int x, int y, char* buffer, int maxLen);
void guardarEnHistorial(const char* juego, const char* ganador);
void mostrarHistorial();

// Continental
void menuContinental();
void pantallaConfiguracion();
void pantallaJugadores();
void pantallaMano();
void dibujarTabla();
void pantallaFinal();

// Chinchón
void menuChinchon();
void pantallaConfiguracionChinchon();
void pantallaJugadoresChinchon();
void pantallaRondaChinchon();
void dibujarTablaChinchon();
void pantallaFinalChinchon();

// Tute (Prototipos del código incrustado)
void jugarTute();
void pantallaConfigurarTute(struct PartidaTute *tute);
void pantallaMesaTute(struct PartidaTute *tute);
void pantallaEditarEquipo(struct JugadorTute equipo[2], int numEquipo);

// =================================================================
// MAIN
// =================================================================
int main() {
    iniciarModoGrafico();
    
    int opcion = 0;
    while (opcion != 5) {
        dibujarMenu();
        opcion = controlarMenu();
        if (opcion != 5) {
            ejecutarJuego(opcion);
        }
    }
    
    closegraph();
    return 0;
}

// =================================================================
// IMPLEMENTACIÓN DE FUNCIONES GENERALES
// =================================================================

void iniciarModoGrafico() {
    int gd = DETECT, gm;
    initwindow(1280, 720, "PROGRAMA DE GESTION DE PARTIDAS");
}

void dibujarMenu() {
    cleardevice();
    setbkcolor(COLOR(30, 30, 30));
    cleardevice();
    
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 6);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(640, 100, (char*)"MENU PRINCIPAL");
    
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    rectangle(440, 200, 840, 260); outtextxy(640, 230, (char*)"1. JUGAR CONTINENTAL");
    rectangle(440, 280, 840, 340); outtextxy(640, 310, (char*)"2. JUGAR CHINCHON");
    rectangle(440, 360, 840, 420); outtextxy(640, 390, (char*)"3. JUGAR TUTE");
    rectangle(440, 440, 840, 500); outtextxy(640, 470, (char*)"4. VER HISTORIAL");
    rectangle(440, 520, 840, 580); outtextxy(640, 550, (char*)"5. SALIR");
}

int controlarMenu() {
    int x, y;
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= 440 && x <= 840) {
                if (y >= 200 && y <= 260) return 1;
                if (y >= 280 && y <= 340) return 2;
                if (y >= 360 && y <= 420) return 3;
                if (y >= 440 && y <= 500) return 4;
                if (y >= 520 && y <= 580) return 5;
            }
        }
        delay(10);
    }
}

void ejecutarJuego(int opcionSeleccionada) {
    switch (opcionSeleccionada) {
        case 1: menuContinental(); break;
        case 2: menuChinchon(); break;
        case 3: jugarTute(); break;
        case 4: mostrarHistorial(); break;
    }
}

void leerTextoGrafico(int x, int y, char* buffer, int maxLen) {
    int i = 0;
    char c;
    buffer[0] = '\0';
    while (true) {
        if (kbhit()) {
            c = getch();
            if (c == 13) break; 
            if (c == 8 && i > 0) { 
                i--;
                buffer[i] = '\0';
            } else if (i < maxLen - 1 && c >= 32 && c <= 126) {
                buffer[i] = c;
                i++;
                buffer[i] = '\0';
            }
            setfillstyle(SOLID_FILL, COLOR(30, 30, 30));
            bar(x - 150, y - 20, x + 150, y + 20);
            outtextxy(x, y, buffer);
        }
    }
}

void guardarEnHistorial(const char* juego, const char* ganador) {
    FILE* f = fopen("historial.dat", "ab");
    if (f) {
        struct EntradaHistorial entrada;
        strcpy(entrada.juego, juego);
        strcpy(entrada.ganador, ganador);
        fwrite(&entrada, sizeof(struct EntradaHistorial), 1, f);
        fclose(f);
    }
}

void mostrarHistorial() {
    cleardevice();
    setcolor(WHITE);
    outtextxy(640, 50, (char*)"HISTORIAL DE GANADORES");
    FILE* f = fopen("historial.dat", "rb");
    if (!f) {
        outtextxy(640, 300, (char*)"No hay registros aun.");
    } else {
        struct EntradaHistorial entrada;
        int y = 120;
        while (fread(&entrada, sizeof(struct EntradaHistorial), 1, f)) {
            char linea[100];
            sprintf(linea, "%s - Ganador: %s", entrada.juego, entrada.ganador);
            outtextxy(640, y, linea);
            y += 40;
        }
        fclose(f);
    }
    rectangle(540, 660, 740, 700);
    outtextxy(640, 680, (char*)"VOLVER");
    int x, my;
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, my);
            if (x >= 540 && x <= 740 && my >= 660 && my <= 700) return;
        }
    }
}

// =================================================================
// JUEGO 3: TUTE (CÓDIGO INCRUSTADO)
// =================================================================

void jugarTute() {
    miPartidaTute.cantidadManos = 10;
    miPartidaTute.precioPunto = 0.10;
    miPartidaTute.manoActual = 0; 
    
    for(int i=0; i<20; i++) { miPartidaTute.puntosEq1[i] = 0; miPartidaTute.puntosEq2[i] = 0; }

    strcpy(miPartidaTute.equipo1[0].nombre, "ALEJANDRO");
    strcpy(miPartidaTute.equipo1[1].nombre, "JOAO");
    strcpy(miPartidaTute.equipo2[0].nombre, "JUGADOR 3");
    strcpy(miPartidaTute.equipo2[1].nombre, "JUGADOR 4");

    pantallaConfigurarTute(&miPartidaTute);
    pantallaMesaTute(&miPartidaTute);
}

void pantallaConfigurarTute(struct PartidaTute *tute) {
    bool salirConfig = false;
    int clickX, clickY;

    while (!salirConfig) {
        cleardevice();
        settextjustify(LEFT_TEXT, TOP_TEXT);
        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(100, 50, (char*)"TUTE");

        setcolor(RED); settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(100, 120, (char*)"IMPORTANTE PARA SABER CUANDO ACABA Y CALCULAR LOS PUNTOS");

        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(100, 180, (char*)"CANTIDAD DE MANOS:");
        rectangle(350, 175, 450, 215);
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
            
            if (clickX >= 350 && clickX <= 450 && clickY >= 175 && clickY <= 215) {
                char bufCant[10] = "";
                setcolor(YELLOW); outtextxy(520, 195, (char*)"NUEVA CANTIDAD:");
                leerTextoGrafico(650, 195, bufCant, 5);
                if(strlen(bufCant) > 0) sscanf(bufCant, "%d", &tute->cantidadManos);
                setcolor(WHITE);
            }
        }
        delay(20);
    }
}

void pantallaMesaTute(struct PartidaTute *tute) {
    bool finPartida = false;
    int clickX, clickY;
    int totalEq1 = 0, totalEq2 = 0;

    while (!finPartida) {
        cleardevice();
        int mX = 1280 / 2;
        
        settextjustify(CENTER_TEXT, TOP_TEXT);
        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(mX, 30, (char*)"TUTE");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        char bufM[30]; sprintf(bufM, "MANO %d", tute->manoActual + 1);
        outtextxy(100, 80, bufM);

        rectangle(mX + 150, 20, mX + 450, 70);
        outtextxy(mX + 300, 35, (char*)"SIGUIENTE MANO");

        char nEq1[60], nEq2[60];
        sprintf(nEq1, "%s Y %s", tute->equipo1[0].nombre, tute->equipo1[1].nombre);
        sprintf(nEq2, "%s Y %s", tute->equipo2[0].nombre, tute->equipo2[1].nombre);
        
        outtextxy(mX - 250, 150, nEq1);
        outtextxy(mX + 250, 150, nEq2);

        line(mX, 180, mX, 600); 
        outtextxy(mX - 350, 200, (char*)"PUNTOS"); outtextxy(mX - 150, 200, (char*)"CANTOS");
        outtextxy(mX + 150, 200, (char*)"PUNTOS"); outtextxy(mX + 350, 200, (char*)"CANTOS");
        line(50, 230, 1280 - 50, 230);

        totalEq1 = 0; totalEq2 = 0;
        for(int i=0; i <= tute->manoActual; i++) {
            int yFila = 260 + (i * 35);
            if(tute->puntosEq1[i] > 0 || tute->puntosEq2[i] > 0) {
                char p1[10], p2[10];
                sprintf(p1, "%d", tute->puntosEq1[i]); sprintf(p2, "%d", tute->puntosEq2[i]);
                outtextxy(mX - 350, yFila, p1);
                outtextxy(mX + 150, yFila, p2);
            }
            totalEq1 += tute->puntosEq1[i];
            totalEq2 += tute->puntosEq2[i];
        }

        rectangle(mX - 120, 620, mX + 120, 680);
        outtextxy(mX, 640, (char*)"ANADIR PUNTOS");

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= mX - 120 && clickX <= mX + 120 && clickY >= 620 && clickY <= 680) {
                char pBuf1[10]="", pBuf2[10]="";
                setcolor(YELLOW);
                settextjustify(LEFT_TEXT, CENTER_TEXT);
                outtextxy(mX - 300, 650, (char*)"Pts Eq1:");
                leerTextoGrafico(mX - 180, 650, pBuf1, 5);
                outtextxy(mX + 150, 650, (char*)"Pts Eq2:");
                leerTextoGrafico(mX + 270, 650, pBuf2, 5);
                if(strlen(pBuf1) > 0) sscanf(pBuf1, "%d", &tute->puntosEq1[tute->manoActual]);
                if(strlen(pBuf2) > 0) sscanf(pBuf2, "%d", &tute->puntosEq2[tute->manoActual]);
                setcolor(WHITE);
            }
            if (clickX >= mX + 150 && clickX <= mX + 450 && clickY >= 20 && clickY <= 70) {
                tute->manoActual++;
                if (tute->manoActual >= tute->cantidadManos) {
                    cleardevice();
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
                    outtextxy(mX, 200, (char*)"FIN DE PARTIDA");
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                    char res[100];
                    if (totalEq1 > totalEq2) {
                        sprintf(res, "GANADOR: EQUIPO 1 (%d pts)", totalEq1);
                        guardarEnHistorial("Tute", "Equipo 1");
                    } else if (totalEq2 > totalEq1) {
                        sprintf(res, "GANADOR: EQUIPO 2 (%d pts)", totalEq2);
                        guardarEnHistorial("Tute", "Equipo 2");
                    } else strcpy(res, "EMPATE");
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
    int mX = 1280 / 2;
    while (!volver) {
        cleardevice();
        settextjustify(CENTER_TEXT, CENTER_TEXT); setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        char t[30]; sprintf(t, "EQUIPO %d", numEquipo); outtextxy(mX, 100, t);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(mX - 100, 250, equipo[0].nombre); rectangle(mX + 50, 225, mX + 200, 275);
        outtextxy(mX - 100, 350, equipo[1].nombre); rectangle(mX + 50, 325, mX + 200, 375);
        rectangle(mX - 100, 500, mX + 100, 550); outtextxy(mX, 525, (char*)"VOLVER");
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x >= mX+50 && x <= mX+200 && y >= 225 && y <= 275) { 
                char nomBuf[30]="";
                leerTextoGrafico(mX + 250, 250, nomBuf, 25);
                if(strlen(nomBuf)>0) strcpy(equipo[0].nombre, nomBuf);
            }
            if (x >= mX+50 && x <= mX+200 && y >= 325 && y <= 375) { 
                char nomBuf[30]="";
                leerTextoGrafico(mX + 250, 350, nomBuf, 25);
                if(strlen(nomBuf)>0) strcpy(equipo[1].nombre, nomBuf);
            }
            if (x >= mX-100 && x <= mX+100 && y >= 500 && y <= 550) volver = true;
        }
        delay(20);
    }
}

// =================================================================
// LÓGICA RESTANTE (Continental y Chinchón originales de V3)
// =================================================================

void menuContinental() {
    pantallaConfiguracion();
    pantallaJugadores();
    miPartida.manoActual = 0;
    while (miPartida.manoActual < 11) {
        pantallaMano();
        miPartida.manoActual++;
    }
    pantallaFinal();
}

void pantallaConfiguracion() {
    cleardevice();
    outtextxy(640, 100, (char*)"NUEVA PARTIDA: CONTINENTAL");
    outtextxy(640, 300, (char*)"¿CUANTOS JUGADORES? (2-8)");
    rectangle(590, 330, 690, 380);
    char buffer[10] = "";
    leerTextoGrafico(640, 355, buffer, 2);
    sscanf(buffer, "%d", &miPartida.cantidadJugadores);
}

void pantallaJugadores() {
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        cleardevice();
        char msg[50];
        sprintf(msg, "NOMBRE JUGADOR %d:", i + 1);
        outtextxy(640, 300, msg);
        rectangle(440, 330, 840, 380);
        leerTextoGrafico(640, 355, miPartida.listaJugadores[i].nombre, 25);
        miPartida.listaJugadores[i].puntosTotales = 0;
    }
}

void dibujarTabla() {
    cleardevice();
    int anchoCol = 1180 / miPartida.cantidadJugadores;
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int x = 50 + (i * anchoCol);
        outtextxy(x + (anchoCol / 2), 30, miPartida.listaJugadores[i].nombre);
        line(x, 20, x, 650);
        for (int j = 0; j < miPartida.manoActual; j++) {
            char p[10]; sprintf(p, "%d", miPartida.listaJugadores[i].puntosPorMano[j]);
            outtextxy(x + (anchoCol / 2), 60 + (j * 45), p);
        }
    }
}

void pantallaMano() {
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        dibujarTabla();
        char msg[100]; sprintf(msg, "Puntos para %s", miPartida.listaJugadores[i].nombre);
        outtextxy(640, 680, msg);
        char bufP[10] = "";
        leerTextoGrafico(640, 705, bufP, 4);
        int pts; sscanf(bufP, "%d", &pts);
        miPartida.listaJugadores[i].puntosPorMano[miPartida.manoActual] = pts;
        miPartida.listaJugadores[i].puntosTotales += pts;
    }
}

void pantallaFinal() {
    cleardevice();
    int menor = 9999, win = 0;
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        if (miPartida.listaJugadores[i].puntosTotales < menor) {
            menor = miPartida.listaJugadores[i].puntosTotales;
            win = i;
        }
    }
    guardarEnHistorial("Continental", miPartida.listaJugadores[win].nombre);
    outtextxy(640, 300, (char*)"FIN DE PARTIDA");
    getch();
}

void menuChinchon() {
    pantallaConfiguracionChinchon();
    pantallaJugadoresChinchon();
    miPartidaChinchon.rondaActual = 0;
    bool fin = false;
    while (!fin) {
        pantallaRondaChinchon();
        miPartidaChinchon.rondaActual++;
        for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
            if (miPartidaChinchon.listaJugadores[i].puntosTotales >= 100) fin = true;
        }
    }
    pantallaFinalChinchon();
}

void pantallaConfiguracionChinchon() {
    cleardevice();
    outtextxy(640, 100, (char*)"NUEVA PARTIDA: CHINCHÓN");
    rectangle(590, 330, 690, 380);
    char buffer[10] = "";
    leerTextoGrafico(640, 355, buffer, 2);
    sscanf(buffer, "%d", &miPartidaChinchon.cantidadJugadores);
}

void pantallaJugadoresChinchon() {
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        cleardevice();
        outtextxy(640, 300, (char*)"NOMBRE JUGADOR:");
        rectangle(440, 330, 840, 380);
        leerTextoGrafico(640, 355, miPartidaChinchon.listaJugadores[i].nombre, 25);
        miPartidaChinchon.listaJugadores[i].puntosTotales = 0;
    }
}

void dibujarTablaChinchon() {
    cleardevice();
    int anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int x = 50 + (i * anchoCol);
        outtextxy(x + (anchoCol / 2), 30, miPartidaChinchon.listaJugadores[i].nombre);
        line(x, 20, x, 650);
    }
}

void pantallaRondaChinchon() {
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        dibujarTablaChinchon();
        char msg[100]; sprintf(msg, "Puntos para %s", miPartidaChinchon.listaJugadores[i].nombre);
        outtextxy(640, 680, msg);
        char bufP[10] = "";
        leerTextoGrafico(640, 705, bufP, 4);
        int pts; sscanf(bufP, "%d", &pts);
        miPartidaChinchon.listaJugadores[i].puntosTotales += pts;
    }
}

void pantallaFinalChinchon() {
    cleardevice();
    outtextxy(640, 300, (char*)"FIN CHINCHON");
    getch();
}
