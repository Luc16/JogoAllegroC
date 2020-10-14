#define NUM_JOGADORES  2
#define NUM_PALAVRAS 8
#define NUM_CORONAS 10
#define NUM_PLATAFORMAS 52
#define NUM_PLATAFORMAS_MORTAIS 1
#define NUM_ALCOOLS 26
#define NUM_MINIONS 16
#define MAX_DESENHO_MINION 4
#define MAX_DESENHO_PARADO 12
#define MAX_DESENHO_PULANDO 21
#define MAX_DESENHO_ANDANDO 8

// estrutura de movimento dos personagens
typedef struct Struct_Posicao {
  float x, y, altura, largura, col_altura, col_largura;
} Posicao;

// estrutura corona
typedef struct Struct_Corona {
  Posicao pos_corona;
  bool morto;
  bool ativado;
  float velocidade;
  float pos_nascer;
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
  P_Ordem plvr[NUM_PALAVRAS];
  bool pulando, chao, mov_direita, mov_esquerda, morto;
  float posicao_y0, ajuste_pos,  x_anterior, y_anterior;
  int i_cair, i_pulo, lado, com_alcool;
} Jogador;

// estrutura alcool
typedef struct Struct_Alcool {
  Posicao pos_alcool;
  int lado, idx, i_esg, i_chefao;
  bool morto, pego, esguichando, chefao;
  ALLEGRO_BITMAP *bm_alcool;
  ALLEGRO_BITMAP *bm_esguicho;
} Alcool;

//estrutura minions
typedef struct Struct_Minion{
    Posicao pos_minion;
    float vel;
    float limite1;
    float limite2;
    int lado;
    bool morto;
    ALLEGRO_BITMAP *bm_minion;
    int reg_x, reg_y, espera, i_frames, atual; 

} Minion;

//FUNCOES

bool jog_esta_no_limite_direita(Jogador *jog);
bool jog_esta_no_limite_esquerda(Jogador *jog);
void mover_tela_direita(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[], Minion minions[]);
void mover_tela_esquerda(float *mov_tela, Jogador *jog_limite, Jogador *jog_outro, Corona covs[], Alcool alcools[], Minion minions[]);
void mover_tela(Jogador jogs[], Corona covs[], Alcool alcools[], Minion minions[], float *mov_tela);

bool jog_colidindo(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog);
void jog_colisao_lateral(Jogador *jog, float x_esquerda, float x_direita, bool *deixar_tras);
void jog_colisao_teto(Jogador *jog, float y_baixo);
void jog_colisao_chao(Jogador *jog, float y_cima);
void jog_colisao_geral(float plataformas[], Jogador *jog, float mov_tela, bool *deixar_tras);
bool jog_colidindo_mortal(float x_esquerda, float x_direita, float y_cima, float y_baixo, Jogador *jog);
void jog_colisao_mortal(float plataformas_mortais[], Jogador *jog, float mov_tela);
void desenhar_plataformas(float plataformas[][4], float plataformas_mortais[][4], float mov_tela);

void jog_ativar_pulo(Jogador *jog);
void jog_mover_esquerda(Jogador *jog, int _lado);
void jog_mover_direita(Jogador *jog, int _lado);
void jog_mover_lado(Jogador *jog);
void jog_pular(Jogador *jog);
void jog_cair(Jogador *jog);

void minion_iniciar(Minion minions[], float pos[][5]);
void minion_colide(Minion minions[], Jogador jogs[]);
void minion_atualizar(Minion minions[]);
void minion_vira_direita(Minion *minions);
void minion_vira_esquerda(Minion *minions);
void minion_desenhar(Minion minions[]);

void corona_iniciar(Corona covs[], float pos[][4]);
void corona_movimento(Corona *cov, Jogador jogs[]);
void corona_atualizar(Corona covs[], Jogador jogs[], float mov);
void corona_colide(Corona *cov, Jogador jogs[]);
void corona_desenhar(Corona covs[]);

void p_ordem_iniciar(P_Ordem plvr[]);
void p_ordem_atirar(Jogador *jog);
void p_ordem_colide(Jogador jogs[], Minion minions[]);
void p_ordem_atualizar(Jogador jogs[]);
void p_ordem_desenhar(Jogador jogs[], ALLEGRO_FONT *fonte2);

void alcool_iniciar(Alcool alcools[], float pos[][3]);
bool alcool_colidindo_jog(Alcool *alcool, Jogador *jog);
bool alcool_colidindo_corona(Alcool *alcool, Corona *cov);
void alcool_atirar(Alcool alcools[], Jogador *jog);
void alcool_chefao(Alcool alcools[], Jogador jogs[], float mov, int *k);
void alcool_atualizar(Alcool alcools[], Jogador jogs[], Corona covs[]);
void alcool_desenhar(Alcool alcools[]);

void jog_iniciar(Jogador jogs[]);
void jog_atualizar_pos_anterior(Jogador jogs[]);
void jog_atualizar_geral(Jogador jogs[], float plataformas[][4], float plataformas_mortais[][4], float mov_tela);
void jog_desenhar(Jogador jogs[], int *i_desenho, int *espera, int *desenho_atual, ALLEGRO_BITMAP *pato_parado[], ALLEGRO_BITMAP *pato_pulando[], ALLEGRO_BITMAP *pato_andando[]);
void apertar_botao(ALLEGRO_EVENT evento, Jogador jogs[], Alcool alcools[], float mover_tela);
void soltar_botao(ALLEGRO_EVENT evento, Jogador jogs[]);

int jogo(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *fundo, ALLEGRO_FONT *fonte,  ALLEGRO_FONT *fonte2, ALLEGRO_EVENT evento, ALLEGRO_TIMER *temporizador, int num_jogs, int *pontuacao);
