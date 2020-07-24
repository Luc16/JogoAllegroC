#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// undef MOSTRAR_HITBOX
#define MOSTRAR_HITBOX

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
  bool pulando, chao, mov_direita, mov_esquerda, morto;
  float posicao_y0, ajuste_pos; 
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
 
  fonte = al_load_font("Roboto-Regular.ttf", 100, 0);
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
bool colidindo(float x_esquerda, float x_direita, float y_cima, float y_baixo, struct jogador *jog, int raio){
    return jog->mov_jogador.y - raio < y_baixo &&
     jog->mov_jogador.y + raio > y_cima &&
      jog->mov_jogador.x + raio > x_esquerda &&
       jog->mov_jogador.x - raio < x_direita;
}

void colisao_lateral(struct jogador *jog, float x_esquerda, float x_direita, int raio){
    if (jog->mov_jogador.x_anterior <= x_esquerda)
      jog->mov_jogador.x = x_esquerda - 0.001 - raio; 
    else if (jog->mov_jogador.x_anterior >= x_direita)
      jog->mov_jogador.x = x_direita + 0.001 + raio;
}

void colisao_teto(struct jogador *jog, float y_baixo, int raio){
    if (jog->mov_jogador.y_anterior - raio > y_baixo) {
      jog->mov_jogador.y = y_baixo + 0.001 + raio;
      jog->posicao_y0 = jog->mov_jogador.y;
      jog->i_cair = 0;
      jog->pulando = false;
    }
}

void colisao_chao(struct jogador *jog, float y_cima, int raio){
    if (jog->mov_jogador.y_anterior + raio < y_cima){
      jog->mov_jogador.y = y_cima - 0.001 - raio;
      jog->i_cair = 0;
      if(jog->chao == false){
        jog->posicao_y0 = jog->mov_jogador.y;
        jog->chao = true;
        jog->pulando = false;
      }
    }
}

void colisao_geral(float x_esquerda, float x_direita, float y_cima, float y_baixo, struct jogador *jog, int raio){
    if (colidindo(x_esquerda, x_direita, y_cima, y_baixo, jog, raio)){
        colisao_lateral(jog, x_esquerda, x_direita, raio);
        colisao_teto(jog, y_baixo, raio);
        colisao_chao(jog, y_cima, raio);
    }
    #ifdef MOSTRAR_HITBOX
      al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 0));
    #endif
}

void colisao_mortal(float x_esquerda, float x_direita, float y_cima, float y_baixo, struct jogador *jog, int raio){
  if (colidindo(x_esquerda, x_direita, y_cima, y_baixo, jog, raio)){
    jog->morto = true;
  }
  #ifdef MOSTRAR_HITBOX
      al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 255));
  #endif
}

// movimentos
void ativar_pulo(struct jogador *jog){
  jog->chao = false;
  jog->pulando = true;
  jog->i_pulo = 0;
  jog->posicao_y0 = jog->mov_jogador.y; 
}

void mover_esquerda(struct jogador *jog, int _lado){
  jog->mov_esquerda = true; 
  jog->lado = _lado;
}

void mover_direita(struct jogador *jog, int _lado){
  jog->mov_direita = true; 
  jog->lado = _lado;
}

void mover_lado(struct jogador *jog){
  jog->mov_jogador.x += jog->lado*4;
}

void pular(struct jogador *jog){
  jog->mov_jogador.y = jog->posicao_y0 + 400*jog->i_pulo*jog->i_pulo/3600.0 - 600*jog->i_pulo/60.0;
  jog->i_pulo++;
}

void cair(struct jogador *jog){
  jog->chao = false;
  jog->mov_jogador.y = jog->posicao_y0 + 400*jog->i_cair*jog->i_cair/3600.0;
}

bool esta_no_limite_direita(struct jogador *jog){
  return jog->mov_jogador.x >= LARGURA_TELA*3/4;
}

bool esta_no_limite_esquerda(struct jogador *jog){
  return jog->mov_jogador.x <= LARGURA_TELA*1/10;
}

void mover_tela_direita(float *mov_tela, struct jogador *jog_limite, struct jogador *jog_outro){
  jog_limite->ajuste_pos = jog_limite->mov_jogador.x - LARGURA_TELA*3/4;
  *mov_tela += jog_limite->ajuste_pos;
  jog_limite->mov_jogador.x = LARGURA_TELA*3/4;
  jog_outro->mov_jogador.x -= jog_limite->ajuste_pos;
  if (esta_no_limite_esquerda(jog_outro))
    jog_outro->mov_jogador.x = LARGURA_TELA*1/10;
}

void mover_tela_esquerda(float *mov_tela, struct jogador *jog_limite, struct jogador *jog_outro){
  jog_limite->ajuste_pos = LARGURA_TELA*1/10 - jog_limite->mov_jogador.x;
  *mov_tela -= jog_limite->ajuste_pos;
  jog_limite->mov_jogador.x = LARGURA_TELA*1/10;
  jog_outro->mov_jogador.x += jog_limite->ajuste_pos;
}

// lidar com botoes
void apertar_botao(ALLEGRO_EVENT evento, struct jogador *jog){
  switch (evento.keyboard.keycode){
    case ALLEGRO_KEY_UP:
      if (jog[0].chao){
        ativar_pulo(&jog[0]);
      }
      break;
    case ALLEGRO_KEY_DOWN:
      jog[0].mov_jogador.y += 20;
      break;
    case ALLEGRO_KEY_LEFT:
      mover_esquerda(&jog[0], -1);
      break; 
    case ALLEGRO_KEY_RIGHT:
      mover_direita(&jog[0], 1);
      break;
    case ALLEGRO_KEY_W:
      if (jog[1].chao){
        ativar_pulo(&jog[1]);
      }
      break;
    case ALLEGRO_KEY_S:
      jog[1].mov_jogador.y += 20;
      break;
    case ALLEGRO_KEY_A:
      mover_esquerda(&jog[1], -1);
      break; 
    case ALLEGRO_KEY_D:
      mover_direita(&jog[1], 1);
      break;
  }
}

void soltar_botao(ALLEGRO_EVENT evento, struct jogador *jog){
  switch (evento.keyboard.keycode){
    case ALLEGRO_KEY_RIGHT:
      jog[0].mov_direita = false;
      break;
    case ALLEGRO_KEY_LEFT:
      jog[0].mov_esquerda = false;
      break;
    case ALLEGRO_KEY_D:
      jog[1].mov_direita = false;
      break;
    case ALLEGRO_KEY_A:
      jog[1].mov_esquerda = false;
      break;
  }
}

int main(){
  struct jogador jogadores[numero_de_jogadores];
  bool sair = false, fim = false;
  int r = 20, i_jog;
  float mov_tela = 0;
  double tempo_inicial, tempo_final;

  // inicializando os jogadores
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogadores[i_jog].mov_jogador.x = LARGURA_TELA/2;
    jogadores[i_jog].mov_jogador.x_anterior = LARGURA_TELA/2;
    jogadores[i_jog].mov_jogador.y = 0;
    jogadores[i_jog].mov_jogador.y_anterior = 0;
    jogadores[i_jog].chao = false;
    jogadores[i_jog].pulando = false;
    jogadores[i_jog].mov_esquerda = false; 
    jogadores[i_jog].mov_direita = false; 
    jogadores[i_jog].morto = false;
    jogadores[i_jog].posicao_y0 = 0; 
    jogadores[i_jog].lado = 0;
    jogadores[i_jog].i_cair = 0;
    jogadores[i_jog].i_pulo = 0;
  }

  if (!inicializar()) return -1;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){  
    tempo_inicial = al_get_time();

    for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
      jogadores[i_jog].mov_jogador.x_anterior = jogadores[i_jog].mov_jogador.x;
      jogadores[i_jog].mov_jogador.y_anterior = jogadores[i_jog].mov_jogador.y;
    }

    while(!al_is_event_queue_empty(fila_eventos)){
      al_wait_for_event(fila_eventos, &evento);

      if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        apertar_botao(evento, jogadores);
      else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        soltar_botao(evento, jogadores);

      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        sair = true;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fundo, 0-mov_tela, 0, 0);
    
    //movimentos da tela
    if (esta_no_limite_direita(&jogadores[0]) && jogadores[0].mov_jogador.x > jogadores[1].mov_jogador.x){
      mover_tela_direita(&mov_tela, &jogadores[0], &jogadores[1]);
    }
    else if (esta_no_limite_direita(&jogadores[1])){
      mover_tela_direita(&mov_tela, &jogadores[1], &jogadores[0]);
    }  
    else if (esta_no_limite_esquerda(&jogadores[0]) && jogadores[0].mov_jogador.x < jogadores[1].mov_jogador.x){
      mover_tela_esquerda(&mov_tela, &jogadores[0], &jogadores[1]);
    }
    else if (esta_no_limite_esquerda(&jogadores[1])){
      mover_tela_esquerda(&mov_tela, &jogadores[1], &jogadores[0]);
    }

    for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
      jogadores[i_jog].ajuste_pos = 0;

      if (!jogadores[i_jog].morto){
        //pular e cair
        if (jogadores[i_jog].pulando){
          pular(&jogadores[i_jog]);    
        } else {
          cair(&jogadores[i_jog]);
        }
        // movimento lateral
        if (jogadores[i_jog].mov_direita || jogadores[i_jog].mov_esquerda){
          mover_lado(&jogadores[i_jog]);
        }
        // chão 
        colisao_geral(0-mov_tela, 10000-mov_tela, 690, ALTURA_TELA, &jogadores[i_jog], r);
        // baixo esquerda cima
        colisao_geral(1100-mov_tela, LARGURA_TELA-mov_tela, 380, 420, &jogadores[i_jog], r);
        // baixo esquerda baixo
        colisao_geral(800-mov_tela, 1050-mov_tela, 520, 560, &jogadores[i_jog], r);
        // baixo direita cima
        colisao_geral(1750-mov_tela, 2000-mov_tela, 380, 420, &jogadores[i_jog], r);
        // baixo direta baixo
        colisao_geral(1450-mov_tela, 1700-mov_tela, 520, 560, &jogadores[i_jog], r);
        //linha que mata
        colisao_mortal(380-mov_tela, 420-mov_tela, 600, 690, &jogadores[i_jog], r);

        if (jogadores[i_jog].mov_jogador.y >= ALTURA_TELA + 1000) jogadores[i_jog].morto = true;
        jogadores[i_jog].i_cair++;
      } else {
        jogadores[i_jog].mov_jogador.x = LARGURA_TELA/2;
      }
    }

    if(!jogadores[0].morto) al_draw_filled_circle(jogadores[0].mov_jogador.x, jogadores[0].mov_jogador.y, r, al_map_rgb(255, 0, 0));
    if(!jogadores[1].morto) al_draw_filled_circle(jogadores[1].mov_jogador.x, jogadores[1].mov_jogador.y, r, al_map_rgb(0, 0, 255));

    // acabar o jogo
    if (fim){
      al_rest(2);
      sair = true;
    }
    // morte
    if (jogadores[0].morto && jogadores[1].morto){
      al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2-100, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU O JOGO");
      fim = true;
    }
    
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
