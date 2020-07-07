// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>

// Para utilizarmos a função fprintf
#include <stdio.h>

// Atributos da tela
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
int i = 0;
int j = 0;
int r = 75;
int a = 0;

int main(void)
{
  // A nossa janela
  ALLEGRO_DISPLAY *janela = NULL;

  // O nosso arquivo de fonte
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_FONT *fonte_da_Ana = NULL;

  // Inicialização da biblioteca Allegro
  if (!al_init())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }

  // Inicialização do add-on para uso de fontes
  al_init_font_addon();

  // Inicialização do add-on para uso de fontes True Type
  if (!al_init_ttf_addon())
  {
    fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
    return -1;
  }

  // Criação da nossa janela
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela)
  {
    fprintf(stderr, "Falha ao criar janela.\n");
    return -1;
  }

  // Carregando o arquivo de fonte
//   ALLEGRO_FONT *_al_load_bitmap_font(const char *filename, int size, int flags);
  fonte = al_load_font("Roboto-Regular.ttf", 30, 0);
  fonte_da_Ana = al_load_font("Roboto-Regular.ttf", 60, 0);
  if (fonte == NULL || fonte_da_Ana == NULL)
  {
    al_destroy_display(janela);
    fprintf(stderr, "Falha ao carregar fonte.\n");
    return -1;
  }

  // Preenchemos a tela com a cor branca
  al_clear_to_color(al_map_rgb(255, 255, 255));

  // Texto alinhado à esquerda
  // eq circ: (x-x0)^2+(y-y0)^2=r^2
  for (i=0;i<360;i++){
    if (sin(i)*360<80){
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 245+r*cos(i), 200+r*sin(i), ALLEGRO_ALIGN_LEFT, ".");
        al_draw_text(fonte, al_map_rgb(255, 0, 0), 395+r*cos(i), 200+r*sin(i), ALLEGRO_ALIGN_LEFT, ".");
    }
    for (j=0;j<360;j++){
        if (i == j){
            al_draw_text(fonte, al_map_rgb(255, 0, 0), 170+j*5/12, 200+i*5/9, ALLEGRO_ALIGN_LEFT, ".");
            al_draw_text(fonte, al_map_rgb(255, 0, 0), 320+j*5/12, 400-i*5/9, ALLEGRO_ALIGN_LEFT, ".");
        }
      }
  }

  // Exemplo de impressão de valores variáveis
  char *texto = "A Ana é perfeita";
  al_draw_textf(fonte_da_Ana, al_map_rgb(0, 192, 159), LARGURA_TELA / 2, 250, ALLEGRO_ALIGN_CENTRE, texto);

  // Atualizamos a tela
  al_flip_display();

  // E aguardamos 10 segundos
  al_rest(15.0);

  // Desalocação da fonte e da janela
  al_destroy_font(fonte);
  al_destroy_display(janela);

  return 0;
}