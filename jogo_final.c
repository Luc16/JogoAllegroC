#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "jsmn/jsmn.h"
#include <curl/curl.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include "cena_podio.h"
#include "cena_inicio.h"
#include "cena_jogo.h"

#define LARGURA_TELA 1360
#define ALTURA_TELA 750
#define FPS  60

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte0 = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_FONT *fonte3 = NULL;
ALLEGRO_FONT *fonte4 = NULL;
ALLEGRO_EVENT evento;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_TIMER *temporizador = NULL;

char *plano_de_fundo = "Images/fundo.png";

bool inicializar(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    al_init_font_addon();
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_init_image_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    if (!al_install_audio()){
        return false;
    }

    if (!al_init_acodec_addon()){
        return false;
    }
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    if (!al_install_mouse()){
        al_destroy_display(janela);
        return false;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        al_destroy_display(janela);
        return false;
    }

    al_set_window_title(janela, "Patolindo e Bobola X 2020");

    fonte = al_load_font("Roboto-Regular.ttf", 40, 0);
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
    fonte0 = al_load_font("Roboto-Regular.ttf", 80, 0);
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
    fonte2 = al_load_font("Roboto-Regular.ttf", 30, 0);
    if (!fonte2){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
    fonte3 = al_load_font("Roboto-Regular.ttf", 100, 0);
    if (!fonte3){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }

    fonte4 = al_load_font("Roboto-Regular.ttf", 10, 0);
    if (!fonte4){
        fprintf(stderr, "Falha ao carregar \"fonte2 Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fundo = al_load_bitmap(plano_de_fundo);
    if (!fundo){
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    temporizador = al_create_timer(1.0/FPS);
    if (!temporizador){
        fprintf(stderr, "Falha ao setar temporizador.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }
    musica = al_load_audio_stream("musica_fundo.ogg", 4, 1024);
    if (!musica)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }


    al_register_event_source(fila_eventos, al_get_timer_event_source(temporizador));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playing(musica, true);
    al_start_timer(temporizador);

    return true;
}

int main(){
    int var_inicio = 0, var_podio = 0, var_jogo = 0, num_jogs = 2, pontos = 0;
    bool sair = false;
    if (!inicializar()) return -1;

    while(!sair){
        var_inicio = inicio(janela, musica, fila_eventos, fonte2, fonte0, evento, &num_jogs);
        if (var_inicio != 2){
            var_jogo = jogo(janela, fila_eventos, fundo, fonte3, fonte4, evento, temporizador, num_jogs, &pontos);
//            if (var_jogo != 2)
//                var_podio = podio(pontos, janela, fila_eventos, fonte, evento, temporizador);
        }
        if (var_inicio == 2 || var_jogo == 2 || var_podio == 2)
            sair = true;
    }

    al_destroy_font(fonte);
    al_destroy_font(fonte0);
    al_destroy_font(fonte2);
    al_destroy_font(fonte3);
    al_destroy_font(fonte4);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_audio_stream(musica);
    return 0;
}
