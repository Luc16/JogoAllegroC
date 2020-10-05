#include <stdio.h>

typedef struct StructTopJogador{
    int pontos;
    char nome[30];
} TopJogador;

typedef struct StructString {
  char *ptr;
  size_t len;
} String;

bool inicializar();
void iniciar_string(String *s);
size_t funcao_escrever(void *ptr, size_t size, size_t nmemb, String *s);
int comparador(const TopJogador* lhs, const TopJogador* rhs);
void manipular_entrada(ALLEGRO_EVENT evento, char nome[]);
void exibir_texto_centralizado(char nome[]);
bool valida_nome(char nome[], TopJogador top[]);
int podio(int pontuacao);