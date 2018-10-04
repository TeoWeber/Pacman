#define PROBDIRECAOFANTASMA 0.3
#define CIMA 1
#define BAIXO 2
#define ESQUERDA 3
#define DIREITA 4
#define PARADO 0
#define PAUSA 420
#define INVALIDO 1337
#define AGGRO 1
#define DEFF 0
#define VIDA 2
#define LINHASLAB 30
#define COLUNASLAB 100
#define VELOCIDADEJOGO 100
#define MAXFANTASMAS 5
#define RESPAWNFANT 50
#define SUICIDIO 42
#define HIGHSCORE "highscore.bin"
#define CORPAREDE 9
#define CORPASTILHA 7
#define CORSUPERPAST 11
#define CORPACMAN 14
#define LETRASNOME 3
#define AZUL 11
#define VERDE 10
#define VERMELHO 12
#define ROSA 13
#define MAGENTA 5
#define CINZA 8
#define AZUL_ESCURO 1


typedef struct Posicao
{
    int x;
    int y;
}POS;

typedef struct Fantasma
{
    POS posfant;
    POS spawnfant;
    int direcao; //1 Cima, 2 Baixo, 3 Esquerda, 4 Direita
    int cor;
    int direcaomem;
    int respawntimer;
    int status;
}FANT;

typedef struct Pacman
{
    POS pospac;
    POS spawnpac;
    int direcao;
    int direcaomemoria;
    int colisao;
    int pausa;
    int status; //2 = Está super ; 1 = Comeu Super Pastilha ; 0 = Normal
    int vida; //3 nro de VIDAS

}PACMAN;

typedef struct Usuarios
{
    char nome[LETRASNOME];
    int pontos;
    int cor;
}USUARIO;

typedef struct Cursor
{
    int cor;
    POS poscursor;
}CURSOR;

//DECLARAÇÃO DAS FUNÇÕES DA LIBRARY.C
void menu(USUARIO* usuario, CURSOR* cursor, int* exit);
void gotoxy(int x,int y);
int dirPacman();
void movePac(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas);
void comeCoisas(PACMAN* pacman, char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score);
void encerra (int* pastilha, int*score, USUARIO* usuario, int cor);
void pausaJogo(char labirinto[LINHASLAB][COLUNASLAB]);
void LeMapa();
void hidecursor();
void moveFant(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int totalfant, PACMAN* pacman);
int colisaoCheck(char labirinto[LINHASLAB][COLUNASLAB],POS posicao, int direcao);
int canMove (char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i);
int noBack(int direcao,int direcaomem, int* beco);
int deuRuim (POS pospac, FANT* fantasmas,int totalfant);
int escolheCor(int cor, int status);
void reGame(char labirinto[LINHASLAB][COLUNASLAB], int vida, FANT fantasmas[], int cor);
void fantRand(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i);
void fantSegue(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, PACMAN* pacman);
void startGame(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas);
void superPacman(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas);
void comeFantasmas(PACMAN* pacman,FANT fantasmas[],int* score, int totalfant);
void fantFoge(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i, PACMAN* pacman);
void fantSegue(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, PACMAN* pacman);
void fantReage(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, PACMAN* pacman, int* direcoes);
void trueNoBack(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, int segue, PACMAN* pacman);
void highscore(int* score, USUARIO* usuario, int cor);
void imprimeScoreboard(USUARIO* usuario, int cor);
void SetColor(int ForgC);
void loadHS(USUARIO* usuario);
void molduraJogo(int cor);
void molduraJogo2(char labirinto[LINHASLAB][COLUNASLAB],int cor);
void zeraSave(USUARIO* usuario);
void musicaIntro();
