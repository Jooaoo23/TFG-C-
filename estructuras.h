#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// =================================================================
// LIBRERÍAS
// =================================================================
#include <stdio.h>    
#include <conio.h>    
#include <string.h>   
#include <graphics.h> 
#include <time.h>     

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
    int puntosEq1[100]; 
    int puntosEq2[100];
    int triunfoEq1[100];            
    int triunfoEq2[100];
    int tantosEq1;
    int tantosEq2;
    int manoActual;                
};

// =================================================================
// VARIABLES GLOBALES (Declaradas como extern)
// =================================================================
extern struct PartidaContinental miPartida;
extern struct PartidaChinchon miPartidaChinchon; 
extern struct PartidaTute miPartidaTute; 
extern int paginaActual; 
extern bool mostrandoErrorContinental;
extern bool mostrandoErrorChinchon;
extern bool tuteEditandoManos;
extern bool tuteEditandoPuntos;
extern bool tuteVolverMenu;
extern int tuteNumEquipoEditando;

// =================================================================
// PROTOTIPOS DE FUNCIONES 
// =================================================================

// Generales
void iniciarModoGrafico();
void actualizarPantalla(); 
void dibujarMenu();
int controlarMenu();
void ejecutarJuego(int opcionSeleccionada);
void leerTextoGrafico(int x, int y, char* resultado, int maxLongitud, int pantallaOrigen);
void guardarEnHistorial(const char* juego, const char* ganador);
void mostrarPantallaHistorial();
void prepararPantallaJuego(const char* nombreDelJuego);

// Módulo 1: Continental
void jugarContinental();
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

#endif // ESTRUCTURAS_H
