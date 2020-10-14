#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn/jsmn.h"
#include <curl/curl.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdbool.h>
#include "cena_podio.h"

#define LARGURA_TELA 1360
#define ALTURA_TELA 750
#define NUM_PODIO 6
#define FPS 60

void iniciar_string(String *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t funcao_escrever(void *ptr, size_t size, size_t nmemb, String *s){
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int comparador(const TopJogador* lhs, const TopJogador* rhs){
    return -(lhs->pontos - rhs->pontos);
}

void manipular_entrada(ALLEGRO_EVENT evento, char nome[]){
  if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
    if (strlen(nome) <= 16){
      char temp[] = {evento.keyboard.unichar, '\0'};
      if (evento.keyboard.unichar == ' '){
        strcat(nome, temp);
      }
      else if (evento.keyboard.unichar >= '0' &&
          evento.keyboard.unichar <= '9'){
        strcat(nome, temp);
      }
      else if (evento.keyboard.unichar >= 'A' &&
          evento.keyboard.unichar <= 'Z'){
        strcat(nome, temp);
      }
      else if (evento.keyboard.unichar >= 'a' &&
          evento.keyboard.unichar <= 'z'){
        strcat(nome, temp);
      }
    }
 
    if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(nome) != 0)
    {
      nome[strlen(nome) - 1] = '\0';
    }
  }
}

void exibir_texto_centralizado(char nome[], ALLEGRO_FONT *fonte){
  if (strlen(nome) > 0)
    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2,
        (ALTURA_TELA - al_get_font_ascent(fonte)) / 2,
        ALLEGRO_ALIGN_CENTRE, nome);
}

bool valida_nome(char nome[], TopJogador top[]){
  int i, k = 0;
  for (i = 0; i < 5; i++){
    if (strcmp(nome, top[i].nome) != 0){
      k++;
    }
  }
  if (k == 5) return true;
  else return false;
}

int podio(int pontuacao, ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_FONT *fonte,  ALLEGRO_EVENT evento, ALLEGRO_TIMER *temporizador){
  // variáveis básicas
  CURL *curl;
  CURLcode res;
  char nome[17] = "", json[500];
  bool sair = false, concluido = false, xis = false, redesenhar = false;
  ALLEGRO_BITMAP *podio = al_load_bitmap("Images/Podio.png");
  int num_parsed, i, j = 0;
  TopJogador top[NUM_PODIO]; 
  jsmn_parser parser;
  jsmntok_t t[128];

  if (!podio){
      fprintf(stderr, "Falha ao carregar imagem do podio.\n");
    }
  // iniciando o parser de json, o curl e o allegro
  jsmn_init(&parser);
  curl = curl_easy_init();
  // if (!inicializar()) return -1;

  // só faz se tiver internet
  if(curl) {
    // inicializando string
    String s;
    iniciar_string(&s);

    // inicia o curl e coloca o conteudo para dentro da string
    curl_easy_setopt(curl, CURLOPT_URL, "https://jogoallegro.firebaseio.com/podio.json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, funcao_escrever);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    // parse a string
    num_parsed = jsmn_parse(&parser, s.ptr, s.len, t, sizeof(t) / sizeof(t[0]));

    if (num_parsed < 0) {
        printf("Failed to parse JSON: %d\n", num_parsed);
        return 1;
    }
    // inicializa o vetor de struct TopJogador e ordena
    for (i = 1; i < num_parsed; i+=2){
        strncpy(top[j].nome, s.ptr+t[i].start, t[i].end - t[i].start);
        top[j].nome[t[i].end - t[i].start] = '\0';

        top[j].pontos = atoi(s.ptr + t[i+1].start);
        j++;
    }
    qsort(top, NUM_PODIO-1, sizeof top[0], comparador);

    while(!sair){
      al_wait_for_event(fila_eventos, &evento);

      if (evento.type == ALLEGRO_EVENT_TIMER){
        redesenhar = true;
      }
        // checa se entra no podio
        if (pontuacao > top[NUM_PODIO-2].pontos){
          top[5].pontos = pontuacao;
          if(!concluido){
            // escreve o nome
            manipular_entrada(evento, nome);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
              // valida o nome e escreve coloca ele como top e ordena o vetor
              if(valida_nome(nome, top)){
                concluido = true;
                strcpy(top[5].nome, nome);
                qsort(top, NUM_PODIO, sizeof top[0], comparador);

                // coloca no servidor
                sprintf(json, "{ \"%s\": %d, \"%s\": %d, \"%s\": %d, \"%s\": %d, \"%s\": %d }", top[0].nome, top[0].pontos, 
                  top[1].nome, top[1].pontos, top[2].nome, top[2].pontos, top[3].nome, top[3].pontos, top[4].nome, top[4].pontos);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); 
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
                res = curl_easy_perform(curl);
              } else {
                // caso o nome seja inválido
                al_show_native_message_box(NULL,"Alerta","Alerta",
                      "Esse nome já foi utilizado, tente colocar outro",
                          NULL,ALLEGRO_MESSAGEBOX_OK_CANCEL);
              }
            }
          }
        }
      
      if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        sair = true;
        xis = true;
      }
      if (concluido){
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN && evento.keyboard.keycode == ALLEGRO_KEY_SPACE){
          sair = true;
        }
      }
      if(redesenhar && al_is_event_queue_empty(fila_eventos)){
      // desenho
        al_clear_to_color(al_map_rgb(0, 0, 0));
        if (pontuacao <= top[NUM_PODIO-2].pontos) concluido = true;
        if (!concluido){
          exibir_texto_centralizado(nome, fonte);
          al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2,
            (ALTURA_TELA / 2 - al_get_font_ascent(fonte)) / 2,
            ALLEGRO_ALIGN_CENTRE, "Parabéns pela pontuação! Nome:");
        } else {
          al_draw_bitmap(podio, 0, 0, 0);
          // DESENHAR PÓDIO
          al_draw_multiline_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 270, 2000, 40, ALLEGRO_ALIGN_CENTRE,
          "%s: \n%d", top[0].nome, top[0].pontos); 
          al_draw_multiline_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 - 215, 370, 2000, 40, ALLEGRO_ALIGN_CENTRE,
          "%s: \n%d", top[1].nome, top[1].pontos); 
          al_draw_multiline_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 + 215, 370, 2000, 40, ALLEGRO_ALIGN_CENTRE,
          "%s: \n%d", top[2].nome, top[2].pontos); 
          al_draw_multiline_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 - 450, 470, 2000, 40, ALLEGRO_ALIGN_CENTRE,
          "%s: \n%d", top[3].nome, top[3].pontos); 
          al_draw_multiline_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2 + 445, 470, 2000, 40, ALLEGRO_ALIGN_CENTRE,
          "%s: \n%d", top[4].nome, top[4].pontos); 

          // texto de voltar
          al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA-80, ALLEGRO_ALIGN_CENTRE,
            "(Aperte espaço para voltar para o inicio)");
        }
        al_flip_display();

      }  
    }
    // curl -X PUT -d '{ "Luc": 120, "Gil": 100, "Ana": 130, "Mariana": 125, "Ian": 80 }' 'https://jogoallegro.firebaseio.com/podio.json'
    // curl -X POST -d '{"Luc" : 120}' \https://jogoallegro.firebaseio.com/podio.json
    // curl -X DELETE "https://jogoallegro.firebaseio.com/podio/-MGVlHge_5-XN3vOBuLv.json"
    free(s.ptr);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  if (xis) return 2;
  else return 0;
}