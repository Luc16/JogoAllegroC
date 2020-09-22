#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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
ALLEGRO_TIMER *temporizador = NULL;
char *plano_de_fundo = "Images/bk_main.png";

// estrutura de movimento dos personagens
typedef struct Struct_Posicao {
  float x, y, x_anterior, y_anterior, altura, largura, col_altura, col_largura;
} Posicao;

// estrutura dos jogadores
typedef struct Struct_Jogador {
  Posicao pos_jogador;
  bool pulando, chao, mov_direita, mov_esquerda, morto;
  float posicao_y0, ajuste_pos; 
  int i_cair, i_pulo, lado;
} Jogador;

//estrutura corona
typedef struct Struct_Corona {
  Posicao pos_corona;
  bool morto;
  int velocidade;
} Corona;

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
 
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
  al_register_event_source(fila_eventos, al_get_timer_event_source(temporizador));

  al_start_timer(temporizador);
 
  return true;
}

// colisões
bool jog_colidindo(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
    return jog->pos_jogador.y - jog->pos_jogador.altura < y_baixo &&
     jog->pos_jogador.y + jog->pos_jogador.altura > y_cima &&
      jog->pos_jogador.x + jog->pos_jogador.largura > x_esquerda &&
       jog->pos_jogador.x - jog->pos_jogador.largura < x_direita;
}

void jog_colisao_lateral(Jogador *jog, float x_esquerda, float x_direita){
  if (jog->pos_jogador.x_anterior + jog->pos_jogador.largura <= x_esquerda)
    jog->pos_jogador.x = x_esquerda - 0.001 - jog->pos_jogador.largura; 
  else if (jog->pos_jogador.x_anterior - jog->pos_jogador.largura >= x_direita)
    jog->pos_jogador.x = x_direita + 0.001 + jog->pos_jogador.largura;
}

void jog_colisao_teto(Jogador *jog, float y_baixo){
  if (jog->pos_jogador.y_anterior - jog->pos_jogador.altura > y_baixo) {
    jog->pos_jogador.y = y_baixo + 0.001 + jog->pos_jogador.altura;
    jog->posicao_y0 = jog->pos_jogador.y;
    jog->i_cair = 0;
    jog->pulando = false;
  }
}

void jog_colisao_chao(Jogador *jog, float y_cima){
  if (jog->pos_jogador.y_anterior + jog->pos_jogador.altura < y_cima){
    jog->pos_jogador.y = y_cima - 0.001 - jog->pos_jogador.altura;
    jog->i_cair = 0;
    if(jog->chao == false){
      jog->posicao_y0 = jog->pos_jogador.y;
      jog->chao = true;
      jog->pulando = false;
    }
  }
}

void jog_colisao_geral(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
  if (!(x_esquerda > LARGURA_TELA || x_direita < 0)){
    if (jog_colidindo(x_esquerda, x_direita, y_cima, y_baixo, jog)){
      jog_colisao_lateral(jog, x_esquerda, x_direita);
      jog_colisao_teto(jog, y_baixo);
      jog_colisao_chao(jog, y_cima);
    }
    #ifdef MOSTRAR_HITBOX
      al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 0));
    #endif
  }
}

// colisões mortais
bool jog_colidindo_mortal(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
    return jog->pos_jogador.y - jog->pos_jogador.col_altura < y_baixo &&
     jog->pos_jogador.y + jog->pos_jogador.col_altura > y_cima &&
      jog->pos_jogador.x + jog->pos_jogador.col_largura > x_esquerda &&
       jog->pos_jogador.x - jog->pos_jogador.col_largura < x_direita;
}

void jog_colisao_mortal(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
  if (jog_colidindo_mortal(x_esquerda, x_direita, y_cima, y_baixo, jog)){
    jog->morto = true;
  }
  #ifdef MOSTRAR_HITBOX
      al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 255));
  #endif
}

// movimentos
void jog_ativar_pulo(Jogador *jog){
  jog->chao = false;
  jog->pulando = true;
  jog->i_pulo = 0;
  jog->posicao_y0 = jog->pos_jogador.y; 
}

void jog_mover_esquerda(Jogador *jog, int _lado){
  jog->mov_esquerda = true; 
  jog->lado = _lado;
}

void jog_mover_direita(Jogador *jog, int _lado){
  jog->mov_direita = true; 
  jog->lado = _lado;
}

void jog_mover_lado(Jogador *jog){
  jog->pos_jogador.x += jog->lado*4;
}

void jog_pular(Jogador *jog){ 
  jog->pos_jogador.y = jog->posicao_y0 + 400*jog->i_pulo*jog->i_pulo/3600.0 - 600*jog->i_pulo/60.0;
  jog->i_pulo++;
}

void jog_cair(Jogador *jog){
  jog->chao = false;
  jog->pos_jogador.y = jog->posicao_y0 + 400*jog->i_cair*jog->i_cair/3600.0;
}

// movimento de tela
bool jog_esta_no_limite_direita(Jogador *jog){
  return jog->pos_jogador.x >= LARGURA_TELA*3/4;
}

bool jog_esta_no_limite_esquerda(Jogador *jog){
  return jog->pos_jogador.x <= LARGURA_TELA*1/10;
}

void mover_tela_direita(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona *cov){
  jog_limite->ajuste_pos = jog_limite->pos_jogador.x - LARGURA_TELA*3/4;
  *mov_tela += jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*3/4;
  jog_outro->pos_jogador.x -= jog_limite->ajuste_pos;
  cov->pos_corona.x -= jog_limite->ajuste_pos;
  if (jog_esta_no_limite_esquerda(jog_outro))
    jog_outro->pos_jogador.x = LARGURA_TELA*1/10;
}

void mover_tela_esquerda(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona *cov){
  jog_limite->ajuste_pos = LARGURA_TELA*1/10 - jog_limite->pos_jogador.x;
  *mov_tela -= jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*1/10;
  jog_outro->pos_jogador.x += jog_limite->ajuste_pos;
  cov->pos_corona.x += jog_limite->ajuste_pos;
}

void mover_tela(Jogador jogs[], Corona *cov, float *mov_tela){
  if (jog_esta_no_limite_direita(&jogs[0]) && jogs[0].pos_jogador.x > jogs[1].pos_jogador.x){
    mover_tela_direita(mov_tela, &jogs[0], &jogs[1], cov);
  }
  else if (jog_esta_no_limite_direita(&jogs[1])){
    mover_tela_direita(mov_tela, &jogs[1], &jogs[0], cov);
  }  
  else if (jog_esta_no_limite_esquerda(&jogs[0]) && jogs[0].pos_jogador.x < jogs[1].pos_jogador.x){
    mover_tela_esquerda(mov_tela, &jogs[0], &jogs[1], cov);
  }
  else if (jog_esta_no_limite_esquerda(&jogs[1])){
    mover_tela_esquerda(mov_tela, &jogs[1], &jogs[0], cov);
  }
}

// atualizar dados jogador
void jog_iniciar(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogs[i_jog].pos_jogador.x = LARGURA_TELA/2;
    jogs[i_jog].pos_jogador.x_anterior = LARGURA_TELA/2;
    jogs[i_jog].pos_jogador.y = 0;
    jogs[i_jog].pos_jogador.y_anterior = 0;
    jogs[i_jog].pos_jogador.altura = 20;
    jogs[i_jog].pos_jogador.largura = 20;
    jogs[i_jog].pos_jogador.col_altura = 15;
    jogs[i_jog].pos_jogador.col_largura = 15;
    jogs[i_jog].chao = false;
    jogs[i_jog].pulando = false;
    jogs[i_jog].mov_esquerda = false; 
    jogs[i_jog].mov_direita = false; 
    jogs[i_jog].morto = false;
    jogs[i_jog].posicao_y0 = 0; 
    jogs[i_jog].lado = 0;
    jogs[i_jog].i_cair = 0;
    jogs[i_jog].i_pulo = 0;
  }
}

void jog_atualizar_pos_anterior(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogs[i_jog].pos_jogador.x_anterior = jogs[i_jog].pos_jogador.x;
    jogs[i_jog].pos_jogador.y_anterior = jogs[i_jog].pos_jogador.y;
  }
}

void jog_atualizar_geral(Jogador jogs[], float mov_tela){
  int i_jog;
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogs[i_jog].ajuste_pos = 0;

    if (!jogs[i_jog].morto){
      //pular e cair
      if (jogs[i_jog].pulando){
        jog_pular(&jogs[i_jog]);    
      } else {
        jog_cair(&jogs[i_jog]);
      }
      // movimento lateral
      if (jogs[i_jog].mov_direita || jogs[i_jog].mov_esquerda){
        jog_mover_lado(&jogs[i_jog]);
      }
      // chão 
      jog_colisao_geral(0-mov_tela, 10000-mov_tela, 690, ALTURA_TELA, &jogs[i_jog]);
      // baixo esquerda cima
      jog_colisao_geral(1100-mov_tela, LARGURA_TELA-mov_tela, 380, 420, &jogs[i_jog]);
      // baixo esquerda baixo
      jog_colisao_geral(800-mov_tela, 1050-mov_tela, 520, 560, &jogs[i_jog]);
      // baixo direita cima
      jog_colisao_geral(1750-mov_tela, 2000-mov_tela, 380, 420, &jogs[i_jog]);
      // baixo direta baixo
      jog_colisao_geral(1450-mov_tela, 1700-mov_tela, 520, 560, &jogs[i_jog]);
      //linha que mata
      jog_colisao_mortal(380-mov_tela, 420-mov_tela, 600, 690, &jogs[i_jog]);

      if (jogs[i_jog].pos_jogador.y >= ALTURA_TELA + 1000) jogs[i_jog].morto = true;
      jogs[i_jog].i_cair++;
    } else {
      jogs[i_jog].pos_jogador.x = LARGURA_TELA/2;
    }
  }
}

// funções corona
void corona_iniciar(Corona *cov){
  cov->pos_corona.altura = 40;
  cov->pos_corona.largura = 40;
  cov->pos_corona.col_altura = 35;
  cov->pos_corona.col_largura = 35;
  cov->pos_corona.x = 0;
  cov->pos_corona.y = 0;
  cov->pos_corona.x_anterior = 0;
  cov->pos_corona.y_anterior = 0;
  cov->morto = false;
  cov->velocidade = 2;
}

void corona_movimento(Corona *cov, Jogador jogs[]){
  int i;
  float distancia = MAXFLOAT, dx, dy;
  for (i = 0; i < numero_de_jogadores; i++){
    if (!jogs[i].morto){
      float dxi = jogs[i].pos_jogador.x - cov->pos_corona.x;
      float dyi = jogs[i].pos_jogador.y - cov->pos_corona.y;
      float di = sqrt(pow(dxi, 2) + pow(dyi, 2));
      if(di < distancia){
        distancia = di;
        dx = dxi;
        dy = dyi;
      }
    }
  }
  float proporcao = 1.0 - (distancia - cov->velocidade)/distancia;
  cov->pos_corona.x += dx*proporcao;
  cov->pos_corona.y += dy*proporcao;
}

void corona_atualizar(Corona *cov, Jogador jogs[]){
  if(!cov->morto){
    corona_movimento(cov, jogs);
  }
} 

void corona_colide(Corona *cov, Jogador jogs[]){
  int i;
  for(i = 0; i < numero_de_jogadores; i++){
    if (!jogs[i].morto){
      if (jogs[i].pos_jogador.y - jogs[i].pos_jogador.col_altura < cov->pos_corona.y + cov->pos_corona.col_altura &&
        jogs[i].pos_jogador.y + jogs[i].pos_jogador.col_altura > cov->pos_corona.y - cov->pos_corona.col_altura &&
          jogs[i].pos_jogador.x + jogs[i].pos_jogador.col_largura > cov->pos_corona.x - cov->pos_corona.col_largura &&
          jogs[i].pos_jogador.x - jogs[i].pos_jogador.col_largura < cov->pos_corona.x + cov->pos_corona.col_largura){
      
      jogs[i].morto = true;
      cov->morto = true;
      }
    }
  }
}

// lidar com botões
void apertar_botao(ALLEGRO_EVENT evento, Jogador jogs[]){
  switch (evento.keyboard.keycode){
    case ALLEGRO_KEY_UP:
      if (jogs[0].chao){
        jog_ativar_pulo(&jogs[0]);
      }
      break;
    case ALLEGRO_KEY_DOWN:
      jogs[0].pos_jogador.y += 20;
      break;
    case ALLEGRO_KEY_LEFT:
      jog_mover_esquerda(&jogs[0], -1);
      break; 
    case ALLEGRO_KEY_RIGHT:
      jog_mover_direita(&jogs[0], 1);
      break;
    case ALLEGRO_KEY_W:
      if (jogs[1].chao){
        jog_ativar_pulo(&jogs[1]);
      }
      break;
    case ALLEGRO_KEY_S:
      jogs[1].pos_jogador.y += 20;
      break;
    case ALLEGRO_KEY_A:
      jog_mover_esquerda(&jogs[1], -1);
      break; 
    case ALLEGRO_KEY_D:
      jog_mover_direita(&jogs[1], 1);
      break;
  }
}

void soltar_botao(ALLEGRO_EVENT evento, Jogador jogs[]){
  switch (evento.keyboard.keycode){
    case ALLEGRO_KEY_RIGHT:
      jogs[0].mov_direita = false;
      break;
    case ALLEGRO_KEY_LEFT:
      jogs[0].mov_esquerda = false;
      break;
    case ALLEGRO_KEY_D:
      jogs[1].mov_direita = false;
      break;
    case ALLEGRO_KEY_A:
      jogs[1].mov_esquerda = false;
      break;
  }
}

int main(){
  Jogador jogadores[numero_de_jogadores];
  Corona covid;
  bool sair = false, fim = false, ajuste_e_desenho = false;
  int r = 20;
  float mov_tela = 0; 

  // inicializando os jogadores
  jog_iniciar(jogadores);
  corona_iniciar(&covid);

  if (!inicializar()) return -1;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){  
    al_wait_for_event(fila_eventos, &evento);

    if (evento.type == ALLEGRO_EVENT_TIMER){
      mover_tela(jogadores, &covid, &mov_tela);      
      jog_atualizar_pos_anterior(jogadores);
      ajuste_e_desenho = true;
    }

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
      apertar_botao(evento, jogadores);
    else if (evento.type == ALLEGRO_EVENT_KEY_UP)
      soltar_botao(evento, jogadores);

    else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      sair = true;

    if (ajuste_e_desenho && al_is_event_queue_empty(fila_eventos)){
      ajuste_e_desenho = false;
      
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_bitmap(fundo, 0-mov_tela, 0, 0);

      jog_atualizar_geral(jogadores, mov_tela);
      corona_atualizar(&covid, jogadores);
      corona_colide(&covid, jogadores);

      if(!jogadores[0].morto) al_draw_filled_circle(jogadores[0].pos_jogador.x, jogadores[0].pos_jogador.y, r, al_map_rgb(255, 0, 0));
      if(!jogadores[1].morto) al_draw_filled_circle(jogadores[1].pos_jogador.x, jogadores[1].pos_jogador.y, r, al_map_rgb(0, 0, 255));
      if(!covid.morto) al_draw_filled_circle(covid.pos_corona.x, covid.pos_corona.y, covid.pos_corona.altura, al_map_rgb(255, 255, 255));

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
    }
  }

  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_timer(temporizador);
  al_destroy_event_queue(fila_eventos);
 
  return 0;
}
