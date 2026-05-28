#include <estructuras.h>

// =================================================================
// FUNCIONES GENERALES Y MENÚ
// =================================================================

void actualizarPantalla() {
    static int pagina = 0;
    setvisualpage(pagina);    // Muestra la página actual
    pagina = 1 - pagina;     // Cambia 0 a 1 o 1 a 0
    setactivepage(pagina);    // Dibuja en la página oculta
}

void dibujarMenu() {
    cleardevice();
    int mitadX = 1280 / 2;
    int mitadY = 720 / 2;
    int medioBotonX = 170;
    int medioBotonY = 22; 
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(mitadX, mitadY - 200, (char*)"BARAJA ESPANOLA");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    rectangle(mitadX - medioBotonX, mitadY - 110 - medioBotonY, mitadX + medioBotonX, mitadY - 110 + medioBotonY);
    outtextxy(mitadX, mitadY - 110, (char*)"CONTINENTAL");
    rectangle(mitadX - medioBotonX, mitadY - 50 - medioBotonY, mitadX + medioBotonX, mitadY - 50 + medioBotonY);
    outtextxy(mitadX, mitadY - 50, (char*)"CHINCHON");
    rectangle(mitadX - medioBotonX, mitadY + 10 - medioBotonY, mitadX + medioBotonX, mitadY + 10 + medioBotonY);
    outtextxy(mitadX, mitadY + 10, (char*)"TUTE");
    rectangle(mitadX - medioBotonX, mitadY + 130 - medioBotonY, mitadX + medioBotonX, mitadY + 130 + medioBotonY);
    outtextxy(mitadX, mitadY + 130, (char*)"HISTORIAL");
    rectangle(mitadX - medioBotonX, mitadY + 190 - medioBotonY, mitadX + medioBotonX, mitadY + 190 + medioBotonY);
    outtextxy(mitadX, mitadY + 190, (char*)"SALIR");
    actualizarPantalla(); 
}

int controlarMenu() {
    int clickX, clickY;
    int mitadX = 1280 / 2;
    int mitadY = 720 / 2;
    int medioBotonX = 170;
    int medioBotonY = 22;
    clearmouseclick(WM_LBUTTONDOWN); // Limpiar clics previos
    while(1) {
        if (kbhit()) { if (getch() == 27) return 6; }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= (mitadX - medioBotonX) && clickX <= (mitadX + medioBotonX)) {
                if (clickY >= (mitadY - 110 - medioBotonY) && clickY <= (mitadY - 110 + medioBotonY)) return 1;
                if (clickY >= (mitadY - 50 - medioBotonY) && clickY <= (mitadY - 50 + medioBotonY)) return 2;
                if (clickY >= (mitadY + 10 - medioBotonY) && clickY <= (mitadY + 10 + medioBotonY)) return 3;
                if (clickY >= (mitadY + 130 - medioBotonY) && clickY <= (mitadY + 130 + medioBotonY)) return 5; 
                if (clickY >= (mitadY + 190 - medioBotonY) && clickY <= (mitadY + 190 + medioBotonY)) return 6; 
            }
        }
        delay(20); // Mayor respiro para el procesador
    }
}

void ejecutarJuego(int opcionSeleccionada) {
    switch(opcionSeleccionada) {
        case 1: jugarContinental(); break;
        case 2: jugarChinchon();    break; 
        case 3: jugarTute();        break;
        case 5: mostrarPantallaHistorial(); break; 
    }
}

void guardarEnHistorial(const char* juego, const char* ganador) {
    FILE *fichero = fopen("historial.txt", "a");
    if (fichero != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        // Usamos guiones para evitar problemas con fscanf de nombres con espacios
        fprintf(fichero, "%s %s %02d/%02d/%04d\n", juego, ganador, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        fclose(fichero);
    }
}

void mostrarPantallaHistorial() {
    cleardevice();
    prepararPantallaJuego("HISTORIAL DE PARTIDAS");
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    FILE *fichero = fopen("historial.txt", "r");
    if (fichero == NULL) {
        outtextxy(1280 / 2, 250, (char*)"No hay partidas registradas todavia.");
    } else {
        struct EntradaHistorial registro;
        int filaY = 150, contador = 1;
        char bufferLinea[150];
        setcolor(LIGHTBLUE);
        outtextxy(1280 / 2, 110, (char*)"N.   -   JUEGO   -   GANADOR   -   FECHA(Hoy)");
        setcolor(WHITE);
        while (fscanf(fichero, "%29s %29s %14s", registro.juego, registro.ganador, registro.fecha) != EOF && contador <= 15) {
            sprintf(bufferLinea, "%d.   %s   ->   %s   [%s]", contador, registro.juego, registro.ganador, registro.fecha);
            outtextxy(1280 / 2, filaY, bufferLinea);
            filaY += 35; contador++;
        }
        fclose(fichero);
    }
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(1280 / 2, 680, (char*)"Mi madre me dio la vida y el proyecto de C las ganas de dejar de vivirla");
    outtextxy(1280 / 2, 700, (char*)"         -  Gracias Andres.");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(YELLOW);
    outtextxy(1280 / 2, 630, (char*)"Presiona cualquier tecla para volver al Menu Principal");
    setcolor(WHITE);
    actualizarPantalla();
    clearmouseclick(WM_LBUTTONDOWN);
    getch(); 
}

void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, int pantallaOrigen) {
    int pos = strlen(resultado);
    char tecla;
    clearmouseclick(WM_LBUTTONDOWN); // Evitar clics fantasmas mientras escribes
    while (1) {
        if (pantallaOrigen == 1) redibujarFondoConfigurarContinental();
        else if (pantallaOrigen == 2) redibujarFondoConfigurarChinchon();
        else if (pantallaOrigen == 3) redibujarFondoConfigurarTute();
        else if (pantallaOrigen == 4) redibujarFondoMesaTute();
        else if (pantallaOrigen == 5) redibujarFondoEditarEquipo();
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(x, y, resultado);
        outtextxy(x + textwidth(resultado), y, (char*)"_"); 
        actualizarPantalla();
        if (kbhit()) {
            tecla = getch(); 
            if (tecla == 13) break;
            else if (tecla == 8 && pos > 0) { pos--; resultado[pos] = '\0'; }
            else if (tecla >= 32 && tecla <= 126 && pos < maxLongitud) {
                resultado[pos] = tecla; pos++; resultado[pos] = '\0';
            }
        }
        delay(20); 
    }
}

void prepararPantallaJuego(const char* nombreDelJuego) {
    cleardevice();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(1280 / 2, 35, (char*)nombreDelJuego);
}
