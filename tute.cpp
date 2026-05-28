#include <estructuras.h>

// =================================================================
// JUEGO 3: TUTE 
// =================================================================

void jugarTute() {
    miPartidaTute.cantidadManos = 10; miPartidaTute.precioPunto = 0.10;
    miPartidaTute.manoActual = 0; miPartidaTute.tantosEq1 = 0; miPartidaTute.tantosEq2 = 0;
    for(int i=0; i<100; i++) { miPartidaTute.puntosEq1[i] = 0; miPartidaTute.puntosEq2[i] = 0; miPartidaTute.triunfoEq1[i] = 0; miPartidaTute.triunfoEq2[i] = 0; }
    strcpy(miPartidaTute.equipo1[0].nombre, "JUGADOR 1"); strcpy(miPartidaTute.equipo1[1].nombre, "JUGADOR 2");
    strcpy(miPartidaTute.equipo2[0].nombre, "JUGADOR 3"); strcpy(miPartidaTute.equipo2[1].nombre, "JUGADOR 4");
    tuteVolverMenu = false; pantallaConfigurarTute();
    if (!tuteVolverMenu) pantallaMesaTute();
}

void redibujarFondoConfigurarTute() {
    cleardevice(); int mX = 1280/2; prepararPantallaJuego("TUTE");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); setcolor(WHITE);
    outtextxy(mX - 100, 165, (char*)"CANTIDAD MANOS:"); char bC[10]; sprintf(bC, "%d", miPartidaTute.cantidadManos);
    outtextxy(mX + 120, 165, bC); rectangle(mX + 90, 145, mX + 150, 185); outtextxy(mX + 220, 165, (char*)"EDITAR");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3); outtextxy(mX - 250, 250, (char*)"EQUIPO 1"); outtextxy(mX + 250, 250, (char*)"EQUIPO 2");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); outtextxy(mX - 250, 300, miPartidaTute.equipo1[0].nombre); outtextxy(mX - 250, 330, miPartidaTute.equipo1[1].nombre);
    rectangle(mX - 350, 350, mX - 150, 390); outtextxy(mX - 250, 370, (char*)"EDITAR");
    outtextxy(mX + 250, 300, miPartidaTute.equipo2[0].nombre); outtextxy(mX + 250, 330, miPartidaTute.equipo2[1].nombre);
    rectangle(mX + 150, 350, mX + 350, 390); outtextxy(mX + 250, 370, (char*)"EDITAR");
    rectangle(mX - 100, 580, mX + 100, 620); outtextxy(mX, 600, (char*)"JUGAR");
    rectangle(100, 720 - 100, 300, 720 - 50); outtextxy(200, 720 - 75, (char*)"SALIR");
    if(tuteEditandoManos) { setcolor(LIGHTBLUE); outtextxy(mX, 220, (char*)"NUEVA CANTIDAD:"); setcolor(WHITE); }
}

void pantallaConfigurarTute() {
    bool salirC = false; int x, y;
    while(!salirC) {
        redibujarFondoConfigurarTute(); actualizarPantalla();
        if(ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x >= 1280/2-350 && x <= 1280/2-150 && y >= 350 && y <= 390) { tuteNumEquipoEditando = 1; pantallaEditarEquipo(miPartidaTute.equipo1, 1); }
            if(x >= 1280/2+150 && x <= 1280/2+350 && y >= 350 && y <= 390) { tuteNumEquipoEditando = 2; pantallaEditarEquipo(miPartidaTute.equipo2, 2); }
            if(x >= 1280/2-100 && x <= 1280/2+100 && y >= 580 && y <= 620) salirC = true;
            if(x >= 100 && x <= 300 && y >= 620 && y <= 670) { tuteVolverMenu = true; salirC = true; }
            if(x >= 1280/2+90 && x <= 1280/2+150 && y >= 145 && y <= 185) {
                char bC[10] = ""; tuteEditandoManos = true; leerTextoGrafico(1280/2-40, 220, bC, 5, 3);
                tuteEditandoManos = false; if(strlen(bC) > 0) sscanf(bC, "%d", &miPartidaTute.cantidadManos);
                if(miPartidaTute.cantidadManos > 100) miPartidaTute.cantidadManos = 100; 
            }
        }
        delay(20);
    }
}

void redibujarFondoEditarEquipo() {
    cleardevice(); int mX = 1280/2; struct JugadorTute *eq = (tuteNumEquipoEditando == 1) ? miPartidaTute.equipo1 : miPartidaTute.equipo2;
    settextjustify(CENTER_TEXT, CENTER_TEXT); setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char t[30]; sprintf(t, "EQUIPO %d", tuteNumEquipoEditando); outtextxy(mX, 100, t);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); outtextxy(mX - 100, 250, eq[0].nombre); rectangle(mX + 50, 225, mX + 200, 275);
    outtextxy(mX - 100, 350, eq[1].nombre); rectangle(mX + 50, 325, mX + 200, 375);
    rectangle(mX - 100, 500, mX + 100, 550); outtextxy(mX, 525, (char*)"VOLVER");
}

void pantallaEditarEquipo(struct JugadorTute equipo[2], int nE) {
    bool sE = false; int x, y;
    while(!sE) {
        redibujarFondoEditarEquipo(); actualizarPantalla();
        if(ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x >= 1280/2+50 && x <= 1280/2+200 && y >= 225 && y <= 275) { char b[30]=""; leerTextoGrafico(1280/2+250, 250, b, 25, 5); if(strlen(b)>0) strcpy(equipo[0].nombre, b); }
            if(x >= 1280/2+50 && x <= 1280/2+200 && y >= 325 && y <= 375) { char b[30]=""; leerTextoGrafico(1280/2+250, 350, b, 25, 5); if(strlen(b)>0) strcpy(equipo[1].nombre, b); }
            if(x >= 1280/2-100 && x <= 1280/2+100 && y >= 500 && y <= 550) sE = true;
        }
        delay(20);
    }
}

void redibujarFondoMesaTute() {
    cleardevice(); int mX = 1280/2; prepararPantallaJuego("TUTE");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); setcolor(WHITE);
    outtextxy(mX - 450, 110, miPartidaTute.equipo1[0].nombre); outtextxy(mX - 450, 130, miPartidaTute.equipo1[1].nombre);
    outtextxy(mX + 450, 110, miPartidaTute.equipo2[0].nombre); outtextxy(mX + 450, 130, miPartidaTute.equipo2[1].nombre);
    char t1[30] = "TANTOS: ", t2[30] = "TANTOS: ";
    for(int i=0; i<miPartidaTute.tantosEq1; i++) strcat(t1, "I"); for(int i=0; i<miPartidaTute.tantosEq2; i++) strcat(t2, "I");
    settextjustify(RIGHT_TEXT, CENTER_TEXT); outtextxy(mX - 450, 150, t1);
    settextjustify(LEFT_TEXT, CENTER_TEXT); outtextxy(mX + 450, 150, t2);
    settextjustify(CENTER_TEXT, CENTER_TEXT); line(mX, 180, mX, 600);
    outtextxy(mX - 350, 200, (char*)"PUNTOS"); outtextxy(mX - 150, 200, (char*)"TRIUNFO");
    outtextxy(mX + 150, 200, (char*)"PUNTOS"); outtextxy(mX + 350, 200, (char*)"TRIUNFO");
    line(50, 230, 1280 - 50, 230);
    int mIni = (miPartidaTute.manoActual > 12) ? miPartidaTute.manoActual - 12 : 0;
    for(int i=mIni; i <= miPartidaTute.manoActual; i++) {
        int yF = 260 + ((i-mIni) * 32);
        if(miPartidaTute.puntosEq1[i] > 0 || miPartidaTute.puntosEq2[i] > 0) {
            char p1[10], p2[10]; sprintf(p1, "%d", miPartidaTute.puntosEq1[i]); sprintf(p2, "%d", miPartidaTute.puntosEq2[i]);
            outtextxy(mX - 350, yF, p1); outtextxy(mX + 150, yF, p2);
        }
        if (miPartidaTute.triunfoEq1[i] == 1) outtextxy(mX - 150, yF, (char*)"X");
        if (miPartidaTute.triunfoEq2[i] == 1) outtextxy(mX + 350, yF, (char*)"X");
    }
    rectangle(mX - 100, 640, mX + 100, 690); outtextxy(mX, 665, (char*)"ANADIR PUNTOS");
}

void pantallaMesaTute() {
    bool fin = false; int x, y;
    while(!fin) {
        redibujarFondoMesaTute(); actualizarPantalla();
        if(ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x >= 1280/2-100 && x <= 1280/2+100 && y >= 640 && y <= 690) {
                ventanaEmergentePuntosTute();
                if(miPartidaTute.tantosEq1 >= miPartidaTute.cantidadManos || miPartidaTute.tantosEq2 >= miPartidaTute.cantidadManos) {
                    cleardevice(); outtextxy(1280/2, 350, (char*)"PARTIDA FINALIZADA"); actualizarPantalla();
                    guardarEnHistorial("Tute", miPartidaTute.tantosEq1 > miPartidaTute.tantosEq2 ? "EQUIPO 1" : "EQUIPO 2");
                    delay(2000); fin = true;
                }
            }
        }
        delay(20);
    }
}

void ventanaEmergentePuntosTute() {
    char b1[10] = "", b2[10] = ""; int f1 = 0, f2 = 0, foco = 1, cx, cy; bool cont = true;
    clearmouseclick(WM_LBUTTONDOWN);
    while(cont) {
        redibujarFondoMesaTute(); setcolor(WHITE); rectangle(250, 450, 1030, 680); setfillstyle(SOLID_FILL, BLACK); bar(251, 451, 1029, 679);
        settextjustify(CENTER_TEXT, CENTER_TEXT); outtextxy(1280/2, 480, (char*)"INTRODUCIR PUNTOS DE LA MANO");
        settextjustify(LEFT_TEXT, CENTER_TEXT); outtextxy(300, 530, (char*)"EQUIPO 1:"); rectangle(450, 510, 550, 550); outtextxy(460, 530, b1);
        outtextxy(600, 530, (char*)"EQUIPO 2:"); rectangle(750, 510, 850, 550); outtextxy(760, 530, b2);
        outtextxy(300, 580, (char*)"TRIUNFO:"); rectangle(450, 560, 550, 600); if(miPartidaTute.triunfoEq1[miPartidaTute.manoActual]) outtextxy(490, 580, (char*)"X");
        rectangle(750, 560, 850, 600); if(miPartidaTute.triunfoEq2[miPartidaTute.manoActual]) outtextxy(790, 580, (char*)"X");
        settextjustify(CENTER_TEXT, CENTER_TEXT); rectangle(1280/2-100, 620, 1280/2+100, 660); outtextxy(1280/2, 640, (char*)"SIGUIENTE");
        if(foco==1) outtextxy(500, 500, (char*)"*"); else outtextxy(800, 500, (char*)"*");
        actualizarPantalla();
        if(kbhit()) {
            char t = getch();
            if(t==13) cont = false; else if(t==9) foco = (foco==1)?2:1;
            else if(t==8) { if(foco==1 && f1>0) b1[--f1]='\0'; if(foco==2 && f2>0) b2[--f2]='\0'; }
            else if(t>='0' && t<='9') { if(foco==1 && f1<3) { b1[f1++]=t; b1[f1]='\0'; } if(foco==2 && f2<3) { b2[f2++]=t; b2[f2]='\0'; } }
        }
        if(ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, cx, cy);
            if(cx>=450 && cx<=550 && cy>=510 && cy<=550) foco=1; if(cx>=750 && cx<=850 && cy>=510 && cy<=550) foco=2;
            if(cx>=450 && cx<=550 && cy>=560 && cy<=600) { miPartidaTute.triunfoEq1[miPartidaTute.manoActual]=1; miPartidaTute.triunfoEq2[miPartidaTute.manoActual]=0; }
            if(cx>=750 && cx<=850 && cy>=560 && cy<=600) { miPartidaTute.triunfoEq2[miPartidaTute.manoActual]=1; miPartidaTute.triunfoEq1[miPartidaTute.manoActual]=0; }
            if(cx>=1280/2-100 && cx<=1280/2+100 && cy>=620 && cy<=660) {
                int p1=0, p2=0; sscanf(b1, "%d", &p1); sscanf(b2, "%d", &p2);
                if(p1+p2 >= 130 && p1+p2 <= 230) {
                    miPartidaTute.puntosEq1[miPartidaTute.manoActual]=p1; miPartidaTute.puntosEq2[miPartidaTute.manoActual]=p2;
                    if(p1 > p2 || (p1==p2 && miPartidaTute.triunfoEq1[miPartidaTute.manoActual])) miPartidaTute.tantosEq1++; else miPartidaTute.tantosEq2++;
                    miPartidaTute.manoActual++; cont = false;
                }
            }
        }
        delay(20);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}
