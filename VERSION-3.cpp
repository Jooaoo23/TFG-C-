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

// --- PROTOTIPOS DE LAS FUNCIONES ---
void iniciarModoGrafico();
void actualizarPantalla(); 
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);
void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, int pantallaOrigen);

void guardarEnHistorial(const char* juego, const char* ganador);
void mostrarPantallaHistorial();

// Módulo 1: Continental
void jugarContinental();
void prepararPantallaJuego(const char* nombreDelJuego);
void pantallaConfigurarJugadores();
void redibujarFondoConfigurarContinental(); 
void pantallaMesaJuego();
void redibujarFondoMesaContinental(); 
void ventanaEmergentePuntos(int i);
void pantallaResultadosFinales();

// Módulo 2: Chinchón
void jugarChinchon();
void pantallaConfigurarJugadoresChinchon();
void redibujarFondoConfigurarChinchon(); 
void pantallaMesaChinchon();
void redibujarFondoMesaChinchon(); 
void ventanaEmergentePuntosChinchon(int i);
void pantallaResultadosChinchon();

void jugarTute() {}

// Variables globales (Hacen el código más fácil de entender y defender)
struct PartidaContinental miPartida;
struct PartidaChinchon miPartidaChinchon; 
int paginaActual = 0; 

bool mostrandoErrorContinental = false;
bool mostrandoErrorChinchon = false;

// =================================================================
// FLUJO PRINCIPAL
// =================================================================
int main() {
    int opcionMenu = 0;
    iniciarModoGrafico();
    
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

void iniciarModoGrafico() {
    initwindow(1280, 720, (char*)"Baraja Espanola", 0, 0, false, true);
    setactivepage(0);
    setvisualpage(0);
}

void actualizarPantalla() {
    setvisualpage(paginaActual);       
    paginaActual = 1 - paginaActual;   
    setactivepage(paginaActual);       
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

    while(1) {
        if (kbhit()) {
            if (getch() == 27) return 6; 
        }
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
        delay(10);
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

// =================================================================
// PERSISTENCIA (FICHEROS)
// =================================================================

void guardarEnHistorial(const char* juego, const char* ganador) {
    FILE *fichero = fopen("historial.txt", "a");
    if (fichero != NULL) {
        fprintf(fichero, "%s %s\n", juego, ganador);
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
        int filaY = 150;
        int contador = 1;
        char bufferLinea[100];
        
        setcolor(LIGHTBLUE);
        outtextxy(1280 / 2, 110, (char*)"N.   -   JUEGO   -   GANADOR");
        setcolor(WHITE);
        
        while (fscanf(fichero, "%29s %29s", registro.juego, registro.ganador) != EOF && contador <= 15) {
            sprintf(bufferLinea, "%d.   %s   ->   %s", contador, registro.juego, registro.ganador);
            outtextxy(1280 / 2, filaY, bufferLinea);
            filaY += 35;
            contador++;
        }
        fclose(fichero);
    }
    
    setcolor(YELLOW);
    outtextxy(1280 / 2, 650, (char*)"Presiona cualquier tecla para volver al Menu Principal");
    setcolor(WHITE);
    
    actualizarPantalla();
    getch(); 
}

// =================================================================
// MODULO GRÁFICO (REDISEÑO SIMPLE SIN PUNTEROS COMPLEJOS)
// =================================================================

void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, int pantallaOrigen) {
    int pos = strlen(resultado);
    char tecla;

    while (1) {
        // En lugar de punteros a funciones, usamos un entero simple para redibujar el fondo
        if (pantallaOrigen == 1) redibujarFondoConfigurarContinental();
        else if (pantallaOrigen == 2) redibujarFondoConfigurarChinchon();

        settextjustify(LEFT_TEXT, CENTER_TEXT);
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(x, y, resultado);
        outtextxy(x + textwidth(resultado), y, (char*)"_"); 
        
        actualizarPantalla();

        if (kbhit()) {
            tecla = getch(); 

            if (tecla == 13) { // Enter
                break;
            } 
            else if (tecla == 8) { // Retroceso
                if (pos > 0) {
                    pos--;
                    resultado[pos] = '\0';
                }
            } 
            else if (tecla >= 32 && tecla <= 126) { 
                if (pos < maxLongitud) {
                    resultado[pos] = tecla;
                    pos++;
                    resultado[pos] = '\0';
                }
            }
        }
        delay(10); 
    }
}

void prepararPantallaJuego(const char* nombreDelJuego) {
    cleardevice();
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(1280 / 2, 35, (char*)nombreDelJuego);
}

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
    if (miPartida.cantidadJugadores > 0) {
        pantallaMesaJuego();
    }
}

void redibujarFondoConfigurarContinental() {
    cleardevice(); 
    prepararPantallaJuego("CONTINENTAL");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    rectangle(100, 100, 350, 150); 
    outtextxy(225, 125, (char*)"ANADIR JUGADOR");
    rectangle(1280 - 300, 720 - 100, 1280 - 100, 720 - 50);
    outtextxy(1280 - 200, 720 - 75, (char*)"JUGAR");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 720 - 75, (char*)"MAX: 7 JUGADORES");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int filaY = 180 + (i * 55); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        char bufferNombre[50];
        sprintf(bufferNombre, "%s:", miPartida.listaJugadores[i].nombre);
        outtextxy(100, filaY + 20, bufferNombre); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        rectangle(350, filaY, 480, filaY + 40);
        outtextxy(415, filaY + 20, (char*)"EDITAR");
        rectangle(510, filaY, 660, filaY + 40);
        outtextxy(585, filaY + 20, (char*)"ELIMINAR");
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
                    // Pasamos un '1' para avisar que venimos de la pantalla de Continental
                    leerTextoGrafico(390, 125, nuevoNombre, 25, 1);
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(miPartida.listaJugadores[miPartida.cantidadJugadores].nombre, nuevoNombre);
                        miPartida.listaJugadores[miPartida.cantidadJugadores].yaAnoto = false;
                        miPartida.cantidadJugadores++; 
                    }
                }
            }
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (miPartida.cantidadJugadores > 0) salirAjustes = true; 
            }
            for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                int filaY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    for (int j = i; j < miPartida.cantidadJugadores - 1; j++) {
                        strcpy(miPartida.listaJugadores[j].nombre, miPartida.listaJugadores[j + 1].nombre);
                    }
                    miPartida.cantidadJugadores--; 
                    break; 
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditado[30] = ""; 
                    strcpy(nombreEditado, miPartida.listaJugadores[i].nombre);
                    leerTextoGrafico(690, filaY + 20, nombreEditado, 25, 1);
                    if(strlen(nombreEditado) > 0) {
                        strcpy(miPartida.listaJugadores[i].nombre, nombreEditado);
                    }
                    break;
                }
            }
        }
        delay(10); 
    }
}

void redibujarFondoMesaContinental() {
    cleardevice();
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
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(1280 / 2, 35, (char*)"CONTINENTAL");
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(50, 90, bufferMano);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    rectangle(1280 - 280, 70, 1280 - 50, 120);
    outtextxy(1280 - 165, 95, (char*)"SIGUIENTE MANO");
    
    int anchoCol = 1180 / miPartida.cantidadJugadores;
    line(50, 140, 1280 - 50, 140); 
    line(50, 180, 1280 - 50, 180);
    
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 160, miPartida.listaJugadores[i].nombre);
        if(i > 0) line(colX, 140, colX, 720 - 150); 
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        
        for (int m = 0; m < miPartida.manoActual; m++) {
            char bufferPts[60];
            if (m == miPartida.manoActual - 1 && !miPartida.listaJugadores[i].yaAnoto) {
                sprintf(bufferPts, "-"); 
            } else {
                if (m == 0) {
                    sprintf(bufferPts, "%d", miPartida.listaJugadores[i].puntosPorMano[m]);
                } else {
                    sprintf(bufferPts, "%d - %d", miPartida.listaJugadores[i].puntosPorMano[m], miPartida.listaJugadores[i].puntosAcumuladosEnMano[m]);
                }
            }
            outtextxy(colX + (anchoCol / 2), 210 + (m * 35), bufferPts);
        }
        int btnY = 720 - 130;
        rectangle(colX + 10, btnY, colX + anchoCol - 10, btnY + 40);
        if (miPartida.listaJugadores[i].yaAnoto) {
            outtextxy(colX + (anchoCol / 2), btnY + 20, (char*)"EDITAR");
        } else {
            outtextxy(colX + (anchoCol / 2), btnY + 20, (char*)"ANADIR PUNTOS");
        }
    }
    if (mostrandoErrorContinental) {
        setcolor(LIGHTRED);
        rectangle(1280/2 - 250, 720 - 80, 1280/2 + 250, 720 - 20);
        outtextxy(1280/2, 720 - 50, (char*)"FALTAN JUGADORES POR PUNTUAR");
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
            if (clickX >= 1280 - 280 && clickX <= 1280 - 50 && clickY >= 70 && clickY <= 120) {
                bool todosOk = true;
                for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                    if (!miPartida.listaJugadores[i].yaAnoto) todosOk = false;
                }
                if (todosOk) {
                    if (miPartida.manoActual == 7) {
                        terminarMesa = true; 
                        pantallaResultadosFinales(); 
                    } else {
                        for(int i = 0; i < miPartida.cantidadJugadores; i++) miPartida.listaJugadores[i].yaAnoto = false;
                        miPartida.manoActual++; 
                    }
                } else mostrandoErrorContinental = true;
            }
            int btnY = 720 - 130;
            int anchoCol = 1180 / miPartida.cantidadJugadores;
            for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                int colX = 50 + (i * anchoCol);
                if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) {
                    ventanaEmergentePuntos(i); 
                }
            }
        }
        delay(10);
    }
}

void ventanaEmergentePuntos(int i) {
    char bufferEntrada[20] = "";
    int pos = 0;
    int cx, cy;
    bool continuar = true;

    clearmouseclick(WM_LBUTTONDOWN);

    if (miPartida.listaJugadores[i].yaAnoto) {
        sprintf(bufferEntrada, "%d", miPartida.listaJugadores[i].puntosPorMano[miPartida.manoActual - 1]);
        pos = strlen(bufferEntrada);
    }

    while(continuar) {
        redibujarFondoMesaContinental();

        setcolor(WHITE);
        rectangle(250, 500, 1030, 690); 
        setfillstyle(SOLID_FILL, BLACK);
        bar(251, 501, 1029, 689); 
        rectangle(650, 580, 850, 630); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 605, (char*)"SIGUIENTE"); 
        
        char bufferMensaje[100];
        sprintf(bufferMensaje, "PUNTOS JUGADOR - %s: ", miPartida.listaJugadores[i].nombre); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(280, 560, bufferMensaje); 
        
        int inicioEscribirX = 280 + textwidth(bufferMensaje);
        outtextxy(inicioEscribirX, 560, bufferEntrada);
        outtextxy(inicioEscribirX + textwidth(bufferEntrada), 560, (char*)"_");
        
        actualizarPantalla();

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 13) continuar = false; 
            else if (tecla == 8) { 
                if (pos > 0) {
                    pos--;
                    bufferEntrada[pos] = '\0';
                }
            }
            else if (tecla >= '0' && tecla <= '9' && pos < 5) { 
                bufferEntrada[pos] = tecla;
                pos++;
                bufferEntrada[pos] = '\0';
            }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, cx, cy);
            if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) {
                continuar = false; 
            }
        }
        delay(10);
    }

    int ptsInput = 0;
    if (strlen(bufferEntrada) > 0) {
        sscanf(bufferEntrada, "%d", &ptsInput); 
    }
    int m = miPartida.manoActual - 1;
    miPartida.listaJugadores[i].puntosPorMano[m] = ptsInput;
    if (miPartida.manoActual == 1) {
        miPartida.listaJugadores[i].puntosAcumuladosEnMano[0] = ptsInput; 
    } else {
        miPartida.listaJugadores[i].puntosAcumuladosEnMano[m] = miPartida.listaJugadores[i].puntosAcumuladosEnMano[m - 1] + ptsInput; 
    }
    miPartida.listaJugadores[i].yaAnoto = true;
    
    clearmouseclick(WM_LBUTTONDOWN);
}

void pantallaResultadosFinales() {
    cleardevice();
    prepararPantallaJuego("RESULTADOS FINALES - CONTINENTAL");
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        miPartida.listaJugadores[i].puntosTotales = miPartida.listaJugadores[i].puntosAcumuladosEnMano[6]; 
    }
    int indiceGanador = 0;
    int menorPuntuacion = miPartida.listaJugadores[0].puntosTotales;
    for (int i = 1; i < miPartida.cantidadJugadores; i++) {
        if (miPartida.listaJugadores[i].puntosTotales < menorPuntuacion) {
            menorPuntuacion = miPartida.listaJugadores[i].puntosTotales;
            indiceGanador = i;
        }
    }
    
    guardarEnHistorial("Continental", miPartida.listaJugadores[indiceGanador].nombre);
    
    int anchoCol = 1180 / miPartida.cantidadJugadores;
    line(50, 120, 1280 - 50, 120);
    for (int i = 0; i < miPartida.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 150, miPartida.listaJugadores[i].nombre);
        char bufferTotales[50];
        sprintf(bufferTotales, "Total: %d pts", miPartida.listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoCol / 2), 200, bufferTotales);
        if (i == indiceGanador) {
            setcolor(LIGHTGREEN);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"GANADOR!!!");
            setcolor(WHITE);
        } else {
            char bufferResta[50];
            int resta = miPartida.listaJugadores[i].puntosTotales - menorPuntuacion;
            sprintf(bufferResta, "%d - %d = %d", miPartida.listaJugadores[i].puntosTotales, menorPuntuacion, resta);
            outtextxy(colX + (anchoCol / 2), 260, bufferResta);
        }
        if(i > 0) line(colX, 120, colX, 320);
    }
    line(50, 320, 1280 - 50, 320);
    actualizarPantalla();
    getch(); 
}

// =================================================================
// JUEGO 2: CHINCHÓN 
// =================================================================

void jugarChinchon() {
    miPartidaChinchon.cantidadJugadores = 0;
    miPartidaChinchon.rondaActual = 1;      
    mostrandoErrorChinchon = false;
    
    for(int i = 0; i < 8; i++) {
        miPartidaChinchon.listaJugadores[i].puntosTotales = 0;
        miPartidaChinchon.listaJugadores[i].yaAnoto = false;
        for(int j = 0; j < 100; j++) {
            miPartidaChinchon.listaJugadores[i].puntosPorRonda[j] = 0;
            miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[j] = 0;
        }
    }
    
    pantallaConfigurarJugadoresChinchon();
    
    if (miPartidaChinchon.cantidadJugadores > 0) {
        pantallaMesaChinchon();
    }
}

void redibujarFondoConfigurarChinchon() {
    cleardevice(); 
    prepararPantallaJuego("CHINCHON");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    rectangle(100, 100, 350, 150); 
    outtextxy(225, 125, (char*)"ANADIR JUGADOR");
    rectangle(1280 - 300, 720 - 100, 1280 - 100, 720 - 50);
    outtextxy(1280 - 200, 720 - 75, (char*)"JUGAR");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 720 - 75, (char*)"MAX: 7 JUGADORES (LIMITE: 100 PTS)");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int filaY = 180 + (i * 55); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        char bufferNombre[50];
        sprintf(bufferNombre, "%s:", miPartidaChinchon.listaJugadores[i].nombre);
        outtextxy(100, filaY + 20, bufferNombre); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        rectangle(350, filaY, 480, filaY + 40);
        outtextxy(415, filaY + 20, (char*)"EDITAR");
        rectangle(510, filaY, 660, filaY + 40);
        outtextxy(585, filaY + 20, (char*)"ELIMINAR");
    }
}

void pantallaConfigurarJugadoresChinchon() {
    bool salirAjustes = false;
    int clickX, clickY;
    
    while (!salirAjustes) {
        redibujarFondoConfigurarChinchon();
        actualizarPantalla();

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (miPartidaChinchon.cantidadJugadores < 7) {
                    char nuevoNombre[30] = "";
                    // Pasamos un '2' para avisar que venimos de la pantalla de Chinchón
                    leerTextoGrafico(390, 125, nuevoNombre, 25, 2);
                    
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(miPartidaChinchon.listaJugadores[miPartidaChinchon.cantidadJugadores].nombre, nuevoNombre);
                        miPartidaChinchon.listaJugadores[miPartidaChinchon.cantidadJugadores].yaAnoto = false;
                        miPartidaChinchon.cantidadJugadores++; 
                    }
                }
            }
            
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (miPartidaChinchon.cantidadJugadores > 0) salirAjustes = true; 
            }
            
            for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                int filaY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    for (int j = i; j < miPartidaChinchon.cantidadJugadores - 1; j++) {
                        strcpy(miPartidaChinchon.listaJugadores[j].nombre, miPartidaChinchon.listaJugadores[j + 1].nombre);
                    }
                    miPartidaChinchon.cantidadJugadores--; 
                    break; 
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditado[30] = ""; 
                    strcpy(nombreEditado, miPartidaChinchon.listaJugadores[i].nombre);
                    leerTextoGrafico(690, filaY + 20, nombreEditado, 25, 2);
                    if(strlen(nombreEditado) > 0) {
                        strcpy(miPartidaChinchon.listaJugadores[i].nombre, nombreEditado);
                    }
                    break;
                }
            }
        }
        delay(10); 
    }
}

void redibujarFondoMesaChinchon() {
    cleardevice();
    char bufferRonda[100];
    sprintf(bufferRonda, "RONDA ACTIVA: %d (HASTA QUE ALGUIEN LLEGUE A 100)", miPartidaChinchon.rondaActual);
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    outtextxy(1280 / 2, 35, (char*)"CHINCHON");
    
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(50, 90, bufferRonda);
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    rectangle(1280 - 280, 70, 1280 - 50, 120);
    outtextxy(1280 - 165, 95, (char*)"SIGUIENTE RONDA");
    
    int anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
    line(50, 140, 1280 - 50, 140); 
    line(50, 180, 1280 - 50, 180);
    
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 160, miPartidaChinchon.listaJugadores[i].nombre);
        if(i > 0) line(colX, 140, colX, 720 - 150); 
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        
        int inicioMuestra = 0;
        if (miPartidaChinchon.rondaActual > 12) inicioMuestra = miPartidaChinchon.rondaActual - 12;
        
        int contadorLineas = 0;
        for (int m = inicioMuestra; m < miPartidaChinchon.rondaActual; m++) {
            char bufferPts[40];
            if (m == miPartidaChinchon.rondaActual - 1 && !miPartidaChinchon.listaJugadores[i].yaAnoto) {
                sprintf(bufferPts, "-"); 
            } else {
                if (m == 0) {
                    sprintf(bufferPts, "%d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[m]);
                } else {
                    sprintf(bufferPts, "%d - %d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[m], miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m]);
                }
            }
            outtextxy(colX + (anchoCol / 2), 210 + (contadorLineas * 32), bufferPts);
            contadorLineas++;
        }
        
        int btnY = 720 - 130;
        rectangle(colX + 10, btnY, colX + anchoCol - 10, btnY + 40);
        if (miPartidaChinchon.listaJugadores[i].yaAnoto) {
            outtextxy(colX + (anchoCol / 2), btnY + 20, (char*)"EDITAR");
        } else {
            outtextxy(colX + (anchoCol / 2), btnY + 20, (char*)"ANADIR PUNTOS");
        }
    }
    
    if (mostrandoErrorChinchon) {
        setcolor(LIGHTRED);
        rectangle(1280/2 - 250, 720 - 80, 1280/2 + 250, 720 - 20);
        outtextxy(1280/2, 720 - 50, (char*)"FALTAN JUGADORES POR PUNTUAR");
        setcolor(WHITE);
    }
}

void pantallaMesaChinchon() {
    bool terminarMesa = false;
    int clickX, clickY;
    
    while (!terminarMesa) {
        redibujarFondoMesaChinchon();
        actualizarPantalla();

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            mostrandoErrorChinchon = false;
            
            if (clickX >= 1280 - 280 && clickX <= 1280 - 50 && clickY >= 70 && clickY <= 120) {
                bool todosOk = true;
                for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                    if (!miPartidaChinchon.listaJugadores[i].yaAnoto) todosOk = false;
                }
                
                if (todosOk) {
                    bool alguienPerdio = false;
                    int rIndex = miPartidaChinchon.rondaActual - 1;
                    
                    for(int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                        if(miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[rIndex] >= 100) {
                            alguienPerdio = true;
                        }
                    }
                    
                    if (alguienPerdio) {
                        terminarMesa = true; 
                        pantallaResultadosChinchon(); 
                    } else {
                        for(int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) miPartidaChinchon.listaJugadores[i].yaAnoto = false;
                        miPartidaChinchon.rondaActual++; 
                    }
                } else mostrandoErrorChinchon = true;
            }
            
            int btnY = 720 - 130;
            int anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
            for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                int colX = 50 + (i * anchoCol);
                if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) {
                    ventanaEmergentePuntosChinchon(i); 
                }
            }
        }
        delay(10);
    }
}

void ventanaEmergentePuntosChinchon(int i) {
    char bufferEntrada[20] = "";
    int pos = 0;
    int cx, cy;
    bool continuar = true;

    clearmouseclick(WM_LBUTTONDOWN);

    if (miPartidaChinchon.listaJugadores[i].yaAnoto) {
        sprintf(bufferEntrada, "%d", miPartidaChinchon.listaJugadores[i].puntosPorRonda[miPartidaChinchon.rondaActual - 1]);
        pos = strlen(bufferEntrada);
    }

    while(continuar) {
        redibujarFondoMesaChinchon();

        setcolor(WHITE);
        rectangle(250, 500, 1030, 690); 
        setfillstyle(SOLID_FILL, BLACK);
        bar(251, 501, 1029, 689); 
        rectangle(650, 580, 850, 630); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 605, (char*)"SIGUIENTE"); 
        
        char bufferMensaje[100];
        sprintf(bufferMensaje, "PUNTOS JUGADOR - %s: ", miPartidaChinchon.listaJugadores[i].nombre); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(280, 560, bufferMensaje); 
        
        int inicioEscribirX = 280 + textwidth(bufferMensaje);
        outtextxy(inicioEscribirX, 560, bufferEntrada);
        outtextxy(inicioEscribirX + textwidth(bufferEntrada), 560, (char*)"_");
        
        actualizarPantalla();

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 13) continuar = false; 
            else if (tecla == 8) { 
                if (pos > 0) {
                    pos--;
                    bufferEntrada[pos] = '\0';
                }
            }
            else if (tecla >= '0' && tecla <= '9' && pos < 5) { 
                bufferEntrada[pos] = tecla;
                pos++;
                bufferEntrada[pos] = '\0';
            }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, cx, cy);
            if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) {
                continuar = false; 
            }
        }
        delay(10);
    }

    int ptsInput = 0;
    if (strlen(bufferEntrada) > 0) {
        sscanf(bufferEntrada, "%d", &ptsInput); 
    }
    int m = miPartidaChinchon.rondaActual - 1;
    miPartidaChinchon.listaJugadores[i].puntosPorRonda[m] = ptsInput;
    if (miPartidaChinchon.rondaActual == 1) {
        miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[0] = ptsInput; 
    } else {
        miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m] = miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[m - 1] + ptsInput; 
    }
    miPartidaChinchon.listaJugadores[i].yaAnoto = true;
    
    clearmouseclick(WM_LBUTTONDOWN);
}

void pantallaResultadosChinchon() {
    cleardevice();
    prepararPantallaJuego("RESULTADOS - CHINCHON");
    
    int rIndex = miPartidaChinchon.rondaActual - 1;
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        miPartidaChinchon.listaJugadores[i].puntosTotales = miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[rIndex]; 
    }
    
    int indiceGanador = 0;
    int menorPuntuacion = miPartidaChinchon.listaJugadores[0].puntosTotales;
    for (int i = 1; i < miPartidaChinchon.cantidadJugadores; i++) {
        if (miPartidaChinchon.listaJugadores[i].puntosTotales < menorPuntuacion) {
            menorPuntuacion = miPartidaChinchon.listaJugadores[i].puntosTotales;
            indiceGanador = i;
        }
    }
    
    guardarEnHistorial("Chinchon", miPartidaChinchon.listaJugadores[indiceGanador].nombre);
    
    int anchoCol = 1180 / miPartidaChinchon.cantidadJugadores;
    line(50, 120, 1280 - 50, 120);
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 150, miPartidaChinchon.listaJugadores[i].nombre);
        char bufferTotales[50];
        sprintf(bufferTotales, "Total: %d pts", miPartidaChinchon.listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoCol / 2), 200, bufferTotales);
        
        if (miPartidaChinchon.listaJugadores[i].puntosTotales >= 100) {
            setcolor(LIGHTRED);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"ELIMINADO");
            setcolor(WHITE);
        } else if (i == indiceGanador) {
            setcolor(LIGHTGREEN);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"GANADOR!!!");
            setcolor(WHITE);
        } else {
            outtextxy(colX + (anchoCol / 2), 260, (char*)"SALVADO");
        }
        if(i > 0) line(colX, 120, colX, 320);
    }
    line(50, 320, 1280 - 50, 320);
    actualizarPantalla();
    getch(); 
}
