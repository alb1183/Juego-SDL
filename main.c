#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"
#include <sys\timeb.h>

// TDAs
#include "Pantalla.h"
#include "Vector.h"
#include "Listas.h"
#include "Puntuaciones.h"
#include "Particulas.h"
#include "Sierpinski.h"
#include "Buscaminas.h"

// Estructuras
struct Protagonista {
    double x,y;
    char nombre[26];
    int radio,vida,estado,puntos,escudo_s,escudo_c,explosiones,megaexplosiones;
};

// Tipos
typedef struct Protagonista Prota;

// Variables globales
Prota principal;
double mouse_x;
double mouse_y;
//int mov_speed = 3;
int mov_speed = 5; // Velocidad de movimiento del protagonista
int disparo_speed = 6; // Velocidad de la bala
int pantalla_ancho = 848;
int pantalla_alto = 480;
int timer_count = 0;
int dificultad = 0;
int juego = 0; // Variable de juego(indica en que juego se esta)
int partida_estado = -1; // Variable del estado del juego actual
int partida_modo = 0; // Modo de juego
int menu_status = 0; // Opcion del menu seleccionada
int escudo_permitido = 1;

// Constantes
const int menu_num = 5; // Numero de opciones del menu
char* menu_opciones[5] = { "Jugar", "Lateral", "Sandbox", "Buscaminas" ,"Salir" };
// Direcciones de los audios
const char* data_music_main = "Datos/Musica/main.wav";
//const char* data_music_buscaminas = "Datos/Musica/buscaminas.wav";
const char* data_music_buscaminas = "Datos/Musica/main.wav";
const char* data_music_menu = "Datos/Musica/menu.wav";
const char* data_sound_disparo = "Datos/Sonidos/beep38.wav";
const char* data_sound_explosion = "Datos/Sonidos/beep5.wav";
const char* data_sound_habilidad = "Datos/Sonidos/metal_impact.wav";
const char* data_sound_golpe = "Datos/Sonidos/knife_impact_stab_head.wav";
const char* data_sound_vida = "Datos/Sonidos/impact_wrench_removing_nut.wav";
const char* data_sound_escudo = "Datos/Sonidos/snowball_hit_wall.wav";

// Audios
Mix_Music *gMusicMain = NULL;
Mix_Music *gMusicBuscaminas = NULL;
Mix_Music *gMusicMenu = NULL;
Mix_Chunk *sound_disparo = NULL;
Mix_Chunk *sound_explosion = NULL;
Mix_Chunk *sound_habilidad = NULL;
Mix_Chunk *sound_vida = NULL;
Mix_Chunk *sound_escudo = NULL;
Mix_Chunk *sound_golpe = NULL;

void inicializar_juego() {
    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Inicializo el protagonista
    principal.x = pantalla_ancho/2;
    principal.y = pantalla_alto/2;
    principal.radio = 34;
    principal.vida = 8;
    principal.explosiones = 5;
    principal.megaexplosiones = 3;
    principal.puntos = 0;
    principal.estado = 1;
    principal.escudo_c = 400;
    principal.escudo_s = 0;

    //Inicializo el SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2 * 1024 ) < 0 ) {
        printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    //Cargo la musica
    gMusicMain = Mix_LoadMUS(data_music_main);
    if( gMusicMain == NULL ) {
        printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    gMusicBuscaminas = Mix_LoadMUS(data_music_buscaminas);
    if( gMusicBuscaminas == NULL ) {
        printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    gMusicMenu = Mix_LoadMUS(data_music_menu);
    if( gMusicMenu == NULL ) {
        printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    //Cargo los efectos
    sound_disparo = Mix_LoadWAV(data_sound_disparo);
    if( sound_disparo == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    sound_explosion = Mix_LoadWAV(data_sound_explosion);
    if( sound_explosion == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    sound_habilidad = Mix_LoadWAV(data_sound_habilidad);
    if( sound_habilidad == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    sound_golpe = Mix_LoadWAV(data_sound_golpe);
    if( sound_golpe == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    sound_vida = Mix_LoadWAV(data_sound_vida);
    if( sound_vida == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }
    sound_escudo = Mix_LoadWAV(data_sound_escudo);
    if( sound_escudo == NULL ) {
            printf( "SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_PlayMusic( gMusicMenu, -1 );
}

void cerrar_juego() {
    //Libero los efectos
    Mix_FreeChunk(sound_explosion);
    Mix_FreeChunk(sound_disparo);
    sound_disparo = NULL;
    sound_explosion = NULL;

    //Libero la musica
    Mix_FreeMusic( gMusicMenu );
    Mix_FreeMusic( gMusicBuscaminas );
    Mix_FreeMusic( gMusicMain );
    gMusicMenu = NULL;
    gMusicBuscaminas = NULL;
    gMusicMain = NULL;

    // Libero la memoria
    Puntuaciones_liberar();
}

// Funcion para hacer un vector unitario entre el prota y el mouse
Vector mouse_vector() {
    Vector inicial = Vector_new(principal.x, principal.y);
    Vector fin;
    //fin.x = (mouse_x > principal.x) ? mouse_x : principal.x;
    if(partida_modo == 2 && mouse_x < principal.x) {
         fin = Vector_new(principal.x, mouse_y);
    }else{
         fin = Vector_new(mouse_x, mouse_y);
    }
    return Vector_unitary(Vector_points2vector(inicial, fin));
}

// Funcion encargada de dibujar el triangulito del prota
void draw_prota() {
    Vector mouse_v_1 = mouse_vector();
    Vector mouse_v_2 = Vector_rotate(mouse_v_1, 120);
    Vector mouse_v_3 = Vector_rotate(mouse_v_1, -120);

    int size = 20;
    double size_punta = 1.7;
    double p1_x = principal.x + (Vector_get(mouse_v_1, 0) * size * size_punta);
    double p1_y = principal.y + (Vector_get(mouse_v_1, 1) * size * size_punta);
    double p2_x = principal.x + (Vector_get(mouse_v_2, 0) * size);
    double p2_y = principal.y + (Vector_get(mouse_v_2, 1) * size);
    double p3_x = principal.x + (Vector_get(mouse_v_3, 0) * size);
    double p3_y = principal.y + (Vector_get(mouse_v_3, 1)* size);

    Pantalla_ColorRelleno(0,255,0, 255);
    Pantalla_DibujaTriangulo(p1_x, p1_y, p2_x, p2_y, p3_x, p3_y);
}

// Funcion encargada de recorrer, dibujar, actualizar y detectar colisiones de las balas
void disparos(Listas_NodoPtr q, Listas_NodoPtr e) {
    while(Listas_siguiente(q) != NULL) {
        Pantalla_ColorRelleno(255,0,0, 255);

        // Obtengo los valores
        double radio = Listas_obtener_radio(q);
        double pos_x = Listas_obtener_x(q);
        double pos_y = Listas_obtener_y(q);
        double pos_vel_x = Listas_obtener_vel_x(q);
        double pos_vel_y = Listas_obtener_vel_y(q);
        //elemento->dat += 0.2;
        //Pantalla_DibujaCirculo(pos_x, pos_y+sin(elemento->dat)*10, radio);
        // Lo dibujo
        Pantalla_DibujaCirculo(pos_x, pos_y, radio);
        // Lo muevo
        Listas_cambiar_x(q, pos_x + pos_vel_x);
        Listas_cambiar_y(q, pos_y + pos_vel_y);

        // Si se sale de la pantalla
        if(pos_x > pantalla_ancho-radio-10 || pos_x < radio+10 || pos_y > pantalla_alto-radio-10 || pos_y < radio+10) {
            Listas_SuprimeNodo(q);
        } else {
            Listas_NodoPtr r = e;
            int choque = 0;
            // Busqueda de colisiones
            while(Listas_siguiente(r) != NULL && !choque) {
                double radio_e = Listas_obtener_radio(r);
                double pos_x_e = Listas_obtener_x(r);
                double pos_y_e = Listas_obtener_y(r);
                double distancia = sqrt(pow(pos_x_e-pos_x, 2) + pow(pos_y_e-pos_y, 2));
                if(distancia < radio + radio_e) {
                    Listas_SuprimeNodo(r);
                    Listas_SuprimeNodo(q);
                    choque = 1;
                    principal.puntos++;
                    Mix_PlayChannel( -1, sound_explosion, 0 );
                } else {
                    r = Listas_siguiente(r);
                }
            }
            if(!choque)
                q = Listas_siguiente(q);
        }
    }
}

// Funcion encargada de recorrer, dibujar, actualizar y detectar colisiones de los enemigos
void draw_enemigos(Listas_NodoPtr q) {
    while(Listas_siguiente(q) != NULL) {
        // Obtengo los valores
        double radio = Listas_obtener_radio(q);
        double pos_x = Listas_obtener_x(q);
        double pos_y = Listas_obtener_y(q);
        int estado = Listas_obtener_estado(q);
        double pos_vel_x = Listas_obtener_vel_x(q);
        double pos_vel_y = Listas_obtener_vel_y(q);
        // Color segun el tipo de enemigo
        if(estado == 1)
            Pantalla_ColorRelleno(230,100,0, 255);
        else if(estado == 2)
            Pantalla_ColorRelleno(0,150,230, 255);
        else if(estado == 3)
            Pantalla_ColorRelleno(225,255,0, 255);
        else if(estado == 4)
            Pantalla_ColorRelleno(190,0,250, 255);
        Pantalla_DibujaCirculo(pos_x, pos_y, radio);
        Listas_cambiar_x(q, pos_x + pos_vel_x);
        Listas_cambiar_y(q, pos_y + pos_vel_y);
        // Tipo de enemigo que persigue al prota
        if(estado == 4) {
            Vector inicial = Vector_new(pos_x, pos_y);
            Vector fin = Vector_new(principal.x, principal.y);
            Vector enemigo_v = Vector_unitary(Vector_points2vector(inicial, fin));
            double vel = 2 + (dificultad/10);
            Listas_cambiar_vel_x(q, Vector_get(enemigo_v, 0) * vel);
            Listas_cambiar_vel_y(q, Vector_get(enemigo_v, 1) * vel);
        }
        // Distancia del objeto al prota
        double distancia = sqrt(pow(pos_x-principal.x, 2) + pow(pos_y-principal.y, 2));
        if(pos_x > pantalla_ancho-radio || pos_x < radio || pos_y > pantalla_alto-radio || pos_y < radio || distancia < (principal.radio+radio)) {
            Listas_SuprimeNodo(q);
            if(distancia < (principal.radio+radio)) {
                if(estado == 2) {
                    principal.vida++;
                    Mix_PlayChannel( -1, sound_vida, 0 );
                }if(estado == 3) {
                    principal.explosiones++;
                    Mix_PlayChannel( -1, sound_vida, 0 );
                }else{
                    if(!principal.escudo_s) {
                        principal.vida--;
                        Mix_PlayChannel( -1, sound_golpe, 0 );
                        if(principal.vida == 0)
                            partida_estado = 2;
                    }else{
                        Mix_PlayChannel( -1, sound_escudo, 0 );
                    }
                }
            }
        } else {
            q = Listas_siguiente(q);
        }
    }
}

// Funcion para crear un disparo (lo crea en la direccion del mouse)
void crear_disparo(Listas_NodoPtr Cadena_disparos, double x, double y) {
    Vector mouse_v = mouse_vector();
    int size = 20;
    double size_punta = 2;
    double pos_x = principal.x + (Vector_get(mouse_v, 0) * size * size_punta);
    double pos_y = principal.y + (Vector_get(mouse_v, 1) * size * size_punta);

    double vel = disparo_speed;
    double vel_x = Vector_get(mouse_v, 0) * vel;
    double vel_y = Vector_get(mouse_v, 1) * vel;

    Listas_Insertar_crear(Cadena_disparos, pos_x, pos_y, vel_x, vel_y, 3, 0, 1);
}

// Funcion para crear el enemigo
void crear_enemigo_pos(Listas_NodoPtr p) {
    int estado;
    double radio;
    if(round(rand()%15) == 1) {
        estado = 2;
        radio = 5;
    }else if(round(rand()%20) == 1) {
        estado = 3;
        radio = 5;
    }else if(round(rand()%4) == 1) {
        estado = 4;
        radio = 15;
    }else{
        estado = 1;
        radio = 15;
    }

    int random_selector = (partida_modo == 0) ? rand() % 4 : 1;
    int inicial_x = 0;
    int inicial_y = 0;
    if(random_selector == 0) {
        inicial_x = radio+1;
        inicial_y = (radio+1) + (rand()%pantalla_alto-radio*2-1);
    }else if(random_selector == 1) {
        inicial_x = pantalla_ancho-radio-1;
        inicial_y = (radio+1) + (rand()%pantalla_alto-radio*2-1);
    }else if(random_selector == 2) {
        inicial_x = (radio+1) + (rand()%pantalla_ancho-radio*2-1);
        inicial_y = radio+1;
    }else if(random_selector == 3) {
        inicial_x = (radio+1) + (rand()%pantalla_ancho-radio*2-1);
        inicial_y = pantalla_alto-radio-1;
    }

    Vector inicial = Vector_new(inicial_x, inicial_y);
    Vector fin = Vector_new(principal.x, principal.y);
    Vector enemigo_v = Vector_unitary(Vector_points2vector(inicial, fin));

    double vel = 2 + (dificultad/10);
    double vel_x = Vector_get(enemigo_v, 0) * vel;
    double vel_y = Vector_get(enemigo_v, 1) * vel;

    Listas_Insertar_crear(p, inicial_x, inicial_y, vel_x, vel_y, radio, 0, estado);
}

// Funcion para dibujar los datos principales en pantalla
void draw_info() {
    char string_puntos[16];
    char string_vida[18];
    char string_explosiones[18];
    char string_megaexplosiones[24];
    sprintf(string_puntos, "Puntuacion: %d", principal.puntos);
    sprintf(string_vida, "Vida: %d", principal.vida);
    sprintf(string_explosiones, "Explosiones: %d", principal.explosiones);
    sprintf(string_megaexplosiones, "Megaxplosiones: %d", principal.megaexplosiones);
    Pantalla_DibujaTexto(string_puntos, 10, 10);
    Pantalla_DibujaTexto(string_vida, 10, 25);
    Pantalla_DibujaTexto(string_explosiones, 10, 40);
    Pantalla_DibujaTexto(string_megaexplosiones, 10, 55);
    Pantalla_DibujaTexto("Escudo: ", 10, 70);
    double color_change_r;
    double color_change_g;
    if(principal.escudo_c < 200) {
        color_change_r = 255;
        color_change_g = (principal.escudo_c / 200.0) * 255.0;
    }else{
        color_change_r = 255-(((principal.escudo_c-200) / 200.0) * 255.0);
        color_change_g = 255;
    }
    Pantalla_ColorRelleno(color_change_r, color_change_g, 0, 255);
    Pantalla_DibujaRectangulo(75, 70, 5+((principal.escudo_c / 400.0) * 95.0), 15);
}

// Funcion para crear un enemigo(condicional)
void crear_enemigo(Listas_NodoPtr p, int count) {
    if(count == 0) {
        crear_enemigo_pos(p);
    }
}

// Funcion para dibujar el cursor del mouse
void draw_mouse(double x, double y) {
        Pantalla_ColorRelleno(0,0,255, 255);
        Pantalla_DibujaCirculo(x, y, 2);
}

// Funcion para administrar las habilidades
void habilidades(Listas_NodoPtr Cadena_disparos, int tipo) {
    if(tipo == 1) {
        if(principal.explosiones > 0) {
            for(int i = 1; i <= 24; i++) {
                int size = principal.radio;
                double factor_x = cos(M_PI*i/12);
                double factor_y = sin(M_PI*i/12);
                double pos_x = principal.x + (factor_x * size);
                double pos_y = principal.y + (factor_y * size);

                double vel = disparo_speed;
                double vel_x = factor_x * vel;
                double vel_y = factor_y * vel;

                Listas_Insertar_crear(Cadena_disparos, pos_x, pos_y, vel_x, vel_y, 3, 0, 1);
            }
            if(partida_modo != 1)
                principal.explosiones--;
            Mix_PlayChannel( -1, sound_habilidad, 0 );
        }
    }else if(tipo == 2) {
        if(principal.megaexplosiones > 0) {
            for(int k = 0; k <= 1; k++) {
                for(int j = 1; j <= 5; j++) {
                    for(int i = 1; i <= 24; i++) {
                        int size = principal.radio;
                        double factor_x = cos(M_PI*i/12);
                        double factor_y = sin(M_PI*i/12);
                        double pos_x = principal.x + ((40*j - 120) * (k-1)) +(factor_x * size);
                        double pos_y = principal.y + ((40*j - 120) * k) + (factor_y * size);

                        double vel = disparo_speed;
                        double vel_x = factor_x * vel;
                        double vel_y = factor_y * vel;

                        Listas_Insertar_crear(Cadena_disparos, pos_x, pos_y, vel_x, vel_y, 3, 0, 1);
                    }
                }
            }
            if(partida_modo != 1)
                principal.megaexplosiones--;
            Mix_PlayChannel( -1, sound_habilidad, 0 );
        }
    }
}

// Funcion para activar el escudo
void escudo_run() {
    if(principal.escudo_s) {
        Pantalla_ColorRelleno(0,225,255, 100);
        Pantalla_DibujaCirculo(principal.x, principal.y, principal.radio);
    }else if(principal.escudo_c < 400)
        principal.escudo_c++;
}

// Funcion principal
int main(int argc, char **argv)
{
    srand(time(NULL));
    // Creo el contador de tiempo
    struct timeb start, end;
    int diff;

    printf("Escribe tu nombre: ");
    scanf("%s", principal.nombre);
    printf("\n");

    Pantalla_Crea("Prueba", pantalla_ancho,pantalla_alto, 0);
    Pantalla_ColorTrazo(0,0,255, 255);
    Pantalla_ColorRelleno(0,255,0, 255);

    int terminado = 0;
    int mouse_pulsado = 0;
    int tecla_pulsada = 0;
    inicializar_juego();
    Listas_NodoPtr Cadena_disparos = Listas_crear();
    Listas_NodoPtr Cadena_enemigos = Listas_crear();
    Particulas_crea(pantalla_alto, pantalla_ancho);
    Sierpinski_TrianguloPtr Sierpinski_1 = Sierpinski_crear_triangulo(NULL,
                                                                      Sierpinski_Punto_crea(4, 460),
                                                                      Sierpinski_Punto_crea(224, 20),
                                                                      Sierpinski_Punto_crea(444, 460), 1);
    Sierpinski_TrianguloPtr Sierpinski_2 = Sierpinski_crear_triangulo(NULL,
                                                                      Sierpinski_Punto_crea(844, 20),
                                                                      Sierpinski_Punto_crea(624, 460),
                                                                      Sierpinski_Punto_crea(404, 20), 1);

    while (Pantalla_Activa() && !terminado) {
        Pantalla_DibujaRellenoFondo(180, 180, 180, 255);
        Pantalla_RatonCoordenadas(&mouse_x, &mouse_y);
        ftime(&start); // Inicio el cronometro

        if(juego == 0) {
            // Dibujo el fondo
            Pantalla_ColorTrazo(0, 0, 0, 0);
            int scale = 4;
            for(int i = 0; i < pantalla_alto/scale; i++) {
                for(int j = 0; j < pantalla_ancho/scale; j++) {
                    //Pantalla_ColorRelleno(100, 130, 100, (i*i*j*j)%256);
                    Pantalla_ColorRelleno(150, 150, 150, (i*i*j*j)%256);
                    Pantalla_DibujaRectangulo(j*scale,i*scale, scale,scale);
                }
            }

            //Menu
            Pantalla_ColorTrazo(0,0,255, 255);
            Pantalla_DibujaTexto("Menu", 300, 30);
            Pantalla_DibujaLinea(295, 45, 340, 45);

            // Fractal
            Sierpinski_grow_triangulo(Sierpinski_1, Sierpinski_1);
            Sierpinski_grow_triangulo(Sierpinski_2, Sierpinski_2);

            // Algoritmo para el menu
            for(int i = 0; i < menu_num; i++) {
                if(menu_status == i)
                    Pantalla_ColorTrazo(255,0,0, 255);
                else
                    Pantalla_ColorTrazo(0,0,255, 255);

                Pantalla_DibujaTexto(menu_opciones[i], 310+(i*10), 50 + (18*i));
            }
            // Control del menu
            if(Pantalla_TeclaPulsada(SDL_SCANCODE_UP) || Pantalla_TeclaPulsada(SDL_SCANCODE_W)) {
                if(!tecla_pulsada)
                    menu_status = (menu_status == 0) ? menu_num-1 : menu_status-1;
                tecla_pulsada = 1;
            }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_DOWN) || Pantalla_TeclaPulsada(SDL_SCANCODE_S)) {
                if(!tecla_pulsada)
                    menu_status = (menu_status == menu_num-1) ? 0 : menu_status+1;
                tecla_pulsada = 1;
            }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_RETURN) || Pantalla_TeclaPulsada(SDL_SCANCODE_SPACE)) {
                if(!tecla_pulsada) {
                        // Selector de opciones
                        switch(menu_status) {
                           case 0:
                              juego = 1; // Jugar
                              partida_estado = 0;
                              partida_modo = 0;
                              Mix_PlayMusic( gMusicMain, -1 );
                              break;
                           case 1:
                              juego = 1; // Lateral
                              partida_estado = 0;
                              partida_modo = 2;
                              principal.x = principal.radio;
                              Mix_PlayMusic( gMusicMain, -1 );
                              break;
                           case 2: // Sandbox
                              juego = 1;
                              partida_estado = 0;
                              partida_modo = 1;
                              Mix_PlayMusic( gMusicMain, -1 );
                              break;
                           case 3:
                              juego = 2; // Buscaminas
                              partida_estado = 0;
                              partida_modo = 0;
                              Mix_PlayMusic( gMusicBuscaminas, -1 );
                              Buscaminas_iniciar();
                              break;
                           case 4: // Salir
                              terminado = 1;
                              break;
                        }
                }
                tecla_pulsada = 1;
            }else{
                tecla_pulsada = 0;
            }
        } else if(juego == 1) {
            if(partida_estado == 0) {
                // Juego
                if(partida_modo != 1) { // Si no se esta en Sandbox
                    crear_enemigo(Cadena_enemigos, timer_count);
                    if(principal.puntos >= 200)
                        principal.puntos = 199; // TODO: Lo pongo por si acaso
                    dificultad = round(principal.puntos/2);
                    timer_count = (timer_count+1) % (100-dificultad); // Cada ciclo son 15ms
                }


                // Controles - Start
                if(Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)) { // Disparo
                    if(!mouse_pulsado) {
                        crear_disparo(Cadena_disparos, mouse_x, mouse_y);
                        Mix_PlayChannel( -1, sound_disparo, 0 );
                    }

                    mouse_pulsado = 1;
                }else{
                    mouse_pulsado = 0;
                }
                if(Pantalla_RatonBotonPulsado(SDL_BUTTON_RIGHT)) { // Escudo
                    if(escudo_permitido) {
                        if(principal.escudo_c >= 4) {
                            principal.escudo_c -= 4;
                            principal.escudo_s = 1;
                        }else{
                            principal.escudo_s = 0;
                            escudo_permitido = 0;
                        }
                    }
                }else{
                    principal.escudo_s = 0;
                    if(principal.escudo_c >= 100)
                        escudo_permitido = 1;
                }
                // Movimientos
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_LEFT) || Pantalla_TeclaPulsada(SDL_SCANCODE_A)) {
                    if(partida_modo != 2) {
                        principal.x -= mov_speed;
                        if(principal.x < principal.radio)
                            principal.x = principal.radio;
                    }
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_RIGHT) || Pantalla_TeclaPulsada(SDL_SCANCODE_D)) {
                    if(partida_modo != 2) {
                        principal.x += mov_speed;
                        if(principal.x > pantalla_ancho-principal.radio)
                            principal.x = pantalla_ancho-principal.radio;
                    }
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_UP) || Pantalla_TeclaPulsada(SDL_SCANCODE_W)) {
                    principal.y -= mov_speed;
                    if(principal.y < principal.radio)
                        principal.y = principal.radio;
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_DOWN) || Pantalla_TeclaPulsada(SDL_SCANCODE_S)) {
                    principal.y += mov_speed;
                    if(principal.y > pantalla_alto-principal.radio)
                        principal.y = pantalla_alto-principal.radio;
                }

                // Teclas trampa xD
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_Z)) {
                    principal.puntos += 5;
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_X)) {
                    principal.explosiones += 1;
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_C)) {
                    principal.vida += 1;
                }
                if (Pantalla_TeclaPulsada(SDL_SCANCODE_V)) {
                    principal.megaexplosiones += 1;
                }

                // Habilidades
                if(Pantalla_TeclaPulsada(SDL_SCANCODE_Q)) {
                    if(!tecla_pulsada)
                        habilidades(Cadena_disparos, 1);
                    tecla_pulsada = 1;
                }else if(Pantalla_TeclaPulsada(SDL_SCANCODE_E)) {
                    if(!tecla_pulsada)
                        habilidades(Cadena_disparos, 2);
                    tecla_pulsada = 1;
                }else{
                    tecla_pulsada = 0;
                }
                // Controles - End

                // Comprobaciones - Start
                escudo_run();
                for(int i = 0; i < 5; i++) // Creo 5 particulas por ciclo
                    Particulas_insertar(0, principal.puntos);
                disparos(Cadena_disparos, Cadena_enemigos);
                if(principal.puntos >= 200 && partida_modo != 1) // Si no se esta en Sandbox y se ha ganado
                    partida_estado = 1;
                // Comprobaciones - End


                // Dibujado
                draw_prota();
                draw_info();
                Particulas_dibuja(principal.puntos);
                if(partida_modo != 1)
                    draw_enemigos(Cadena_enemigos);

            } else if(partida_estado == 1) {
                // Juego Ganado
                Puntuaciones_escribir(principal.nombre, principal.puntos);
                Pantalla_DibujaTexto("Has ganado!!", pantalla_ancho/2-80, pantalla_alto/2-5);
            } else if(partida_estado == 2) {
                // Juego Perdido
                Puntuaciones_escribir(principal.nombre, principal.puntos);
                Puntuaciones_leer(); // TODO: Leo las puntuaciones
                Pantalla_ColorTrazo(0,0,255, 255);
                Pantalla_DibujaTexto("Has perdido!! D:", pantalla_ancho/2-80, 30);
                char string_puntos[32];
                sprintf(string_puntos, "Puntuacion: %d", principal.puntos);
                Pantalla_DibujaTexto(string_puntos, pantalla_ancho/2-80, 45);
                Pantalla_DibujaTexto("Puntuaciones de otros jugadores:", pantalla_ancho/2-85, 70);

                for(int i = 0; i < Puntuaciones_numero(); i++) {
                    char string_puntos_t[32];
                    if(strcmp(Puntuaciones_array_nombre(i), principal.nombre) == 0)
                        Pantalla_ColorTrazo(255,60,30, 255);
                    else
                        Pantalla_ColorTrazo(30,150,255, 255);
                    sprintf(string_puntos_t, "%s: %d", Puntuaciones_array_nombre(i), Puntuaciones_array_puntos(i));
                    Pantalla_DibujaTexto(string_puntos_t, pantalla_ancho/2-80, 90 + (i*15));

                }

            }
        }else if(juego == 2) {
            // Buscaminas (TDA)
            Buscaminas_loop();
            Buscaminas_draw();
        }

        if (Pantalla_TeclaPulsada(SDL_SCANCODE_ESCAPE)) { // Salir
            //juego = 0;
            terminado = 1;
        }

        draw_mouse(mouse_x, mouse_y); // Dibujo el cursor

        ftime(&end); // Paro el cronometro
        diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm)); // Calculo el retardo
        int diff_p = (diff <= 15) ? diff : 15;
        double fps_t = 1000.0/(diff + (15-diff_p)); // Contador de fps variables
        char counter[4];
        char fps[16];
        sprintf(counter, "%u", diff);
        sprintf(fps, "%.1f", fps_t);
        Pantalla_ColorTrazo(0,0,255, 255);
        Pantalla_DibujaTexto(counter, pantalla_ancho-20, 5);
        Pantalla_DibujaTexto(fps, pantalla_ancho-40, 20);

        Pantalla_Actualiza();
        Pantalla_Espera(15-diff_p);
    }
    // Liberar y salir
    Pantalla_Libera();
    cerrar_juego();
    Listas_liberar(Cadena_disparos);
    Listas_liberar(Cadena_enemigos);
    Particulas_libera();

    return 0;
}

