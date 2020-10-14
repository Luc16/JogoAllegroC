#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "cena_jogo.h"

// Inicializacao de variaveis globais
#define LARGURA_TELA 1360
#define ALTURA_TELA 750
#define FPS  60

// movimento de tela
bool jog_esta_no_limite_direita(Jogador *jog){
  return jog->pos_jogador.x >= LARGURA_TELA*5/8;
}

bool jog_esta_no_limite_esquerda(Jogador *jog){
  return jog->pos_jogador.x <= LARGURA_TELA*1/8;
}

void mover_tela_direita(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[], Minion minions[]){
  int i;
  jog_limite->ajuste_pos = jog_limite->pos_jogador.x - LARGURA_TELA*5/8;
  *mov_tela += jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*5/8;
  jog_outro->pos_jogador.x -= jog_limite->ajuste_pos;
  for (i = 0; i < NUM_ALCOOLS; i++){
    if (!alcools[i].pego)
      alcools[i].pos_alcool.x -= jog_limite->ajuste_pos;
  }
  for (i = 0; i < NUM_CORONAS; i++){
    if(!covs[i].morto){
        covs[i].pos_corona.x -= jog_limite->ajuste_pos;
    }
  }
  for(i=0; i<NUM_MINIONS; i++){
    if(!minions[i].morto){
        minions[i].pos_minion.x -= jog_limite->ajuste_pos;
        minions[i].limite1 -= jog_limite->ajuste_pos;
        minions[i].limite2 -= jog_limite->ajuste_pos;
    }
  }
  if (jog_esta_no_limite_esquerda(jog_outro))
    jog_outro->pos_jogador.x = LARGURA_TELA*1/8;
}

void mover_tela_esquerda(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[], Minion minions[]){
  int i;
  jog_limite->ajuste_pos = LARGURA_TELA*1/8 - jog_limite->pos_jogador.x;
  *mov_tela -= jog_limite->ajuste_pos;
  jog_limite->pos_jogador.x = LARGURA_TELA*1/8;
  jog_outro->pos_jogador.x += jog_limite->ajuste_pos;
  for (i = 0; i < NUM_ALCOOLS; i++){
    if (!alcools[i].pego)
      alcools[i].pos_alcool.x += jog_limite->ajuste_pos;
  }
  for (i = 0; i < NUM_CORONAS; i++){
    if (!covs[i].morto)
      covs[i].pos_corona.x += jog_limite->ajuste_pos;
  }
  for(i=0; i<NUM_MINIONS; i++){
    if(!minions[i].morto)
      minions[i].pos_minion.x += jog_limite->ajuste_pos;
      minions[i].limite1 += jog_limite->ajuste_pos;
      minions[i].limite2 += jog_limite->ajuste_pos;
  }
}

void mover_tela(Jogador jogs[], Corona covs[], Alcool alcools[], Minion minions[], float *mov_tela){
  if (jog_esta_no_limite_direita(&jogs[0]) && jogs[0].pos_jogador.x > jogs[1].pos_jogador.x){
    mover_tela_direita(mov_tela, &jogs[0], &jogs[1], covs, alcools, minions);
  }
  else if (jog_esta_no_limite_direita(&jogs[1])){
    mover_tela_direita(mov_tela, &jogs[1], &jogs[0], covs, alcools, minions);
  }
  else if (jog_esta_no_limite_esquerda(&jogs[0]) && jogs[0].pos_jogador.x < jogs[1].pos_jogador.x){
    mover_tela_esquerda(mov_tela, &jogs[0], &jogs[1], covs, alcools, minions);
  }
  else if (jog_esta_no_limite_esquerda(&jogs[1])){
    mover_tela_esquerda(mov_tela, &jogs[1], &jogs[0], covs, alcools, minions);
  }
}

// colisoes
bool jog_colidindo(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog){
    return jog->pos_jogador.y - jog->pos_jogador.altura < y_baixo &&
     jog->pos_jogador.y + jog->pos_jogador.altura > y_cima &&
      jog->pos_jogador.x + jog->pos_jogador.largura > x_esquerda &&
       jog->pos_jogador.x - jog->pos_jogador.largura < x_direita;
}

void jog_colisao_lateral(Jogador *jog, float x_esquerda, float x_direita, bool *deixar_tras){
  if (jog->x_anterior + jog->pos_jogador.largura <= x_esquerda)
    jog->pos_jogador.x = x_esquerda - 0.001 - jog->pos_jogador.largura;
  else if (jog->x_anterior - jog->pos_jogador.largura >= x_direita)
    jog->pos_jogador.x = x_direita + 0.001 + jog->pos_jogador.largura;
  if (jog_esta_no_limite_esquerda(jog))
    if (jog->x_anterior <= x_esquerda) *deixar_tras = true;
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

void jog_colisao_geral(float plataformas[], Jogador *jog, float mov_tela, bool *deixar_tras){
  float x_esquerda = plataformas[0] - mov_tela, y_cima = plataformas[1];
  float x_direita = plataformas[2] - mov_tela, y_baixo = plataformas[3];

  if (!(x_esquerda > LARGURA_TELA || x_direita < 0)){
    if (jog_colidindo(x_esquerda, x_direita, y_cima, y_baixo, jog)){
      jog_colisao_lateral(jog, x_esquerda, x_direita, deixar_tras);
      jog_colisao_teto(jog, y_baixo);
      jog_colisao_chao(jog, y_cima);
    }
  }
}

void desenhar_plataformas(float plataformas[][4], float plataformas_mortais[][4], float mov_tela){
  int i;
  for (i = 0; i < NUM_PLATAFORMAS; i++)
    al_draw_filled_rectangle(plataformas[i][0]-mov_tela, plataformas[i][1], plataformas[i][2]-mov_tela, plataformas[i][3], al_map_rgb(51, 0, 102));
  for (i = 0; i < NUM_PLATAFORMAS_MORTAIS; i++)
    al_draw_filled_rectangle(plataformas_mortais[i][0]-mov_tela, plataformas_mortais[i][1], plataformas_mortais[i][2]-mov_tela, plataformas_mortais[i][3], al_map_rgb(255, 255, 255));

}

// colisoes mortais
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

// funcoes minions
void minion_iniciar(Minion minions[], float pos[][5]){
    int i;
    for(i = 0; i < NUM_MINIONS; i++)
	{
		minions[i].vel = pos[i][4];
		minions[i].morto = false;
		minions[i].pos_minion.x = pos[i][0];
		minions[i].pos_minion.y = pos[i][1];
		minions[i].pos_minion.col_altura = 20;
		minions[i].pos_minion.col_largura = 20;
		minions[i].limite1 = pos[i][2];
		minions[i].limite2 = pos[i][3];
		minions[i].lado = 1;
	}
}

void minion_colide(Minion minions[], Jogador jogs[]){
    int i, j;
    for(j=0; j<NUM_MINIONS; j++){
        if(!minions[j].morto){
            for(i=0; i<NUM_JOGADORES; i++){
                if(!jogs[i].morto){
                    if(jogs[i].pos_jogador.y - jogs[i].pos_jogador.col_altura < minions[j].pos_minion.y + minions[j].pos_minion.col_altura &&
                       jogs[i].pos_jogador.y + jogs[i].pos_jogador.col_altura > minions[j].pos_minion.y - minions[j].pos_minion.col_altura &&
                       jogs[i].pos_jogador.x + jogs[i].pos_jogador.col_largura > minions[j].pos_minion.x - minions[j].pos_minion.col_largura &&
                       jogs[i].pos_jogador.x - jogs[i].pos_jogador.col_largura < minions[j].pos_minion.x + minions[j].pos_minion.col_largura){

                       jogs[i].morto = true;
                   }
                }
            }
        }
    }

}

void minion_atualizar(Minion minions[]){

    int i;
    for(i = 0; i < NUM_MINIONS; i++)
    {
       if(!minions[i].morto){
           minions[i].pos_minion.x += (minions[i].vel)*(minions[i].lado);

           if(minions[i].pos_minion.x >= minions[i].limite2){
                minions[i].pos_minion.x = minions[i].limite2;
                minions[i].lado = -1;
           }
           else if(minions[i].pos_minion.x <= minions[i].limite1){
                minions[i].pos_minion.x = minions[i].limite1;
                minions[i].lado = 1;
           }
       }
    }

}

void minion_vira_direita(Minion *minions){
    minions->lado = 1;
}

void minion_vira_esquerda(Minion *minions){
    minions->lado = -1;
}

void minion_desenhar(Minion minions[]){
  int i;
  for (i = 0; i < NUM_MINIONS; i++)
  if(!minions[i].morto){
    al_draw_filled_circle(minions[i].pos_minion.x, minions[i].pos_minion.y, 30, al_map_rgb(255, 255, 255));
  }
}

// funcoes corona
void corona_iniciar(Corona covs[], float pos[][4]){
  int i;
  for (i = 0; i < NUM_CORONAS; i++){
    covs[i].pos_corona.altura = 60;
    covs[i].pos_corona.largura = 60;
    covs[i].pos_corona.col_altura = 42;
    covs[i].pos_corona.col_largura = 42;
    covs[i].pos_corona.x = pos[i][0];
    covs[i].pos_corona.y = pos[i][1];
    covs[i].morto = false;
    covs[i].ativado = false;
    covs[i].velocidade = pos[i][3];
    covs[i].pos_nascer = pos[i][2];
    covs[i].bm_corona = al_load_bitmap("Images/Corona.png");
    if (!covs[i].bm_corona){
      fprintf(stderr, "Falha ao carregar imagem do corona.\n");
    }
    al_convert_mask_to_alpha(covs[i].bm_corona, al_map_rgb(255, 0, 0));
  }
}

void corona_movimento(Corona *cov, Jogador jogs[]){
  int i;
  float distancia = 10000, dx, dy;
  for (i = 0; i < NUM_JOGADORES; i++){
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

void corona_atualizar(Corona covs[], Jogador jogs[], float mov){
  int i, j;
  for (i = 0; i < NUM_CORONAS; i++){
    for(j = 0; j < NUM_JOGADORES; j++){
        if(jogs[j].pos_jogador.x + mov >= covs[i].pos_nascer){
            covs[i].ativado = true;
        }
    }
    if(!covs[i].morto && covs[i].ativado)
        corona_movimento(&covs[i], jogs);
  }
}

void corona_colide(Corona *cov, Jogador jogs[]){
  int i, j;
  for(j = 0; j < NUM_CORONAS; j++){
    if (!cov[j].morto && cov[j].ativado){
      for(i = 0; i < NUM_JOGADORES; i++){
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

void corona_desenhar(Corona covs[]){
  int i;
  for (i = 0; i < NUM_CORONAS; i++)
    if(!covs[i].morto && covs[i].ativado){
      al_draw_bitmap(covs[i].bm_corona, covs[i].pos_corona.x - covs[i].pos_corona.largura, covs[i].pos_corona.y - covs[i].pos_corona.altura, 0);
  }
}

// funcoes palavra de ordem
void p_ordem_iniciar(P_Ordem plvr[]){
  int i;
  for (i = 0; i < NUM_PALAVRAS; i++){
    plvr[i].morto = true;
    plvr[i].lado = 0;
    plvr[i].velocidade = 5;
    plvr[i].pos_ordem.altura = 5;
    plvr[i].pos_ordem.largura = 20;
    plvr[i].pos_ordem.col_altura = 10;
    plvr[i].pos_ordem.col_largura = 25;
  }
}

void p_ordem_atirar(Jogador *jog){
  int j;
  if (!jog->morto){
    for (j = 0; j < NUM_PALAVRAS; j++){
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

void p_ordem_colide(Jogador jogs[], Minion minions[])
{
   int i, j, k;
    for(k=0; k<NUM_JOGADORES; k++){
        if(!jogs[k].morto){
            for(j=0; j<NUM_PALAVRAS; j++){
                if(!jogs[k].plvr[j].morto){
                    for(i=0; i<NUM_MINIONS; i++){
                        if(!minions[i].morto){
                            if(jogs[k].plvr[j].pos_ordem.y - jogs[k].plvr[j].pos_ordem.col_altura < minions[i].pos_minion.y + minions[i].pos_minion.col_altura &&
                                jogs[k].plvr[j].pos_ordem.y + jogs[k].plvr[j].pos_ordem.col_altura > minions[i].pos_minion.y - minions[i].pos_minion.col_altura &&
                                jogs[k].plvr[j].pos_ordem.x + jogs[k].plvr[j].pos_ordem.col_largura > minions[i].pos_minion.x - minions[i].pos_minion.col_largura &&
                                jogs[k].plvr[j].pos_ordem.x - jogs[k].plvr[j].pos_ordem.col_largura < minions[i].pos_minion.x + minions[i].pos_minion.col_largura){
                                    if(jogs[k].plvr[j].lado == 1){
                                        minion_vira_direita(&minions[i]);
                                    }
                                    else if(jogs[k].plvr[j].lado == -1){
                                        minion_vira_esquerda(&minions[i]);
                                    }
                                    jogs[k].plvr[j].morto = true;
                         }
                       }
                    }
                }
            }
        }
    }
}

void p_ordem_atualizar(Jogador jogs[]){
  int i, j;
  for (i = 0; i < NUM_JOGADORES; i++){
    if (!jogs[i].morto){
      for (j = 0; j < NUM_PALAVRAS; j++){
        if (!jogs[i].plvr[j].morto){
          if (jogs[i].plvr[j].pos_ordem.x - jogs[i].plvr[j].pos0 >= LARGURA_TELA/2 || jogs[i].plvr[j].pos_ordem.x - jogs[i].plvr[j].pos0 <= -LARGURA_TELA/2)
            jogs[i].plvr[j].morto = true;
          jogs[i].plvr[j].pos_ordem.x += jogs[i].plvr[j].lado*jogs[i].plvr[j].velocidade;
        }
      }
    }
  }
}

void p_ordem_desenhar(Jogador jogs[], ALLEGRO_FONT *fonte2){
  int i, j;
  for (i = 0; i < NUM_JOGADORES; i++){
    if (!jogs[i].morto){
      for (j = 0; j < NUM_PALAVRAS; j++){
        if (!jogs[i].plvr[j].morto){
          al_draw_text(fonte2, al_map_rgb(255, 255, 255), jogs[i].plvr[j].pos_ordem.x,
                        jogs[i].plvr[j].pos_ordem.y - jogs[i].plvr[j].pos_ordem.col_altura, ALLEGRO_ALIGN_CENTRE, "Tira a mao");
          al_draw_text(fonte2, al_map_rgb(255, 255, 255), jogs[i].plvr[j].pos_ordem.x,
                        jogs[i].plvr[j].pos_ordem.y, ALLEGRO_ALIGN_CENTRE, "do meu IFSC");
        }
      }
    }
  }
}

// funcoes alcool
void alcool_iniciar(Alcool alcools[], float pos[][3]){
  int i;
  for (i = 0; i < NUM_ALCOOLS; i++){
    if(pos[i][2] == 1){
        alcools[i].morto = false;
    }
    else {
        alcools[i].morto = true;
    }
    alcools[i].chefao = false;
    alcools[i].pego = false;
    alcools[i].lado = 0;
    alcools[i].idx = i;
    alcools[i].i_esg = 0;
    alcools[i].i_chefao = 0;
    alcools[i].esguichando = false;
    alcools[i].pos_alcool.x = pos[i][0];
    alcools[i].pos_alcool.y = pos[i][1];
    alcools[i].pos_alcool.altura = 15;
    alcools[i].pos_alcool.largura = 15;
    alcools[i].pos_alcool.col_altura = 10;
    alcools[i].pos_alcool.col_largura = 10;
    alcools[i].bm_alcool = al_load_bitmap("Images/Alquingel.png");
    if (!alcools[i].bm_alcool){
      fprintf(stderr, "Falha ao carregar imagem do alcool.\n");
    }
    al_convert_mask_to_alpha(alcools[i].bm_alcool, al_map_rgb(0, 0, 0));

    alcools[i].bm_esguicho = al_load_bitmap("Images/Esguicho.png");
    if (!alcools[i].bm_esguicho){
      fprintf(stderr, "Falha ao carregar imagem do esguicho.\n");
    }
    al_convert_mask_to_alpha(alcools[i].bm_esguicho, al_map_rgb(0, 0, 0));
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
    alcools[jog->com_alcool].lado = jog->lado;
    alcools[jog->com_alcool].i_esg = 0;
    alcools[jog->com_alcool].pos_alcool.y = jog->pos_jogador.y;
    alcools[jog->com_alcool].pos_alcool.x = jog->pos_jogador.x + jog->lado*(jog->pos_jogador.largura+60);
    alcools[jog->com_alcool].pos_alcool.col_altura = 60;
    alcools[jog->com_alcool].pos_alcool.col_largura = 60;
  }
}

void alcool_chefao(Alcool alcools[], Jogador jogs[], float mov, int *k)
{
    int i, j;
    for (j = 0; j < NUM_JOGADORES; j++){
        if(jogs[j].pos_jogador.x + mov >= 600){
            for(i = 6; i < NUM_ALCOOLS; i++){
                alcools[i].chefao = true;
            }
        }
   }

  if (!alcools[*k].pego){
    alcools[*k].morto = false;
  }
  else {
        alcools[*k].i_chefao++;
   }
   if(alcools[*k].i_chefao == 180){
        *k += 1;
   }

}

void alcool_atualizar(Alcool alcools[], Jogador jogs[], Corona covs[]){
  int i, j;
  for (j = 0; j < NUM_ALCOOLS; j++){
    if (!alcools[j].morto){
      if (!alcools[j].pego && !alcools[j].esguichando){
        for (i = 0; i < NUM_JOGADORES; i++){
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
          alcools[j].morto = true;
          for(i = 0; i < NUM_JOGADORES; i++){
            if(jogs[i].com_alcool == j){
              jogs[i].com_alcool = -1;
            }
          }
        } else {
          for(i = 0; i < NUM_CORONAS; i++){
            if (alcool_colidindo_corona(&alcools[j], &covs[i])){
              covs[i].morto = true;
            }
          }
        }
        alcools[j].i_esg++;
      } else {
        for (i = 0; i < NUM_JOGADORES; i++){
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
  for (i = 0; i < NUM_ALCOOLS; i++){
    if (!alcools[i].morto){
      if (!alcools[i].esguichando){
        al_draw_bitmap(alcools[i].bm_alcool, alcools[i].pos_alcool.x-alcools[i].pos_alcool.largura, alcools[i].pos_alcool.y-alcools[i].pos_alcool.altura, 0);
      }
      else {
        if (alcools[i].lado == 1)
          al_draw_bitmap(alcools[i].bm_esguicho, alcools[i].pos_alcool.x-alcools[i].pos_alcool.col_largura, alcools[i].pos_alcool.y-alcools[i].pos_alcool.col_altura, ALLEGRO_FLIP_HORIZONTAL);
        else 
          al_draw_bitmap(alcools[i].bm_esguicho, alcools[i].pos_alcool.x-alcools[i].pos_alcool.col_largura, alcools[i].pos_alcool.y-alcools[i].pos_alcool.col_altura, 0);
      }
    }
  }
}

// atualizar dados jogador
void jog_iniciar(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < NUM_JOGADORES; i_jog++){
    jogs[i_jog].pos_jogador.x = 200;
    jogs[i_jog].x_anterior = 200;
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
    jogs[i_jog].lado = 1;
    jogs[i_jog].i_cair = 0;
    jogs[i_jog].i_pulo = 0;
    p_ordem_iniciar(jogs[i_jog].plvr);
  }
}

void jog_atualizar_pos_anterior(Jogador jogs[]){
  int i_jog;
  for (i_jog = 0; i_jog < NUM_JOGADORES; i_jog++){
    jogs[i_jog].x_anterior = jogs[i_jog].pos_jogador.x;
    jogs[i_jog].y_anterior = jogs[i_jog].pos_jogador.y;
  }
}

void jog_atualizar_geral(Jogador jogs[], float plataformas[][4], float plataformas_mortais[][4], float mov_tela){
  int i_jog, i, atrasado;
  bool deixar_pra_tras = false;
  for (i_jog = 0; i_jog < NUM_JOGADORES; i_jog++){
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
      for (i = 0; i < NUM_PLATAFORMAS; i++)
        jog_colisao_geral(plataformas[i], &jogs[i_jog], mov_tela, &deixar_pra_tras);
      for (i = 0; i < NUM_PLATAFORMAS_MORTAIS; i++)
        jog_colisao_mortal(plataformas_mortais[i], &jogs[i_jog], mov_tela);

      if (jogs[i_jog].pos_jogador.y >= ALTURA_TELA + 1000) jogs[i_jog].morto = true;
      if (deixar_pra_tras){
        atrasado = i_jog;
      }
      jogs[i_jog].i_cair++;
    } else {
      jogs[i_jog].pos_jogador.x = LARGURA_TELA/2;
    }
    if (deixar_pra_tras && jog_esta_no_limite_direita(&jogs[(atrasado-1)*(-1)])){
        jogs[0].morto = true;
        jogs[1].morto = true;
      }
  }
}

void jog_desenhar(Jogador jogs[], int *i_desenho, int *espera, int *desenho_atual, ALLEGRO_BITMAP *pato_parado[], ALLEGRO_BITMAP *pato_pulando[], ALLEGRO_BITMAP *pato_andando[]){
  if(!jogs[1].morto) al_draw_filled_circle(jogs[1].pos_jogador.x, jogs[1].pos_jogador.y, 20, al_map_rgb(0, 0, 255));
  if(!jogs[0].morto) {
    if(!jogs[0].chao){
      *i_desenho += 1;
      if(*i_desenho >= *espera){
        *desenho_atual += 1;
        if(*desenho_atual >= MAX_DESENHO_PULANDO){
          *desenho_atual = 0;
        }
        *i_desenho = 0;
      }
      al_draw_bitmap(pato_pulando[*desenho_atual], jogs[0].pos_jogador.x - 30, jogs[0].pos_jogador.y - 30, 0);
    }
    else if(jogs[0].mov_direita){
      if(*desenho_atual >= MAX_DESENHO_ANDANDO)
        *desenho_atual = 0;
      *i_desenho += 1;
      if(*i_desenho >= *espera){
        *desenho_atual += 1;
        if(*desenho_atual >= MAX_DESENHO_ANDANDO){
          *desenho_atual = 0;
        }
        *i_desenho = 0;
      }
      al_draw_bitmap(pato_andando[*desenho_atual], jogs[0].pos_jogador.x - 30, jogs[0].pos_jogador.y - 30, 0);
    }
    else if(jogs[0].mov_esquerda){
      if(*desenho_atual >= MAX_DESENHO_ANDANDO)
        *desenho_atual = 0;
      *i_desenho += 1;
      if(*i_desenho >= *espera){
        *desenho_atual += 1;
        if(*desenho_atual >= MAX_DESENHO_ANDANDO){
          *desenho_atual = 0;
        }
        *i_desenho = 0;
      }
      al_draw_bitmap(pato_andando[*desenho_atual], jogs[0].pos_jogador.x - 30, jogs[0].pos_jogador.y - 30, ALLEGRO_FLIP_HORIZONTAL);
    } else {
      if(*desenho_atual >= MAX_DESENHO_PARADO)
        *desenho_atual = 0;
      *i_desenho += 1;
      if(*i_desenho >= *espera){
        *desenho_atual += 1;
        if(*desenho_atual >= MAX_DESENHO_PARADO){
          *desenho_atual = 0;
        }
        *i_desenho = 0;
      }
      if (jogs[0].lado == 1){
        al_draw_bitmap(pato_parado[*desenho_atual], jogs[0].pos_jogador.x - 30, jogs[0].pos_jogador.y - 30, 0);
      }
      else 
        al_draw_bitmap(pato_parado[*desenho_atual], jogs[0].pos_jogador.x - 30, jogs[0].pos_jogador.y - 30, ALLEGRO_FLIP_HORIZONTAL);
    }
  }
}

void pato_iniciar(ALLEGRO_BITMAP *pato_parado[], ALLEGRO_BITMAP *pato_pulando[], ALLEGRO_BITMAP *pato_andando[], char local[]){
    int i;
  	for(i=0; i<MAX_DESENHO_PARADO; i++){
        sprintf(local, "Images/pato/pato_idle/pato_idle-%d.png.png", i+1);
        pato_parado[i] = al_load_bitmap(local);
        if(!pato_parado[i]){
          fprintf(stderr, "erro com o pato");
        }
    }
    for(i=0; i<MAX_DESENHO_PULANDO; i++){
        sprintf(local, "Images/pato/pato_jump/pato_jump-%d.png.png", i+1);
        pato_pulando[i] = al_load_bitmap(local);
        if(!pato_pulando[i]){
          fprintf(stderr, "erro com o pato");
        }
    }
    for(i=0; i<MAX_DESENHO_ANDANDO; i++){
        sprintf(local, "Images/pato/pato_walk/pato_walk-%d.png.png", i+1);
        pato_andando[i] = al_load_bitmap(local);
        if(!pato_andando[i]){
          fprintf(stderr, "erro com o pato");
        }
    }
}

// lidar com botoes
void apertar_botao(ALLEGRO_EVENT evento, Jogador jogs[], Alcool alcools[], float mover_tela){
  switch (evento.keyboard.keycode){
    // movimento jog 1
    case ALLEGRO_KEY_UP:
      if (jogs[0].chao){
        jog_ativar_pulo(&jogs[0]);
      }
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

int jogo(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *fundo, ALLEGRO_FONT *fonte,  ALLEGRO_FONT *fonte2, ALLEGRO_EVENT evento, ALLEGRO_TIMER *temporizador, int num_jogs){
  // variaveis iniciais
  Jogador jogadores[NUM_JOGADORES];
  Corona covid[NUM_CORONAS];
  Alcool alcools[NUM_ALCOOLS];
  Minion minions[NUM_MINIONS];
  ALLEGRO_BITMAP *pato_parado[MAX_DESENHO_PARADO], *pato_pulando[MAX_DESENHO_PULANDO], *pato_andando[MAX_DESENHO_ANDANDO];
  char local[100];
  int k_alcool_chefao = 6, i_desenho = 0, espera = 5, desenho_atual = 0;
  float mov_tela = 0;
  bool sair = false, fim = false, atualizar_e_desenho = false, xis = false;
  float plataformas[NUM_PLATAFORMAS][4] = {
                {0, 0, 20, ALTURA_TELA},
                {0, 690, 2000, ALTURA_TELA},
                {600, 520, 900, 560},
                {1100, 380, LARGURA_TELA, 420},
                {1940, 490, 2000, 690},
                {2330, 375, 2700, 415},
                {2900, 240, 3400, 280},
                {3200, 690, 4900, ALTURA_TELA},
                {3750, 200, 4200, 240},
                {4160, 240, 4200, 260},
                {3750, 240, 3790, 260},
                {3950, 500, 4000, 540},
                {4500, 420, 4600, 530},
                {4475, 90, 4625, 240},
                {4650, 520, 4710, 560},
                {4800, 280, 5100, 320},
                {5500, 520, 5560, 560},
                {5760, 320, 5820, 360},
                {6150, 140, 6350, 180},
                {6160, 600, 6260, 640},
                {6420, 420, 6520, 460},
                {6630, 280, 6730, 320},
                {7080, 640, 7180, 680},
                {7400, 690, 7800, ALTURA_TELA},
                {8000, 600, 8250, 640},
                {8450, 420, 8700, 460},
                {8900, 280, 9150, 320},
                {9450, 690, 15000, ALTURA_TELA},
                {10200, 0, 10240, 590},
                {10240, 80, 10320, 120},
                {10240, 300, 10420, 340},
                {10240, 520, 10420, 560},
                {10300, 190, 10500, 230},
                {10300, 410, 10500, 450},
                {10300, 630, 10500, 670},
                {10500, 100, 10540, 690},
                {10640, 360, 10840, 400},
                {10940, 520, 11140, 560},
                {11150, 100, 11250, 140},
                {11400, 280, 11500, 320},
                {11600, 120, 12350, 160},
                {11650, 350, 11750, 390},
                {11950, 450, 12050, 490},
                {12200, 420, 12300, 460},
                {12400, 580, 12500, 620},
                {12500, 80, 12540, 690},
                {12700, 320, 12950, 360},
                {13050, 500, 13400, 540},
                {13175, 160, 13275, 200},
                {13500, 320, 13750, 360},
                {13800, 140, 13860, 180},
                {13860, -1000, 13900, 690}
              };
  float plataformas_mortais[NUM_PLATAFORMAS_MORTAIS][4] = {
                {0, 0, 0, 0}
              };
  float minions_pos[NUM_MINIONS][5] = {
      {1200, 660, 800, 1850, 4},
      {3500, 660, 3200, 3950, 4},
      {4300, 660, 4000, 4700, 4},
      {7600, 660, 7400, 7800, 4},
      {8000, 570, 8000, 8250, 3},
      {8450, 390, 8450, 8700, 3},
      {8900, 250, 8900, 9150, 3},
      {10570, 660, 10570, 12470, 4},
      {10950, 660, 10570, 12470, 4},
      {11330, 660, 10570, 12470, 4},
      {11710, 660, 10570, 12470, 4},
      {12090, 660, 10570, 12470, 4},
      {12470, 660, 10570, 12470, 4},
      {11600, 90, 11600, 12350, 4},
      {11975, 90, 11600, 12350, 4},
      {12350, 90, 11600, 12350, 4},
  };
  float alcools_pos[NUM_ALCOOLS][3] = {
        {3975, 480, 1},
        {6250, 120, 1},
        {7600, 670, 1},
        {10280, 60, 1},
        {11200, 80, 1},
        {13830, 120, 1},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0},
        {13225, 140, 0}
  };
  float corona_pos[NUM_CORONAS][4] = {
        {2000, 0, 2330, 3},
        {4550, 0, 3900, 2},
        {8100, 0, 8575, 4.5},
        {9400, 200, 10100, 2},
        {12620, 0, 13175, 2.5},
        {12620, 300, 13175, 2.5},
        {12620, 600, 13175, 2.5},
        {13780, 0, 13175, 2.5},
        {13780, 300, 13175, 2.5},
        {13780, 600, 13175, 2.5},
  };

  // inicializando as structs
  pato_iniciar(pato_parado, pato_pulando, pato_andando, local);
  alcool_iniciar(alcools, alcools_pos);
  jog_iniciar(jogadores);
  corona_iniciar(covid, corona_pos);
  minion_iniciar(minions, minions_pos);

  if (num_jogs == 1) jogadores[1].morto = true;

  al_draw_bitmap(fundo, 0, 0, 0);

  while (!sair){
    al_wait_for_event(fila_eventos, &evento);

    if (evento.type == ALLEGRO_EVENT_TIMER){
      mover_tela(jogadores, covid, alcools, minions, &mov_tela);
      jog_atualizar_pos_anterior(jogadores);
      atualizar_e_desenho = true;
    }

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
      apertar_botao(evento, jogadores, alcools, mov_tela);
    else if (evento.type == ALLEGRO_EVENT_KEY_UP)
      soltar_botao(evento, jogadores);

    else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
      sair = true;
      xis = true;
    }

    if (atualizar_e_desenho && al_is_event_queue_empty(fila_eventos)){
      atualizar_e_desenho = false;

      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_bitmap(fundo, 0, 0, 0);

      jog_atualizar_geral(jogadores, plataformas, plataformas_mortais, mov_tela);
      p_ordem_atualizar(jogadores);
      corona_atualizar(covid, jogadores, mov_tela);
      corona_colide(covid, jogadores);
      alcool_atualizar(alcools, jogadores, covid);
      alcool_chefao(alcools, jogadores, mov_tela, &k_alcool_chefao);
      minion_atualizar(minions);
      p_ordem_colide(jogadores, minions);
      minion_colide(minions, jogadores);

      p_ordem_desenhar(jogadores, fonte2);
      alcool_desenhar(alcools);
      corona_desenhar(covid);
      minion_desenhar(minions);
      jog_desenhar(jogadores, &i_desenho, &espera, &desenho_atual, pato_parado, pato_pulando, pato_andando);
      desenhar_plataformas(plataformas, plataformas_mortais, mov_tela);

      // ganhar o jogo 
      if (covid[4].morto && covid[5].morto && covid[6].morto && covid[7].morto && covid[8].morto && covid[9].morto){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2-100, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU O JOGO");
        fim = true;
      }
      // acabar o jogo
      if (fim){
        al_rest(4);
        sair = true;
      }
      // morte
      if (jogadores[0].morto && jogadores[1].morto){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2-100, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU O JOGO");
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "(Quando voce deixa seu amigo");
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2+100, ALLEGRO_ALIGN_CENTRE, "para tras, os dois perdem)");
        fim = true;
      }

      al_flip_display();
    }
  }

  if (xis) return 2;
  else return 0;
}
