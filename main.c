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
#include "Puntos.h"
#include "Particulas.h"
#include "Sierpinski.h"

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
int mov_speed = 3;
int pantalla_ancho = 848;
int pantalla_alto = 480;
int timer_count = 0;
int dificultad = 0;
int partida_estado = -1;
int partida_modo = 0;
int menu_status = 0;
int escudo_permitido = 1;

// Constantes
const int menu_num = 3;
char* menu_opciones[3] = { "Jugar", "Sandbox" ,"Salir" };
const char* data_music_main = "Datos/Musica/main.wav";
const char* data_music_menu = "Datos/Musica/menu.wav";
const char* data_sound_disparo = "Datos/Sonidos/beep38.wav";
const char* data_sound_explosion = "Datos/Sonidos/beep5.wav";
const char* data_sound_habilidad = "Datos/Sonidos/metal_impact.wav";
const char* data_sound_golpe = "Datos/Sonidos/knife_impact_stab_head.wav";
const char* data_sound_vida = "Datos/Sonidos/impact_wrench_removing_nut.wav";
const char* data_sound_escudo = "Datos/Sonidos/snowball_hit_wall.wav";

// Audios
Mix_Music *gMusicMain = NULL;
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
    Mix_FreeMusic( gMusicMain );
    gMusicMenu = NULL;
    gMusicMain = NULL;

    // Libero la memoria
    liberar_puntos();
}

Vector mouse_vector() {
    Vector inicial = vector_new(principal.x, principal.y);
    Vector fin = vector_new(mouse_x, mouse_y);
    //fin.x = (mouse_x > principal.x) ? mouse_x : principal.x;
    return unitary_vector(points2vector(inicial, fin));
}

void draw_prota() {
    Vector mouse_v_1 = mouse_vector();
    Vector mouse_v_2 = vector_rotate(mouse_v_1, 120);
    Vector mouse_v_3 = vector_rotate(mouse_v_1, -120);

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


void disparos(NodoPtr q, NodoPtr e) {
    while(q->Sig != NULL) {
        Pantalla_ColorRelleno(255,0,0, 255);
        double radio = q->Sig->Elemento.radio;
        double pos_x = q->Sig->Elemento.x;
        double pos_y = q->Sig->Elemento.y;
        Pantalla_DibujaCirculo(pos_x, pos_y, radio);
        q->Sig->Elemento.x += q->Sig->Elemento.vel_x;
        q->Sig->Elemento.y += q->Sig->Elemento.vel_y;

        // Salidas
        if(pos_x > pantalla_ancho-radio-10 || pos_x < radio+10 || pos_y > pantalla_alto-radio-10 || pos_y < radio+10) {
            SuprimeNodo(q);
        } else {
            NodoPtr r = e;
            int choque = 0;
            while(r->Sig != NULL && !choque) {
                double radio_e = r->Sig->Elemento.radio;
                double pos_x_e = r->Sig->Elemento.x;
                double pos_y_e = r->Sig->Elemento.y;
                double distancia = sqrt(pow(pos_x_e-pos_x, 2) + pow(pos_y_e-pos_y, 2));
                if(distancia < radio + radio_e) {
                    SuprimeNodo(r);
                    SuprimeNodo(q);
                    choque = 1;
                    principal.puntos++;
                    Mix_PlayChannel( -1, sound_explosion, 0 );
                } else {
                    r = r->Sig;
                }
            }
            if(!choque)
                q = q->Sig;
        }
    }
}

void draw_enemigos(NodoPtr q) {
    while(q->Sig != NULL) {
        double radio = q->Sig->Elemento.radio;
        double pos_x = q->Sig->Elemento.x;
        double pos_y = q->Sig->Elemento.y;
        double estado = q->Sig->Elemento.estado;
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
        q->Sig->Elemento.x += q->Sig->Elemento.vel_x;
        q->Sig->Elemento.y += q->Sig->Elemento.vel_y;
        // Tipo de enemigo que persigue al prota
        if(estado == 4) {
            Vector inicial = vector_new(q->Sig->Elemento.x, q->Sig->Elemento.y);
            Vector fin = vector_new(principal.x, principal.y);
            Vector enemigo_v = unitary_vector(points2vector(inicial, fin));
            double vel = 2 + (dificultad/10);
            q->Sig->Elemento.vel_x = Vector_get(enemigo_v, 0) * vel;
            q->Sig->Elemento.vel_y = Vector_get(enemigo_v, 1) * vel;
        }
        // Distancia del objeto al prota
        double distancia = sqrt(pow(pos_x-principal.x, 2) + pow(pos_y-principal.y, 2));
        if(pos_x > pantalla_ancho-radio || pos_x < radio || pos_y > pantalla_alto-radio || pos_y < radio || distancia < (principal.radio+radio)) {
            SuprimeNodo(q);
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
            q = q->Sig;
        }
    }
}

Entidad crear_disparo(double x, double y) {
    Entidad disparo;
    disparo.estado = 1;
    disparo.radio = 3;

    Vector mouse_v = mouse_vector();
    int size = 20;
    double size_punta = 2;
    disparo.x = principal.x + (Vector_get(mouse_v, 0) * size * size_punta);
    disparo.y = principal.y + (Vector_get(mouse_v, 1) * size * size_punta);

    double vel = 4;
    disparo.vel_x = Vector_get(mouse_v, 0) * vel;
    disparo.vel_y = Vector_get(mouse_v, 1) * vel;

    return disparo;
}

Entidad crear_enemigo_pos() {
    Entidad enemigo;
    if(round(rand()%15) == 1) {
        enemigo.estado = 2;
        enemigo.radio = 5;
    }else if(round(rand()%20) == 1) {
        enemigo.estado = 3;
        enemigo.radio = 5;
    }else if(round(rand()%4) == 1) {
        enemigo.estado = 4;
        enemigo.radio = 15;
    }else{
        enemigo.estado = 1;
        enemigo.radio = 15;
    }

    int random_selector =  rand() % 4;
    int inicial_x = 0;
    int inicial_y = 0;
    if(random_selector == 0) {
        inicial_x = enemigo.radio+1;
        inicial_y = (enemigo.radio+1) + (rand()%pantalla_alto-enemigo.radio*2-1);
    }else if(random_selector == 1) {
        inicial_x = pantalla_ancho-enemigo.radio-1;
        inicial_y = (enemigo.radio+1) + (rand()%pantalla_alto-enemigo.radio*2-1);
    }else if(random_selector == 2) {
        inicial_x = (enemigo.radio+1) + (rand()%pantalla_ancho-enemigo.radio*2-1);
        inicial_y = enemigo.radio+1;
    }else if(random_selector == 3) {
        inicial_x = (enemigo.radio+1) + (rand()%pantalla_ancho-enemigo.radio*2-1);
        inicial_y = pantalla_alto-enemigo.radio-1;
    }

    Vector inicial = vector_new(inicial_x, inicial_y);
    Vector fin = vector_new(principal.x, principal.y);
    Vector enemigo_v = unitary_vector(points2vector(inicial, fin));

    enemigo.x = inicial_x;
    enemigo.y = inicial_y;

    double vel = 2 + (dificultad/10);
    enemigo.vel_x = Vector_get(enemigo_v, 0) * vel;
    enemigo.vel_y = Vector_get(enemigo_v, 1) * vel;

    return enemigo;
}

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

void crear_enemigo(NodoPtr p, int count) {
    if(count == 0) {
        Entidad enemigo = crear_enemigo_pos();
        Insertar_Inicio(p, enemigo);
    }
}

void draw_mouse(double x, double y) {
        Pantalla_ColorRelleno(0,0,255, 255);
        Pantalla_DibujaCirculo(x, y, 2);
}

void habilidades(NodoPtr Cadena_disparos, int tipo) {
    if(tipo == 1) {
        if(principal.explosiones > 0) {
            for(int i = 1; i <= 24; i++) {
                Entidad disparo;
                disparo.estado = 1;
                disparo.radio = 3;

                int size = principal.radio;
                double factor_x = cos(M_PI*i/12);
                double factor_y = sin(M_PI*i/12);
                disparo.x = principal.x + (factor_x * size);
                disparo.y = principal.y + (factor_y * size);

                double vel = 4;
                disparo.vel_x = factor_x * vel;
                disparo.vel_y = factor_y * vel;

                Insertar_Inicio(Cadena_disparos, disparo);
            }
            if(partida_modo == 0)
                principal.explosiones--;
            Mix_PlayChannel( -1, sound_habilidad, 0 );
        }
    }else if(tipo == 2) {
        if(principal.megaexplosiones > 0) {
            for(int k = 0; k <= 1; k++) {
                for(int j = 1; j <= 5; j++) {
                    for(int i = 1; i <= 24; i++) {
                        Entidad disparo;
                        disparo.estado = 1;
                        disparo.radio = 3;

                        int size = principal.radio;
                        double factor_x = cos(M_PI*i/12);
                        double factor_y = sin(M_PI*i/12);
                        disparo.x = principal.x + ((40*j - 120) * (k-1)) +(factor_x * size);
                        disparo.y = principal.y + ((40*j - 120) * k) + (factor_y * size);

                        double vel = 4;
                        disparo.vel_x = factor_x * vel;
                        disparo.vel_y = factor_y * vel;

                        Insertar_Inicio(Cadena_disparos, disparo);
                    }
                }
            }
            if(partida_modo == 0)
                principal.megaexplosiones--;
            Mix_PlayChannel( -1, sound_habilidad, 0 );
        }
    }
}

void escudo_run() {
    if(principal.escudo_s) {
        Pantalla_ColorRelleno(0,225,255, 100);
        Pantalla_DibujaCirculo(principal.x, principal.y, principal.radio);
    }else if(principal.escudo_c < 400)
        principal.escudo_c++;
}

/*
void crear_hojas(RamaPtr r, int direccion) {

    RamaPtr der = malloc(sizeof(struct Rama));
    der->Der = NULL;
    der->Izq = NULL;
    der->Elemento.estado = r->Elemento.estado+1;
    der->Elemento.radio = 10;
    der->Elemento.x = r->Elemento.x + (r->Elemento.vel_x * direccion);
    der->Elemento.y = r->Elemento.y + r->Elemento.vel_y;
    der->Elemento.vel_x = 0;
    der->Elemento.vel_y = 0;
    RamaPtr izq = malloc(sizeof(struct Rama));
    izq->Der = NULL;
    izq->Izq = NULL;
    izq->Elemento.estado = r->Elemento.estado+1;
    izq->Elemento.radio = 10;
    izq->Elemento.x = r->Elemento.x + (r->Elemento.vel_x * direccion);
    izq->Elemento.y = r->Elemento.y + r->Elemento.vel_y;
    izq->Elemento.vel_x = 0;
    izq->Elemento.vel_y = 0;

    //r->Elemento.estado = 1;
    r->Der = der;
    r->Izq = izq;
}

void plantar_arbor(RamaPtr a) {
    a->Elemento.estado = 0;
    a->Elemento.x = pantalla_ancho/2;
    a->Elemento.y = 0;
    a->Elemento.vel_x = 0;
    a->Elemento.vel_y = 0;
    a->Elemento.radio = 0;
    a->Elemento.estado = 0;
    crear_hojas(a, 1);

}

void grow_arbor(RamaPtr a) {
    if(a->Der != NULL) {
        double x = a->Der->Elemento.x;
        double y = a->Der->Elemento.y;
        double radio = a->Der->Elemento.radio;
        int estado = a->Der->Elemento.estado;
        double v_x = a->Der->Elemento.vel_x;
        double v_y = a->Der->Elemento.vel_y;

        if(v_y <= radio){
            a->Der->Elemento.vel_y += 0.9;
            a->Der->Elemento.vel_x += 0.7;//+estado/20;
        }else if(a->Der->Der == NULL)
            crear_hojas(a->Der, 1);

        grow_arbor(a->Der);
    }
    if(a->Izq != NULL) {
        double x = a->Izq->Elemento.x;
        double y = a->Izq->Elemento.y;
        double radio = a->Izq->Elemento.radio;
        int estado = a->Izq->Elemento.estado;
        double v_x = a->Izq->Elemento.vel_x;
        double v_y = a->Izq->Elemento.vel_y;

        if(v_y <= radio){
            a->Izq->Elemento.vel_y += 0.9;
            a->Izq->Elemento.vel_x += 0.7;//+estado/20;
        }else if(a->Izq->Izq == NULL && estado % 3 == 0)
            crear_hojas(a->Izq, -1);

        grow_arbor(a->Izq);
    }
}

void draw_arbor(RamaPtr a) {
    if(a->Der != NULL) {
        double x = a->Der->Elemento.x;
        double y = a->Der->Elemento.y;
        double radio = a->Der->Elemento.radio;
        double estado = a->Der->Elemento.estado;
        double v_x = a->Der->Elemento.vel_x;
        double v_y = a->Der->Elemento.vel_y;
        Pantalla_DibujaLinea(x, y, x+v_x, y+v_y);
        draw_arbor(a->Der);
    }
    if(a->Izq != NULL) {
        double x = a->Izq->Elemento.x;
        double y = a->Izq->Elemento.y;
        double radio = a->Izq->Elemento.radio;
        double estado = a->Izq->Elemento.estado;
        double v_x = a->Izq->Elemento.vel_x;
        double v_y = a->Izq->Elemento.vel_y;
        Pantalla_DibujaLinea(x, y, x-v_x, y+v_y);
        draw_arbor(a->Izq);
    }
}*/

int main(int argc, char **argv)
{
    srand(time(NULL));
    // Time
    struct timeb start, end;
    int diff;

    printf("Escribe tu nombre: ");
    scanf("%s", principal.nombre);
    printf("\n");

    Pantalla_Crea("Prueba", pantalla_ancho,pantalla_alto, 0);
    //Pantalla_ColorTrazo(0,0,255, 255);
    Pantalla_ColorTrazo(0,0,255, 255);
    Pantalla_ColorRelleno(0,255,0, 255);

    int terminado = 0;
    int mouse_pulsado = 0;
    int tecla_pulsada = 0;
    inicializar_juego();
    NodoPtr Cadena_disparos = crear_cadena();
    NodoPtr Cadena_enemigos = crear_cadena();
    ParticulaPtr Cadena_particulas = Particulas_cadena(pantalla_alto, pantalla_ancho);
    //RamaPtr Arbol = crear_arbor();
    TrianguloPtr Sierpinski_1 = crear_triangulo(NULL, Punto_crea(4, 460), Punto_crea(224, 20), Punto_crea(444, 460), 1);
    TrianguloPtr Sierpinski_2 = crear_triangulo(NULL, Punto_crea(844, 20), Punto_crea(624, 460), Punto_crea(404, 20), 1);
    //plantar_arbor(Arbol);

    while (Pantalla_Activa() && !terminado) {
        //Pantalla_DibujaRellenoFondo(255,255,255, 255);
        Pantalla_DibujaRellenoFondo(180, 180, 180, 255);
        Pantalla_RatonCoordenadas(&mouse_x, &mouse_y);
        ftime(&start);


        // Juego iniciado
        if(partida_estado == -1) {
            //Menu
            Pantalla_ColorTrazo(0,0,255, 255);
            Pantalla_DibujaTexto("Menu", 300, 30);
            Pantalla_DibujaLinea(295, 45, 340, 45);

            //grow_arbor(Arbol);
            //draw_arbor(Arbol);
            grow_triangulo(Sierpinski_1, Sierpinski_1);
            grow_triangulo(Sierpinski_2, Sierpinski_2);

            // Algoritmo para el menu
            for(int i = 0; i < menu_num; i++) {
                if(menu_status == i)
                    Pantalla_ColorTrazo(255,0,0, 255);
                else
                    Pantalla_ColorTrazo(0,0,255, 255);

                Pantalla_DibujaTexto(menu_opciones[i], 310+(i*15), 50 + (18*i));
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
                        switch(menu_status) {
                           case 0:
                              partida_estado = 0;
                              partida_modo = 0;
                              Mix_PlayMusic( gMusicMain, -1 );
                              break;
                           case 1:
                              partida_estado = 0;
                              partida_modo = 1;
                              Mix_PlayMusic( gMusicMain, -1 );
                              break;
                           case 2:
                              terminado = 1;
                              break;
                        }
                }
                tecla_pulsada = 1;
            }else{
                tecla_pulsada = 0;
            }

        }else if(partida_estado == 0) {
            // Juego
            if(partida_modo == 0) {
                crear_enemigo(Cadena_enemigos, timer_count);
                if(principal.puntos >= 200)
                    principal.puntos = 199; // TODO: Lo pongo por si acaso
                dificultad = round(principal.puntos/2);
                timer_count = (timer_count+1) % (100-dificultad); // Cada ciclo son 15ms
            }


            // Controles - Start
            if(Pantalla_RatonBotonPulsado(SDL_BUTTON_LEFT)) {
                if(!mouse_pulsado) {
                    Entidad disparo = crear_disparo(mouse_x, mouse_y);
                    Insertar_Inicio(Cadena_disparos, disparo);
                    Mix_PlayChannel( -1, sound_disparo, 0 );
                }

                mouse_pulsado = 1;
            }else{
                mouse_pulsado = 0;
            }
            if(Pantalla_RatonBotonPulsado(SDL_BUTTON_RIGHT)) {
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

            if (Pantalla_TeclaPulsada(SDL_SCANCODE_LEFT) || Pantalla_TeclaPulsada(SDL_SCANCODE_A)) {
                principal.x -= mov_speed;
                if(principal.x < 0)
                    principal.x = 0;
            }
            if (Pantalla_TeclaPulsada(SDL_SCANCODE_RIGHT) || Pantalla_TeclaPulsada(SDL_SCANCODE_D)) {
                principal.x += mov_speed;
                if(principal.x > pantalla_ancho)
                    principal.x = pantalla_ancho;
            }
            if (Pantalla_TeclaPulsada(SDL_SCANCODE_UP) || Pantalla_TeclaPulsada(SDL_SCANCODE_W)) {
                principal.y -= mov_speed;
                if(principal.y < 0)
                    principal.y = 0;
            }
            if (Pantalla_TeclaPulsada(SDL_SCANCODE_DOWN) || Pantalla_TeclaPulsada(SDL_SCANCODE_S)) {
                principal.y += mov_speed;
                if(principal.y > pantalla_alto)
                    principal.y = pantalla_alto;
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
                Particulas_crear(Cadena_particulas, 0, principal.puntos);
            disparos(Cadena_disparos, Cadena_enemigos);
            if(principal.puntos >= 200 && partida_modo == 0)
                partida_estado = 1;
            // Comprobaciones - End


            // Dibujado
            draw_prota();
            draw_info();
            Particulas_dibuja(Cadena_particulas, principal.puntos);
            if(partida_modo == 0)
                draw_enemigos(Cadena_enemigos);
            draw_mouse(mouse_x, mouse_y);

        } else if(partida_estado == 1) {
            // Juego Ganado
            escribir_puntuaciones(principal.nombre, principal.puntos);
            Pantalla_DibujaTexto("Has ganado!!", pantalla_ancho/2-80, pantalla_alto/2-5);
        } else if(partida_estado == 2) {
            // Juego Perdido
            escribir_puntuaciones(principal.nombre, principal.puntos);
            leer_puntuaciones(); // TODO: Leo las puntuaciones
            Pantalla_ColorTrazo(0,0,255, 255);
            Pantalla_DibujaTexto("Has perdido!! D:", pantalla_ancho/2-80, 30);
            char string_puntos[32];
            sprintf(string_puntos, "Puntuacion: %d", principal.puntos);
            //sprintf(string_puntos, "Puntuacion: %d", puntuaciones_num);
            Pantalla_DibujaTexto(string_puntos, pantalla_ancho/2-80, 45);
            Pantalla_DibujaTexto("Puntuaciones de otros jugadores:", pantalla_ancho/2-85, 70);

            for(int i = 0; i < puntos_num(); i++) {
                char string_puntos_t[32];
                //if(strcmp(puntos_array_f[i].nombre, principal.nombre) == 0)
                if(strcmp(puntos_array_nombre(i), principal.nombre) == 0)
                    Pantalla_ColorTrazo(255,60,30, 255);
                else
                    Pantalla_ColorTrazo(30,150,255, 255);
                sprintf(string_puntos_t, "%s: %d", puntos_array_nombre(i), puntos_array_puntos(i));
                //sprintf(string_puntos_t, "%d", puntos_array_f[i].puntos);
                Pantalla_DibujaTexto(string_puntos_t, pantalla_ancho/2-80, 90 + (i*15));

            }

        }

        if (Pantalla_TeclaPulsada(SDL_SCANCODE_ESCAPE)) {
            terminado = 1;
        }

        ftime(&end);
        diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
        int diff_p = (diff <= 15) ? diff : 15;
        double fps_t = 1000.0/(diff + (15-diff_p)); // Contador de fps variables
        char counter[4];
        char fps[16];
        sprintf(counter, "%u", diff);
        sprintf(fps, "%.1f", fps_t);
        Pantalla_ColorTrazo(0,0,255, 255);
        Pantalla_DibujaTexto(counter, pantalla_ancho-20, 5);
        Pantalla_DibujaTexto(fps, pantalla_ancho-40, 20);
        //printf("\nTarda %u milisegundos", diff);
        //Pantalla_Espera(20-diff);

        Pantalla_Actualiza();
        Pantalla_Espera(15-diff_p);
    }
    Pantalla_Libera();
    cerrar_juego();
    liberar_cadena(Cadena_disparos);
    liberar_cadena(Cadena_enemigos);
    Particulas_libera(Cadena_particulas);

    return 0;
}

