#include <stdio.h>    // Librería estándar para entrada/salida (printf, scanf)
#include <conio.h>    // Librería para manejo del teclado (kbhit, getch)
#include <string.h>   // Librería para cadenas de texto (strcpy, sprintf)
#include <graphics.h> // Librería gráfica para la interfaz visual

// =================================================================
// ESTRUCTURAS DE DATOS PARA EL JUEGO CONTINENTAL
// =================================================================

// Información individual de cada jugador
struct Jugador {
    char nombre[30];              // Nombre del jugador (ej: "ALEJANDRO")
    int puntosPorMano[7];         // Puntos obtenidos en cada una de las 7 manos
    int puntosTotales;            // Suma acumulada de sus puntos
    bool yaHaPuntuadoManoActual;  // Control para evitar saltarse jugadores sin puntuar
};

// Control de la partida activa
struct PartidaContinental {
    struct Jugador listaJugadores[7]; // Array con espacio para un máximo de 7 jugadores
    int cantidadJugadores;            // Cuántos jugadores se han añadido (empieza en 0)
    int manoActual;                   // Mano que se está jugando (del 1 al 7)
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

// Subpantallas del modo Continental (Modularización)
void prepararPantallaJuego(const char* nombreDelJuego); 
void pantallaConfigurarJugadores(struct PartidaContinental *partida);
void pantallaMesaJuego(struct PartidaContinental *partida);

// Variable global que almacena el estado de la partida de Continental
struct PartidaContinental miPartida;

// =================================================================
// FUNCIÓN PRINCIPAL (MAIN)
// =================================================================
int main() {
    int opcionMenu = 0;
    
    iniciarModoGrafico(); // Inicializa el entorno BGI a pantalla completa
    
    do {
        dibujarMenu();             // Dibuja el menú principal
        opcionMenu = controlarMenu(); // Controla la selección por clic o ESC
        
        if (opcionMenu != 5) { 
            ejecutarJuego(opcionMenu);
        }
    } while (opcionMenu != 5); 
    
    closegraph(); // Cierra los gráficos limpiamente al salir
    return 0;
}

// =================================================================
// IMPLEMENTACIÓN DEL MENÚ PRINCIPAL
// =================================================================

void iniciarModoGrafico() {
    int anchoPantalla = getmaxwidth();  
    int altoPantalla = getmaxheight(); 
    initwindow(anchoPantalla, altoPantalla, (char*)"TFG - Baraja Espanola", 0, 0, false, true); //
    
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
    
    settextjustify(CENTER_TEXT, CENTER_TEXT); // Centra los textos en base a sus coordenadas
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); 
    outtextxy(mitadX, mitadY - 180, (char*)"BARAJA ESPANOLA");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); 
    int medioAnchoBoton = 170; 
    int medioAltoBoton = 25;   
    
    // Botones del menú principal
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
            if (getch() == 27) return 5; // Tecla ESC
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY); //

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

// =================================================================
// LÓGICA DEL MODO DE JUEGO: CONTINENTAL
// =================================================================

void prepararPantallaJuego(const char* nombreDelJuego) {
    cleardevice();
    int mitadX = getmaxx() / 2;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(mitadX, 50, (char*)nombreDelJuego);
}

void jugarContinental() {
    // Inicialización del estado de juego: 0 jugadores y Mano 1
    miPartida.cantidadJugadores = 0;
    miPartida.manoActual = 1;      
    
    // FASE 1: Pantalla de añadir/editar/eliminar jugadores
    pantallaConfigurarJugadores(&miPartida);
    
    // FASE 2: Si el usuario pulsa jugar y se añadieron participantes, entramos a la mesa
    if (miPartida.cantidadJugadores > 0) {
        pantallaMesaJuego(&miPartida);
    }
}

// PANTALLA 1: Configuración de jugadores interactiva y automática (Ref: PantallaContinentalJugadores.png)
void pantallaConfigurarJugadores(struct PartidaContinental *partida) {
    bool salirAjustes = false;
    int clickX, clickY;
    int mitadX = getmaxx() / 2;
    
    while (!salirAjustes) {
        cleardevice(); 
        prepararPantallaJuego("CONTINENTAL");
        
        // --- BOTÓN 1: AÑADIR JUGADOR (Esquina superior izquierda)
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(WHITE);
        rectangle(100, 120, 350, 180); 
        outtextxy(225, 150, (char*)"ANADIR JUGADOR");
        
        // --- BOTÓN 2: JUGAR (Esquina inferior derecha)
        rectangle(getmaxx() - 350, getmaxy() - 120, getmaxx() - 100, getmaxy() - 60);
        outtextxy(getmaxx() - 225, getmaxy() - 90, (char*)"JUGAR");
        
        // --- TEXTO INDICATIVO DE LÍMITE
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(200, getmaxy() - 90, (char*)"MAX: 7 JUGADORES");
        
        // =================================================================
        // DIBUJADO DINÁMICO EN FILAS (TAL COMO EN VUESTRA FOTO)
        // =================================================================
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        
        for (int i = 0; i < partida->cantidadJugadores; i++) {
            // Calcula dinámicamente la altura 'Y' de cada fila para que no se pisen
            int filaY = 220 + (i * 60); 
            
            // 1. Dibuja el Nombre (Alineado a la izquierda)
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            char bufferNombre[50];
            sprintf(bufferNombre, "%s:", partida->listaJugadores[i].nombre);
            outtextxy(100, filaY + 20, bufferNombre); 
            
            // Volvemos a centrar la alineación de texto para los botones rectangulares
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            
            // 2. Dibuja el botón EDITAR
            rectangle(350, filaY, 480, filaY + 40);
            outtextxy(415, filaY + 20, (char*)"EDITAR");
            
            // 3. Dibuja el botón ELIMINAR
            rectangle(510, filaY, 660, filaY + 40);
            outtextxy(585, filaY + 20, (char*)"ELIMINAR");
        }

        // =================================================================
        // GESTIÓN Y DETECCIÓN DE CLICS DEL RATÓN
        // =================================================================
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            // ¿Clic en "ANADIR JUGADOR"?
            if (clickX >= 100 && clickX <= 350 && clickY >= 120 && clickY <= 180) {
                if (partida->cantidadJugadores < 7) {
                    
                    // Congelamos la ventana gráfica momentáneamente para escribir el nombre en consola
                    char nuevoNombre[30];
                    printf("\n==================================\n");
                    printf("INTRODUCE EL NOMBRE DEL JUGADOR %d: ", partida->cantidadJugadores + 1);
                    scanf("%29s", nuevoNombre);
                    printf("==================================\n");
                    
                    // Guarda el nombre en la estructura e incrementa la cuenta para que el bucle lo pinte
                    strcpy(partida->listaJugadores[partida->cantidadJugadores].nombre, nuevoNombre);
                    partida->cantidadJugadores++; 
                }
            }
            
            // ¿Clic en "JUGAR"?
            if (clickX >= (getmaxx() - 350) && clickX <= (getmaxx() - 100) && clickY >= (getmaxy() - 120) && clickY <= (getmaxy() - 60)) {
                if (partida->cantidadJugadores > 0) {
                    salirAjustes = true; // Permite romper el bucle y saltar a la mesa de juego
                }
            }
            
            // Evalúa si el clic impactó sobre los botones internos (EDITAR/ELIMINAR) de alguna fila
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int filaY = 220 + (i * 60);
                
                // Si pulsa en el botón ELIMINAR de la fila 'i'
                if (clickX >= 510 && clickX <= 660 && clickY >= filaY && clickY <= (filaY + 40)) {
                    // Reordenamos el array desplazando los elementos siguientes hacia arriba
                    for (int j = i; j < partida->cantidadJugadores - 1; j++) {
                        strcpy(partida->listaJugadores[j].nombre, partida->listaJugadores[j + 1].nombre);
                    }
                    partida->cantidadJugadores--; // Restamos 1 al total
                    break; 
                }
                
                // Si pulsa en el botón EDITAR de la fila 'i'
                if (clickX >= 350 && clickX <= 480 && clickY >= filaY && clickY <= (filaY + 40)) {
                    char nombreEditado[30];
                    printf("\n==================================\n");
                    printf("EDITAR NOMBRE (ACTUAL: %s): ", partida->listaJugadores[i].nombre);
                    scanf("%29s", nombreEditado);
                    printf("==================================\n");
                    strcpy(partida->listaJugadores[i].nombre, nombreEditado); // Reemplaza por el nuevo nombre
                    break;
                }
            }
        }
        delay(30); // Estabilizador de frames de la ventana
    }
}

// PANTALLA 2: La Mesa de juego (A donde te redirige al pulsar JUGAR)
void pantallaMesaJuego(struct PartidaContinental *partida) {
    cleardevice();
    int mitadX = getmaxx() / 2;
    
    prepararPantallaJuego("MESA DE JUEGO");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(mitadX, getmaxy() / 2 - 50, (char*)"¡Partida Iniciada con Exito!");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(mitadX, getmaxy() / 2 + 50, (char*)"Pulsa cualquier tecla para regresar al menu principal...");
    
    getch(); 
}

// =================================================================
// PROTOTIPOS RESTANTES DE LOS OTROS JUEGOS
// =================================================================
void jugarChinchon() {
    prepararPantallaJuego("MESA DE CHINCHON"); getch();
}
void jugarTute() {
    prepararPantallaJuego("MESA DE TUTE"); getch();
}
void jugarEscoba() {
    prepararPantallaJuego("MESA DE LA ESCOBA"); getch();
}
