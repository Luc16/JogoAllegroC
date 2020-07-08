#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Inicialização de variáveis globais
const int LARGURA_TELA = 1360;
const int ALTURA_TELA = 750;
const int FPS = 60;
const int numero_de_jogadores = 2;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT evento;
char *plano_de_fundo = "Images/bk_main.png";

// estrutura de movimento dos personagens
struct movimento {
  float x, y, x_anterior, y_anterior;
};
// estrutura dos jogadores
struct jogador {
  struct movimento mov_jogador;
  bool pulando, chao, mov_direita, mov_esquerda;
  float posicao_y0; 
  int i_cair, i_pulo, lado;
};
// Função de inicialização
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
 
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela){
    fprintf(stderr, "Falha ao criar janela.\n");
    return false;
  }
 
  al_set_window_title(janela, "Joguinho");
 
  fonte = al_load_font("Roboto-Regular.ttf", 30, 0);
  if (!fonte){
    fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
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
 
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
  return true;
}

// colisões
bool colidindo(float x1, float x2, float y, struct jogador *p, int raio){
    return p->mov_jogador.y - raio < y + 20 &&
     p->mov_jogador.y + raio > y - 20 &&
      p->mov_jogador.x + raio > x1 &&
       p->mov_jogador.x - raio < x2;
}

void colisao_lateral(struct jogador *p, float x1, float x2, int raio){
    if (p->mov_jogador.x_anterior + raio <= x1 || p->mov_jogador.x_anterior - raio >= x2 )
      p->mov_jogador.x = p->mov_jogador.x_anterior; 
}

void colisao_teto(struct jogador *p, float y, int raio){
    if (p->mov_jogador.y_anterior - raio > y + 20) {
      p->mov_jogador.y = y + 20.001 + raio;
      p->pulando = false;
    }
}

void colisao_chao(struct jogador *p, float y, int raio){
    if (p->mov_jogador.y_anterior + raio < y - 20){
      p->mov_jogador.y = y - 20.001 - raio;
      p->chao = true;
      p->i_cair = 0;
      p->pulando = false;
    }
}

void colisao_geral(float x1, float x2, float y, struct jogador *p, int raio){
    if (colidindo(x1, x2, y, p, raio)){
        colisao_lateral(p, x1, x2, raio);
        colisao_teto(p, y, raio);
        colisao_chao(p, y, raio);
    }
}

// movimentos
void ativar_pulo(struct jogador *p){
  p->chao = false;
  p->pulando = true;
  p->i_pulo = 0;
  p->posicao_y0 = p[0].mov_jogador.y; 
}

void mover_esquerda(struct jogador *p, int _lado){
  p->mov_esquerda = true; 
  p->lado = _lado;
}

void mover_direita(struct jogador *p, int _lado){
  p->mov_direita = true; 
  p->lado = _lado;
}

int main(){
  struct jogador jogadores[numero_de_jogadores];
  bool sair = false;
  int r = 20, i_jog;
  double tempo_inicial, tempo_final;

  for (i_jog = 0; i_jog < 2; i_jog++){
    jogadores[i_jog].mov_jogador.x = LARGURA_TELA/2;
    jogadores[i_jog].mov_jogador.x_anterior = LARGURA_TELA/2;
    jogadores[i_jog].mov_jogador.y = 0;
    jogadores[i_jog].mov_jogador.y_anterior = 0;
    jogadores[i_jog].chao = false;
    jogadores[i_jog].pulando = false;
    jogadores[i_jog].mov_esquerda = false; 
    jogadores[i_jog].mov_direita = false; 
    jogadores[i_jog].posicao_y0 = 0; 
    jogadores[i_jog].lado = 0;
    jogadores[i_jog].i_cair = 0;
    jogadores[i_jog].i_pulo = 0;
  }

  if (!inicializar()) return -1;
  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){  
    tempo_inicial = al_get_time();

    for (i_jog = 0; i_jog < 2; i_jog++){
      jogadores[i_jog].mov_jogador.x_anterior = jogadores[i_jog].mov_jogador.x;
      jogadores[i_jog].mov_jogador.y_anterior = jogadores[i_jog].mov_jogador.y;
    }

    while(!al_is_event_queue_empty(fila_eventos)){
      al_wait_for_event(fila_eventos, &evento);

      if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        switch (evento.keyboard.keycode){
          case ALLEGRO_KEY_UP:
            if (jogadores[0].chao){
              ativar_pulo(&jogadores[0]);
            }
            break;
          case ALLEGRO_KEY_DOWN:
            jogadores[0].mov_jogador.y += 20;
            break;
          case ALLEGRO_KEY_LEFT:
            mover_esquerda(&jogadores[0], -1);
            break; 
          case ALLEGRO_KEY_RIGHT:
            mover_direita(&jogadores[0], 1);
            break;
          case ALLEGRO_KEY_W:
            if (jogadores[1].chao){
              ativar_pulo(&jogadores[1]);
            }
            break;
          case ALLEGRO_KEY_S:
            jogadores[1].mov_jogador.y += 20;
            break;
          case ALLEGRO_KEY_A:
            mover_esquerda(&jogadores[1], -1);
            break; 
          case ALLEGRO_KEY_D:
            mover_direita(&jogadores[1], 1);
            break;
        }
      }
      else if (evento.type == ALLEGRO_EVENT_KEY_UP){
        switch (evento.keyboard.keycode){
          case ALLEGRO_KEY_RIGHT:
            jogadores[0].mov_direita = false;
            break;
          case ALLEGRO_KEY_LEFT:
            jogadores[0].mov_esquerda = false;
            break;
          case ALLEGRO_KEY_D:
            jogadores[1].mov_direita = false;
            break;
          case ALLEGRO_KEY_A:
            jogadores[1].mov_esquerda = false;
            break;
        }
      }
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        sair = true;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fundo, 0, 0, 0);

    for (i_jog = 0; i_jog < 2; i_jog++){
      if (jogadores[i_jog].pulando){
        jogadores[i_jog].mov_jogador.y = jogadores[i_jog].posicao_y0 + 400*jogadores[i_jog].i_pulo*jogadores[i_jog].i_pulo/3600.0 - 600*jogadores[i_jog].i_pulo/60.0;
        jogadores[i_jog].i_pulo++;
      } else {
        jogadores[i_jog].chao = false;
        jogadores[i_jog].mov_jogador.y += 10*jogadores[i_jog].i_cair*jogadores[i_jog].i_cair/3600.0;
      }
      if (jogadores[i_jog].mov_direita || jogadores[i_jog].mov_esquerda){
        jogadores[i_jog].mov_jogador.x += jogadores[i_jog].lado*4;
      }
      // chão 
      colisao_geral(0, LARGURA_TELA, 710, &jogadores[i_jog], r);
      // baixo direita canto
      colisao_geral(1100, LARGURA_TELA, 400, &jogadores[i_jog], r);
      // baixo direta meio
      colisao_geral(800, 1050, 540, &jogadores[i_jog], r);

      if (jogadores[i_jog].mov_jogador.y >= ALTURA_TELA)
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU O JOGO");
      if (jogadores[i_jog].mov_jogador.y >= ALTURA_TELA + 1000) sair = true;
      jogadores[i_jog].i_cair++;
    }

    // chão 
    al_draw_line(0, 710, LARGURA_TELA, 710, al_map_rgb(0, 255, 0), 40);
    // baixo direita canto
    al_draw_line(1100, 400, LARGURA_TELA, 400, al_map_rgb(255, 255, 0), 40);
    // baixo direta meio
    al_draw_line(800, 540, 1050, 540, al_map_rgb(255, 255, 0), 40);


    al_draw_filled_circle(jogadores[0].mov_jogador.x, jogadores[0].mov_jogador.y, r, al_map_rgb(255, 0, 0));
    al_draw_filled_circle(jogadores[1].mov_jogador.x, jogadores[1].mov_jogador.y, r, al_map_rgb(0, 0, 255));

    
    al_flip_display();

    // setando fps
    tempo_final = al_get_time() - tempo_inicial;
    if (tempo_final < 1.0/60.0)
      al_rest(1.0/60.0-tempo_final);
  }

  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
 
  return 0;
}
