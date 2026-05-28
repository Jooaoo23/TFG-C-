#include <stdio.h>    
#include <conio.h>    
#include <string.h>   
#include <graphics.h> 
#include <time.h>     // Añadido para las fechas del historial

// =================================================================
// STRUCTS SIMPLES 
// =================================================================

struct EntradaHistorial {
    char juego[30];
    char ganador[30];
    char fecha[15];
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

// --- Estructuras para el JUEGO 3: TUTE ---
struct JugadorTute {
    char nombre[30];
};

struct PartidaTute {
    int cantidadManos;
    float precioPunto;
    struct JugadorTute equipo1[2]; 
    struct JugadorTute equipo2[2];
    int puntosEq1[20];             
    int puntosEq2[20];
    int triunfoEq1[20];            
    int triunfoEq2[20];
    int tantosEq1;
    int tantosEq2;
    int manoActual;                
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

// Módulo 3: Tute
void jugarTute();
void pantallaConfigurarTute();
void redibujarFondoConfigurarTute();
void pantallaEditarEquipo(struct JugadorTute equipo[2], int numEquipo);
void redibujarFondoEditarEquipo();
void pantallaMesaTute(); 
void redibujarFondoMesaTute();
void ventanaEmergentePuntosTute(); 

// Variables globales
struct PartidaContinental miPartida;
struct PartidaChinchon miPartidaChinchon; 
struct PartidaTute miPartidaTute; 

int paginaActual = 0; 

bool mostrandoErrorContinental = false;
bool mostrandoErrorChinchon = false;

// Banderas de estado
bool tuteEditandoManos = false;
bool tuteEditandoPuntos = false;
bool tuteVolverMenu = false;
int tuteNumEquipoEditando = 1;

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
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
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
        int filaY = 150;
        int contador = 1;
        char bufferLinea[120];
        
        setcolor(LIGHTBLUE);
        outtextxy(1280 / 2, 110, (char*)"N.   -   JUEGO   -   GANADOR   -   FECHA(Hoy)");
        setcolor(WHITE);
        
        while (fscanf(fichero, "%29s %29s %14s", registro.juego, registro.ganador, registro.fecha) != EOF && contador <= 15) {
            sprintf(bufferLinea, "%d.   %s   ->   %s   [%s]", contador, registro.juego, registro.ganador, registro.fecha);
            outtextxy(1280 / 2, filaY, bufferLinea);
            filaY += 35;
            contador++;
        }
        fclose(fichero);
    }
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(1280 / 2, 680, (char*)"Mi madre me dio la vida y el proyecto de C las ganas de dejar de vivirla");
    outtextxy(1280 / 2, 700, (char*)"Gracias Andres.");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(YELLOW);
    outtextxy(1280 / 2, 630, (char*)"Presiona cualquier tecla para volver al Menu Principal");
    setcolor(WHITE);
    
    actualizarPantalla();
    getch(); 
}

// =================================================================
// MODULO GRÁFICO
// =================================================================

void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, int pantallaOrigen) {
    int pos = strlen(resultado);
    char tecla;

    while (1) {
        if (pantallaOrigen == 1) redibujarFondoConfigurarContinental();
        else if (pantallaOrigen == 2) redibujarFondoConfigurarChinchon();
        else if (pantallaOrigen == 3) redibujarFondoConfigurarTute();
        else if (pantallaOrigen == 4) redibujarFondoMesaTute();
        else if (pantallaOrigen == 5) redibujarFondoEditarEquipo();

        settextjustify(LEFT_TEXT, CENTER_TEXT);
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(x, y, resultado);
        outtextxy(x + textwidth(resultado), y, (char*)"_"); 
        
        actualizarPantalla();

        if (kbhit()) {
            tecla = getch(); 

            if (tecla == 13) { 
                break;
            } 
            else if (tecla == 8) { 
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
    if (miPartida.cantidadJugadores > 1) { 
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
    
    rectangle(100, 720 - 100, 300, 720 - 50);
    outtextxy(200, 720 - 75, (char*)"SALIR");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 720 - 110, (char*)"MAX: 7 JUGADORES");
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
                miPartida.cantidadJugadores = 0; 
                salirAjustes = true;
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
        outtextxy(1280/2, 720 - 50, (char*)"FALTAN JUGADORES POR PUNTUAR O DATOS INVALIDOS");
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
                int negativos = 0;
                bool alguienBajo5 = false;
                
                for (int i = 0; i < miPartida.cantidadJugadores; i++) {
                    if (!miPartida.listaJugadores[i].yaAnoto) todosOk = false;
                    else {
                        int p = miPartida.listaJugadores[i].puntosPorMano[miPartida.manoActual - 1];
                        if (p < -100 || p > 500) todosOk = false;
                        if (p < 0) negativos++;
                        if (p <= 5) alguienBajo5 = true;
                    }
                }
                
                if (negativos > 1) todosOk = false;
                if (!alguienBajo5) todosOk = false;
                
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
            else if ((tecla >= '0' && tecla <= '9') || (tecla == '-' && pos == 0)) { 
                if (pos < 5) {
                    bufferEntrada[pos] = tecla;
                    pos++;
                    bufferEntrada[pos] = '\0';
                }
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
    
    if (miPartidaChinchon.cantidadJugadores >= 2) {
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
    
    rectangle(100, 720 - 100, 300, 720 - 50);
    outtextxy(200, 720 - 75, (char*)"SALIR");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(200, 720 - 110, (char*)"MAX: 4 JUGADORES (LIMITE: 100 PTS)");
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
                if (miPartidaChinchon.cantidadJugadores < 4) {
                    char nuevoNombre[30] = "";
                    leerTextoGrafico(390, 125, nuevoNombre, 25, 2);
                    
                    if(strlen(nuevoNombre) > 0) {
                        strcpy(miPartidaChinchon.listaJugadores[miPartidaChinchon.cantidadJugadores].nombre, nuevoNombre);
                        miPartidaChinchon.listaJugadores[miPartidaChinchon.cantidadJugadores].yaAnoto = false;
                        miPartidaChinchon.cantidadJugadores++; 
                    }
                }
            }
            
            if (clickX >= (1280 - 300) && clickX <= (1280 - 100) && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                if (miPartidaChinchon.cantidadJugadores >= 2) salirAjustes = true; 
            }
            if (clickX >= 100 && clickX <= 300 && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                miPartidaChinchon.cantidadJugadores = 0; 
                salirAjustes = true;
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
        outtextxy(1280/2, 720 - 50, (char*)"FALTAN JUGADORES POR PUNTUAR O DATOS INVALIDOS");
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
                int negativos = 0;
                bool alguienBajo5 = false;
                
                for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
                    if (!miPartidaChinchon.listaJugadores[i].yaAnoto) todosOk = false;
                    else {
                        int p = miPartidaChinchon.listaJugadores[i].puntosPorRonda[miPartidaChinchon.rondaActual - 1];
                        if (p < -20) todosOk = false;
                        if (p < 0) negativos++;
                        if (p <= 5) alguienBajo5 = true;
                    }
                }
                
                if (negativos > 1) todosOk = false;
                if (!alguienBajo5) todosOk = false;
                
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
            else if ((tecla >= '0' && tecla <= '9') || (tecla == '-' && pos == 0)) { 
                if (pos < 5) {
                    bufferEntrada[pos] = tecla;
                    pos++;
                    bufferEntrada[pos] = '\0';
                }
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
    
    int menorPuntuacion = miPartidaChinchon.listaJugadores[0].puntosTotales;
    for (int i = 1; i < miPartidaChinchon.cantidadJugadores; i++) {
        if (miPartidaChinchon.listaJugadores[i].puntosTotales < menorPuntuacion) {
            menorPuntuacion = miPartidaChinchon.listaJugadores[i].puntosTotales;
        }
    }
    
    int empates = 0;
    int indiceGanador = 0;
    for (int i = 0; i < miPartidaChinchon.cantidadJugadores; i++) {
        if (miPartidaChinchon.listaJugadores[i].puntosTotales == menorPuntuacion) {
            empates++;
            indiceGanador = i;
        }
    }
    
    if (empates == 1) {
        guardarEnHistorial("Chinchon", miPartidaChinchon.listaJugadores[indiceGanador].nombre);
    } else {
        guardarEnHistorial("Chinchon", "EMPATE");
    }
    
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
        } else if (miPartidaChinchon.listaJugadores[i].puntosTotales == menorPuntuacion) {
            if (empates > 1) {
                setcolor(LIGHTGREEN);
                outtextxy(colX + (anchoCol / 2), 260, (char*)"EMPATE!!!");
                setcolor(WHITE);
            } else {
                setcolor(LIGHTGREEN);
                outtextxy(colX + (anchoCol / 2), 260, (char*)"GANADOR!!!");
                setcolor(WHITE);
            }
        } else {
            outtextxy(colX + (anchoCol / 2), 260, (char*)"SALVADO");
        }
        if(i > 0) line(colX, 120, colX, 320);
    }
    line(50, 320, 1280 - 50, 320);
    actualizarPantalla();
    getch(); 
}

// =================================================================
// JUEGO 3: TUTE 
// =================================================================

void jugarTute() {
    miPartidaTute.cantidadManos = 10;
    miPartidaTute.precioPunto = 0.10;
    miPartidaTute.manoActual = 0; 
    miPartidaTute.tantosEq1 = 0;
    miPartidaTute.tantosEq2 = 0;
    
    for(int i=0; i<20; i++) { 
        miPartidaTute.puntosEq1[i] = 0; 
        miPartidaTute.puntosEq2[i] = 0; 
        miPartidaTute.triunfoEq1[i] = 0; 
        miPartidaTute.triunfoEq2[i] = 0;
    }

    strcpy(miPartidaTute.equipo1[0].nombre, "ALEJANDRO");
    strcpy(miPartidaTute.equipo1[1].nombre, "JOAO");
    strcpy(miPartidaTute.equipo2[0].nombre, "JUGADOR 3");
    strcpy(miPartidaTute.equipo2[1].nombre, "JUGADOR 4");

    tuteVolverMenu = false;
    pantallaConfigurarTute();
    if (!tuteVolverMenu) {
        pantallaMesaTute();
    }
}

void redibujarFondoConfigurarTute() {
    cleardevice();
    int mX = 1280 / 2;
    
    settextjustify(CENTER_TEXT, TOP_TEXT);
    setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(mX, 50, (char*)"TUTE");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(mX - 50, 150, (char*)"CANTIDAD DE MANOS:");
    rectangle(mX + 90, 145, mX + 150, 185);
    char bufM[5]; sprintf(bufM, "%d", miPartidaTute.cantidadManos); 
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(mX + 120, 165, bufM);

    if (tuteEditandoManos) {
        setcolor(YELLOW); 
        settextjustify(RIGHT_TEXT, CENTER_TEXT);
        outtextxy(mX - 50, 220, (char*)"NUEVA CANTIDAD:");
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        setcolor(WHITE);
    }

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(mX - 100, 310, (char*)"EQUIPO 1:"); rectangle(mX + 50, 290, mX + 200, 330);
    outtextxy(mX + 125, 310, (char*)"EDITAR");
    outtextxy(mX - 100, 390, (char*)"EQUIPO 2:"); rectangle(mX + 50, 370, mX + 200, 410);
    outtextxy(mX + 125, 390, (char*)"EDITAR");

    rectangle(mX - 125, 550, mX + 125, 620); 
    outtextxy(mX, 585, (char*)"JUGAR");
    
    rectangle(100, 720 - 100, 300, 720 - 50);
    outtextxy(200, 720 - 75, (char*)"SALIR");
}

void pantallaConfigurarTute() {
    bool salirConfig = false;
    int clickX, clickY;
    int mX = 1280 / 2;

    while (!salirConfig) {
        redibujarFondoConfigurarTute();
        actualizarPantalla(); 

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            if (clickX >= mX + 50 && clickX <= mX + 200 && clickY >= 290 && clickY <= 330) pantallaEditarEquipo(miPartidaTute.equipo1, 1);
            if (clickX >= mX + 50 && clickX <= mX + 200 && clickY >= 370 && clickY <= 410) pantallaEditarEquipo(miPartidaTute.equipo2, 2);
            if (clickX >= mX - 125 && clickX <= mX + 125 && clickY >= 550 && clickY <= 620) salirConfig = true;
            
            if (clickX >= 100 && clickX <= 300 && clickY >= (720 - 100) && clickY <= (720 - 50)) {
                tuteVolverMenu = true; 
                salirConfig = true;
            }
            
            if (clickX >= mX + 90 && clickX <= mX + 150 && clickY >= 145 && clickY <= 185) {
                char bufCant[10] = "";
                tuteEditandoManos = true;
                leerTextoGrafico(mX - 40, 220, bufCant, 5, 3);
                tuteEditandoManos = false;
                if(strlen(bufCant) > 0) sscanf(bufCant, "%d", &miPartidaTute.cantidadManos);
            }
        }
        delay(10);
    }
}

void redibujarFondoEditarEquipo() {
    cleardevice();
    int mX = 1280 / 2;
    struct JugadorTute *equipo = (tuteNumEquipoEditando == 1) ? miPartidaTute.equipo1 : miPartidaTute.equipo2;

    settextjustify(CENTER_TEXT, CENTER_TEXT); setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    char t[30]; sprintf(t, "EQUIPO %d", tuteNumEquipoEditando); outtextxy(mX, 100, t);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    outtextxy(mX - 100, 250, equipo[0].nombre); rectangle(mX + 50, 225, mX + 200, 275);
    outtextxy(mX - 100, 350, equipo[1].nombre); rectangle(mX + 50, 325, mX + 200, 375);
    
    rectangle(mX - 100, 500, mX + 100, 550); outtextxy(mX, 525, (char*)"VOLVER");
}

void pantallaEditarEquipo(struct JugadorTute equipo[2], int numEquipo) {
    bool volver = false; int x, y;
    int mX = 1280 / 2;
    tuteNumEquipoEditando = numEquipo;

    while (!volver) {
        redibujarFondoEditarEquipo();
        actualizarPantalla();

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            if (x >= mX+50 && x <= mX+200 && y >= 225 && y <= 275) { 
                char nomBuf[30]="";
                leerTextoGrafico(mX + 250, 250, nomBuf, 25, 5);
                if(strlen(nomBuf)>0) strcpy(equipo[0].nombre, nomBuf);
            }
            if (x >= mX+50 && x <= mX+200 && y >= 325 && y <= 375) { 
                char nomBuf[30]="";
                leerTextoGrafico(mX + 250, 350, nomBuf, 25, 5);
                if(strlen(nomBuf)>0) strcpy(equipo[1].nombre, nomBuf);
            }
            if (x >= mX-100 && x <= mX+100 && y >= 500 && y <= 550) volver = true;
        }
        delay(10);
    }
}

void redibujarFondoMesaTute() {
    cleardevice();
    int mX = 1280 / 2;
    
    settextjustify(CENTER_TEXT, TOP_TEXT);
    setcolor(WHITE); settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(mX, 30, (char*)"TUTE");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char bufM[30]; sprintf(bufM, "MANO %d", miPartidaTute.manoActual + 1);
    outtextxy(100, 80, bufM);

    rectangle(mX + 150, 20, mX + 450, 70);
    outtextxy(mX + 300, 35, (char*)"SIGUIENTE MANO");

    char nEq1[60], nEq2[60];
    sprintf(nEq1, "%s Y %s", miPartidaTute.equipo1[0].nombre, miPartidaTute.equipo1[1].nombre);
    sprintf(nEq2, "%s Y %s", miPartidaTute.equipo2[0].nombre, miPartidaTute.equipo2[1].nombre);
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(mX - 250, 150, nEq1);
    outtextxy(mX + 250, 150, nEq2);

    char tantos1[30] = "TANTOS: ";
    for(int i=0; i<miPartidaTute.tantosEq1; i++) strcat(tantos1, "I");
    char tantos2[30] = "TANTOS: ";
    for(int i=0; i<miPartidaTute.tantosEq2; i++) strcat(tantos2, "I");
    
    settextjustify(RIGHT_TEXT, CENTER_TEXT);
    outtextxy(mX - 450, 150, tantos1);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    outtextxy(mX + 450, 150, tantos2);

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    line(mX, 180, mX, 600); 
    outtextxy(mX - 350, 200, (char*)"PUNTOS"); outtextxy(mX - 150, 200, (char*)"TRIUNFO");
    outtextxy(mX + 150, 200, (char*)"PUNTOS"); outtextxy(mX + 350, 200, (char*)"TRIUNFO");
    line(50, 230, 1280 - 50, 230);

    for(int i=0; i <= miPartidaTute.manoActual; i++) {
        int yFila = 260 + (i * 35);
        if(miPartidaTute.puntosEq1[i] > 0 || miPartidaTute.puntosEq2[i] > 0) {
            char p1[10], p2[10];
            sprintf(p1, "%d", miPartidaTute.puntosEq1[i]); sprintf(p2, "%d", miPartidaTute.puntosEq2[i]);
            outtextxy(mX - 350, yFila, p1);
            outtextxy(mX + 150, yFila, p2);
        }
        if (miPartidaTute.triunfoEq1[i] == 1) outtextxy(mX - 150, yFila, (char*)"X");
        if (miPartidaTute.triunfoEq2[i] == 1) outtextxy(mX + 350, yFila, (char*)"X");
    }

    rectangle(mX - 120, 620, mX + 120, 680);
    outtextxy(mX, 640, (char*)"ANADIR PUNTOS");
}

void ventanaEmergentePuntosTute() {
    char bufE1[10] = "";
    char bufE2[10] = "";
    int pos1 = 0, pos2 = 0;
    int cx, cy;
    bool continuar = true;
    int t1 = 0, t2 = 0; 
    int foco = 1;

    clearmouseclick(WM_LBUTTONDOWN);

    while(continuar) {
        redibujarFondoMesaTute();

        setcolor(WHITE);
        rectangle(250, 450, 1030, 690); 
        setfillstyle(SOLID_FILL, BLACK);
        bar(251, 451, 1029, 689); 
        rectangle(650, 600, 850, 650); 
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(750, 625, (char*)"SIGUIENTE"); 
        
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        outtextxy(280, 500, (char*)"PUNTOS EQUIPO 1: "); 
        outtextxy(280 + textwidth((char*)"PUNTOS EQUIPO 1: "), 500, bufE1);
        if (foco == 1) outtextxy(280 + textwidth((char*)"PUNTOS EQUIPO 1: ") + textwidth(bufE1), 500, (char*)"_");
        
        outtextxy(280, 550, (char*)"PUNTOS EQUIPO 2: "); 
        outtextxy(280 + textwidth((char*)"PUNTOS EQUIPO 2: "), 550, bufE2);
        if (foco == 2) outtextxy(280 + textwidth((char*)"PUNTOS EQUIPO 2: ") + textwidth(bufE2), 550, (char*)"_");

        rectangle(750, 480, 950, 520);
        if (t1) { setcolor(YELLOW); outtextxy(850, 500, (char*)"TRIUNFO (X)"); setcolor(WHITE); }
        else outtextxy(850, 500, (char*)"TRIUNFO");

        rectangle(750, 530, 950, 570);
        if (t2) { setcolor(YELLOW); outtextxy(850, 550, (char*)"TRIUNFO (X)"); setcolor(WHITE); }
        else outtextxy(850, 550, (char*)"TRIUNFO");
        
        actualizarPantalla();

        if (kbhit()) {
            char tecla = getch();
            if (tecla == 9) foco = (foco == 1) ? 2 : 1; 
            else if (tecla == 13) foco = (foco == 1) ? 2 : 1; 
            else if (tecla == 8) { 
                if (foco == 1 && pos1 > 0) { pos1--; bufE1[pos1] = '\0'; }
                if (foco == 2 && pos2 > 0) { pos2--; bufE2[pos2] = '\0'; }
            }
            else if (tecla >= '0' && tecla <= '9') { 
                if (foco == 1 && pos1 < 4) { bufE1[pos1] = tecla; pos1++; bufE1[pos1] = '\0'; }
                if (foco == 2 && pos2 < 4) { bufE2[pos2] = tecla; pos2++; bufE2[pos2] = '\0'; }
            }
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, cx, cy);
            if (cx >= 650 && cx <= 850 && cy >= 600 && cy <= 650) {
                int p1 = 0, p2 = 0;
                if (strlen(bufE1) > 0) sscanf(bufE1, "%d", &p1);
                if (strlen(bufE2) > 0) sscanf(bufE2, "%d", &p2);
                if ((p1 + p2 >= 130) && (p1 + p2 <= 230)) {
                    miPartidaTute.puntosEq1[miPartidaTute.manoActual] = p1;
                    miPartidaTute.puntosEq2[miPartidaTute.manoActual] = p2;
                    miPartidaTute.triunfoEq1[miPartidaTute.manoActual] = t1;
                    miPartidaTute.triunfoEq2[miPartidaTute.manoActual] = t2;
                    continuar = false; 
                } 
            }
            if (cx >= 750 && cx <= 950 && cy >= 480 && cy <= 520) { t1 = 1; t2 = 0; foco = 1; }
            if (cx >= 750 && cx <= 950 && cy >= 530 && cy <= 570) { t2 = 1; t1 = 0; foco = 2; }
            
            if (cy >= 480 && cy <= 520 && cx < 700) foco = 1;
            if (cy >= 530 && cy <= 570 && cx < 700) foco = 2;
        }
        delay(10);
    }
    clearmouseclick(WM_LBUTTONDOWN);
}

void pantallaMesaTute() {
    bool finPartida = false;
    int clickX, clickY;

    while (!finPartida) {
        redibujarFondoMesaTute();
        actualizarPantalla(); 
        int mX = 1280 / 2;

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            if (clickX >= mX - 120 && clickX <= mX + 120 && clickY >= 620 && clickY <= 680) {
                ventanaEmergentePuntosTute();
            }

            if (clickX >= mX + 150 && clickX <= mX + 450 && clickY >= 20 && clickY <= 70) {
                if (miPartidaTute.puntosEq1[miPartidaTute.manoActual] > 0 || miPartidaTute.puntosEq2[miPartidaTute.manoActual] > 0) {
                    
                    int p1 = miPartidaTute.puntosEq1[miPartidaTute.manoActual];
                    int p2 = miPartidaTute.puntosEq2[miPartidaTute.manoActual];
                    
                    if (p1 > p2) miPartidaTute.tantosEq1++;
                    else if (p2 > p1) miPartidaTute.tantosEq2++;
                    else {
                        if (miPartidaTute.triunfoEq1[miPartidaTute.manoActual] == 1) miPartidaTute.tantosEq1++;
                        else if (miPartidaTute.triunfoEq2[miPartidaTute.manoActual] == 1) miPartidaTute.tantosEq2++;
                    }

                    miPartidaTute.manoActual++;
                    if (miPartidaTute.manoActual >= miPartidaTute.cantidadManos) {
                        cleardevice();
                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                        settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
                        outtextxy(mX, 200, (char*)"FIN DE PARTIDA");
                        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                        char res[100];
                        char ganadorNombre[50] = "";
                        
                        if (miPartidaTute.tantosEq1 > miPartidaTute.tantosEq2) {
                            sprintf(res, "GANADOR: EQUIPO 1 (%d Tantos)", miPartidaTute.tantosEq1);
                            strcpy(ganadorNombre, "EQUIPO 1");
                        }
                        else if (miPartidaTute.tantosEq2 > miPartidaTute.tantosEq1) {
                            sprintf(res, "GANADOR: EQUIPO 2 (%d Tantos)", miPartidaTute.tantosEq2);
                            strcpy(ganadorNombre, "EQUIPO 2");
                        }
                        else {
                            strcpy(res, "EMPATE");
                            strcpy(ganadorNombre, "EMPATE");
                        }
                        outtextxy(mX, 350, res);
                        outtextxy(mX, 500, (char*)"Pulsa para salir...");
                        
                        actualizarPantalla();
                        guardarEnHistorial("Tute", ganadorNombre); 
                        
                        getch(); finPartida = true;
                    }
                }
            }
        }
        delay(10);
    }
}
