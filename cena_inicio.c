#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "cena_inicio.h"
#include <stdio.h>

// Atributos da tela
#define LARGURA_TELA 1360
#define ALTURA_TELA 750

bool botao_colisao(float Xm, float Ym, float Xc, float Yc, ALLEGRO_BITMAP *botao_generico)
{
   return Xm >= Xc - al_get_bitmap_width(botao_generico)/2 &&
   Xm <= Xc + al_get_bitmap_width(botao_generico)/2 &&
   Ym >= Yc - al_get_bitmap_height(botao_generico)/2 &&
   Ym <= Yc + al_get_bitmap_height(botao_generico)/2;
}

int inicio(ALLEGRO_DISPLAY *janela, ALLEGRO_AUDIO_STREAM *musica, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_FONT *fonte, ALLEGRO_FONT *fonte0, ALLEGRO_EVENT evento, int *num_jogs){
    ALLEGRO_BITMAP *botao_ajuste = NULL, *botao_iniciar = NULL, *botao_ajuda = NULL, *botao_jogador = NULL, *botao_jogadores = NULL, *fundo_inicio = NULL;
    ALLEGRO_BITMAP *botao_musica = NULL, *botao_controle = NULL, *botao_jogabilidade = NULL, *fundo_jogabilidade = NULL, *fundo_controle = NULL, *engrenagem = NULL;

    fundo_inicio = al_load_bitmap("Images/fundo_inicio.png");
    if (!fundo_inicio){
      fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
    }
    engrenagem = al_load_bitmap("Images/engrenagem.png");
    if (!engrenagem){
      fprintf(stderr, "Falha ao carregar imagem da engrenagem.\n");
    }
    al_convert_mask_to_alpha(engrenagem, al_map_rgb(255, 255, 255));
    fundo_jogabilidade = al_load_bitmap("Images/Jogabilidade.png");
    if (!fundo_jogabilidade){
      fprintf(stderr, "Falha ao carregar imagem fundo_jogabilidade.\n");
    }
    fundo_controle = al_load_bitmap("Images/Controles.png");
    if (!fundo_controle){
      fprintf(stderr, "Falha ao carregar imagem fundo_controle.\n");
    }

    //botão de iniciar o jogo
    botao_iniciar = al_create_bitmap(LARGURA_TELA / 5, ALTURA_TELA / 8);
    if (!botao_iniciar){
        al_destroy_display(janela);
        fprintf(stderr, "erro com os bitmaps");
        return -1;
    }
    //botão de ajustes
    botao_ajuste = al_create_bitmap(100, 50);
    if (!botao_ajuste){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_display(janela);
        return -1;
    }
    //botão de ajuda
    botao_ajuda = al_create_bitmap(100, 50);
    if (!botao_ajuda){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_display(janela);
        return -1;
    }
    //botão para 1 jogador
    botao_jogador = al_create_bitmap(LARGURA_TELA / 5, 50);
    if(!botao_jogador){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_bitmap(botao_ajuda);
        al_destroy_display(janela);
    }
    //botão para 2 jogador
    botao_jogadores = al_create_bitmap(LARGURA_TELA / 5, 50);
    if(!botao_jogadores){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_bitmap(botao_ajuda);
        al_destroy_bitmap(botao_jogador);
        al_destroy_display(janela);
    }
    //botão música
    botao_musica = al_create_bitmap(LARGURA_TELA / 5, 50);
    if(!botao_musica){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_bitmap(botao_ajuda);
        al_destroy_bitmap(botao_jogador);
        al_destroy_bitmap(botao_jogadores);
        al_destroy_display(janela);
    }
    //botao para as instruções de controle
    botao_controle = al_create_bitmap(400, 60);
    if(!botao_controle){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_bitmap(botao_ajuda);
        al_destroy_bitmap(botao_jogador);
        al_destroy_bitmap(botao_jogadores);
        al_destroy_bitmap(botao_musica);
        al_destroy_display(janela);
    }
    //botão para as instruções de jogabilidade
    botao_jogabilidade = al_create_bitmap(400, 60);
    if(!botao_jogabilidade){
        al_destroy_bitmap(botao_iniciar);
        al_destroy_bitmap(botao_ajuste);
        al_destroy_bitmap(botao_ajuda);
        al_destroy_bitmap(botao_jogador);
        al_destroy_bitmap(botao_jogadores);
        al_destroy_bitmap(botao_musica);
        al_destroy_bitmap(botao_controle);
        al_destroy_display(janela);
    }

    bool sair = false, xis = false;
    int tela = 1;
    bool no_botao_inicio = false;
    bool no_botao_ajuste = false;
    bool no_botao_ajuda = false;
    bool no_botao_jogador = false;
    bool no_botao_jogadores = false;
    bool no_botao_musica = false;
    bool no_botao_controle = false;
    bool no_botao_jogabilidade = false;
    bool tocando = true;
    bool clic_botao_jogador = false;
    bool clic_botao_jogadores = false;
    bool clic_botao_controle = true;
    bool clic_botao_jogabilidade = false;

    while (!sair){
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(fila_eventos)){
            al_wait_for_event(fila_eventos, &evento);

            // Se o evento foi de movimentação do mouse
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                // Verificamos se ele está sobre a região do retângulo central
                if (botao_colisao(evento.mouse.x, evento.mouse.y, LARGURA_TELA/2, ALTURA_TELA/2, botao_iniciar) && tela == 1){
                    no_botao_inicio = true;
                }
                else{
                    no_botao_inicio = false;
                }
                if (botao_colisao(evento.mouse.x, evento.mouse.y, 1300, 35, botao_ajuda)){
                    no_botao_ajuda = true;
                }
                else{
                    no_botao_ajuda = false;
                }
                if (botao_colisao(evento.mouse.x, evento.mouse.y, 60, 35, botao_ajuste) && tela == 1){
                    no_botao_ajuste = true;
                }
                else{
                    no_botao_ajuste = false;
                }
                if (botao_colisao(evento.mouse.x, evento.mouse.y, LARGURA_TELA/2, 290, botao_jogador) && tela == 3){
                    no_botao_jogador = true;
                }
                else{
                    no_botao_jogador = false;
                }
                if (botao_colisao(evento.mouse.x, evento.mouse.y, LARGURA_TELA/2, 375, botao_jogadores) && tela == 3){
                    no_botao_jogadores = true;
                }
                else{
                    no_botao_jogadores = false;
                }
                if(botao_colisao(evento.mouse.x, evento.mouse.y, LARGURA_TELA/2, 460, botao_musica) && tela == 3){
                    no_botao_musica = true;
                }
                else{
                    no_botao_musica = false;
                }
                if (botao_colisao(evento.mouse.x, evento.mouse.y, LARGURA_TELA/4, 130, botao_controle) && tela == 2){
                    no_botao_controle = true;
                }
                else{
                    no_botao_controle = false;
                }
                if(botao_colisao(evento.mouse.x, evento.mouse.y, 1020, 130, botao_jogabilidade) && tela == 2){
                    no_botao_jogabilidade = true;
                }
                else{
                    no_botao_jogabilidade = false;
                }
            }
            // Ou se o evento foi um clique do mouse
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (no_botao_inicio){
                    sair = true;
                }
                else if(no_botao_ajuda){
                    if(tela == 1)
                        tela = 2;
                    else
                        tela = 1;
                }
                else if(no_botao_ajuste){
                    tela = 3;
                }
                else if(no_botao_jogador){
                    *num_jogs = 1;
                    clic_botao_jogador = true;
                    clic_botao_jogadores = false;
                }
                else if(no_botao_jogadores){
                    *num_jogs = 2;
                    clic_botao_jogadores = true;
                    clic_botao_jogador = false;
                }
                else if(no_botao_musica){
                    if(tocando){
                        tocando = false;
                        al_set_audio_stream_playing(musica, false);
                    }
                    else{
                        tocando = true;
                        al_set_audio_stream_playing(musica, true);
                    }
                }
                else if(no_botao_controle){
                    clic_botao_controle = true;
                    clic_botao_jogabilidade = false;
                }
                else if(no_botao_jogabilidade){
                    clic_botao_controle = false;
                    clic_botao_jogabilidade = true;

                }
            } 
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = true;
                xis = true;
            }

        }

        // Limpar a tela
        al_clear_to_color(al_map_rgb(0, 0, 0));

        //desenho das telas
        if(tela == 1){
            //Colorir o bitmap do botão inicio
            al_set_target_bitmap(botao_iniciar);
            if (!no_botao_inicio){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(200, 200, 200));
            }

            // Colorir o bitmap do botão ajuda
            al_set_target_bitmap(botao_ajuda);
            if(!no_botao_ajuda){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 0, 200));
            }
            //Colorir o bitmap do botao ajuste
            al_set_target_bitmap(botao_ajuste);
            if(!no_botao_ajuste){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 0, 200));
            }

            al_set_target_bitmap(al_get_backbuffer(janela));
            al_draw_bitmap(fundo_inicio, 0, 0, 0);
            //botão inicio
            al_draw_text(fonte0, al_map_rgb(255, 255, 0), LARGURA_TELA/2, ALTURA_TELA/2 - 200, ALLEGRO_ALIGN_CENTRE, "Patolindo e Bobola X 2020");

            al_draw_bitmap(botao_iniciar, LARGURA_TELA / 2 - al_get_bitmap_width(botao_iniciar) / 2,
            ALTURA_TELA / 2 - al_get_bitmap_height(botao_iniciar) / 2, 0);

            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 - 15, ALLEGRO_ALIGN_CENTRE, "INICIO");

            //botão ajuda
            al_draw_bitmap(botao_ajuda, LARGURA_TELA - al_get_bitmap_width(botao_ajuda) - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda) - 690 , 0);
            al_draw_text(fonte, al_map_rgb(50, 50, 50), LARGURA_TELA - al_get_bitmap_width(botao_ajuda)/2 - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda)/2 -705, ALLEGRO_ALIGN_CENTRE, "AJUDA");


            //botão ajuste
            al_draw_bitmap(botao_ajuste, LARGURA_TELA - al_get_bitmap_width(botao_ajuste) - 1250,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuste) - 690 , 0);
            al_draw_bitmap(engrenagem, LARGURA_TELA - al_get_bitmap_width(botao_ajuste) - 1250,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuste) - 690 , 0);

            al_draw_text(fonte, al_map_rgb(255, 255, 0), 20, ALTURA_TELA-80, ALLEGRO_ALIGN_LEFT, "Feito por: Ana Luisa Holthausen de Carvalho e Luc Joffily Ribas");
        }
        if(tela == 2){
            if(clic_botao_controle){
                al_draw_bitmap(fundo_controle, 0, 0, 0);
            }
            else if(clic_botao_jogabilidade){
                al_draw_bitmap(fundo_jogabilidade, 0, 0, 0);
            }

            al_set_target_bitmap(botao_ajuda);
            if(!no_botao_ajuda){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 0, 200));
            }

            al_set_target_bitmap(botao_controle);
            if(!no_botao_controle){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(200, 200, 200));
            }

            al_set_target_bitmap(botao_jogabilidade);
            if(!no_botao_jogabilidade){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(200, 200, 200));
            }

            al_set_target_bitmap(al_get_backbuffer(janela));

            //botão sair
            al_draw_bitmap(botao_ajuda, LARGURA_TELA - al_get_bitmap_width(botao_ajuda) - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda) - 690 , 0);
            al_draw_text(fonte, al_map_rgb(50, 50, 50), LARGURA_TELA - al_get_bitmap_width(botao_ajuda)/2 - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda)/2 -705, ALLEGRO_ALIGN_CENTRE, "VOLTA");

            //botão controle
            al_draw_bitmap(botao_controle, 140, 100, 0);

            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 115, ALLEGRO_ALIGN_CENTRE, "CONTROLES");

            //botão jogabilidade
            al_draw_bitmap(botao_jogabilidade, 820, 100, 0);

            al_draw_text(fonte, al_map_rgb(0, 0, 0), 1020, 115, ALLEGRO_ALIGN_CENTRE, "JOGABILIDADE");

        }
        if(tela == 3){
            al_set_target_bitmap(botao_ajuda);
            if(!no_botao_ajuda){
                al_clear_to_color(al_map_rgb(255, 255, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 0, 200));
            }
            al_set_target_bitmap(botao_jogador);
            if (!no_botao_jogador && !clic_botao_jogador){
                al_clear_to_color(al_map_rgb(0, 0, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 20, 180));
            }
            al_set_target_bitmap(botao_jogadores);
            if (!no_botao_jogadores && !clic_botao_jogadores){
                al_clear_to_color(al_map_rgb(0, 0, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(255, 20, 180));
            }
            al_set_target_bitmap(botao_musica);
            if (!no_botao_musica){
                al_clear_to_color(al_map_rgb(0, 0, 255));
            }
            else{
                al_clear_to_color(al_map_rgb(0, 0, 200));
            }
            al_set_target_bitmap(al_get_backbuffer(janela));
            //botão fechar
            al_draw_bitmap(botao_ajuda, LARGURA_TELA - al_get_bitmap_width(botao_ajuda) - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda) - 690 , 0);
            al_draw_text(fonte, al_map_rgb(50, 50, 50), LARGURA_TELA - al_get_bitmap_width(botao_ajuda)/2 - 10,
            ALTURA_TELA - al_get_bitmap_height(botao_ajuda)/2 -705, ALLEGRO_ALIGN_CENTRE, "VOLTA");

            //botão 1 jogador
            al_draw_bitmap(botao_jogador, LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogador) / 2,
            265, 0);

            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 275, ALLEGRO_ALIGN_CENTRE, "1 Jogador");

            //botão 2 jogadores
            al_draw_bitmap(botao_jogadores, LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogadores) / 2,
            350, 0);

            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 - 15, ALLEGRO_ALIGN_CENTRE, "2 Jogadores");

            //botão música
            al_draw_bitmap(botao_musica, LARGURA_TELA / 2 - al_get_bitmap_width(botao_musica) / 2,
            435, 0);

            if(tocando){
                al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 445, ALLEGRO_ALIGN_CENTRE, "Musica: On");
            }
            else{
               al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 445, ALLEGRO_ALIGN_CENTRE, "Musica: Off");
            }
        }

        // Atualiza a tela
        al_flip_display();
    }

    // Desaloca os recursos utilizados na aplicação
    al_destroy_bitmap(botao_ajuda);
    al_destroy_bitmap(botao_iniciar);
    al_destroy_bitmap(botao_ajuste);
    al_destroy_bitmap(botao_jogador);
    al_destroy_bitmap(botao_jogadores);
    al_destroy_bitmap(botao_musica);
    al_destroy_bitmap(botao_controle);
    al_destroy_bitmap(botao_jogabilidade);
    al_destroy_bitmap(fundo_jogabilidade);
    al_destroy_bitmap(fundo_controle);
    al_destroy_bitmap(engrenagem);
    al_destroy_bitmap(fundo_inicio);

    if (!xis) return 0;
    else return 2;
}
