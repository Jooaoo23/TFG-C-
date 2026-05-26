#include <stdio.h>   
#include <conio.h>   
#include <string.h>   
#include <graphics.h> 

// ==========================================
// PROTOTIPOS DE LAS FUNCIONES DEL MENÚ
// ==========================================
// Declaramos las funciones antes de usarlas para que el compilador sepa que existen.
void iniciarModoGrafico();
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);

// ==========================================
// PROTOTIPOS DE LOS CUATRO JUEGOS
// ==========================================
void jugarContinental();
void jugarChinchon();
void jugarTute();
void jugarEscoba();
void prepararPantallaJuego(char* nombreDelJuego); 

// ==========================================
// FUNCIÓN PRINCIPAL (MAIN)
// ==========================================
int main() {
    // Variable para guardar el juego que elija el usuario (1, 2, 3, 4 o 5 para salir)
    int opcionMenu = 0;
    
    // Llamamos a la función que configura y abre la pantalla completa
    iniciarModoGrafico();
    
    // Bucle principal: se ejecuta siempre a menos que se elija la opción 5 (Salir)
    do {
        dibujarMenu();             // Pinta el título y los 4 botones rectangulares
        opcionMenu = controlarMenu(); // Se queda esperando un clic o la tecla ESC
        
        // Si el usuario no ha elegido salir (opción 5), salta al juego seleccionado
        if (opcionMenu != 5) { 
            ejecutarJuego(opcionMenu);
        }
    } while (opcionMenu != 5); // Condición de parada del programa
    
    closegraph(); // Libera la memoria gráfica y cierra la ventana al terminar el bucle
    return 0;
}

// ==========================================
// IMPLEMENTACIÓN DE LAS FUNCIONES DEL MENÚ
// ==========================================

void iniciarModoGrafico() {
    // getmaxwidth() y getmaxheight() detectan la resolución real (píxeles) de tu monitor
    int anchoPantalla = getmaxwidth();
    int altoPantalla = getmaxheight();
    
    // initwindow crea la ventana gráfica. 
    // Los últimos parámetros (false, true) activan el modo "Pantalla completa" (Fullscreen)
    initwindow(anchoPantalla, altoPantalla, (char*)"TFG - Baraja Espanola", 0, 0, false, true);
    
    // graphresult() comprueba si la ventana gráfica se abrió correctamente sin errores
    if (graphresult() != grOk) {
        printf("Error al iniciar el entorno grafico.");
        getch(); // Pausa para leer el error en la consola negra trasera
        exit(1); // Cierra el programa de golpe indicando un fallo
    }
}

void dibujarMenu() {
    cleardevice(); // Borra todo lo que haya en pantalla para dibujar el menú desde cero
    
    // Calculamos el centro exacto de la pantalla dividiendo la resolución total entre 2
    int mitadX = getmaxx() / 2;
    int mitadY = getmaxy() / 2;
    
    // ¡MUY IMPORTANTE!: Hace que las coordenadas de outtextxy apunten al CENTRO del texto, 
    // logrando que las palabras queden perfectamente alineadas dentro de los botones.
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    
    // --- TÍTULO PRINCIPAL ---
    setcolor(WHITE); // Color blanco para el texto y las líneas
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4); // Tipo de fuente por defecto, en horizontal, tamaño 4 (grande)
    outtextxy(mitadX, mitadY - 180, (char*)"BARAJA ESPANOLA"); // Dibuja el texto centrado arriba
    
    // --- DISEÑO DE LOS BOTONES ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); // Bajamos el tamaño de la letra a 2 (mediano) para los botones
    
    // Definimos el tamaño del botón desde su centro: medirá 340 de ancho (170 a cada lado) y 50 de alto (25 arriba y abajo)
    int medioAnchoBoton = 170; 
    int medioAltoBoton = 25;   
    
    // Botón 1: CONTINENTAL
    // rectangle(X_Izquierda, Y_Arriba, X_Derecha, Y_Abajo)
    rectangle(mitadX - medioAnchoBoton, mitadY - 100 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY - 100 + medioAltoBoton);
    outtextxy(mitadX, mitadY - 100, (char*)"CONTINENTAL");
    
    // Botón 2: CHINCHON
    rectangle(mitadX - medioAnchoBoton, mitadY - 30 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY - 30 + medioAltoBoton);
    outtextxy(mitadX, mitadY - 30, (char*)"CHINCHON");
    
    // Botón 3: TUTE
    rectangle(mitadX - medioAnchoBoton, mitadY + 40 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY + 40 + medioAltoBoton);
    outtextxy(mitadX, mitadY + 40, (char*)"TUTE");
    
    // Botón 4: ESCOBA
    rectangle(mitadX - medioAnchoBoton, mitadY + 110 - medioAltoBoton, mitadX + medioAnchoBoton, mitadY + 110 + medioAltoBoton);
    outtextxy(mitadX, mitadY + 110, (char*)"ESCOBA");
    
    // --- PIE DE PÁGINA ---
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); // Letra tamaño 1 (pequeña) para las instrucciones
    outtextxy(mitadX, mitadY + 200, (char*)"Presiona ESC para salir o haz clic en un juego");
}

int controlarMenu() {
    int clickX, clickY; // Variables para almacenar la coordenada X e Y de donde pinche el ratón
    int mitadX = getmaxx() / 2;
    int mitadY = getmaxy() / 2;
    int medioAnchoBoton = 170;
    int medioAltoBoton = 25;

    // Bucle infinito: el programa se queda aquí hasta que el usuario haga una acción válida
    while(1) {
        // CONTROL POR TECLADO (Alternativa por si no se usa el ratón)
        if (kbhit()) { // Si se ha pulsado una tecla...
            if (getch() == 27) { // Compara si esa tecla es ESC (su código ASCII es el 27)
                return 5; // Devuelve un 5 al main para cerrar el programa
            }
        }

        // CONTROL POR RATÓN
        if (ismouseclick(WM_LBUTTONDOWN)) { // Pregunta si se ha detectado un clic izquierdo (LBUTTONDOWN)
            getmouseclick(WM_LBUTTONDOWN, clickX, clickY); // "Captura" los píxeles X e Y exactos del clic

            // PRIMERA VALIDACIÓN: ¿El clic está dentro del ancho horizontal (X) que comparten todos los botones?
            if (clickX >= (mitadX - medioAnchoBoton) && clickX <= (mitadX + medioAnchoBoton)) {
                
                // SEGUNDA VALIDACIÓN: Comprobamos la altura (Y) para saber en cuál de los 4 rectángulos cayó el clic
                
                // ¿El clic cayó en la altura de CONTINENTAL?
                if (clickY >= (mitadY - 100 - medioAltoBoton) && clickY <= (mitadY - 100 + medioAltoBoton)) {
                    return 1; // Devuelve 1 para iniciar Continental
                }
                // ¿El clic cayó en la altura de CHINCHON?
                if (clickY >= (mitadY - 30 - medioAltoBoton) && clickY <= (mitadY - 30 + medioAltoBoton)) {
                    return 2; // Devuelve 2 para iniciar Chinchón
                }
                // ¿El clic cayó en la altura de TUTE?
                if (clickY >= (mitadY + 40 - medioAltoBoton) && clickY <= (mitadY + 40 + medioAltoBoton)) {
                    return 3; // Devuelve 3 para iniciar Tute
                }
                // ¿El clic cayó en la altura de ESCOBA?
                if (clickY >= (mitadY + 110 - medioAltoBoton) && clickY <= (mitadY + 110 + medioAltoBoton)) {
                    return 4; // Devuelve 4 para iniciar la Escoba
                }
            }
        }
    }
}

void ejecutarJuego(int opcionSeleccionada) {
    // Un switch-case clásico que actúa como un "desviador de tráfico". 
    // Envía el programa a la función del juego que corresponda según el número recibido.
    switch(opcionSeleccionada) {
        case 1: jugarContinental(); break;
        case 2: jugarChinchon();    break;
        case 3: jugarTute();        break;
        case 4: jugarEscoba();      break;
    }
}

// ==========================================
// ZONA DE DESARROLLO DE LOS JUEGOS
// ==========================================

void prepararPantallaJuego(char* nombreDelJuego) {
    cleardevice(); // Borra el menú principal de la pantalla
    int mitadX = getmaxx() / 2;
    int mitadY = getmaxy() / 2;
    
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(WHITE);
    
    // Dibuja un título superior en la nueva pantalla con el nombre del juego que le pasemos
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(mitadX, mitadY - 100, nombreDelJuego);
}

void jugarContinental() {
    // Llama a la función de arriba pasando el título para limpiar la pantalla
    prepararPantallaJuego((char*)"MESA DE CONTINENTAL");
    
    // Mensajes informativos provisionales en la pantalla del juego
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"[Espacio para programar la logica de Continental]");
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para regresar al menu...");
    
    getch(); // Detiene el programa aquí. Cuando el usuario pulse una tecla, la función termina y regresa al menú.
}

void jugarChinchon() {
    prepararPantallaJuego((char*)"MESA DE CHINCHON");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"[Espacio para programar la logica de Chinchon]");
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para regresar al menu...");
    
    getch(); // Espera tecla para volver al menú principal
}

void jugarTute() {
    prepararPantallaJuego((char*)"MESA DE TUTE");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"[Espacio para programar la logica de Tute]");
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para regresar al menu...");
    
    getch(); // Espera tecla para volver al menú principal
}

void jugarEscoba() {
    prepararPantallaJuego((char*)"MESA DE LA ESCOBA");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(getmaxx()/2, getmaxy()/2, (char*)"[Espacio para programar la logica de la Escoba]");
    outtextxy(getmaxx()/2, getmaxy()/2 + 100, (char*)"Pulsa cualquier tecla para regresar al menu...");
    
    getch(); // Espera tecla para volver al menú principal
}
