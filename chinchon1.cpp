#include <estructuras.h>

// =================================================================
// JUEGO 2: CHINCHÓN (Similitud lógica con Continental, corregido loops)
// =================================================================

void jugarChinchon() {
    miPartidaChinchon.cantidadJugadores = 0; miPartidaChinchon.rondaActual = 1;
    mostrandoErrorChinchon = false;
    for(int i=0; i<8; i++) {
        miPartidaChinchon.listaJugadores[i].puntosTotales = 0; miPartidaChinchon.listaJugadores[i].yaAnoto = false;
        for(int j=0; j<100; j++) { miPartidaChinchon.listaJugadores[i].puntosPorRonda[j] = 0; miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[j] = 0; }
    }
    pantallaConfigurarJugadoresChinchon();
    if (miPartidaChinchon.cantidadJugadores > 1) pantallaMesaChinchon();
}

void redibujarFondoConfigurarChinchon() {
    cleardevice(); prepararPantallaJuego("CHINCHON");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); setcolor(WHITE);
    rectangle(100, 100, 350, 150); outtextxy(225, 125, (char*)"ANADIR JUGADOR");
    rectangle(1280 - 300, 720 - 100, 1280 - 100, 720 - 50); outtextxy(1280 - 200, 720 - 75, (char*)"JUGAR");
    rectangle(100, 720 - 100, 300, 720 - 50); outtextxy(200, 720 - 75, (char*)"SALIR");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); outtextxy(200, 720 - 110, (char*)"MAX: 4 JUGADORES");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int filaY = 180 + (i * 55); settextjustify(LEFT_TEXT, CENTER_TEXT);
        char bufN[50]; sprintf(bufN, "%s:", miPartidaChinchon.listaJugadores[i].nombre); outtextxy(100, filaY + 20, bufN);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        rectangle(350, filaY, 480, filaY + 40); outtextxy(415, filaY + 20, (char*)"EDITAR");
        rectangle(510, filaY, 660, filaY + 40); outtextxy(585, filaY + 20, (char*)"ELIMINAR");
    }
}

void pantallaConfigurarJugadoresChinchon() {
    bool salirAjustes = false; int clickX, clickY;
    while (!salirAjustes) {
        redibujarFondoConfigurarChinchon(); actualizarPantalla();
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (miPartidaChinchon.cantidadJugadores < 4) {
                    char nuevoNombre[30] = ""; leerTextoGrafico(390, 125, nuevoNombre, 25, 2);
                    if(strlen(nuevoNombre) > 0) { strcpy(miPartidaChinchon.listaJugadores[miPartidaChinchon.cantidadJugadores].nombre, nuevoNombre); miPartidaChinchon.cantidadJugadores++; }
                }
            }
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) if (miPartidaChinchon.cantidadJugadores >= 2) salirAjustes = true;
            if (clickX >= 100 && clickX <= 300 && clickY >= (720 - 100) && clickY <= (720 - 50)) { miPartidaChinchon.cantidadJugadores = 0; salirAjustes = true; }
            for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                int fY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= fY && clickY <= (fY + 40)) {
                    for (int j = i; j < miPartidaChinchon.cantidadJugadores - 1; j++) strcpy(miPartidaChinchon.listaJugadores[j].nombre, miPartidaChinchon.listaJugadores[j + 1].nombre);
                    miPartidaChinchon.cantidadJugadores--; break;
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= fY && clickY <= (fY + 40)) {
                    char nE[30] = ""; strcpy(nE, miPartidaChinchon.listaJugadores[i].nombre); leerTextoGrafico(690, fY + 20, nE, 25, 2);
                    if(strlen(nE) > 0) strcpy(miPartidaChinchon.listaJugadores[i].nombre, nE); break;
                }
            }
        }
        delay(20);
    }
}

void redibujarFondoMesaChinchon() {
    cleardevice(); char bufferR[50]; sprintf(bufferR, "RONDA: %d", miPartidaChinchon.rondaActual);
    settextjustify(CENTER_TEXT, CENTER_TEXT); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3); setcolor(WHITE);
    outtextxy(1280 / 2, 35, (char*)"CHINCHON");
    settextjustify(LEFT_TEXT, CENTER_TEXT); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); outtextxy(50, 90, bufferR);
    settextjustify(CENTER_TEXT, CENTER_TEXT); rectangle(1280 - 280, 70, 1280 - 50, 120); outtextxy(1280 - 165, 95, (char*)"SIGUIENTE RONDA");
    int anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
    line(50, 140, 1280 - 50, 140); line(50, 180, 1280 - 50, 180);
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol); outtextxy(colX + (anchoCol / 2), 160, miPartidaChinchon.listaJugadores[i].nombre);
        if(i > 0) line(colX, 140, colX, 720 - 150);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        int mInicio = (miPartidaChinchon.rondaActual > 15) ? miPartidaChinchon.rondaActual - 15 : 0;
        for (int m = mInicio; m < miPartidaChinchon.rondaActual; m++) {
            char bP[60];
            if (m == miPartidaChinchon.rondaActual - 1 && !miPartidaChinchon.listaJugadores[i].yaAnoto) sprintf(bP, "-");
            else {
                if (m == 0) sprintf(bP, "%d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[m]);
                else sprintf(bP, "%d - %d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[m], miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m]);
            }
            outtextxy(colX + (anchoCol / 2), 210 + ((m - mInicio) * 28), bP);
        }
        int btnY = 720 - 130; rectangle(colX + 10, btnY, colX + anchoCol - 10, btnY + 40);
        outtextxy(colX + (anchoCol / 2), btnY + 20, miPartidaChinchon.listaJugadores[i].yaAnoto ? (char*)"EDITAR" : (char*)"ANADIR PUNTOS");
    }
    if (mostrandoErrorChinchon) {
        setcolor(LIGHTRED); rectangle(1280/2 - 250, 720 - 80, 1280/2 + 250, 720 - 20);
        outtextxy(1280/2, 720 - 50, (char*)"FALTAN JUGADORES POR PUNTUAR O DATOS INVALIDOS"); setcolor(WHITE);
    }
}

void pantallaMesaChinchon() {
    bool terminarMesa = false; int clickX, clickY;
    while (!terminarMesa) {
        redibujarFondoMesaChinchon(); actualizarPantalla();
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY); mostrandoErrorChinchon = false;
            if (clickX >= 1280 - 280 && clickX <= 1280 - 50 && clickY >= 70 && clickY <= 120) {
                bool todosOk = true; int negativos = 0; bool alguienBajo5 = false;
                for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                    if (!miPartidaChinchon.listaJugadores[i].yaAnoto) todosOk = false;
                    else {
                        int p = miPartidaChinchon.listaJugadores[i].puntosPorRonda[miPartidaChinchon.rondaActual - 1];
                        if (p < -100 || p > 500) todosOk = false;
                        if (p < 0) negativos++; if (p <= 5) alguienBajo5 = true;
                    }
                }
                if (negativos > 1 || !alguienBajo5) todosOk = false;
                if (todosOk) {
                    bool algun100 = false;
                    for(int i=0; i<miPartidaChinchon.cantidadJugadores; i++) if(miPartidaChinchon.listaJugadores[i].puntosTotales >= 100) algun100 = true;
                    if (algun100) { terminarMesa = true; pantallaResultadosChinchon(); }
                    else { for(int i=0; i<miPartidaChinchon.cantidadJugadores; i++) miPartidaChinchon.listaJugadores[i].yaAnoto = false; miPartidaChinchon.rondaActual++; }
                } else mostrandoErrorChinchon = true;
            }
            int btnY = 720 - 130, anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
            for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                int colX = 50 + (i * anchoCol);
                if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) ventanaEmergentePuntosChinchon(i);
            }
        }
        delay(20);
    }
}

void ventanaEmergentePuntosChinchon(int i) {
    char bufferE[20] = ""; int pos = 0, cx, cy; bool continuar = true; clearmouseclick(WM_LBUTTONDOWN);
    if (miPartidaChinchon.listaJugadores[i].yaAnoto) { sprintf(bufferE, "%d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[miPartidaChinchon.rondaActual - 1]); pos = strlen(bufferE); }
    while(continuar) {
        redibujarFondoMesaChinchon(); setcolor(WHITE); rectangle(250, 500, 1030, 690); setfillstyle(SOLID_FILL, BLACK); bar(251, 501, 1029, 689);
        rectangle(650, 580, 850, 630); settextjustify(CENTER_TEXT, CENTER_TEXT); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 605, (char*)"SIGUIENTE"); char msg[100]; sprintf(msg, "PUNTOS JUGADOR - %s: ", miPartidaChinchon.listaJugadores[i].nombre);
        settextjustify(LEFT_TEXT, CENTER_TEXT); outtextxy(280, 560, msg); outtextxy(280 + textwidth(msg), 560, bufferE); outtextxy(280 + textwidth(msg) + textwidth(bufferE), 560, (char*)"_");
        actualizarPantalla();
        if (kbhit()) {
            char t = getch(); if (t == 13) continuar = false;
            else if (t == 8 && pos > 0) { pos--; bufferE[pos] = '\0'; }
            else if ((t >= '0' && t <= '9') || (t == '-' && pos == 0)) if (pos < 5) { bufferE[pos] = t; pos++; bufferE[pos] = '\0'; }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) { getmouseclick(WM_LBUTTONDOWN, cx, cy); if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) continuar = false; }
        delay(20);
    }
    int ptsI = 0; if (strlen(bufferE) > 0) sscanf(bufferE, "%d", &ptsI);
    int m = miPartidaChinchon.rondaActual - 1; miPartidaChinchon.listaJugadores[i].puntosPorRonda[m] = ptsI;
    if (m == 0) miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m] = ptsI;
    else miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m] = miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m-1] + ptsI;
    miPartidaChinchon.listaJugadores[i].puntosTotales = miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m];
    miPartidaChinchon.listaJugadores[i].yaAnoto = true; clearmouseclick(WM_LBUTTONDOWN);
}

void pantallaResultadosChinchon() {
    cleardevice(); prepararPantallaJuego("RESULTADOS FINALES - CHINCHON");
    int indiceGanador = 0, menorP = 9999, empates = 0;
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) if (miPartidaChinchon.listaJugadores[i].puntosTotales < menorP) { menorP = miPartidaChinchon.listaJugadores[i].puntosTotales; indiceGanador = i; }
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) if (miPartidaChinchon.listaJugadores[i].puntosTotales == menorP) empates++;
    guardarEnHistorial("Chinchon", miPartidaChinchon.listaJugadores[indiceGanador].nombre);
    int aC = 1180 / miPartidaChinchon.cantidadJugadores; line(50, 120, 1280 - 50, 120);
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int cX = 50 + (i * aC); outtextxy(cX + (aC / 2), 150, miPartidaChinchon.listaJugadores[i].nombre);
        char bT[50]; sprintf(bT, "Total: %d pts", miPartidaChinchon.listaJugadores[i].puntosTotales); outtextxy(cX + (aC / 2), 200, bT);
        if (miPartidaChinchon.listaJugadores[i].puntosTotales >= 100) { setcolor(LIGHTRED); outtextxy(cX + (aC / 2), 260, (char*)"ELIMINADO"); setcolor(WHITE); }
        else if (miPartidaChinchon.listaJugadores[i].puntosTotales == menorP) { setcolor(LIGHTGREEN); outtextxy(cX + (aC / 2), 260, empates > 1 ? (char*)"EMPATE!!!" : (char*)"GANADOR!!!"); setcolor(WHITE); }
        else outtextxy(cX + (aC / 2), 260, (char*)"SALVADO");
        if(i > 0) line(cX, 120, cX, 320);
    }
    line(50, 320, 1280 - 50, 320); actualizarPantalla(); clearmouseclick(WM_LBUTTONDOWN); getch();
}
