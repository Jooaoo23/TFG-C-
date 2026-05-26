#include <stdio.h>    // Entrada/salida estándar
#include <conio.h>    // Control de teclado (kbhit, getch)
#include <string.h>   // Cadenas de texto (strcpy, sprintf)
#include <graphics.h> // Interfaz gráfica (WinBGIm)

// =================================================================
// ESTRUCTURAS DE DATOS PARA EL JUEGO CONTINENTAL
// =================================================================

struct Jugador {
    char nombre[30];              // Nombre del jugador
    int puntosPorMano[7];         // Array que guarda los puntos de cada una de las 7 manos
    int puntosAcumuladosEnMano[7];// Guarda el total acumulado en esa mano para el historial (ej: 84, 202...)
    int puntosTotales;            // Suma final de toda la partida
    bool yaHaPuntuadoManoActual;  // Candado de seguridad por ronda
};

struct PartidaContinental {
    struct Jugador listaJugadores[7]; // Soporte de 1 a 7 jugadores
    int cantidadJugadores;            // Cuántos juegan en esta partida
    int manoActual;                   // Ronda activa (1 a 7)
};

// =================================================================
// PROTOTIPOS DE LAS FUNCIONES
// =================================================================
void iniciarModoGrafico();
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);

void jugarContinental();
void jugarChinchon();
void jugarTute();
void jugarEscoba();

// Subpantallas modulares de Continental
void prepararPantallaJuego(const char* nombreDelJuego); 
void pantallaConfigurarJugadores(struct PartidaContinental *partida);
void pantallaMesaJuego(struct PartidaContinental *partida);
void ventanaEmergentePuntos(struct PartidaContinental *partida, int indiceJugador);
void pantallaResultadosFinales(struct PartidaContinental *partida);

struct PartidaContinental miPartida;

// =================================================================
// MAIN
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

// =================================================================
// LÓGICA DEL MODO DE JUEGO: CONTINENTAL
// =================================================================

void prepararPantallaJuego(const char* nombreDelJuego) {
    cleardevice();
    int mitadX = getmaxx() / 2;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(mitadX, 35, (char*)nombreDelJuego);
}

void jugarContinental() {
    miPartida.cantidadJugadores = 0;
    miPartida.manoActual = 1;      
    
    // Inicializar matriz de puntos a cero
    for(int i=0; i<7; i++) {
        for(int j=0; j<7; j++) {
            miPartida.listaJugadores[i].puntosPorMano[j] = 0;
            miPartida.listaJugadores[i].puntosAcumuladosEnMano[j] = 0;
        }
        miPartida.listaJugadores[i].puntosTotales = 0;
    }
    
    pantallaConfigurarJugadores(&miPartida);
    
    if (miPartida.cantidadJugadores > 0) {
        pantallaMesaJuego(&miPartida);
    }
}

// PANTALLA 1: Ajustes de Jugadores (Añadir/Editar/Eliminar)
void pantallaConfigurarJugadores(struct PartidaContinental *partida) {
    bool salirAjustes = false;
    int clickX, clickY;
    
    while (!salirAjustes) {
        cleardevice(); 
        prepararPantallaJuego("CONTINENTAL");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(WHITE);
        rectangle(100, 100, 350, 150); 
        outtextxy(225, 125, (char*)"ANADIR JUGADOR");
        
        rectangle(getmaxx() - 300, getmaxy() - 100, getmaxx() - 100, getmaxy() - 50);
        outtextxy(getmaxx() - 200, getmaxy() - 75, (char*)"JUGAR");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(200, getmaxy() - 75, (char*)"MAX: 7 JUGADORES");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        for (int i = 0; i < partida->cantidadJugadores; i++) {
            int filaY = 180 + (i * 55); 
            
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            char bufferNombre[50];
            sprintf(bufferNombre, "%s:", partida->listaJugadores[i].nombre);
            outtextxy(100, filaY + 20, bufferNombre); 
            
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            rectangle(350, filaY, 480, filaY + 40);
            outtextxy(415, filaY + 20, (char*)"EDITAR");
            
            rectangle(510, filaY, 660, filaY + 40);
            outtextxy(585, filaY + 20, (char*)"ELIMINAR");
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            
            if (clickX >= 100 && clickX <= 350 && clickY >= 100 && clickY <= 150) {
                if (partida->cantidadJugadores < 7) {
                    char nuevoNombre[30];
                    printf("\n==================================\n");
                    printf("INTRODUCE EL NOMBRE DEL JUGADOR %d: ", partida->cantidadJugadores + 1);
                    scanf("%29s", nuevoNombre);
                    printf("==================================\n");
                    
                    strcpy(partida->listaJugadores[partida->cantidadJugadores].nombre, nuevoNombre);
                    partida->cantidadJugadores++; 
                }
            }
            
            if (clickX >= (getmaxx() - 300) && clickX <= (getmaxx() - 100) && clickY >= (getmaxy() - 100) && clickY <= (getmaxy() - 50)) {
                if (partida->cantidadJugadores > 0) {
                    salirAjustes = true; 
                }
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
                    char nombreEditado[30];
                    printf("\n==================================\n");
                    printf("EDITAR NOMBRE (ACTUAL: %s): ", partida->listaJugadores[i].nombre);
                    scanf("%29s", nombreEditado);
                    printf("==================================\n");
                    strcpy(partida->listaJugadores[i].nombre, nombreEditado);
                    break;
                }
            }
        }
        delay(30); 
    }
}

// PANTALLA 2: Mesa de Juego y Tabla por Rondas (Ref: PantallaContinentalPuntosaAñadirPuntos.png)
void pantallaMesaJuego(struct PartidaContinental *partida) {
    bool terminarMesa = false;
    int clickX, clickY;
    bool mostrarErrorPuntuacion = false;
    
    while (!terminarMesa) {
        cleardevice();
        
        // --- CABECERA DINÁMICA DE LA MANO ACTUAL ---
        char bufferMano[100];
        switch(partida->manoActual) {
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
        outtextxy(getmaxx() / 2, 35, (char*)"CONTINENTAL");
        
        settextjustify(LEFT_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(50, 90, bufferMano); // Texto rojo/blanco de la mano
        
        // --- BOTÓN SUPERIOR: SIGUIENTE MANO ---
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        rectangle(getmaxx() - 280, 70, getmaxx() - 50, 120);
        outtextxy(getmaxx() - 165, 95, (char*)"SIGUIENTE MANO");
        
        // --- DIBUJAR TABLA DE PUNTUACIONES ---
        int anchoColumna = (getmaxx() - 100) / partida->cantidadJugadores;
        line(50, 140, getmaxx() - 50, 140); // Línea horizontal superior de la tabla
        line(50, 180, getmaxx() - 50, 180); // Línea divisoria de cabecera
        
        for (int i = 0; i < partida->cantidadJugadores; i++) {
            int colX = 50 + (i * anchoColumna);
            
            // Pintar Nombre del jugador
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            outtextxy(colX + (anchoColumna / 2), 160, partida->listaJugadores[i].nombre);
            
            // Línea vertical divisoria
            if(i > 0) line(colX, 140, colX, getmaxy() - 150);
            
            // Pintar historial de puntos anotados en las manos anteriores hacia abajo
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
            for (int m = 0; m < partida->manoActual; m++) {
                char bufferPuntos[40];
                if (m == partida->manoActual - 1 && !partida->listaJugadores[i].yaHaPuntuadoManoActual) {
                    sprintf(bufferPuntos, "-"); // No ha introducido puntos aún en esta mano
                } else {
                    if (m == 0) {
                        sprintf(bufferPuntos, "%d", partida->listaJugadores[i].puntosPorMano[m]);
                    } else {
                        // Formato idéntico a vuestro diseño: "PuntosDeMano - SumaAcumulada"
                        sprintf(bufferPuntos, "%d - %d", partida->listaJugadores[i].puntosPorMano[m], partida->listaJugadores[i].puntosAcumuladosEnMano[m]);
                    }
                }
                outtextxy(colX + (anchoColumna / 2), 210 + (m * 35), bufferPuntos);
            }
            
            // --- BOTÓN DE AÑADIR/EDITAR PUNTOS ---
            int btnY = getmaxy() - 130;
            rectangle(colX + 10, btnY, colX + anchoColumna - 10, btnY + 40);
            if (partida->listaJugadores[i].yaHaPuntuadoManoActual) {
                outtextxy(colX + (anchoColumna / 2), btnY + 20, (char*)"EDITAR");
            } else {
                outtextxy(colX + (anchoColumna / 2), btnY + 20, (char*)"ANADIR PUNTOS");
            }
        }
        
        // --- MOSTRAR ALERTA EN CAJA SI FALTA GENTE POR PUNTUAR ---
        if (mostrarErrorPuntuacion) {
            setcolor(LIGHTRED);
            rectangle(getmaxx()/2 - 250, getmaxy() - 80, getmaxx()/2 + 250, getmaxy() - 20);
            outtextxy(getmaxx()/2, getmaxy() - 50, (char*)"FALTAN JUGADORES POR PUNTUAR");
            setcolor(WHITE);
        }

        // CONTROL DE CLICS DE LA MESA
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
            mostrarErrorPuntuacion = false; // Limpia el error al hacer un nuevo clic
            
            // ¿Clic en "SIGUIENTE MANO"?
            if (clickX >= getmaxx() - 280 && clickX <= getmaxx() - 50 && clickY >= 70 && clickY <= 120) {
                bool todosPuntuados = true;
                for (int i = 0; i < partida->cantidadJugadores; i++) {
                    if (!partida->listaJugadores[i].yaHaPuntuadoManoActual) todosPuntuados = false;
                }
                
                if (todosPuntuados) {
                    if (partida->manoActual == 7) {
                        terminarMesa = true; // Fin de la ronda 7, saltamos a los resultados finales
                        pantallaResultadosFinales(partida);
                    } else {
                        partida->manoActual++; // Pasamos de mano limpiamente
                    }
                } else {
                    mostrarErrorPuntuacion = true; // Activa el letrero de advertencia
                }
            }
            
            // Comprobar clics en los botones de añadir puntos de cada columna
            int btnY = getmaxy() - 130;
            for (int i = 0; i < partida->cantidadJugadores; i++) {
                int colX = 50 + (i * anchoColumna);
                if (clickX >= colX + 10 && clickX <= colX + anchoColumna - 10 && clickY >= btnY && clickY <= btnY + 40) {
                    ventanaEmergentePuntos(partida, i);
                }
            }
        }
        delay(30);
    }
}

// VENTANA EMERGENTE (Pop-up): Pide los puntos por consola (Ref: PantallaContinentalPuntosaAñadirPuntos.png -> "APARECE ESTO")
void ventanaEmergentePuntos(struct PartidaContinental *partida, int indiceJugador) {
    // Dibujamos visualmente el recuadro del Pop-up en la pantalla gráfica
    int cX = getmaxx() / 2;
    int cY = getmaxy() / 2;
    
    setfillstyle(SOLID_FILL, BLACK);
    bar(cX - 300, cY - 150, cX + 300, cY + 150);
    rectangle(cX - 300, cY - 150, cX + 300, cY + 150);
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(cX, cY - 100, (char*)"APARECE ESTO");
    
    char bufferPregunta[100];
    sprintf(bufferPregunta, "PUNTOS PARA %s:", partida->listaJugadores[indiceJugador].nombre);
    outtextxy(cX, cY - 20, bufferPregunta);
    outtextxy(cX, cY + 80, (char*)"[Mira la consola negra para escribir]");
    
    // Entrada de datos real
    int puntos = 0;
    printf("\n==================================\n");
    printf("TECLEA LOS PUNTOS DE %s EN LA MANO %d: ", partida->listaJugadores[indiceJugador].nombre, partida->manoActual);
    scanf("%d", &puntos);
    printf("==================================\n");
    
    // Guardar en la estructura
    partida->listaJugadores[indiceJugador].puntosPorMano[partida->manoActual - 1] = puntos;
    
    // Calcular el acumulado de esta mano sumándole el de la mano anterior
    if (partida->manoActual == 1) {
        partida->listaJugadores[indiceJugador].puntosAcumuladosEnMano[0] = puntos;
    } else {
        partida->listaJugadores[indiceJugador].puntosAcumuladosEnMano[partida->manoActual - 1] = 
            partida->listaJugadores[indiceJugador].puntosAcumuladosEnMano[partida->manoActual - 2] + puntos;
    }
    
    partida->listaJugadores[indiceJugador].yaHaPuntuadoManoActual = true; // Validación completada
}

// PANTALLA 3: Resultados, Ganador y Operaciones Matemáticas (Ref: PantallaContinentalPuntosFinal.png)
void pantallaResultadosFinales(struct PartidaContinental *partida) {
    cleardevice();
    prepararPantallaJuego("RESULTADOS FINALES - CONTINENTAL");
    
    // 1. Calcular puntos totales finales de cada uno (acumulado en la última mano)
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        partida->listaJugadores[i].puntosTotales = partida->listaJugadores[i].puntosAcumuladosEnMano[6];
    }
    
    // 2. Encontrar quién tiene MENOS puntos (Regla del Continental: gana el menor)
    int indiceGanador = 0;
    int puntosMinimos = partida->listaJugadores[0].puntosTotales;
    
    for (int i = 1; i < partida->cantidadJugadores; i++) {
        if (partida->listaJugadores[i].puntosTotales < puntosMinimos) {
            puntosMinimos = partida->listaJugadores[i].puntosTotales;
            indiceGanador = i;
        }
    }
    
    // 3. Pintar los resultados y las restas correspondientes en pantalla
    int anchoColumna = (getmaxx() - 100) / partida->cantidadJugadores;
    line(50, 120, getmaxx() - 50, 120);
    
    for (int i = 0; i < partida->cantidadJugadores; i++) {
        int colX = 50 + (i * anchoColumna);
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(colX + (anchoColumna / 2), 150, partida->listaJugadores[i].nombre);
        
        char bufferPuntosFinales[50];
        sprintf(bufferPuntosFinales, "Total: %d pts", partida->listaJugadores[i].puntosTotales);
        outtextxy(colX + (anchoColumna / 2), 200, bufferPuntosFinales);
        
        if (i == indiceGanador) {
            setcolor(LIGHTGREEN);
            outtextxy(colX + (anchoColumna / 2), 260, (char*)"GANADOR!!!");
            setcolor(WHITE);
        } else {
            // Muestra la resta exacta matemática: PuntosJugador - PuntosGanador
            char bufferResta[50];
            sprintf(bufferResta, "%d - %d = %d", partida->listaJugadores[i].puntosTotales, puntosMinimos, partida->listaJugadores[i].puntosTotales - puntosMinimos);
            outtextxy(colX + (anchoColumna / 2), 260, bufferResta);
        }
        
        if(i > 0) line(colX, 120, colX, 320);
    }
    
    line(50, 320, getmaxx() - 50, 320);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx() / 2, getmaxy() - 100, (char*)"Partida Completada de forma reglamentaria.");
    outtextxy(getmaxx() / 2, getmaxy() - 70, (char*)"Presiona cualquier tecla para regresar al Menu Principal...");
    
    getch(); 
}

// =================================================================
// PROTOTIPOS RESTANTES DE LOS OTROS MODOS
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
