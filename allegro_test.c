#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
 
const int LARGURA_TELA = 1360;
const int ALTURA_TELA = 750;
struct move {
  float posx, posy, posx_a, posy_a;
};
struct player{
  struct move move_p;
  bool up, chao, side;
  float posy0, t;
  int i, j, sidex;
};
 
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_EVENT evento;
char *plano_de_fundo = "Images/bk_main.png";

bool inicializar()
{
  if (!al_init())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return false;
  }
 
  al_init_font_addon();
 
  if (!al_init_ttf_addon())
  {
    fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
    return false;
  }
 
  if (!al_init_image_addon())
  {
    fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
    return false;
  }
 
  if (!al_install_keyboard())
  {
    fprintf(stderr, "Falha ao inicializar o teclado.\n");
    return false;
  }
 
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela)
  {
    fprintf(stderr, "Falha ao criar janela.\n");
    return false;
  }
 
  al_set_window_title(janela, "Joguinho");
 
  fonte = al_load_font("Roboto-Regular.ttf", 30, 0);
  if (!fonte)
  {
    fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
    al_destroy_display(janela);
    return false;
  }
 
  fila_eventos = al_create_event_queue();
  if (!fila_eventos)
  {
    fprintf(stderr, "Falha ao criar fila de eventos.\n");
    al_destroy_display(janela);
    return false;
  }
 
  fundo = al_load_bitmap(plano_de_fundo);
  if (!fundo)
  {
    fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return false;
  }
 
  al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
  return true;
}

void colisao(float x1, float x2, float y, struct player *p, int raio){
  if (p->move_p.posy - raio < y + 20 && p->move_p.posy + raio > y - 20 && p->move_p.posx + raio > x1 && p->move_p.posx - raio < x2){
    if (p->move_p.posx_a + raio <= x1 || p->move_p.posx_a - raio >= x2 )
      p->move_p.posx = p->move_p.posx_a; 
    if (p->move_p.posy_a - raio > y + 20) {
      p->move_p.posy = y + 20.001 + raio;
      p->up = false;
    }
    if (p->move_p.posy_a + raio < y - 20){
      p->move_p.posy = y - 20.001 - raio;
      p->chao = true;
      p->i = 0;
      p->up  = false;
    }
  }
}

void lateral(struct player *p, int pm){
  p->side = true; 
  p->sidex = pm;
}

int main(){

  struct player p[2];
  bool sair = false;
  int r = 20, n;
  double time, time2;
  
  for (n = 0; n < 2; n++){
    p[n].move_p.posx = LARGURA_TELA/2;
    p[n].move_p.posx_a = LARGURA_TELA/2;
    p[n].move_p.posy = 0;
    p[n].move_p.posy_a = 0;
    p[n].chao = false;
    p[n].up = false;
    p[n].side = false; 
    p[n].t = 0;
    p[n].posy0 = 0; 
    p[n].sidex = 0;
    p[n].i = 0;
    p[n].j = 0;
  }
  if (!inicializar()) return -1;
 
  al_draw_bitmap(fundo, 0, 0, 0);
 
  while (!sair)
  {  
    time = al_get_time();
    for (n = 0; n < 2; n++){
      p[n].move_p.posx_a = p[n].move_p.posx;
      p[n].move_p.posy_a = p[n].move_p.posy;
    }

    while(!al_is_event_queue_empty(fila_eventos)){
      al_wait_for_event(fila_eventos, &evento);
 
      if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        switch (evento.keyboard.keycode){
          case ALLEGRO_KEY_UP:
            if (p[0].chao){
              p[0].chao = false;
              p[0].up = true;
              p[0].j = 0;
              p[0].posy0 = p[0].move_p.posy;
            }
            break;
          case ALLEGRO_KEY_DOWN:
            p[0].move_p.posy += 20;
            break;
          case ALLEGRO_KEY_LEFT:
            if (!p[0].side){
              lateral(&p[0], -1);
              // p[0].side = true; 
              // p[0].sidex = -1;               
            }
            break; 
          case ALLEGRO_KEY_RIGHT:
            if (!p[0].side){
              lateral(&p[0], 1);
              // p[0].side = true; 
              // p[0].sidex = 1;               
            }            
            break;
          case ALLEGRO_KEY_W:
            if (p[1].chao){
              p[1].chao = false;
              p[1].up = true;
              p[1].j = 0;
              p[1].posy0 = p[1].move_p.posy;
            }
            break;
          case ALLEGRO_KEY_S:
            p[1].move_p.posy += 20;
            break;
          case ALLEGRO_KEY_A:
            if (!p[1].side){
              p[1].side = true; 
              p[1].sidex = -1;               
            }
            break; 
          case ALLEGRO_KEY_D:
            if (!p[1].side){
              p[1].side = true; 
              p[1].sidex = 1;               
            }            
            break;
        }
      }
      else if (evento.type == ALLEGRO_EVENT_KEY_UP){
        switch (evento.keyboard.keycode){
          case ALLEGRO_KEY_RIGHT:
            p[0].side = false;
            break;
          case ALLEGRO_KEY_LEFT:
            p[0].side = false;
            break;
          case ALLEGRO_KEY_D:
            p[1].side = false;
            break;
          case ALLEGRO_KEY_A:
            p[1].side = false;
            break;
        }
      }
      else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        sair = true;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fundo, 0, 0, 0);
    
    for (n = 0; n < 2; n++){
      if (p[n].up){
        p[n].move_p.posy = p[n].posy0 + 400*p[n].j*p[n].j/3600.0 - 600*p[n].j/60.0;
        p[n].j++;
      } else {
        p[n].chao = false;
        p[n].move_p.posy += 10*p[n].i*p[n].i/3600.0;
      }
      if (p[n].side){
        p[n].move_p.posx += p[n].sidex*4;
      }
      // chão 
      colisao(0, LARGURA_TELA, 710, &p[n], r);
      // baixo direita canto
      colisao(1100, LARGURA_TELA, 400, &p[n], r);
      // baixo direta meio
      colisao(800, 1050, 540, &p[n], r);
      if (p[n].move_p.posy >= ALTURA_TELA)
        al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU O JOGO");
      if (p[n].move_p.posy >= ALTURA_TELA + 1000) sair = true;
      p[n].i++;
    }
    // chão 
    al_draw_line(0, 710, LARGURA_TELA, 710, al_map_rgb(0, 255, 0), 40);
    // baixo direita canto
    al_draw_line(1100, 400, LARGURA_TELA, 400, al_map_rgb(255, 255, 0), 40);
    // baixo direta meio
    al_draw_line(800, 540, 1050, 540, al_map_rgb(255, 255, 0), 40);


    al_draw_filled_circle(p[0].move_p.posx, p[0].move_p.posy, r, al_map_rgb(255, 0, 0));
    al_draw_filled_circle(p[1].move_p.posx, p[1].move_p.posy, r, al_map_rgb(0, 0, 255));

    
    al_flip_display();

    // setando fps
    time2 = al_get_time() - time;
    if (time2 < 1.0/60.0)
      al_rest(1.0/60.0-time2);

  }
 
  al_destroy_font(fonte);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);
 
  return 0;
} 
