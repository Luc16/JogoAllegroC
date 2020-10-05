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
const int numero_de_palavras = 8;
const int numero_de_coronas = 1;
const int numero_de_plataformas = 5;
const int numero_de_plataformas_mortais = 1;
const int numero_de_alcools = 1;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL, *fonte2 = NULL;
ALLEGRO_EVENT evento;
ALLEGRO_TIMER *temporizador = NULL;
char *plano_de_fundo = "Images/bk_main.png";

// estrutura de movimento dos personagens
typedef struct Struct_Posicao {
  float x, y, altura, largura, col_altura, col_largura;
} Posicao;

// estrutura corona
typedef struct Struct_Corona {
  Posicao pos_corona;
  bool morto;
  bool ativado;
  int velocidade;
  ALLEGRO_BITMAP *bm_corona;
} Corona;

// estrutura palavra de ordem
typedef struct Struct_P_Ordem {
  Posicao pos_ordem;
  float pos0;
  bool morto;
  int velocidade, lado;
} P_Ordem;

// estrutura dos jogadores
typedef struct Struct_Jogador {
  Posicao pos_jogador;
  P_Ordem plvr[numero_de_palavras];
  bool pulando, chao, mov_direita, mov_esquerda, morto;
  float posicao_y0, ajuste_pos,  x_anterior, y_anterior; 
  int i_cair, i_pulo, lado, com_alcool;
} Jogador;

// estrutura alcool
typedef struct Struct_Alcool {
  Posicao pos_alcool;
  int lado, idx, i_esg;
  bool morto, pego, esguichando;
} Alcool;

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

  fonte2 = al_load_font("Roboto-Regular.ttf", 10, 0);
  if (!fonte2){
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
  if (jog->x_anterior + jog->pos_jogador.largura <= x_esquerda)
    jog->pos_jogador.x = x_esquerda - 0.001 - jog->pos_jogador.largura; 
  else if (jog->x_anterior - jog->pos_jogador.largura >= x_direita)
    jog->pos_jogador.x = x_direita + 0.001 + jog->pos_jogador.largura;
}

void jog_colisao_teto(Jogador *jog, float y_baixo){
  if (jog->y_anterior - jog->pos_jogador.altura > y_baixo) {
    jog->pos_jogador.y = y_baixo + 0.001 + jog->pos_jogador.altura;
    jog->posicao_y0 = jog->pos_jogador.y;
    jog->i_cair = 0;
    jog->pulando = false;
  }
}

void jog_colisao_chao(Jogador *jog, float y_cima){
  if (jog->y_anterior + jog->pos_jogador.altura < y_cima){
    jog->pos_jogador.y = y_cima - 0.001 - jog->pos_jogador.altura;
    jog->i_cair = 0;
    if(jog->chao == false){
      jog->posicao_y0 = jog->pos_jogador.y;
      jog->chao = true;
      jog->pulando = false;
    }
  }
}

void jog_colisao_geral(float plataformas[], Jogador *jog, float mov_tela){
  float x_esquerda = plataformas[0] - mov_tela, y_cima = plataformas[1];
  float x_direita = plataformas[2] - mov_tela, y_baixo = plataformas[3];
  if (!(x_esquerda > LARGURA_TELA || x_direita < 0)){
    if (jog_colidindo(x_esquerda, x_direita, y_cima, y_baixo, jog)){
      jog_colisao_lateral(jog, x_esquerda, x_direita);
      jog_colisao_teto(jog, y_baixo);
      jog_colisao_chao(jog, y_cima);
    }
    // #ifdef MOSTRAR_HITBOX
    //   al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 0));
    // #endif
  }
}

// colisões mortais
bool jog_colidindo_mortal(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
    return jog->pos_jogador.y - jog->pos_jogador.col_altura < y_baixo &&
     jog->pos_jogador.y + jog->pos_jogador.col_altura > y_cima &&
      jog->pos_jogador.x + jog->pos_jogador.col_largura > x_esquerda &&
       jog->pos_jogador.x - jog->pos_jogador.col_largura < x_direita;
}

void jog_colisao_mortal(float plataformas_mortais[], Jogador *jog, float mov_tela){
  float x_esquerda = plataformas_mortais[0] - mov_tela, y_cima = plataformas_mortais[1];
  float x_direita = plataformas_mortais[2] - mov_tela, y_baixo = plataformas_mortais[3];
  if (jog_colidindo_mortal(x_esquerda, x_direita, y_cima, y_baixo, jog)){
    jog->morto = true;
  }
  #ifdef MOSTRAR_HITBOX
      al_draw_filled_rectangle(x_esquerda, y_cima, x_direita, y_baixo, al_map_rgb(255, 255, 255));
  #endif
}

// movimentos e acoes
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

void mover_tela_direita(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[]){
  int i;
  jog_limite->ajuste_pos = jog_limite->pos_jogador.x - LARGURA_TELA*3/4;
  *mov_tela += jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*3/4;
  jog_outro->pos_jogador.x -= jog_limite->ajuste_pos;
  for (i = 0; i < numero_de_alcools; i++){
    if (!alcools[i].pego)
      alcools[i].pos_alcool.x -= jog_limite->ajuste_pos;
  }
  for (i = 0; i < numero_de_coronas; i++)
    covs[i].pos_corona.x -= jog_limite->ajuste_pos;
  if (jog_esta_no_limite_esquerda(jog_outro))
    jog_outro->pos_jogador.x = LARGURA_TELA*1/10;
}

void mover_tela_esquerda(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[]){
  int i;
  jog_limite->ajuste_pos = LARGURA_TELA*1/10 - jog_limite->pos_jogador.x;
  *mov_tela -= jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*1/10;
  jog_outro->pos_jogador.x += jog_limite->ajuste_pos;
  for (i = 0; i < numero_de_alcools; i++){
    if (!alcools[i].pego && !alcools[i].morto)
      alcools[i].pos_alcool.x += jog_limite->ajuste_pos;
  }
  for (i = 0; i < numero_de_coronas; i++){
    if (!covs[i].morto)
      covs[i].pos_corona.x += jog_limite->ajuste_pos;
  }
}

void mover_tela(Jogador jogs[], Corona covs[], Alcool alcools[], float *mov_tela){
  if (jog_esta_no_limite_direita(&jogs[0]) && jogs[0].pos_jogador.x > jogs[1].pos_jogador.x){
    mover_tela_direita(mov_tela, &jogs[0], &jogs[1], covs, alcools);
  }
  else if (jog_esta_no_limite_direita(&jogs[1])){
    mover_tela_direita(mov_tela, &jogs[1], &jogs[0], covs, alcools);
  }  
  else if (jog_esta_no_limite_esquerda(&jogs[0]) && jogs[0].pos_jogador.x < jogs[1].pos_jogador.x){
    mover_tela_esquerda(mov_tela, &jogs[0], &jogs[1], covs, alcools);
  }
  else if (jog_esta_no_limite_esquerda(&jogs[1])){
    mover_tela_esquerda(mov_tela, &jogs[1], &jogs[0], covs, alcools);
  }
}

// funções corona
void corona_iniciar(Corona covs[]){
  int i;
  for (i = 0; i < numero_de_coronas; i++){
    covs[i].pos_corona.altura = 60;
    covs[i].pos_corona.largura = 60;
    covs[i].pos_corona.col_altura = 42;
    covs[i].pos_corona.col_largura = 42;
    covs[i].pos_corona.x = 0;
    covs[i].pos_corona.y = 0;
    covs[i].morto = false;
    covs[i].ativado = true;
    covs[i].velocidade = 3;
    covs[i].bm_corona = al_load_bitmap("./Images/corona.png");
    if (!covs[i].bm_corona){
      fprintf(stderr, "Falha ao carregar imagem do corona.\n");
      al_destroy_display(janela);
      al_destroy_event_queue(fila_eventos);
  }
    al_convert_mask_to_alpha(covs[i].bm_corona, al_map_rgb(255, 0, 0));
  }
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

void corona_atualizar(Corona covs[], Jogador jogs[]){
  int i;
  for (i = 0; i < numero_de_coronas; i++){
    if(!covs[i].morto && covs[i].ativado){
      corona_movimento(&covs[i], jogs);
    }
  }
} 

void corona_colide(Corona *cov, Jogador jogs[]){
  int i, j;
  for(j = 0; j < numero_de_coronas; j++){
    if (!cov[j].morto && cov[j].ativado){
      for(i = 0; i < numero_de_jogadores; i++){
        if (!jogs[i].morto && !cov[j].morto){
          if (jogs[i].pos_jogador.y - jogs[i].pos_jogador.col_altura < cov[j].pos_corona.y + cov[j].pos_corona.col_altura &&
              jogs[i].pos_jogador.y + jogs[i].pos_jogador.col_altura > cov[j].pos_corona.y - cov[j].pos_corona.col_altura &&
              jogs[i].pos_jogador.x + jogs[i].pos_jogador.col_largura > cov[j].pos_corona.x - cov[j].pos_corona.col_largura &&
              jogs[i].pos_jogador.x - jogs[i].pos_jogador.col_largura < cov[j].pos_corona.x + cov[j].pos_corona.col_largura){
          
          jogs[i].morto = true;
          cov[j].morto = true;
          }
        }
      }
    }
  }
}

// funções palavra de ordem
void p_ordem_iniciar(P_Ordem plvr[]){
  int i;
  for (i = 0; i < numero_de_palavras; i++){
    plvr[i].morto = true;
    plvr[i].lado = 0;
    plvr[i].velocidade = 5;
    plvr[i].pos_ordem.altura = 5;
    plvr[i].pos_ordem.largura = 20;
    plvr[i].pos_ordem.col_altura = plvr[i].pos_ordem.altura;
    plvr[i].pos_ordem.col_largura = plvr[i].pos_ordem.largura;
  }
}

void p_ordem_atirar(Jogador *jog){
  int j;
  if (!jog->morto){
    for (j = 0; j < numero_de_palavras; j++){
      if (jog->plvr[j].morto){
        jog->plvr[j].morto = false;
        jog->plvr[j].lado = jog->lado;
        jog->plvr[j].pos_ordem.x = jog->pos_jogador.x + 45*jog->lado;
        jog->plvr[j].pos0 = jog->pos_jogador.x + 45*jog->lado;
        jog->plvr[j].pos_ordem.y = jog->pos_jogador.y;
        break;
      }
    }
  }
}

void p_ordem_atualizar(Jogador jogs[]){
  int i, j;
  for (i = 0; i < numero_de_jogadores; i++){
    if (!jogs[i].morto){
      for (j = 0; j < numero_de_palavras; j++){
        if (!jogs[i].plvr[j].morto){
          if (jogs[i].plvr[j].pos_ordem.x - jogs[i].plvr[j].pos0 >= LARGURA_TELA/2 || jogs[i].plvr[j].pos_ordem.x - jogs[i].plvr[j].pos0 <= -LARGURA_TELA/2)
            jogs[i].plvr[j].morto = true;
          jogs[i].plvr[j].pos_ordem.x += jogs[i].plvr[j].lado*jogs[i].plvr[j].velocidade;
        }
      }
    }
  }
}

void p_ordem_desenhar(Jogador jogs[]){
  int i, j;
  for (i = 0; i < numero_de_jogadores; i++){
    if (!jogs[i].morto){
      for (j = 0; j < numero_de_palavras; j++){
        if (!jogs[i].plvr[j].morto){
          al_draw_text(fonte2, al_map_rgb(255, 255, 255), jogs[i].plvr[j].pos_ordem.x, 
                        jogs[i].plvr[j].pos_ordem.y-10, ALLEGRO_ALIGN_CENTRE, "Tira a mao");
          al_draw_text(fonte2, al_map_rgb(255, 255, 255), jogs[i].plvr[j].pos_ordem.x, 
                        jogs[i].plvr[j].pos_ordem.y, ALLEGRO_ALIGN_CENTRE, "do meu IFSC");
        }
      }
    }
  }
}

// funções alcool
void alcool_iniciar(Alcool alcools[]){ // colocar array de posição dos alcools
  int i;
  for (i = 0; i < numero_de_alcools; i++){
    alcools[i].morto = false;
    alcools[i].pego = false;
    alcools[i].lado = 0;
    alcools[i].idx = i;
    alcools[i].i_esg = 0;
    alcools[i].esguichando = false;
    alcools[i].pos_alcool.x = 1600;
    alcools[i].pos_alcool.y = 200;
    alcools[i].pos_alcool.altura = 15;
    alcools[i].pos_alcool.largura = 15;
    alcools[i].pos_alcool.col_altura = 10;
    alcools[i].pos_alcool.col_largura = 10;
  }
}

bool alcool_colidindo_jog(Alcool *alcool, Jogador *jog){
  return jog->pos_jogador.y - jog->pos_jogador.col_altura < alcool->pos_alcool.y + alcool->pos_alcool.col_altura &&
          jog->pos_jogador.y + jog->pos_jogador.col_altura > alcool->pos_alcool.y - alcool->pos_alcool.col_altura &&
          jog->pos_jogador.x + jog->pos_jogador.col_largura > alcool->pos_alcool.x - alcool->pos_alcool.col_largura &&
          jog->pos_jogador.x - jog->pos_jogador.col_largura < alcool->pos_alcool.x + alcool->pos_alcool.col_largura;
}

bool alcool_colidindo_corona(Alcool *alcool, Corona *cov){
  return cov->pos_corona.y - cov->pos_corona.col_altura < alcool->pos_alcool.y + alcool->pos_alcool.col_altura &&
          cov->pos_corona.y + cov->pos_corona.col_altura > alcool->pos_alcool.y - alcool->pos_alcool.col_altura &&
          cov->pos_corona.x + cov->pos_corona.col_largura > alcool->pos_alcool.x - alcool->pos_alcool.col_largura &&
          cov->pos_corona.x - cov->pos_corona.col_largura < alcool->pos_alcool.x + alcool->pos_alcool.col_largura;
}

void alcool_atirar(Alcool alcools[], Jogador *jog){
  if (jog->com_alcool != -1){
    alcools[jog->com_alcool].esguichando = true;
    alcools[jog->com_alcool].pego = false;
    alcools[jog->com_alcool].lado = jog->lado;
    alcools[jog->com_alcool].i_esg = 0;
    alcools[jog->com_alcool].pos_alcool.y = jog->pos_jogador.y;
    alcools[jog->com_alcool].pos_alcool.x = jog->pos_jogador.x + jog->lado*(jog->pos_jogador.largura+60);
    alcools[jog->com_alcool].pos_alcool.col_altura = 60;
    alcools[jog->com_alcool].pos_alcool.col_largura = 60;
  }
}

void alcool_atualizar(Alcool alcools[], Jogador jogs[], Corona covs[]){
  int i, j;
  for (j = 0; j < numero_de_alcools; j++){
    if (!alcools[j].morto){
      if (!alcools[j].pego && !alcools[j].esguichando){
        for (i = 0; i < numero_de_jogadores; i++){
          if (!jogs[i].morto && jogs[i].com_alcool == -1){
            if (alcool_colidindo_jog(&alcools[j], &jogs[i])){
              jogs[i].com_alcool = alcools[j].idx;
              alcools[j].pego = true;
              break;
            }
          }
        }
      } else if (alcools[j].esguichando){
        if (alcools[j].i_esg >= 10){
          alcools[j].pego = false;
          alcools[j].morto = true;
          for(i = 0; i < numero_de_jogadores; i++){
            if(jogs[i].com_alcool == j){
              jogs[i].com_alcool = -1;
            }
          }         
        } else {
          for(i = 0; i < numero_de_coronas; i++){
            if (alcool_colidindo_corona(&alcools[j], &covs[i])){
              covs[i].morto = true;
            }
          }
        }
        alcools[j].i_esg++;
      } else {
        for (i = 0; i < numero_de_jogadores; i++){
          if(jogs[i].com_alcool == j){
            if (jogs[i].morto)
              alcools[j].morto = true;
            alcools[j].pos_alcool.x = jogs[i].pos_jogador.x;
            alcools[j].pos_alcool.y = jogs[i].pos_jogador.y - jogs[i].pos_jogador.altura - alcools[j].pos_alcool.altura - 5;
          }
        }
      }
    }
  }
}

void alcool_desenhar(Alcool alcools[]){
  int i; 
  for (i = 0; i < numero_de_alcools; i++){
    if (!alcools[i].morto){
      if (!alcools[i].esguichando)
        al_draw_filled_circle(alcools[i].pos_alcool.x, alcools[i].pos_alcool.y, alcools[i].pos_alcool.altura, al_map_rgb(255, 255, 255));
      else {
        al_draw_filled_rectangle(alcools[i].pos_alcool.x - alcools[i].pos_alcool.col_largura,
         alcools[i].pos_alcool.y + alcools[i].pos_alcool.col_altura, 
          alcools[i].pos_alcool.x + alcools[i].pos_alcool.col_largura,
           alcools[i].pos_alcool.y - alcools[i].pos_alcool.col_altura, al_map_rgb(255, 255, 255));
      }
    }
  }
}

// atualizar dados jogador
void jog_iniciar(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogs[i_jog].pos_jogador.x = LARGURA_TELA/2;
    jogs[i_jog].x_anterior = LARGURA_TELA/2;
    jogs[i_jog].pos_jogador.y = 0;
    jogs[i_jog].y_anterior = ALTURA_TELA/2;
    jogs[i_jog].pos_jogador.altura = 20;
    jogs[i_jog].pos_jogador.largura = 20;
    jogs[i_jog].pos_jogador.col_altura = 15;
    jogs[i_jog].pos_jogador.col_largura = 15;
    jogs[i_jog].chao = false;
    jogs[i_jog].pulando = false;
    jogs[i_jog].mov_esquerda = false; 
    jogs[i_jog].mov_direita = false; 
    jogs[i_jog].morto = false;
    jogs[i_jog].com_alcool = -1;
    jogs[i_jog].posicao_y0 = 0; 
    jogs[i_jog].lado = 0;
    jogs[i_jog].i_cair = 0;
    jogs[i_jog].i_pulo = 0;
    p_ordem_iniciar(jogs[i_jog].plvr);
  }
}

void jog_atualizar_pos_anterior(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < numero_de_jogadores; i_jog++){
    jogs[i_jog].x_anterior = jogs[i_jog].pos_jogador.x;
    jogs[i_jog].y_anterior = jogs[i_jog].pos_jogador.y;
  }
}

void jog_atualizar_geral(Jogador jogs[], float plataformas[][4], float plataformas_mortais[][4], float mov_tela){
  int i_jog, i;
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
      for (i = 0; i < numero_de_plataformas; i++)
        jog_colisao_geral(plataformas[i], &jogs[i_jog], mov_tela);
      for (i = 0; i < numero_de_plataformas_mortais; i++)
        jog_colisao_mortal(plataformas_mortais[i], &jogs[i_jog], mov_tela);

      if (jogs[i_jog].pos_jogador.y >= ALTURA_TELA + 1000) jogs[i_jog].morto = true;
      jogs[i_jog].i_cair++;
    } else {
      jogs[i_jog].pos_jogador.x = LARGURA_TELA/2;
    }
  }
}

// lidar com botões
void apertar_botao(ALLEGRO_EVENT evento, Jogador jogs[], Alcool alcools[]){
  switch (evento.keyboard.keycode){
    // movimento jog 1
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
    // movimento jog 2
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
    // ataques jog 1
    case ALLEGRO_KEY_COMMA:
      p_ordem_atirar(&jogs[0]);
      break;
    case ALLEGRO_KEY_FULLSTOP:
      alcool_atirar(alcools, &jogs[0]);
      break;
    // ataques jog 2
    case ALLEGRO_KEY_H:
      p_ordem_atirar(&jogs[1]);
      break;
    case ALLEGRO_KEY_G:
      alcool_atirar(alcools, &jogs[1]);
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
  Corona covid[numero_de_coronas];
  Alcool alcools[numero_de_alcools];
  bool sair = false, fim = false, atualizar_e_desenho = false;
  float plataformas[numero_de_plataformas][4] = {
                {0, 690, 10000, ALTURA_TELA}, 
                {1100, 380, LARGURA_TELA, 420},
                {800, 520, 1050, 560},
                {1750, 380, 2000, 420},
                {1450, 520, 1700, 560},
              };
  float plataformas_mortais[numero_de_plataformas_mortais][4] = {{380, 600, 420, 690}};
  int r = 20;
  float mov_tela = 0; 

  if (!inicializar()) return -1;

  // inicializando as structs
  alcool_iniciar(alcools);
  jog_iniciar(jogadores);
  corona_iniciar(covid);

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){  
    al_wait_for_event(fila_eventos, &evento);

    if (evento.type == ALLEGRO_EVENT_TIMER){
      mover_tela(jogadores, covid, alcools, &mov_tela);      
      jog_atualizar_pos_anterior(jogadores);
      atualizar_e_desenho = true;
    }

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
      apertar_botao(evento, jogadores, alcools);
    else if (evento.type == ALLEGRO_EVENT_KEY_UP)
      soltar_botao(evento, jogadores);

    else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      sair = true;

    if (atualizar_e_desenho && al_is_event_queue_empty(fila_eventos)){
      atualizar_e_desenho = false;
      
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_bitmap(fundo, 0-mov_tela, 0, 0);

      jog_atualizar_geral(jogadores, plataformas, plataformas_mortais, mov_tela);
      p_ordem_atualizar(jogadores);
      corona_atualizar(covid, jogadores);
      corona_colide(covid, jogadores);
      alcool_atualizar(alcools, jogadores, covid);
      
      p_ordem_desenhar(jogadores);
      alcool_desenhar(alcools);

      #ifdef MOSTRAR_HITBOX
        int i;
        for (i = 0; i < numero_de_plataformas; i++)
          al_draw_filled_rectangle(plataformas[i][0]-mov_tela, plataformas[i][1], plataformas[i][2]-mov_tela, plataformas[i][3], al_map_rgb(255, 255, 0));
        for (i = 0; i < numero_de_plataformas_mortais; i++)
          al_draw_filled_rectangle(plataformas_mortais[i][0]-mov_tela, plataformas_mortais[i][1], plataformas_mortais[i][2]-mov_tela, plataformas_mortais[i][3], al_map_rgb(255, 255, 255));
      #endif

      if(!jogadores[0].morto) al_draw_filled_circle(jogadores[0].pos_jogador.x, jogadores[0].pos_jogador.y, r, al_map_rgb(255, 0, 0));
      if(!jogadores[1].morto) al_draw_filled_circle(jogadores[1].pos_jogador.x, jogadores[1].pos_jogador.y, r, al_map_rgb(0, 0, 255));
      for (i = 0; i < numero_de_coronas; i++)
        if(!covid[i].morto && covid[i].ativado){
          al_draw_filled_circle(covid[i].pos_corona.x, covid[i].pos_corona.y, covid[i].pos_corona.altura, al_map_rgb(255, 255, 255));
          al_draw_bitmap(covid[i].bm_corona, covid[i].pos_corona.x, covid[i].pos_corona.y, 0);
          al_draw_filled_rectangle(covid[i].pos_corona.x + covid[i].pos_corona.col_altura, covid[i].pos_corona.y - covid[i].pos_corona.col_altura,
           covid[i].pos_corona.x - covid[i].pos_corona.col_altura, covid[i].pos_corona.y + covid[i].pos_corona.col_altura, al_map_rgb(255, 255, 0));
        } 

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
