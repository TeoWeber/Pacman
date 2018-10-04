/*PACMAN MUITO MANEIRO "WASDX" E SETAS MOVEM, "P" PAUSA, NAVEGAÇÃO DE MENUS FEITAS PELAS SETAS E TECLA "ENTER*/

#include <stdio.h>
#include <stdlib.h>
#include <library.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
    system("mode 100,40");

    //--------------Criação e primeira inicializaçao de variaveis--------------
    FILE *arq;
    FANT vetfant[MAXFANTASMAS];
    PACMAN pacman;
    CURSOR cursor;
    USUARIO usuario[10];
    int fantcont=0, pastcont=0, vida=VIDA, i, exit=0, cor;
    char labirinto[LINHASLAB][COLUNASLAB];
    srand(time(NULL));
    cursor.cor=CINZA;
    hidecursor();
    menu(usuario, &cursor, &exit);
    cor = cursor.cor;

    vetfant[0].cor = 3;
    vetfant[1].cor = 6;
    vetfant[2].cor = 10;
    vetfant[3].cor = 12;
    vetfant[4].cor = 13;

    if(exit==0) //Checa se a opção "sair" NÃO foi selecionada
    {
    /*---------------------Leitura do Mapa-----------------------------*/

    arq = fopen("labirinto.txt", "rt");
    LeMapa(labirinto, arq, vetfant, &pacman, &pastcont, &fantcont, cor);
    musicaIntro();

    //---------------Segunda Inicialização de variaveis-----------------------
    int score = 0;

    SetColor(15);
    gotoxy(42,31);
    printf("Pontuacao: %d",score);
    gotoxy(72,31);
    printf("SUPER METER : |     |");

    for (i=0; i<fantcont; i++)
    {
        vetfant[i].direcao = 0;
        vetfant[i].direcaomem = 1;
        vetfant[i].respawntimer = 0;
        vetfant[i].status = 0;
    }

    pacman.status = 0;
    /*-----------------Movimentação e Jogabilidade---------------------*/
    while(vida>=0 && pastcont!=0) //Permanece no jogo enquanto as vidas não zerarem e ainda existirem pastilhas
    {
        gotoxy(12,31);
        printf("Vidas : %d",vida);

        startGame(labirinto,&pastcont,&score, &pacman, fantcont, vetfant);//MATRIZ,PONTEIRO,VALOR.
        vida--; //Caso ele esteja aqui é porque morreu e a vida necessita ser reduzida

        //Retorna o pacman ao seu spawn
        pacman.pospac.x = pacman.spawnpac.x;
        pacman.pospac.y = pacman.spawnpac.y;

        //Retorna os fantasmas para seus spawns
        for (i=0; i<fantcont; i++)
        {
            vetfant[i].posfant.x = vetfant[i].spawnfant.x;
            vetfant[i].posfant.y = vetfant[i].spawnfant.y;
            vetfant[i].respawntimer = 0;
        }

        //Caso ele ainda possua vidas, entrar na função que informa ele da morte e reprinta o labirinto
        if(vida>=0 && pastcont!=0)
            reGame(labirinto,vida,vetfant,cor);

        SetColor(15);
    }
    /*--------------------------------------------------*/
    //Encerramento
    encerra(&pastcont,&score, usuario, cor); //Caso ele esteja aqui ou as vidas acabaram ou as pastilhas acabaram
    }
    system("cls");
    return 0;
}
