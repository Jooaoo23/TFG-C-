#include <stdio.h>    // sprintf, sscanf
#include <conio.h>    // getch, kbhit
#include <string.h>   // strcpy, strlen
#include <graphics.h> // Funciones de WinBGIm

// =================================================================
// STRUCTS SIMPLES (Nivel 1º DAM)
// =================================================================

// --- Estructuras para el JUEGO 1: CONTINENTAL ---
struct Jugador {
    char nombre[30];
    int puntosPorMano[7];          
    int puntosAcumuladosEnMano[7]; 
    int puntosTotales;             
    bool yaAnoto;                  
};

struct PartidaContinental {
    struct Jugador listaJugadores[7]; 
    int cantidadJugadores;            
    int manoActual;                    
};

// --- Estructuras para el JUEGO 2: CHINCHÓN ---
struct JugadorChinchon {
    char nombre[30];
    int puntosPorRonda[50];          // Historial de puntos sueltos por ronda
    int puntosAcumuladosEnRonda[50]; // Sumas parciales acumuladas
    int puntosTotales;               
    bool yaAnoto;                    
};

struct PartidaChinchon {
    struct JugadorChinchon listaJugadores[7]; // Máximo 7 participantes
    int cantidadJugadores;            
    int rondaActual;                  // Contador dinámico de rondas
};

// --- PROTOTIPOS DE LAS FUNCIONES ---
void iniciarModoGrafico();
void actualizarPantalla(); // Intercambia los buffers/páginas de video
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);
void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, void (*redibujarFondo)());

// Prototipos Módulo 1: Continental
void jugarContinental();
void prepararPantallaJuego(const char* nombreDelJuego);
void pantallaConfigurarJugadores(struct PartidaContinental *partida);
void redibujarFondoConfigurarContinental(); // Auxiliar antiparpadeo
void pantallaMesaJuego(struct PartidaContinental *partida);
void redibujarFondoMesaContinental(); // Auxiliar antiparpadeo
void ventanaEmergentePuntos(struct PartidaContinental *partida, int i);
void pantallaResultadosFinales(struct PartidaContinental *partida);

// Prototipos Módulo 2: Chinchón
void jugarChinchon();
void pantallaConfigurarJugadoresChinchon(struct PartidaChinchon *partida);
void redibujarFondoConfigurarChinchon(); // Auxiliar antiparpadeo
void pantallaMesaChinchon(struct PartidaChinchon *partida);
void redibujarFondoMesaChinchon(); // Auxiliar antiparpadeo
void ventanaEmergentePuntosChinchon(struct PartidaChinchon *partida, int i);
void pantallaResultadosChinchon(struct PartidaChinchon *partida);

// Prototipos Módulos restantes
void jugarTute();
void jugarEscoba();

// Variables globales de las partidas e hilos gráficos
struct PartidaContinental miPartida;
struct PartidaChinchon miPartidaChinchon; 
int paginaActual = 0; // Control de Double Buffering alterno

// Variables de estado auxiliares para poder redibujar los fondos desde cualquier función
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
        if (opcionMenu != 5) {
            ejecutarJuego(opcionMenu);
        }
    } while (opcionMenu != 5);
    
    closegraph();
    return 0;
}

void iniciarModoGrafico() {
    initwindow(1280, 720, (char*)"TFG - Baraja Espanola", 0, 0, false, true);
    setactivepage(0);
    setvisualpage(0);
}

void actualizarPantalla() {
    setvisualpage(paginaActual);       // Muestra la página terminada
    paginaActual = 1 - paginaActual;   // Cambia el índice de página (0 a 1, o 1 a 0)
    setactivepage(paginaActual);       // Prepara la otra página para dibujar a ciegas
}

void dibujarMenu() {
    cleardevice();
    int mitadX = 1280 / 2;
    int mitadY = 720 / 2;
    int medioBotonX = 170;
    int medioBotonY = 25;
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(mitadX, mitadY - 180, (char*)"BARAJA ESPANOLA");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    rectangle(mitadX - medioBotonX, mitadY - 100 - medioBotonY, mitadX + medioBotonX, mitadY - 100 + medioBotonY);
    outtextxy(mitadX, mitadY - 100, (char*)"CONTINENTAL");
    
    rectangle(mitadX - medioBotonX, mitadY - 30 - medioBotonY, mitadX + medioBotonX, mitadY - 30 + medioBotonY);
    outtextxy(mitadX, mitadY - 30, (char*)"CHINCHON");
    
    rectangle(mitadX - medioBotonX, mitadY + 40 - medioBotonY, mitadX + medioBotonX, mitadY + 40 + medioBotonY);
    outtextxy(mitadX, mitadY + 40, (char*)"TUTE");
    
    rectangle(mitadX - medioBotonX, mitadY + 110 - medioBotonY, mitadX + medioBotonX, mitadY + 110 + medioBotonY);
    outtextxy(mitadX, mitadY + 110, (char*)"ESCOBA");

    actualizarPantalla(); 
}

int controlarMenu() {
    int clickX, clickY;
    int mitadX = 1280 / 2;
    int mitadY = 720 / 2;
    int medioBotonX = 170;
    int medioBotonY = 25;

    while(1) {
        if (kbhit()) {
            if (getch() == 27) return 5; 
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= (mitadX - medioBotonX) && clickX <= (mitadX + medioBotonX)) {
                if (clickY >= (mitadY - 100 - medioBotonY) && clickY <= (mitadY - 100 + medioBotonY)) return 1;
                if (clickY >= (mitadY - 30 - medioBotonY) && clickY <= (mitadY - 30 + medioBotonY)) return 2;
                if (clickY >= (mitadY + 40 - medioBotonY) && clickY <= (mitadY + 40 + medioBotonY)) return 3;
                if (clickY >= (mitadY + 110 - medioBotonY) && clickY <= (mitadY + 110 + medioBotonY)) return 4;
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
        case 4: jugarEscoba();      break;
    }
}

// =================================================================
// MODULO BASE GRÁFICO CORREGIDO (ANTIPARPADEO)
// =================================================================

// Ahora recibe un puntero a función para redibujar de forma transparente lo que hay detrás
void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, void (*redibujarFondo)()) {
    int pos = strlen(resultado);
    char tecla;

    while (1) {
        // 1. Renderizamos de fondo de la pantalla completa de forma limpia en el buffer oculto
        if (redibujarFondo != NULL) {
            redibujarFondo();
        }

        // 2. Dibujamos el texto actual y el cursor simulado encima
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(x, y, resultado);
        outtextxy(x + textwidth(resultado), y, (char*)"_"); 
        
        // 3. Volcamos al monitor de forma sincronizada
        actualizarPantalla();

        if (kbhit()) {
            tecla = getch(); 

            if (tecla == 13) { // ENTER
                break;
            } 
            else if (tecla == 8) { // BACKSPACE
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
    
    for(int i = 0; i < 7; i++) {
        miPartida.listaJugadores[i].puntosTotales = 0;
        miPartida.listaJugadores[i].yaAnoto = false;
        for(int j = 0; j < 7; j++) {
            miPartida.listaJugadores[i].puntosPorMano[j] = 0;
            miPartida.listaJugadores[i].puntosAcumuladosEnMano[j] = 0;
        }
    }
    pantallaConfigurarJugadores(&miPartida);
    if (miPartida.cantidadJugadores > 0) {
        pantallaMesaJuego(&miPartida);
    }
}

// Función encargada de dibujar de forma estática la configuración de jugadores
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

void pantallaConfigurarJugadores(struct PartidaContinental *partida) {
    bool salirAjustes = false;
    int clickX, clickY;
    while (!salirAjustes) {
        redibujarFondoConfigurarContinental();
        actualizarPantalla(); 

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (partida->cantidadJugadores < 7) {
                    char nuevoNombre[30] = "";
                    // Pasamos la función de fondo para que al tipear no borre las líneas del programa
                    leerTextoGrafico(390, 125, nuevoNombre, 25, redibujarFondoConfigurarContinental);
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(partida->listaJugadores[partida->cantidadJugadores].nombre, nuevoNombre);
                        partida->listaJugadores[partida->cantidadJugadores].yaAnoto = false;
                        partida->cantidadJugadores++; 
                    }
                }
            }
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (partida->cantidadJugadores > 0) salirAjustes = true; 
            }
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int filaY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    for (int j = i; j < partida->cantidadJugadores - 1; j++) {
                        strcpy(partida->listaJugadores[j].nombre, partida->listaJugadores[j + 1].nombre);
                    }
                    partida->cantidadJugadores--; 
                    break; 
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditado[30] = "";
                    strcpy(nombreEditado, partida->listaJugadores[i].nombre);
                    leerTextoGrafico(690, filaY + 20, nombreEditado, 25, redibujarFondoConfigurarContinental);
                    if(strlen(nombreEditado) > 0) {
                        strcpy(partida->listaJugadores[i].nombre, nombreEditado);
                    }
                    break;
                }
            }
        }
        delay(10); 
    }
}

// Dibuja la matriz del juego limpia en el buffer oculto
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
            char bufferPts[40];
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

void pantallaMesaJuego(struct PartidaContinental *partida) {
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
                for (int i = 0; i < partida->cantidadJugadores; i++) {
                    if (!partida->listaJugadores[i].yaAnoto) todosOk = false;
                }
                if (todosOk) {
                    if (partida->manoActual == 7) {
                        terminarMesa = true; 
                        pantallaResultadosFinales(partida); 
                    } else {
                        for(int i = 0; i < partida->cantidadJugadores; i++) partida->listaJugadores[i].yaAnoto = false;
                        partida->manoActual++; 
                    }
                } else mostrandoErrorContinental = true;
            }
            int btnY = 720 - 130;
            int anchoCol = 1180 / partida->cantidadJugadores;
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int colX = 50 + (i * anchoCol);
                if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) {
                    ventanaEmergentePuntos(partida, i); 
                }
            }
        }
        delay(10);
    }
}

void ventanaEmergentePuntos(struct PartidaContinental *partida, int i) {
    char bufferEntrada[20] = "";
    int pos = 0;
    int cx, cy;
    bool continuar = true;

    while(continuar) {
        // Redibujamos la mesa de fondo primero para evitar que desaparezcan líneas
        redibujarFondoMesaContinental();

        // Superponemos la ventana emergente en el buffer oculto
        setcolor(WHITE);
        rectangle(250, 500, 1030, 690); 
        setfillstyle(SOLID_FILL, BLACK);
        bar(251, 501, 1029, 689); 
        rectangle(650, 580, 850, 630); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 605, (char*)"SIGUIENTE"); 
        
        char bufferMensaje[100];
        sprintf(bufferMensaje, "PUNTOS JUGADOR - %s: ", partida->listaJugadores[i].nombre); 
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
            if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) continuar = false; 
        }
        delay(10);
    }
    int ptsInput = 0;
    sscanf(bufferEntrada, "%d", &ptsInput);
    int m = partida->manoActual - 1;
    partida->listaJugadores[i].puntosPorMano[m] = ptsInput;
    if (partida->manoActual == 1) {
        partida->listaJugadores[i].puntosAcumuladosEnMano[0] = ptsInput; 
    } else {
        partida->listaJugadores[i].puntosAcumuladosEnMano[m] = partida->listaJugadores[i].puntosAcumuladosEnMano[m - 1] + ptsInput; 
    }
    partida->listaJugadores[i].yaAnoto = true;
}

void pantallaResultadosFinales(struct PartidaContinental *partida) {
    cleardevice();
    prepararPantallaJuego("RESULTADOS FINALES - CONTINENTAL");
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        partida->listaJugadores[i].puntosTotales = partida->listaJugadores[i].puntosAcumuladosEnMano[6]; 
    }
    int indiceGanador = 0;
    int menorPuntuacion = partida->listaJugadores[0].puntosTotales;
    for (int i = 1; i < partida->cantidadJugadores; i++) {
        if (partida->listaJugadores[i].puntosTotales < menorPuntuacion) {
            menorPuntuacion = partida->listaJugadores[i].puntosTotales;
            indiceGanador = i;
        }
    }
    int anchoCol = 1180 / partida->cantidadJugadores;
    line(50, 120, 1280 - 50, 120);
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 150, partida->listaJugadores[i].nombre);
        char bufferTotales[50];
        sprintf(bufferTotales, "Total: %d pts", partida->listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoCol / 2), 200, bufferTotales);
        if (i == indiceGanador) {
            setcolor(LIGHTGREEN);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"GANADOR!!!");
            setcolor(WHITE);
        } else {
            char bufferResta[50];
            int resta = partida->listaJugadores[i].puntosTotales - menorPuntuacion;
            sprintf(bufferResta, "%d - %d = %d", partida->listaJugadores[i].puntosTotales, menorPuntuacion, resta);
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
    
    for(int i = 0; i < 7; i++) {
        miPartidaChinchon.listaJugadores[i].puntosTotales = 0;
        miPartidaChinchon.listaJugadores[i].yaAnoto = false;
        for(int j = 0; j < 50; j++) {
            miPartidaChinchon.listaJugadores[i].puntosPorRonda[j] = 0;
            miPartidaChinchon.listaJugadores[i].puntosAcumuladosEnRonda[j] = 0;
        }
    }
    
    pantallaConfigurarJugadoresChinchon(&miPartidaChinchon);
    
    if (miPartidaChinchon.cantidadJugadores > 0) {
        pantallaMesaChinchon(&miPartidaChinchon);
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

void pantallaConfigurarJugadoresChinchon(struct PartidaChinchon *partida) {
    bool salirAjustes = false;
    int clickX, clickY;
    
    while (!salirAjustes) {
        redibujarFondoConfigurarChinchon();
        actualizarPantalla();

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (partida->cantidadJugadores < 7) {
                    char nuevoNombre[30] = "";
                    leerTextoGrafico(390, 125, nuevoNombre, 25, redibujarFondoConfigurarChinchon);
                    
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(partida->listaJugadores[partida->cantidadJugadores].nombre, nuevoNombre);
                        partida->listaJugadores[partida->cantidadJugadores].yaAnoto = false;
                        partida->cantidadJugadores++; 
                    }
                }
            }
            
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (partida->cantidadJugadores > 0) salirAjustes = true; 
            }
            
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int filaY = 180 + (i * 55);
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    for (int j = i; j < partida->cantidadJugadores - 1; j++) {
                        strcpy(partida->listaJugadores[j].nombre, partida->listaJugadores[j + 1].nombre);
                    }
                    partida->cantidadJugadores--; 
                    break; 
                }
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditated[30] = "";
                    strcpy(nombreEditated, partida->listaJugadores[i].nombre);
                    leerTextoGrafico(690, filaY + 20, nombreEditated, 25, redibujarFondoConfigurarChinchon);
                    if(strlen(nombreEditated) > 0) {
                        strcpy(partida->listaJugadores[i].nombre, nombreEditated);
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

void pantallaMesaChinchon(struct PartidaChinchon *partida) {
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
                for (int i = 0; i < partida->cantidadJugadores; i++) {
                    if (!partida->listaJugadores[i].yaAnoto) todosOk = false;
                }
                
                if (todosOk) {
                    bool alguienPerdio = false;
                    int rIndex = partida->rondaActual - 1;
                    
                    for(int i = 0; i < partida->cantidadJugadores; i++) {
                        if(partida->listaJugadores[i].puntosAcumuladosEnRonda[rIndex] >= 100) {
                            alguienPerdio = true;
                        }
                    }
                    
                    if (alguienPerdio) {
                        terminarMesa = true; 
                        pantallaResultadosChinchon(partida); 
                    } else {
                        for(int i = 0; i < partida->cantidadJugadores; i++) {
                            partida->listaJugadores[i].yaAnoto = false;
                        }
                        partida->rondaActual++; 
                    }
                } else {
                    mostrandoErrorChinchon = true;
                }
            }
            
            int btnY = 720 - 130;
            int anchoCol = 1180 / partida->cantidadJugadores;
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int colX = 50 + (i * anchoCol);
                if (clickX >= colX + 10 && clickX <= colX + anchoCol - 10 && clickY >= btnY && clickY <= btnY + 40) {
                    ventanaEmergentePuntosChinchon(partida, i); 
                }
            }
        }
        delay(10);
    }
}

void ventanaEmergentePuntosChinchon(struct PartidaChinchon *partida, int i) {
    char bufferEntrada[20] = "";
    int pos = 0;
    int cx, cy;
    bool continuar = true;
    
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
        sprintf(bufferMensaje, "PUNTOS CHINCHON - %s: ", partida->listaJugadores[i].nombre); 
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(280, 560, bufferMensaje); 
        
        int inicioEscribirX = 280 + textwidth(bufferMensaje);
        outtextxy(inicioEscribirX, 560, bufferEntrada);
        outtextxy(inicioEscribirX + textwidth(bufferEntrada), 560, (char*)"_");

        actualizarPantalla();

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 13) { 
                continuar = false;
            }
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
            if (cx >= 650 && cx <= 850 && cy >= 580 && cy <= 630) continuar = false; 
        }
        delay(10);
    }
    int ptsInput = 0;
    sscanf(bufferEntrada, "%d", &ptsInput);
    int m = partida->rondaActual - 1;
    partida->listaJugadores[i].puntosPorRonda[m] = ptsInput;
    if (partida->rondaActual == 1) {
        partida->listaJugadores[i].puntosAcumuladosEnRonda[0] = ptsInput; 
    } else {
        partida->listaJugadores[i].puntosAcumuladosEnRonda[m] = partida->listaJugadores[i].puntosAcumuladosEnRonda[m - 1] + ptsInput; 
    }
    partida->listaJugadores[i].yaAnoto = true;
}

void pantallaResultadosChinchon(struct PartidaChinchon *partida) {
    cleardevice();
    prepararPantallaJuego("RESULTADOS FINALES - CHINCHON");
    
    int rIndex = partida->rondaActual - 1;
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        partida->listaJugadores[i].puntosTotales = partida->listaJugadores[i].puntosAcumuladosEnRonda[rIndex]; 
    }
    
    int anchoCol = 1180 / partida->cantidadJugadores;
    line(50, 120, 1280 - 50, 120);
    
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        int colX = 50 + (i * anchoCol);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoCol / 2), 150, partida->listaJugadores[i].nombre);
        
        char bufferTotales[50];
        sprintf(bufferTotales, "Total: %d pts", partida->listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoCol / 2), 200, bufferTotales);
        
        if (partida->listaJugadores[i].puntosTotales >= 100) {
            setcolor(LIGHTRED);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"ELIMINADO");
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREEN);
            outtextxy(colX + (anchoCol / 2), 260, (char*)"SALVADO");
            setcolor(WHITE);
        }
        if(i > 0) line(colX, 120, colX, 320);
    }
    line(50, 320, 1280 - 50, 320);
    actualizarPantalla();
    getch(); 
}

// =================================================================
// PROTOTIPOS RESTANTES VACÍOS
// =================================================================
void jugarTute() {}
void jugarEscoba() {}
