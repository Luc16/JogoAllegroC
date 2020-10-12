#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "jsmn/jsmn.h"
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

#define LARGURA_TELA 1360
#define ALTURA_TELA 750

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_EVENT evento;
ALLEGRO_AUDIO_STREAM *musica = NULL;

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

    al_set_window_title(janela, "Joguinho");

    fonte = al_load_font("Roboto-Regular.ttf", 50, 0);
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
    fonte2 = al_load_font("Roboto-Regular.ttf", 30, 0);
    if (!fonte){
        fprintf(stderr, "Falha ao carregar \"fonte Roboto-Regular.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
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

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playing(musica, true);

    return true;
}

int main(){
    int var_inicio, var_podio;
    if (!inicializar()) return -1;

    var_inicio = inicio(janela, musica, fila_eventos, fonte2, evento);
    if (var_inicio != 2)
        var_podio = podio(100, janela, fila_eventos, fonte, evento);

    al_destroy_font(fonte);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_audio_stream(musica);
    return 0;
}