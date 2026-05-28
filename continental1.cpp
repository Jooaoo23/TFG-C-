#include <estructuras.h>

// =================================================================
// JUEGO 1: CONTINENTAL
// =================================================================

void jugarContinental() {
    miPartida.cantidadJugadores = 0;
    miPartida.manoActual = 1;      
    mostrandoErrorContinental = false;
    for(int i = 0; i < 8; i++) {
        miPartida.listaJugadores[i].puntosTotales = 0;
        miPartida.listaJugadores[i].yaAnoto = false;
        for(int j = 0; j < 12; j++) {
            miPartida.listaJugadores[i].puntosPorMano[j] = 0;
            miPartida.listaJugadores[i].puntosAcumuladosEnMano[j] = 0;
        }
    }
    pantallaConfigurarJugadores();
    if (miPartida.cantidadJugadores > 1) pantallaMesaJuego();
}

void redibujarFondoConfigurarContinental() {
    cleardevice(); 
    prepararPantallaJuego("CONTINENTAL");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); setcolor(WHITE);
    rectangle(100, 100, 350, 150); outtextxy(225, 125, (char*)"ANADIR JUGADOR");
    rectangle(1280 - 300, 720 - 100, 1280 - 100, 720 - 50); outtextxy(1280 - 200, 720 - 75, (char*)"JUGAR");
    rectangle(100, 720 - 100, 300, 720 - 50); outtextxy(200, 720 - 75, (char*)"SALIR");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 720 - 110, (char*)"MAX: 7 JUGADORES");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int filaY = 180 + (i * 55); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        char bufferNombre[50]; sprintf(bufferNombre, "%s:", miPartida.listaJugadores[i].nombre);
        outtextxy(100, filaY + 20, bufferNombre); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        rectangle(350, filaY, 480, filaY + 40); outtextxy(415, filaY + 20, (char*)"EDITAR");
        rectangle(510, filaY, 660, filaY + 40); outtextxy(585, filaY + 20, (char*)"ELIMINAR");
    }
}

void pantallaConfigurarJugadores() {
    bool salirAjustes = false;
    int clickX, clickY;
    while (!salirAjustes) {
        redibujarFondoConfigurarContinental();
        actualizarPantalla(); 
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (miPartida.cantidadJugadores < 7) {
                    char nuevoNombre[30] = "";
                    leerTextoGrafico(390, 125, nuevoNombre, 25, 1);
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(miPartida.listaJugadores[miPartida.cantidadJugadores].nombre, nuevoNombre);
                        miPartida.listaJugadores[miPartida.cantidadJugadores].yaAnoto = false;
                        miPartida.cantidadJugadores++; 
                    }
                }
            }
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (miPartida.cantidadJugadores >= 2) salirAjustes = true; 
            }
            if (clickX >= 100 && clickX <= 300 && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                miPartida.cantidadJugadores = 0; salirAjustes = true;
            }
            for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                int filaY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    for (int j = i; j < miPartida.cantidadJugadores - 1; j++) strcpy(miPartida.listaJugadores[j].nombre, miPartida.listaJugadores[j + 1].nombre);
                    miPartida.cantidadJugadores--; break; 
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditado[30] = ""; strcpy(nombreEditado, miPartida.listaJugadores[i].nombre);
                    leerTextoGrafico(690, filaY + 20, nombreEditado, 25, 1);
                    if(strlen(nombreEditado) > 0) strcpy(miPartida.listaJugadores[i].nombre, nombreEditado);
                    break;
                }
            }
        }
        delay(20); 
    }
}

void redibujarFondoMesaContinental() {
    cleardevice();
    if (miPartida.cantidadJugadores <= 0) return; // Seguridad
    char bufferMano[100];
    switch(miPartida.manoActual) {
        case 1: sprintf(bufferMano, "MANO 1: DOS TRIOS"); break;
        case 2: sprintf(bufferMano, "MANO 2: ESCALERA Y TRIO"); break;
        case 3: sprintf(bufferMano, "MANO 3: DOS ESCALERAS"); break;
        case 4: sprintf(bufferMano, "MANO 4: TRES TRIOS"); break;
        case 5: sprintf(bufferMano, "MANO 5: DOS TRIOS Y ESCALERA"); break;
        case 6: sprintf(bufferMano, "MANO 6: DOS ESCALERAS Y TRIO"); break;
        case 7: sprintf(bufferMano, "MANO 7: TRES ESCALERAS"); break;
        default: sprintf(bufferMano, "MANO EXTRA"); break;
    }
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3); setcolor(WHITE);
    outtextxy(1280 / 2, 35, (char*)"CONTINENTAL");
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(50, 90, bufferMano);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    rectangle(1280 - 280, 70, 1280 - 50, 120);
    outtextxy(1280 - 165, 95, (char*)"SIGUIENTE MANO");
    
    int anchoCol = 1180 / miPartida.cantidadJugadores;
    line(50, 140, 1280 - 50, 140); line(50, 180, 1280 - 50, 180);
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 160, miPartida.listaJugadores[i].nombre);
        if(i > 0) line(colX, 140, colX, 720 - 150); 
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        for (int m = 0; m < miPartida.manoActual; m++) {
            char bufferPts[60];
            if (m == miPartida.manoActual - 1 && !miPartida.listaJugadores[i].yaAnoto) sprintf(bufferPts, "-"); 
            else {
                if (m == 0) sprintf(bufferPts, "%d", miPartida.listaJugadores[i].puntosPorMano[m]);
                else sprintf(bufferPts, "%d - %d", miPartida.listaJugadores[i].puntosPorMano[m], miPartida.listaJugadores[i].puntosAcumuladosEnMano[m]);
            }
            outtextxy(colX + (anchoCol / 2), 210 + (m * 35), bufferPts);
        }
        int btnY = 720 - 130;
        rectangle(colX + 10, btnY, colX + anchoCol - 10, btnY + 40);
        outtextxy(colX + (anchoCol / 2), btnY + 20, miPartida.listaJugadores[i].yaAnoto ? (char*)"EDITAR" : (char*)"ANADIR PUNTOS");
    }
    if (mostrandoErrorContinental) {
        setcolor(LIGHTRED);
        rectangle(1280/2 - 350, 720 - 80, 1280/2 + 350, 720 - 20);
        outtextxy(1280/2, 720 - 50, (char*)"ERROR: SOLO 1 JUGADOR PUEDE TENER 5 PUNTOS O MENOS");
        setcolor(WHITE);
    }
}

void pantallaMesaJuego() {
    bool terminarMesa = false;
    int clickX, clickY;
    while (!terminarMesa) {
        redibujarFondoMesaContinental();
        actualizarPantalla(); 
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            mostrandoErrorContinental = false;
            // Botón Siguiente Mano
            if (clickX >= 1280 - 280 && clickX <= 1280 - 50 && clickY >= 70 && clickY <= 120) {
                bool todosOk = true; 
                int contadorBajo5 = 0;
                for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                    if (!miPartida.listaJugadores[i].yaAnoto) {
                        todosOk = false;
                    } else {
                        int p = miPartida.listaJugadores[i].puntosPorMano[miPartida.manoActual - 1];
                        if (p < -100 || p > 500) todosOk = false;
                        if (p <= 5) contadorBajo5++;
                    }
                }
                // NUEVA CONDICION: Exactamente uno debe tener 5 o menos
                if (contadorBajo5 != 1) todosOk = false;

                if (todosOk) {
                    if (miPartida.manoActual == 7) { terminarMesa = true; pantallaResultadosFinales(); }
                    else { for(int i=0; i<miPartida.cantidadJugadores; i++) miPartida.listaJugadores[i].yaAnoto = false; miPartida.manoActual++; }
                } else mostrandoErrorContinental = true;
            }
            // Botones de añadir puntos
            if (miPartida.cantidadJugadores > 0) {
                int btnY = 720 - 130, anchoCol = 1180 / miPartida.cantidadJugadores;
                for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                    int colX = 50 + (i * anchoCol);
                    if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) ventanaEmergentePuntos(i); 
                }
            }
        }
        delay(20);
    }
}

void ventanaEmergentePuntos(int i) {
    char bufferEntrada[20] = "";
    int pos = 0, cx, cy;
    bool continuar = true;
    clearmouseclick(WM_LBUTTONDOWN);
    if (miPartida.listaJugadores[i].yaAnoto) {
        sprintf(bufferEntrada, "%d", miPartida.listaJugadores[i].puntosPorMano[miPartida.manoActual - 1]);
        pos = strlen(bufferEntrada);
    }
    while(continuar) {
        redibujarFondoMesaContinental();
        setcolor(WHITE); rectangle(250, 500, 1030, 690); 
        setfillstyle(SOLID_FILL, BLACK); bar(251, 501, 1029, 689); 
        rectangle(650, 580, 850, 630); 
        settextjustify(CENTER_TEXT, CENTER_TEXT); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 605, (char*)"SIGUIENTE"); 
        char bufferMensaje[100];
        sprintf(bufferMensaje, "PUNTOS JUGADOR - %s: ", miPartida.listaJugadores[i].nombre); 
        settextjustify(LEFT_TEXT, CENTER_TEXT); outtextxy(280, 560, bufferMensaje); 
        outtextxy(280 + textwidth(bufferMensaje), 560, bufferEntrada);
        outtextxy(280 + textwidth(bufferMensaje) + textwidth(bufferEntrada), 560, (char*)"_");
        actualizarPantalla();
        if (kbhit()) {
            char tecla = getch();
            if (tecla == 13) continuar = false; 
            else if (tecla == 8 && pos > 0) { pos--; bufferEntrada[pos] = '\0'; }
            else if ((tecla >= '0' && tecla <= '9') || (tecla == '-' && pos == 0)) {
                if (pos < 5) { bufferEntrada[pos] = tecla; pos++; bufferEntrada[pos] = '\0'; }
            }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, cx, cy);
            if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) continuar = false;
        }
        delay(20);
    }
    int ptsInput = 0;
    if (strlen(bufferEntrada) > 0) sscanf(bufferEntrada, "%d", &ptsInput);
    int m = miPartida.manoActual - 1;
    miPartida.listaJugadores[i].puntosPorMano[m] = ptsInput;
    if (m == 0) miPartida.listaJugadores[i].puntosAcumuladosEnMano[m] = ptsInput;
    else miPartida.listaJugadores[i].puntosAcumuladosEnMano[m] = miPartida.listaJugadores[i].puntosAcumuladosEnMano[m-1] + ptsInput;
    miPartida.listaJugadores[i].yaAnoto = true;
    clearmouseclick(WM_LBUTTONDOWN);
}

void pantallaResultadosFinales() {
    cleardevice(); prepararPantallaJuego("RESULTADOS FINALES - CONTINENTAL");
    for (int i = 0; i < miPartida.cantidadJugadores; i++) miPartida.listaJugadores[i].puntosTotales = miPartida.listaJugadores[i].puntosAcumuladosEnMano[6];
    int indiceGanador = 0, menorPuntuacion = miPartida.listaJugadores[0].puntosTotales;
    for (int i = 1; i < miPartida.cantidadJugadores; i++) {
        if (miPartida.listaJugadores[i].puntosTotales < menorPuntuacion) {
            menorPuntuacion = miPartida.listaJugadores[i].puntosTotales; indiceGanador = i;
        }
    }
    guardarEnHistorial("Continental", miPartida.listaJugadores[indiceGanador].nombre);
    int anchoCol = 1180 / (miPartida.cantidadJugadores > 0 ? miPartida.cantidadJugadores : 1);
    line(50, 120, 1280 - 50, 120);
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 150, miPartida.listaJugadores[i].nombre);
        char bufferTotales[50]; sprintf(bufferTotales, "Total: %d pts", miPartida.listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoCol / 2), 200, bufferTotales);
        if (i == indiceGanador) { setcolor(LIGHTGREEN); outtextxy(colX + (anchoCol / 2), 260, (char*)"GANADOR!!!"); setcolor(WHITE); }
        else {
            char bufferResta[50]; sprintf(bufferResta, "%d - %d = %d", miPartida.listaJugadores[i].puntosTotales, menorPuntuacion, miPartida.listaJugadores[i].puntosTotales - menorPuntuacion);
            outtextxy(colX + (anchoCol / 2), 260, bufferResta);
        }
        if(i > 0) line(colX, 120, colX, 320);
    }
    line(50, 320, 1280 - 50, 320); actualizarPantalla(); clearmouseclick(WM_LBUTTONDOWN); getch();
}
