#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn/jsmn.h"
#include <curl/curl.h>

const int num_podio = 5;

struct top_jogador{
    int idx;
    int pontos;
    char nome[30];
};

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
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

int comparator (const struct top_jogador* lhs, const struct top_jogador* rhs){
    return -(lhs->pontos - rhs->pontos);
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  int num_parsed, i, j = 0, k;
  struct top_jogador top[num_podio]; 
  jsmn_parser parser;
  jsmntok_t t[128];

  jsmn_init(&parser);
  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, "https://jogoallegro.firebaseio.com/podio.json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    num_parsed = jsmn_parse(&parser, s.ptr, s.len, t, sizeof(t) / sizeof(t[0]));

    if (num_parsed < 0) {
        printf("Failed to parse JSON: %d\n", num_parsed);
        return 1;
    }
    for (i = 1; i < num_parsed; i+=2){
        strncpy(top[j].nome, s.ptr+t[i].start, t[i].end - t[i].start);
        top[j].nome[t[i].end - t[i].start] = '\0';

        top[j].pontos = atoi(s.ptr + t[i+1].start);
        j++;
    }
    qsort(top, num_podio, sizeof top[0], comparator);

    for (i = 0; i < num_podio; i++){
        printf("O(A) jogador(a) %s ficou em %do lugar, com %d pontos\n", top[i].nome, i+1, top[i].pontos);
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{ \"Luc\": 120, \"Gil\": 110, \"Ana\": 130, \"Mariana\": 125, \"Ian\": 100 }");
    res = curl_easy_perform(curl);

    // curl -X PUT -d '{ "Luc": 120, "Gil": 100, "Ana": 130, "Mariana": 125, "Ian": 80 }' 'https://jogoallegro.firebaseio.com/podio.json'
    // curl -X POST -d '{"Luc" : 120}' \https://jogoallegro.firebaseio.com/podio.json
    // curl -X DELETE "https://jogoallegro.firebaseio.com/podio/-MGVlHge_5-XN3vOBuLv.json"
    free(s.ptr);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}