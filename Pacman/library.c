#include <library.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



/*menu: Nesta fun��o, existe um cursor programado de forma que sua posi��o
s� pode alterar no eixo y, ou seja, pra cima, ou pra baixo. Logo, o jogador
tem acesso � um leque de op��es atrav�s de uma interface bastante simples e intuitiva.
A fun��o chamar� a ordem apropriada � posi��o do cursor no momento em que �Enter� for apertado.
Por exemplo: Se cursor estiver no �cursor->poscursor.y == 17�, ele abrir� o menu escolheCor e
permitir� ao jogador escolher uma nova cor para o tema do jogo; quando terminar de executar, chamar�
a fun��o �menu� recursivamente, dando a impress�o ao jogador de estar navegando entre as diferentes abas.*/
void menu (USUARIO* usuario, CURSOR* cursor, int* exit)
{
    system("cls");
    molduraJogo(cursor->cor);
    int direct=0;
    cursor->poscursor.y = 15;
    cursor->poscursor.x = 43;
    SetColor(cursor->cor);

    /*-------------------------------*/
    // Imprime PACMAN na tela

    gotoxy(31,5);
    printf(" _ __   __ _  ___ _ __ ___   __ _ _ __  ");
    gotoxy(31,6);
    printf("| '_ \\ / _` |/ __| '_ ` _ \\ / _` | '_  \\ ");
    gotoxy(31,7);
    printf("| |_) | (_| | (__| | | | | | (_| | | | |");
    gotoxy(31,8);
    printf("| .__/ \\__,_|\\___|_| |_| |_|\\__,_|_| |_|");
    gotoxy(31,9);
    printf("| |");
    gotoxy(31,10);
    printf("|_|");

    /*-------------------------------*/
    // Imprime os bot�es do menu principal

    gotoxy(cursor->poscursor.x, cursor->poscursor.y);
    printf("%c", 175);
    gotoxy(46,15);
    printf("Iniciar");
    SetColor(15);
    gotoxy(46,17);
    printf("Temas");
    gotoxy(46,19);
    printf("Ranking");
    gotoxy(46,21);
    printf("Sair");
    gotoxy(38,28);
    printf("Press 'Enter' to select.");

    /*-------------------------------*/
    // Movimenta��o do cursor

    while(direct!=13) // Continua at� receber input 'Enter'
    {
        direct = getch();
        if (direct == 224)
        {
            direct = getch();
            switch (direct)
            {
            case 72: /* Setinha pra Cima */
                gotoxy(cursor->poscursor.x, cursor->poscursor.y); // Apaga a antiga posi��o
                printf(" ");
                gotoxy(cursor->poscursor.x+3, cursor->poscursor.y);
                if(cursor->poscursor.y == 15) // Reescreve a mensagem do lado na cor branca
                    printf("Iniciar");
                else if(cursor->poscursor.y == 17) // Reescreve a mensagem do lado na cor branca
                    printf("Temas");
                else if(cursor->poscursor.y == 19) // Reescreve a mensagem do lado na cor branca
                    printf("Ranking");
                else
                    printf("Sair");
                cursor->poscursor.y -= 2; // Desce
                break;
            case 80: /* Setinha pra Baixo */
                gotoxy(cursor->poscursor.x, cursor->poscursor.y); // Apaga a antiga posi��o
                printf(" ");
                gotoxy(cursor->poscursor.x+3, cursor->poscursor.y);
                if(cursor->poscursor.y == 15) // Reescreve na cor branca
                    printf("Iniciar");
                else if(cursor->poscursor.y == 17)
                    printf("Temas");
                else if(cursor->poscursor.y == 19)
                    printf("Ranking");
                else
                    printf("Sair");
                cursor->poscursor.y += 2; // Sobe
                break;
            }
        }
        if(cursor->poscursor.y < 15) cursor->poscursor.y = 21; // Se subir al�m da primeira posi��o, vai pra ultima
        else if(cursor->poscursor.y > 21) cursor->poscursor.y = 15; // Se descer al�m da �ltima posi��o, vai pra primeira
        SetColor(cursor->cor); // Fun��o que pinta o cursor da cor armazenada
        gotoxy(cursor->poscursor.x, cursor->poscursor.y);
        printf("%c", 175); // Imprime na nova posi��o
        gotoxy(cursor->poscursor.x+3, cursor->poscursor.y); // Reimprime a Op��o da nova posi��o na cor armazenada
        if(cursor->poscursor.y == 15)
            printf("Iniciar");
        else if(cursor->poscursor.y == 17)
            printf("Temas");
        else if(cursor->poscursor.y == 19)
            printf("Ranking");
        else
            printf("Sair");
        SetColor(15);
    }
    //printf("\a"); // Posi��o onde ficaria o Beep
    if(cursor->poscursor.y == 19)
    {
        loadHS(usuario); // Carrega um array de Usuarios com conte�do de um arquivo bin�rio
        imprimeScoreboard(usuario, cursor->cor); // Mostra em um template bonitinho
        menu(usuario, cursor, exit);
    }
    else if(cursor->poscursor.y == 17)
    {
        cursor->cor = escolheCor(cursor->cor,1); // Permite mudar a cor do cursor e da borda
        Sleep(100);
        menu(usuario, cursor, exit); // Volta para o menu recursivamente
    }
    else if(cursor->poscursor.y == 21) // Torna verdadeira a condi��o pra encerrar o jogo
        *exit=1;
}


/*LeMapa: L� um arquivo de texto que cont�m as informa��es do mapa e armazena em uma matriz com [30][100]
posi��es. Al�m disso, desenha seu conte�do na tela e colhe informa��es importantes. Como a posi��o de spawns
dos Fantasmas e do Pacman, quantidades de pastilhas e fantasmas.*/
void LeMapa (char labirinto[LINHASLAB][COLUNASLAB], FILE* arq, FANT spawnFant[MAXFANTASMAS], PACMAN* pacman, int* pastcont, int* fantcont, int cor)
{
    system("cls");
    molduraJogo(cor);
    char str[101];
    int i=0, j=0, cont=0;
    if (arq == NULL)
    {
        printf("Problemas na abertura do arquivo.\n");
    }
    while (!feof(arq) && i<30)
    {
        fgets(str, 101, arq); // L� uma linha
        strcpy(labirinto[i], str); // Armazena ela na matriz labirinto
        fseek(arq,2,SEEK_CUR); // Vai pra pr�xima linha
        i++; // Conta linhas
    }
    fclose(arq);

    gotoxy(0,0);
    for(i=0; i<LINHASLAB; i++)
    {
        for(j=0; j<COLUNASLAB; j++) // Looping pra printar das cores certas, cada item
        {
            if(labirinto[i][j] == '#') SetColor(CORPAREDE);
            else if(labirinto[i][j] == 'o') SetColor(CORPASTILHA);
            else if(labirinto[i][j] == '*') SetColor(CORSUPERPAST);
            else if(labirinto[i][j] == 'C') SetColor(CORPACMAN);
            else if(labirinto[i][j] == 'W')
            {
                SetColor(spawnFant[cont].cor); // Cor de cada fantasma
                cont++;
            }
            printf("%c",labirinto[i][j]); // Printa da cor certa

        }
    }


    for (i=0; i<LINHASLAB; i++)
        // Looping que armazena as posi��es �teis do labirintos. Como spawns e n�mero de pastilhas e fantasmas.
    {
        for (j=0; j<=COLUNASLAB; j++)
        {
            if (labirinto[i][j]=='C') // Armazena posi��o inicial e spawn
            {
                pacman->pospac.y = i;
                pacman->pospac.x = j;
                pacman->spawnpac.y = i;
                pacman->spawnpac.x = j;
            }
            else if (labirinto[i][j]=='W' && (*fantcont)<MAXFANTASMAS)
                // Armazena posi��o inicial e spawn de cada fantasma com um contador que guarda o numero de fantasmas
            {
                spawnFant[(*fantcont)].posfant.y = i;
                spawnFant[(*fantcont)].posfant.x = j;
                spawnFant[(*fantcont)].spawnfant.y = i;
                spawnFant[(*fantcont)].spawnfant.x = j;
                (*fantcont)++;
            }
            else if (labirinto[i][j]=='o' || labirinto[i][j]=='*') // Conta pastilhas
            {
                (*pastcont)++;
            }
        }
    }

    molduraJogo2(labirinto, cor); //Printa a borda do jogo sobre o labirinto
}

//dirPacman:transforma as entradas do teclado em inputs mais simples de se lidar com durante o programa.
int dirPacman() //Necessario apenas um "if (kbhit)" antes da fun��o, todo controle de vari�veis � realizado dentro dela
{
    int direct;

    direct = getch();//Obt�m o valor int da tecla pressionada

    //Este bloco transforma as setas em suas respectivas teclas WAXD.
    if (direct == 224)//Caso uma seta seja pressionada, s�o recebidos dois valores, o primeiro sempre sendo 224 seguido de algo
    {
        direct = getch();//Neste caso como ser� 224 xxx realizamos outro getch para ter direct como xxx.

        switch (direct)
        {
        case 75: /* Setinha pra Esquerda */
            direct = 65;
            break;
        case 72: /* Setinha pra Cima */
            direct = 87;
            break;
        case 77: /* Setinha pra Direita */
            direct = 68;
            break;
        case 80: /* Setinha pra Baixo */
            direct = 88;
            break;
        }
    }
    direct = toupper(direct);


    //Cima = 87 // Esquerda = 65 // Direita = 68 // Baixo = 88 // Parado = 83
    switch(direct)
    {
    case 87:
        return CIMA;
        break;
    case 88:
        return BAIXO;
        break;
    case 65:
        return ESQUERDA;
        break;
    case 68:
        return DIREITA;
        break;
    case 83:
        return PARADO;
        break;
    case 80:
        return PAUSA;
        break;
    default:
        return INVALIDO;
    }

}


/*gotoxy:Fun��o originalmente da conio completa, recebe valores
para x e y e posiciona o cursor na devida posi��o.*/
void gotoxy(int x,int y)
{
    COORD p= {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
}


/*startGame:Controla os rel�gios do jogo e quando movimenta��es devem acontecer, tamb�m
controla as rotinas de morte e vit�ria caso as condi��es para uma dessas sejam atingidas.*/
void startGame(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas)
{


    clock_t timerinicio, timer;
    timerinicio = clock();
    int morte = 0, i;
    pacman->direcao = 0;

    while ((*pastcont)>0 && morte == 0)//Vit�ria/Morte
    {
        timer = clock();
        if(timer - timerinicio > VELOCIDADEJOGO)
        {
            movePac(labirinto,pastcont,score,pacman,totalfant,fantasmas); //MOVIMENTACAO PACMAN
            //----------------Check de colis�o com fantasma-------------
            if (morte == 0)
                morte = morte + deuRuim(pacman->pospac,fantasmas,totalfant);
            if(pacman->status == 2)
            {
                for(i=0; i<totalfant; i++) fantasmas[i].direcao=0;
            }
            moveFant(labirinto, fantasmas, totalfant, pacman);     //MOVIMENTACAO FANTASMA
            if (morte == 0)
                morte = deuRuim(pacman->pospac,fantasmas,totalfant);
            //----------------------------------------------------------
            timerinicio = clock();
        }
    }
}


/*movePac: Realiza toda a manipula��o da dire��o e movimenta��o do pacman, lida com os inputs do teclado e atualiza a
posi��o do mesmo. Ela utiliza de um sistema de dire��o dupla, sendo uma a mem�ria da dire��o anterior e outra a dire��o
pressionada mais recentemente, caso a dire��o recente colida com uma parede, o pacman ir� se mover em sua dire��o memorizada
at� conseguir ir no sentido da dire��o recente, que ent�o se torna sua nova dire��o memorizada.*/
void movePac(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas)
{
    if(kbhit()) pacman->direcao = dirPacman();
    gotoxy(pacman->pospac.x, pacman->pospac.y);
    if(pacman->direcao != PARADO)
        printf(" ");

    //A funcao dirPacman retorna 1337 caso a tecla pressionada seja inv�lida
    if (pacman->direcao == INVALIDO || pacman->pausa == 1)
        pacman->direcao = pacman->direcaomemoria;
    if (pacman->pausa == 1)
        pacman->pausa = 0;


    //Switch que altera a posi��o xy do pacman de acordo com a dire��o na qual ele est� se movendo
    switch(pacman->direcao)
    {
    //Sempre que ele consegue se mover sem ocorrer alguma colis�o, ele guarda esta dire��o na "dire�aomemoria" e define "colisaomemoria = 0"
    case CIMA:
        //Ao checar no topo teleporte para baixo
        if(pacman->pospac.y == 0)
            pacman->pospac.y = LINHASLAB-1;
        else if (colisaoCheck(labirinto, pacman->pospac,pacman->direcao)== 0)
        {
            pacman->pospac.y=pacman->pospac.y - 1;
            pacman->colisao= 0;
        }
        else pacman->colisao = 1;
        break;

    case BAIXO:
        //Ao chegar na parte mais baixa teleporte para o topo
        if (pacman->pospac.y == LINHASLAB-1)
            pacman->pospac.y=0;
        else if(colisaoCheck(labirinto, pacman->pospac,pacman->direcao)==0)
        {
            pacman->pospac.y++;
            pacman->colisao = 0;
        }
        else pacman->colisao = 1;
        break;

    case ESQUERDA:
        //Ao chegar na esquerda teleporte para a direita
        if(pacman->pospac.x ==0)
            pacman->pospac.x = COLUNASLAB-1;
        else if(colisaoCheck(labirinto, pacman->pospac,pacman->direcao)==0)
        {
            pacman->pospac.x = pacman->pospac.x - 1;
            pacman->colisao = 0;
        }
        else pacman->colisao = 1;
        break;

    case DIREITA:
        //Ao chegar na direita teleporte para a esquerda
        if (pacman->pospac.x == COLUNASLAB-1)
            pacman->pospac.x=0;
        else if (colisaoCheck(labirinto, pacman->pospac,pacman->direcao)==0)
        {
            pacman->pospac.x++;
            pacman->colisao = 0;
        }
        else pacman->colisao = 1;
        break;

    case PARADO:
        pacman->pospac.x = pacman->pospac.x;
        pacman->colisao = 0;
        break;
    case PAUSA:
        SetColor(14);
        gotoxy(pacman->pospac.x, pacman->pospac.y);
        putchar('C');
        pausaJogo(labirinto);
        gotoxy(pacman->pospac.x, pacman->pospac.y);
        putchar(' ');
        pacman->pausa = 1;
        break;
    }
    //Sempre que ele consegue se mover sem ocorrer alguma colis�o, ele guarda esta dire��o na "dire�aomemoria" e define "colisaomemoria = 0"

    if (pacman->direcao != INVALIDO && pacman->colisao == 0 && pacman->direcao != PAUSA)//Caso a tecla n�o seja invalida e
        pacman->direcaomemoria = pacman->direcao;


    //CASO A DIRE��O ATUAL N�O SEJA POSS�VEL, ELE SE MOVE UTILIZANDO A DIRE��O MEMORIA
    //No momento em que a dire��o pressionada se torna poss�vel, esta sobreescreve a dire��o memoria
    else switch(pacman->direcaomemoria)
        {
        case CIMA:
            if(pacman->pospac.y == 0)
                pacman->pospac.y = LINHASLAB-1;
            else if (colisaoCheck(labirinto, pacman->pospac,pacman->direcaomemoria)== 0)
                pacman->pospac.y=pacman->pospac.y - 1;
            break;

        case BAIXO:
            if (pacman->pospac.y == LINHASLAB-1)
                pacman->pospac.y=0;
            else if(colisaoCheck(labirinto, pacman->pospac,pacman->direcaomemoria)==0)
                pacman->pospac.y++;
            break;

        case ESQUERDA:
            if(pacman->pospac.x ==0)
                pacman->pospac.x = COLUNASLAB-1;
            else if(colisaoCheck(labirinto, pacman->pospac,pacman->direcaomemoria)==0)
                pacman->pospac.x = pacman->pospac.x - 1;
            break;

        case DIREITA:
            if (pacman->pospac.x == COLUNASLAB-1)
                pacman->pospac.x=0;
            else if (colisaoCheck(labirinto, pacman->pospac,pacman->direcaomemoria)==0)
                pacman->pospac.x++;
            break;

        case PARADO:
            pacman->pospac.x = pacman->pospac.x;
            break;
        }


    if (labirinto[pacman->pospac.y][pacman->pospac.x] != ' ')
        comeCoisas(pacman, labirinto, pastcont, score);

    if (pacman->status == 1) //PACMAN COMEU UMA PASTILHA MUITO SUPER E FICOU PODEROSO E PA
    {
        pacman->status = 2;
        superPacman(labirinto,pastcont,score,pacman,totalfant,fantasmas);
    }

    if(pacman->status == 2)
        comeFantasmas(pacman,fantasmas,score,totalfant);

    SetColor(14);
    gotoxy(pacman->pospac.x, pacman->pospac.y);
    putchar('C');
}


/*comeCoisas: Recebe a matriz do labirinto, uma struct PACMAN
e ponteiros para o contador de pastilhas restantes e para o contador
de pontos, ela checa se o pacman est� na mesma posi��o que uma pastilha/super-pastilha,
remove ela do labirinto e adiciona os pontos na pontua��o total.*/
void comeCoisas(PACMAN* pacman, char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score)
{
    SetColor(15);
    switch(labirinto[pacman->pospac.y][pacman->pospac.x])
    {
    case 'o':
        Beep(500,2);
        *score = *score + 10;
        labirinto[pacman->pospac.y][pacman->pospac.x] = ' ';
        gotoxy(42,31);
        printf("Pontuacao: %d",*score);
        (*pastcont)--;
        break;
    case '*':
        Beep(1500,10);
        *score = *score + 50;
        labirinto[pacman->pospac.y][pacman->pospac.x] = ' ';
        gotoxy(42,31);
        printf("Pontuacao: %d",*score);
        (*pastcont)--;
        pacman->status = 1;
        break;
    }
}


/*pausaJogo: Essa fun��o � chamada ao se pressionar a tecla P,
mant�m o jogo em uma estase at� outra tecla ser pressionada, ao
receber outro input ele realiza uma contagem e retoma a movimenta��o do jogo.*/
void pausaJogo(char labirinto[LINHASLAB][COLUNASLAB])
{
    int i,j,X;
    char p;
    X = 219;

    SetColor(15);

    //Grande PAUSE
    gotoxy(26,11);
    printf("/----------------------------------------------\\");
    gotoxy(26,12);
    printf("|                                              |");
    gotoxy(26,13);
    printf("| %c%c%c%c%c%c%c    %c%c%c%c   %c%c    %c%c   %c%c%c%c%c%c %c%c%c%c%c%c%c%c |",X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X);
    gotoxy(26,14);
    printf("| %c%c   %c%c%c %c%c    %c%c %c%c    %c%c %c%c       %c%c       |",X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X);
    gotoxy(26,15);
    printf("| %c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c %c%c    %c%c   %c%c%c%c   %c%c%c%c%c%c%c%c |",X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X);
    gotoxy(26,16);
    printf("| %c%c       %c%c    %c%c %c%c    %c%c       %c%c %c%c       |",X,X,X,X,X,X,X,X,X,X,X,X,X,X);
    gotoxy(26,17);
    printf("| %c%c       %c%c    %c%c   %c%c%c%c   %c%c%c%c%c%c   %c%c%c%c%c%c%c%c |",X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X);
    gotoxy(26,18);
    printf("|               Press P to resume              |");
    gotoxy(26,19);
    printf("\\----------------------------------------------/");

    //while(!kbhit);
    while(p != 'P') //Mantem o programa travado at� receber o input da tecla P
    {
       p = getch();
       while(p==-32) // Setas tamb�m representavam P por algum motivo, esta se��o remove essa possibilidade
            {
                p=getch(); // Armazena o segundo valor da seta
                p=200; // Joga o segundo valor fora e armazena um que n�o � P
            }
       p = toupper(p);
    }

    // 3
    gotoxy(26,11);
    printf("/----------------------------------------------\\");
    gotoxy(26,12);
    printf("|                                              |");
    gotoxy(26,13);
    printf("|                   %c%c%c%c%c%c%c%c                   |",X,X,X,X,X,X,X,X);
    gotoxy(26,14);
    printf("|                         %c%c                   |",X,X);
    gotoxy(26,15);
    printf("|                   %c%c%c%c%c%c%c%c                   |",X,X,X,X,X,X,X,X);
    gotoxy(26,16);
    printf("|                         %c%c                   |",X,X);
    gotoxy(26,17);
    printf("|                   %c%c%c%c%c%c%c%c                   |",X,X,X,X,X,X,X,X);
    gotoxy(26,18);
    printf("|                                              |");
    gotoxy(26,19);
    printf("\\----------------------------------------------/");
    Sleep(1000);

    // 2
    gotoxy(26,11);
    printf("/----------------------------------------------\\");
    gotoxy(26,12);
    printf("|                                              |");
    gotoxy(26,13);
    printf("|                     %c%c%c%c                     |",X,X,X,X);
    gotoxy(26,14);
    printf("|                   %c%c    %c%c                   |",X,X,X,X);
    gotoxy(26,15);
    printf("|                      %c%c%c                     |",X,X,X);
    gotoxy(26,16);
    printf("|                    %c%c%c                       |",X,X,X);
    gotoxy(26,17);
    printf("|                   %c%c%c%c%c%c%c%c                   |",X,X,X,X,X,X,X,X);
    gotoxy(26,18);
    printf("|                                              |");
    gotoxy(26,19);
    printf("\\----------------------------------------------/");
    Sleep(1000);

    // 1
    gotoxy(26,11);
    printf("/----------------------------------------------\\");
    gotoxy(26,12);
    printf("|                                              |");
    gotoxy(26,13);
    printf("|                      %c%c%c                     |",X,X,X);
    gotoxy(26,14);
    printf("|                    %c%c%c%c%c                     |",X,X,X,X,X);
    gotoxy(26,15);
    printf("|                      %c%c%c                     |",X,X,X);
    gotoxy(26,16);
    printf("|                      %c%c%c                     |",X,X,X);
    gotoxy(26,17);
    printf("|                   %c%c%c%c%c%c%c%c                   |",X,X,X,X,X,X,X,X);
    gotoxy(26,18);
    printf("|                                              |");
    gotoxy(26,19);
    printf("\\----------------------------------------------/");
    Sleep(1000);

    //---------------------------------------------------------------
    //Bloco de reinserir apenas a parte do labirinto sobreescrita pela tela de pausa
    gotoxy(0,0);
    for (i=10; i<21; i++)
    {
        for(j=25; j<75; j++)
        {
            gotoxy(j,i);
            if(labirinto[i][j]!= 'C' && labirinto[i][j] != 'W')
            {
                if(labirinto[i][j] == '#') SetColor(CORPAREDE);
                else if(labirinto[i][j] == 'o') SetColor(CORPASTILHA);
                else if(labirinto[i][j] == '*') SetColor(CORSUPERPAST);
                printf("%c",labirinto[i][j]);
            }
            else putch(' ');
        }
    }
    //-------------------------------------------------------------------
}


/*moveFant: Fun��o que analisa o modo dos fantasmas (entre persegui��o e fuga) e decide qual fun��o
de movimento chamar para eles, al�m de gerar um valor aleat�rio que � comparado com a �PROBDIRECAOFANTASMA�
para decidir se estes ir�o seguir o pacman ou escolher uma dire��o aleat�ria, ap�s escolher o modo de
movimenta��o ela tamb�m atualiza a posi��o dos fantasmas,movimentando eles.*/
void moveFant(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int totalfant, PACMAN* pacman)
{
    int i;
    float chance;
    //---------------------------------------------------------------------
    //Bloco de escolha do movimento
    for (i=0; i<totalfant; i++)
    {
        if(fantasmas[i].respawntimer == 0) //Reduz em 10 a cada segundo, utilizado para definir tempo de respawn
        {
            if(canMove(labirinto,fantasmas,i) || fantasmas[i].direcao == 0 || pacman->status == 2)
            {
                if(pacman->status == 2)
                {
                    fantFoge(labirinto,fantasmas,i,pacman);
                }
                else
                {
                    chance = rand()%100;

                    if (chance/100 <= PROBDIRECAOFANTASMA)
                        fantRand(labirinto,fantasmas,i); //MOVIMENTO ALEATORIO


                    else
                        fantSegue(labirinto,fantasmas,i,pacman); //MOVIMENTO IDEAL
                }
            }

            //---------------------------------------------------------------
            //Apaga o fantasma da posi��o atual
            gotoxy(fantasmas[i].posfant.x,fantasmas[i].posfant.y);
            if (labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x] == 'W' ||
                    labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x] == 'C')
                printf(" ");
            else
            {
                switch(labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x])
                {
                case '#':
                    SetColor(CORPAREDE);
                    break;
                case 'o':
                    SetColor(CORPASTILHA);
                    break;
                case '*':
                    SetColor(CORSUPERPAST);
                    break;
                }
                printf("%c",labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x]);
            }

            //-------------------------------------------------------------------------
            //Altera��o na posi��o de cada fantasma
            switch (fantasmas[i].direcao)
            {
            case CIMA:
                if (fantasmas[i].posfant.y == 0) fantasmas[i].posfant.y = LINHASLAB - 1;
                else fantasmas[i].posfant.y = fantasmas[i].posfant.y - 1;
                break;

            case BAIXO:
                if (fantasmas[i].posfant.y == LINHASLAB - 1) fantasmas[i].posfant.y = 0;
                else fantasmas[i].posfant.y = fantasmas[i].posfant.y + 1;
                break;

            case ESQUERDA:
                if (fantasmas[i].posfant.x == 0) fantasmas[i].posfant.x = COLUNASLAB-1;
                else fantasmas[i].posfant.x = fantasmas[i].posfant.x - 1;
                break;

            case DIREITA:
                if (fantasmas[i].posfant.x == COLUNASLAB - 1 ) fantasmas[i].posfant.x = 0;
                else fantasmas[i].posfant.x = fantasmas[i].posfant.x + 1;
                break;
            }
            fantasmas[i].direcaomem = fantasmas[i].direcao;
            //--------------------------------------------------------------
            //Print do fantasma em sua posi��o nova
            gotoxy(fantasmas[i].posfant.x,fantasmas[i].posfant.y);
            if (pacman->status != 2)
            {
                SetColor(fantasmas[i].cor);
                printf("W");
            }
            else
            {
                SetColor(11);
                printf("w");
            }


        }
        else fantasmas[i].respawntimer = fantasmas[i].respawntimer - 1;
        SetColor(15);
    }
}


/*colisaoCheck:Recebe a matriz do labirinto, uma struct POS e uma vari�vel de dire��o,
utilizando estas informa��es ela retorna 1 caso a pr�xima casa de movimenta��o
seja uma parede ou 0 caso a pr�xima casa seja uma v�lida para a movimenta��o.*/
int colisaoCheck(char labirinto[LINHASLAB][COLUNASLAB],POS posicao, int direcao)
{
    int colisao;
    //1 = CIMA ; 2 = BAIXO; 3 = ESQUERDA; 4 = DIREITA
    switch(direcao)
    {
    case CIMA:
        if (labirinto[posicao.y-1][posicao.x] == '#') colisao = 1;
        else colisao = 0;
        break;

    case BAIXO:
        if (labirinto[posicao.y+1][posicao.x] == '#') colisao = 1;
        else colisao = 0;
        break;

    case ESQUERDA:
        if (posicao.x == 0) posicao.x = COLUNASLAB-1;
        if (labirinto[posicao.y][posicao.x-1] == '#') colisao = 1;
        else colisao = 0;
        break;

    case DIREITA:
        if (posicao.x == COLUNASLAB-1) colisao = 0;
        else if (labirinto[posicao.y][posicao.x+1] == '#') colisao = 1;
        else colisao = 0;
        break;
    }
    return colisao;

}


/*canMove:Recebe a matriz do labirinto e uma struct FANTASMA, utilizando do
valor fantasma.direc�o e do fantasma.posfant ela retorna valores 1 ou 0
correspondentemente caso o fantasma esteja em uma bifurca��o/beco ou n�o. */
int canMove (char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i)
{
    int canmove=0;
    switch(fantasmas[i].direcao)
    {
    case CIMA:
        if (labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x+1] != '#' ||
                labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x-1] != '#' ||
                labirinto[fantasmas[i].posfant.y-1][fantasmas[i].posfant.x] == '#' )
            canmove = 1;
        else
            canmove = 0;
        break;

    case BAIXO:
        if (labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x+1] != '#' ||
                labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x-1] != '#' ||
                labirinto[fantasmas[i].posfant.y+1][fantasmas[i].posfant.x] == '#' )
            canmove = 1;
        else
            canmove = 0;
        break;

    case ESQUERDA:
        if (labirinto[fantasmas[i].posfant.y+1][fantasmas[i].posfant.x] != '#' ||
                labirinto[fantasmas[i].posfant.y-1][fantasmas[i].posfant.x] != '#' ||
                labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x-1] == '#' )
            canmove = 1;
        else
            canmove = 0;
        break;

    case DIREITA:
        if (labirinto[fantasmas[i].posfant.y+1][fantasmas[i].posfant.x] != '#' ||
                labirinto[fantasmas[i].posfant.y-1][fantasmas[i].posfant.x] != '#' ||
                labirinto[fantasmas[i].posfant.y][fantasmas[i].posfant.x+1] == '#' )
            canmove = 1;
        else
            canmove = 0;
        break;
    default:
        canmove = 0;
    }

    return canmove;
}


/*noBack: Fun��o utilizada durante a movimenta��o dos fantasmas,
impede que os fantasmas escolham uma dire��o que far� eles voltar
para onde vieram, sua maior utilidade � fazer os fantasmas se
moverem mais fluidamente mesmo quando escolhem dire��es aleat�rias.*/
int noBack(int direcao,int direcaomem, int* beco)
{
    int back;

    if (*beco < 6) //Caso passe de 6 itera��es ele permite ir para a dire��o da onde veio, pois o fantasma estar� em um beco
    {
        //Switch que impede ele de ir para a dire��o daonde veio
        switch(direcaomem)
        {
        case CIMA:
            if (direcao == BAIXO) back = 1;
            else back = 0;
            break;
        case BAIXO:
            if (direcao == CIMA) back = 1;
            else back = 0;
            break;
        case ESQUERDA:
            if (direcao == DIREITA) back = 1;
            else back = 0;
            break;
        case DIREITA:
            if (direcao == ESQUERDA) back = 1;
            else back = 0;
            break;
        default:
            back = 1;
        }
        *beco = *beco + 1;
        return back;
    }
    else
        return 0;
}


/*deuRuim:checa se a struct PACMAN est� na mesma posi��o que uma das struct FANTASMA
 caso seja verdadeiro, ela retorna um valor 1 que representa a morte do pacman e faz
 o programa entrar na rotina de morte.*/
int deuRuim (POS pospac, FANT* fantasmas,int totalfant)
{
    int i, morreu = 0;
    for (i=0; i<totalfant; i++)
    {
        //Checa se pacman e fantasmas est�o na mesma posicao
        if ((pospac.x == fantasmas[i].posfant.x && pospac.y == fantasmas[i].posfant.y) && fantasmas[i].respawntimer == 0)
            morreu = 1;
    }
    return morreu;
}


/*reGame: Respons�vel pela reposi��o dos personagens do jogo
quando este entra na rotina de morte, informa o usu�rio de que
ele morreu, posiciona pacman e fantasmas em suas posi��es iniciais
e aguarda por um input para seguir o jogo na pr�xima vida do jogador.*/
void reGame(char labirinto[LINHASLAB][COLUNASLAB], int vida, FANT fantasmas[], int cor)
{
    int i,j,k=0;
    char c;

    Sleep(500); //Sleep inicial para a morte n�o ser instant�nea

    //Apaga o labirinto
    gotoxy(0,0);
    for(i=0; i<LINHASLAB; i++)
    {
        for(j=0; j<COLUNASLAB; j++)
        {
            putch(' ');
        }
    }
    Sleep(10);

    //Reprinta o labirinto com tudo em suas posi��es iniciais
    gotoxy(0,0);
    for(i=0; i<LINHASLAB; i++)
    {
        for(j=0; j<COLUNASLAB; j++)
        {
            if(labirinto[i][j] == '#') SetColor(CORPAREDE);
            else if(labirinto[i][j] == 'o') SetColor(CORPASTILHA);
            else if(labirinto[i][j] == '*') SetColor(CORSUPERPAST);
            else if(labirinto[i][j] == 'W')
            {
                SetColor(fantasmas[k].cor) ;
                k++;
            }
            else if(labirinto[i][j] == 'C') SetColor(CORPACMAN);
            printf("%c",labirinto[i][j]);
        }
    }
    Sleep(10);

    SetColor(15);
    gotoxy(20,12);
    printf("/------------------------------------------------------------\\");
    gotoxy(20,13);
    printf("|                        Voce morreu!                        |");
    gotoxy(20,14);
    printf("|                                                            |");
    gotoxy(20,15);
    printf("|            Pressione 'Enter' para tentar novamente         |");
    gotoxy(20,16);
    printf("|                                                            |");
    gotoxy(20,17);
    printf("|                    Vidas restantes : %d                     |", vida);
    gotoxy(20,18);
    printf("\\------------------------------------------------------------/");

    molduraJogo2(labirinto, cor);

    SetColor(15);
    gotoxy(12,31);
    printf("Vidas : %d",vida); //Atualiza contador de vidas

    while (c!=13) c = getch(); //Aguarda pelo enter ser pressionado

    k=0;

    //Reprinta o labirinto novamente sem a mensagem de "voce morreu" em cima dele
    gotoxy(0,0);
    for(i=0; i<LINHASLAB; i++)
    {
        for(j=0; j<COLUNASLAB; j++)
        {
            if(labirinto[i][j] == '#') SetColor(CORPAREDE);
            else if(labirinto[i][j] == 'o') SetColor(CORPASTILHA);
            else if(labirinto[i][j] == '*') SetColor(CORSUPERPAST);
            else if(labirinto[i][j] == 'W')
            {
                SetColor(fantasmas[k].cor) ;
                k++;
            }
            else if(labirinto[i][j] == 'C') SetColor(CORPACMAN);
            printf("%c",labirinto[i][j]);
        }
    }
    //Segue com o jogo
    SetColor(15);
    molduraJogo2(labirinto, cor);
}


/*fantRand:Gerencia a movimenta��o aleat�ria do fantasma, gera um n�mero
aleat�rio entre 1 e 4 e testa se � poss�vel ir para essa dire��o(utilizando
de checks com noBack e colisaoCheck),caso os checks falhem ele testa as dire��es
que s�o poss�veis e escolhe uma delas de um modo controladamente aleat�rio para
limitar o n�mero de itera��es.*/
void fantRand(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i)
{
    int beco=0;

    //Sele��o da dire��o do fantasma atual
    fantasmas[i].direcao = 1 + (rand()%4);

    while (colisaoCheck(labirinto,fantasmas[i].posfant,fantasmas[i].direcao) ||
            noBack(fantasmas[i].direcao, fantasmas[i].direcaomem, &beco))
        //La�o para fazer todos fantasmas escolherem uma dire��o valida
    {
        if (fantasmas[i].direcao < 5)fantasmas[i].direcao = fantasmas[i].direcao + 1;
        else fantasmas[i].direcao = 1;
        beco++;
    }
}


/*fantFoge:seleciona e executa de maneira adequada o vetor de Fuga na fantReage.
Caso o Pacman tenha acabado de pegar uma super-pastilha, ela n�o ativa o trueNoBack,
ou seja, o Fantasma ganha permiss�o exclusiva pra seguir na dire��o oposta e fugir do Pacman.*/
void fantFoge(char labirinto[LINHASLAB][COLUNASLAB], FANT fantasmas[], int i, PACMAN* pacman)
{
    int direcoes[4]= {BAIXO, CIMA, DIREITA, ESQUERDA}; // Define a forma como a fantReage vai se relacionar com o Pacman
    // FUGA
    fantReage(labirinto, fantasmas, i, pacman, direcoes); // Devolve uma nova dire��o de acordo com a posi��o do Pacman
    if(fantasmas[i].status == 1)
        trueNoBack(labirinto, fantasmas, i, 0, pacman); // Impede de voltar pra posi��o de onde veio, a menos que n�o tenha escolha
    else fantasmas[i].status = 1;
}


/*fantSegue: Fun��o que seleciona e executa de maneira adequada o vetor de modo Agressivo na fantReage.*/
void fantSegue(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, PACMAN* pacman)
{
    int direcoes[4]= {CIMA, BAIXO, ESQUERDA, DIREITA}; // Define a forma como a fantReage vai se relacionar com o Pacman
    // AGRESSIVO
    fantReage(labirinto, fantasmas, i, pacman, direcoes); // Devolve uma nova dire��o de acordo com a posi��o do Pacman
    trueNoBack(labirinto, fantasmas, i, 1, pacman); // Impede de voltar pra posi��o de onde veio, a menos que n�o tenha escolha
}


/*fantReage:permite que o Fantasma reaja � presen�a do Pacman de forma adequada.
Ela recebe um vetor com informa��es de Dire��es em ordem ESPEC�FICA
e executa eles apropriadamente.*/
void fantReage(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, PACMAN* pacman, int* direcoes)
{
    int distanciaX = pacman->pospac.x-fantasmas[i].posfant.x,
        distanciaY = pacman->pospac.y-fantasmas[i].posfant.y;

    /*
        Devolver� dire��es baseadas na posi��o atual do Pacman.

        Modo FUGA, dire��o pra longe
        Modo AGRESSIVO, dire��o pra perto
    */

    if(distanciaX == 0)
    {
        if(distanciaY < 0) // Caso Pacman esteja a CIMA
        {
            if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[0]))
                fantasmas[i].direcao = direcoes[0];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
        else if(distanciaY > 0) // Caso Pacman esteja a BAIXO
        {
            if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[1]))
                fantasmas[i].direcao = direcoes[1];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
    }
    else if(distanciaY == 0)
    {
        if(distanciaX < 0) // Caso Pacman esteja a ESQUERDA
        {
            if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[2]))
                fantasmas[i].direcao = direcoes[2];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }

        else if(distanciaX > 0) // Caso Pacman esteja a DIREITA
        {
            if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[3]))
                fantasmas[i].direcao = direcoes[3];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }

    }
    else if(distanciaX > 0)
    {
        if(distanciaY < 0) // Caso Pacman esteja a NORDESTE
        {
            if (!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[0]) &&
                    !colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[3]))
            {
                if(rand()%2 == 0) fantasmas[i].direcao = direcoes[0];
                else fantasmas[i].direcao = direcoes[3];
            }
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[0]))
                fantasmas[i].direcao = direcoes[0];
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[3]))
                fantasmas[i].direcao = direcoes[3];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
        else if(distanciaY > 0)// Caso Pacman esteja a SUDESTE
        {
            if (!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[1]) &&
                    !colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[3]))
            {
                if(rand()%2 == 0) fantasmas[i].direcao = direcoes[1];
                else fantasmas[i].direcao = direcoes[3];
            }
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[1]))
                fantasmas[i].direcao = direcoes[1];
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[3]))
                fantasmas[i].direcao = direcoes[3];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
    }
    else if(distanciaX < 0)
    {
        if(distanciaY < 0)// Caso Pacman esteja a NOROESTE
        {
            if (!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[0]) &&
                    !colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[2]))
            {
                if(rand()%2 == 0) fantasmas[i].direcao = direcoes[0];
                else fantasmas[i].direcao = direcoes[2];
            }
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[0]))
                fantasmas[i].direcao = direcoes[0];
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[2]))
                fantasmas[i].direcao = direcoes[2];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
        else if(distanciaY > 0)// Caso Pacman esteja a SUDOESTE
        {
            if (!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[1]) &&
                    !colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[2]))
            {
                if(rand()%2 == 0) fantasmas[i].direcao = direcoes[1];
                else fantasmas[i].direcao = direcoes[2];
            }
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[1]))
                fantasmas[i].direcao = direcoes[1];
            else if(!colisaoCheck(labirinto,fantasmas[i].posfant,direcoes[2]))
                fantasmas[i].direcao = direcoes[2];
            else fantRand(labirinto,fantasmas,i); // Caso n�o possa ir nas dire��es certas, chama aleat�rio
        }
    }
}


/*trueNoBack: Fun��o auxiliar �s fun��es fantFoge e fantSegue. Seu �nico objetivo � filtrar as mudan�as
de dire��es de modo a impedir que o fantasma tenha a op��o de voltar na dire��o oposta � que vem. Logo,
 um fantasma que est� indo pra cima, nunca virar� pra baixo enquanto est� fun��o filtr�-lo.*/
void trueNoBack(char labirinto[LINHASLAB][COLUNASLAB], FANT* fantasmas, int i, int segue, PACMAN* pacman)
{
    /*
        Impede a dire��o oposta! Pro fantasma n�o ficar tremendo (indo e vindo)
                                                                                */


    switch(fantasmas[i].direcao)
    {
    case CIMA:
        if(fantasmas[i].direcaomem==BAIXO) fantRand(labirinto,fantasmas, i); // Caso esteja indo pra CIMA, n�o vai pra BAIXO
        break;
    case BAIXO:
        if(fantasmas[i].direcaomem==CIMA) fantRand(labirinto,fantasmas, i); // Caso esteja indo pra BAIXO, n�o vai pra CIMA
        break;
    case ESQUERDA:
        if(fantasmas[i].direcaomem==DIREITA) fantRand(labirinto,fantasmas, i); // Caso esteja indo pra ESQUERDA, n�o vai pra DIREITA
        break;
    case DIREITA:
        if(fantasmas[i].direcaomem==ESQUERDA) fantRand(labirinto,fantasmas, i); // Caso esteja indo pra DIREITA, n�o vai pra ESQUERDA
        break;
    default:
        return;
    }
}


/*superPacman: Gerencia a rotina do pacman ao comer uma super pastilha, gerencia
movimenta��es diferentes por 5 segundos com a possibilidade do pacman comer
fantasmas e a movimenta��o dos fantasmas 30% mais lentas que o normal.*/
void superPacman(char labirinto[LINHASLAB][COLUNASLAB], int* pastcont, int* score, PACMAN* pacman, int totalfant, FANT* fantasmas)
{
    clock_t startpac,startfant, timerpac, timerfant, temposuper, limitesuper;
    startpac = clock();
    startfant = clock();
    temposuper = clock();
    limitesuper = clock();
    int i,metercheck=0;

    gotoxy(87,31);
    printf("%c%c%c%c%c",254,254,254,254,254); // Preenche o super meter com 5 blocos
    for(i=0; i<totalfant; i++) fantasmas[i].status = 0;

    while (temposuper - limitesuper < 5000)//A fun��o dura 5 segundos
    {
        //Contadores de tempo decorrido
        timerpac = clock();
        timerfant = clock();

        if(timerpac - startpac> VELOCIDADEJOGO) //Pacman se move de acordo com a velocidade do jogo
        {
            movePac(labirinto,pastcont,score,pacman,totalfant,fantasmas); //MOVIMENTACAO PACMAN
            startpac = clock();
        }

        if(timerfant - startfant > VELOCIDADEJOGO*1.3)// Vel fantasma = 70% --> Vel jogo 130%
        {
            moveFant(labirinto, fantasmas, totalfant, pacman);
            startfant = clock();
            comeFantasmas(pacman,fantasmas,score,totalfant);
        }
        temposuper = clock();

        //Ifs para esvaziar o super meter com o passar do tempo
        if(temposuper - limitesuper > 1000 && metercheck == 0) //1 segundo passou
        {
            gotoxy(91,31);
            printf(" ");
            Beep(2000,10);
            metercheck = 1;
        }
        if(temposuper - limitesuper > 2000 && metercheck == 1) //2 segundos
        {
            gotoxy(90,31);
            printf(" ");
            Beep(2000,10);
            metercheck = 2;
        }
        if(temposuper - limitesuper > 3000 && metercheck == 2) //3 segundos
        {
            gotoxy(89,31);
            printf(" ");
            Beep(2000,10);
            metercheck = 3;
        }
        if(temposuper - limitesuper > 4000 && metercheck == 3) //4 segundos
        {
            gotoxy(88,31);
            printf(" ");
            Beep(2000,10);
            metercheck = 4;
        }
    }
    gotoxy(87,31);
    printf("     "); // Esvazia o super meter
    Beep(1000,10);
    pacman->status = 0; //Retorna ao normal ao final dos 5 segundos e volta para a movePac
}


/*comeFantasmas:checa se a struct PACMAN est� na mesma posi��o que uma das struct FANTASMA,
caso isso ocorra o fantasma � removido do labirinto e retorna ao seu spawn ap�s um timer,
os pontos s�o adicionados a pontua��o do pacman (comeFantasmas s� � chamada durante a rotina da super pastilha).*/
void comeFantasmas(PACMAN* pacman,FANT fantasmas[],int* score, int totalfant)
{
    int i;
    SetColor(15);
    for(i=0; i<totalfant; i++)
    {
        if((pacman->pospac.x == fantasmas[i].posfant.x &&
                pacman->pospac.y == fantasmas[i].posfant.y) &&
                fantasmas[i].respawntimer == 0)
        {
            Beep(1500,100);
            Beep(1000,50);
            *score = *score + 200;
            fantasmas[i].posfant = fantasmas[i].spawnfant;
            fantasmas[i].respawntimer = RESPAWNFANT; //5 segundos para respawn, 10 iteracoes por segundo
            gotoxy(42,31);
            printf("Pontuacao: %d",*score);//Printa a nova pontuacao
        }
    }
}


/*encerra:decide se o jogador ganhou ou perdeu e informa este do
correspondente, ap�s ela receber um ENTER ela segue para a fun��o do
scoreboard para registrar a pontua��o do usu�rio.*/
void encerra (int* pastilha, int*score, USUARIO* usuario, int cor) // Imprime mensagem de Vitoria/Derrota e chama Highscore
{
    char c;
    system("cls");
    molduraJogo(cor);
    gotoxy(38,15);
    if (*pastilha==0) // Condi��o de Vit�ria
        printf("Voce ganhou! Pontuacao: %d\n", *score);
    else
        printf("Voce perdeu! Pontuacao: %d\n", *score);
    gotoxy(38,28);
    printf("Press 'Enter' to select.");
    while(c!=13) c=getch(); // S� continua quando receber 'Enter' como input
    highscore(score, usuario, cor); // Fun��o que pergunta o nome do usuario, caso ele esteja entre os 10 melhores
}


/*imprimeScoreboard: Imprime um vetor do tipo USUARIO na tela,
este vetor foi tratado anteriormente pela fun��o highscore e
cont�m apenas os usu�rios aptos a continuarem no Top 10. L� um
 arquivo de texto com um template, imprime o conte�do na tela
 para criar um apelo visual e imprime, por cima, a informa��o
 Nome e Pontos de cada usu�rio com suas respectivas cores.*/
void imprimeScoreboard(USUARIO* usuario, int cor)
{
    system("cls");
    molduraJogo(cor);
    FILE *arq;
    char highscore[21][100], c = ' ';
    arq = fopen("highscore.txt", "rt");
    char str[51];
    int i=0;
    if (arq == NULL)
    {
        printf("Problemas na abertura do highscore.\n");
    }
    while (!feof(arq) && i<21)
    {

        SetColor(15);
        gotoxy(25,2+i);
        fgets(str, 51, arq); // L� a linha de um arquivo e armazena em uma string
        strcpy(highscore[i], str); // Passa a informa��o da string pra matriz 'highscore'
        str[51] = fgetc(arq); // Pula os \n
        printf("%s", str); // Printa a linha
        if(i>=9 && i<=18)
        {
            gotoxy(50,2+i);

            /* Dependendo a linha que esteja sendo printada, ele imprime as informa��es na linha certa. */

            switch(i)
            {
            case 9:
                SetColor(usuario[9].cor);
                printf("%s   %d",usuario[9].nome,usuario[9].pontos);
                break;
            case 10:
                SetColor(usuario[8].cor);
                printf("%s   %d",usuario[8].nome,usuario[8].pontos);
                break;
            case 11:
                SetColor(usuario[7].cor);
                printf("%s   %d",usuario[7].nome,usuario[7].pontos);
                break;
            case 12:
                SetColor(usuario[6].cor);
                printf("%s   %d",usuario[6].nome,usuario[6].pontos);
                break;
            case 13:
                SetColor(usuario[5].cor);
                printf("%s   %d",usuario[5].nome,usuario[5].pontos);
                break;
            case 14:
                SetColor(usuario[4].cor);
                printf("%s   %d",usuario[4].nome,usuario[4].pontos);
                break;
            case 15:
                SetColor(usuario[3].cor);
                printf("%s   %d",usuario[3].nome,usuario[3].pontos);
                break;
            case 16:
                SetColor(usuario[2].cor);
                printf("%s   %d",usuario[2].nome,usuario[2].pontos);
                break;
            case 17:
                SetColor(usuario[1].cor);
                printf("%s   %d",usuario[1].nome,usuario[1].pontos);
                break;
            case 18:
                SetColor(usuario[0].cor);
                printf("%s   %d",usuario[0].nome,usuario[0].pontos);
                break;
            }
        }
        SetColor(15);
        i++; // Conta linhas
    }
    fclose(arq);
    gotoxy(25,25);
    printf("        Press 'ESC' to clean the scoreboard.          ");
    gotoxy(25,26);
    printf("               Press 'Enter' to quit.                  ");
    while(c!=13 && c!=27) c=getch(); // N�o continua at� receber input 'Enter' ou 'ESC'
    if(c==27) // Caso 'ESC', zera a scoreboard.
    {
        zeraSave(usuario);
    }
}


/*highscore:Chama a fun��o loadHS que devolve um vetor de structs do tipo �USUARIO�
com informa��es do arquivo armazenado na pasta fonte. Depois, compara com as
notas atuais do jogador e verifica se ele pode ir para o Top 10 ou n�o, caso
possa, ser� ordenado tomando lugar no vetor. Substituindo o atual usuario da
posi��o e fazendo-o descer junto aos outros com pontua��o inferior; ser� enviado
para uma tela de leitura de nome, depois para escolha de cor e imprimir� o novo
ScoreBoard. Caso n�o, n�o chamar� pedido de nome e imprimir� sem o novo jogador.*/
void highscore(int* score, USUARIO* usuario, int cor)
{
    FILE *f=NULL;
    int i, cont=0; // Contadores
    char letra[LETRASNOME], c; // Auxiliar
    system("cls");
    molduraJogo(cor);
    loadHS(usuario); // Carrega, Ordena os dados salvos e armazena em *usuario.
    for(i=0; i<10; i++) // Itera��o pra contar posicoes abaixo do Score atual
        if(*score>=usuario[i].pontos) cont++;
    if(cont > 0) // Constroi uma struct para o novo Usuario, CASO ele tenha alcan�ado o suficiente.
    {
        i=0;
        while(i<cont) // Faz todos os Usuarios do top 10 que estiverem a baixo do atual, des�am uma posi��o
        {
            usuario[i-1]=usuario[i];
            i++;
        }
        usuario[cont-1].pontos=*score; //Passa o Score atual para essa struct
        gotoxy(37,14);
        printf("Informe seu nick [3 letras]");
        gotoxy(49,16);
        printf("___");
        Sleep(500);
        for (i=0; i<LETRASNOME; i++)
        {
            gotoxy(49+i,16);
            letra[i]=getch(); // L� primeira letra do nome
            while(letra[i]==-32) // No lugar de 224, setinha � indicada por -32, por que letra[i] esta sendo declarada como CHAR
            {
                letra[i]=getch(); // Armazena o segundo valor da setinha
                letra[i]=200; // Joga o segundo valor fora e armazena um que n�o acerta a pr�xima condi��o
            }
            if((letra[i]>=65 && letra[i]<=90) || (letra[i]>=97 && letra[i]<=122))
            {
                letra[i]=toupper(letra[i]); // letra[i] passa a ser maiuscula
                gotoxy(49+i,16);
                printf("%c",letra[i]); // Impressa na tela em tempo real
            }
            else i--; // Caso a condi��o anterior seja falsa, o For vai refazer esta itera��o
        }
        letra[LETRASNOME]='\0'; // Final da String
        strcpy(usuario[cont-1].nome,letra); // Copia a string guardada em 'letra' na struct 'usuario'
        gotoxy(38,28);
        printf("Press 'Enter' to select.");
        Sleep(1000); // Importante pra evitar missclicks do jogador
        while(c!=13) c = getch(); // N�o continua at� receber input 'Enter'
        usuario[cont-1].cor=escolheCor(cor,2); // Permite ao usuario escolher a cor com a qual seu nome ser� impresso
        f = fopen(HIGHSCORE,"wb"); // Abre o Arquivo pra salvar os dados recolhidos
        if(f==NULL)
        {
            printf("\nProblemas na abertura do arquivo.");
            return;
        }
        fwrite(usuario,sizeof(USUARIO),10,f); // Armazena os dados recolhidos
        fclose(f);
    }
    else // Quando o usuario n�o tem pontos suficientes pra entrar no Top 10
    {
        gotoxy(41,15);
        printf("Not enough points!");
        gotoxy(38,28); // 38,35
        printf("Press 'Enter' to select.");
        while(c!=13) c = getch(); // N�o continua at� receber input 'Enter'
    }
    imprimeScoreboard(usuario, cor); // Imprime o Highscore
}


/*loadHS: Fun��o que l� um arquivo de highscores e armazena
em um vetor do tipo USUARIO, com todas as informa��es necess�rias
para a cria��o de um template, ou ser alterado pela fun��o highscores.
 Veja que esta fun��o � �til em dois momentos do c�digo: Quando os
 highscores s�o chamados no menu e quando a fun��o �highscore� o
 chama pra trabalhar em uma poss�vel nova ordena��o ou pra simplesmente
 enviar pro ScoreBoard (quando os pontos do jogador n�o s�o o suficiente).*/
void loadHS(USUARIO* usuario)
{
    FILE *arq;
    USUARIO temp; // Struct Auxiliar
    int i, j; // Contadores
    arq=fopen(HIGHSCORE,"rb");
    if(arq==NULL)
    {
        fclose(arq); // Fecha modo de leitura
        zeraSave(usuario); // Cria um arquivo com structs zeradas
        arq=fopen(HIGHSCORE,"rb"); // Reabre em modo de leitura

    }
    i=0;
    while(!feof(arq) && i<10) // L� os itens do arquivo binario e armazena em um array de structs
    {
        fread(&temp,sizeof(USUARIO),1,arq);
        usuario[i]=temp;
        i++;
    }
    for(i=0; i<10; i++) //Ordena os usuarios salvos em ordem crescente (Por Pontua��o)
        for(j=i+1; j<10; j++)
            if(usuario[i].pontos>usuario[j].pontos)
            {
                temp=usuario[i];
                usuario[i]=usuario[j];
                usuario[j]=temp;
            }
}


/*zeraSave:adiciona uma vantagem extra ao usu�rio. Com esta fun��o � poss�vel �zerar� o arquivo bin�rio
que armazena os dados da ScoreBoard. Isso � �til para o usu�rio que pretende apagar uma lista
lotada, ou simplesmente se livrar do seu desempenho passado. Pode ser acessada no menu principal
atrav�s da aba Ranking e no final do jogo, quando pode ver o ScoreBoard mais uma vez. Um efeito
semelhante acontece caso n�o haja um arquivo bin�rio.*/
void zeraSave(USUARIO* usuario) // Cria um array de structs zeradas
{
    FILE *arq;
    int i;
    arq=fopen(HIGHSCORE,"wb"); // Inicia modo de escrita
    for(i=0; i<10; i++) // Coloca modelos de Structs vazias em um array
    {
        usuario[i].pontos=0;
        strcpy(usuario[i].nome," - ");
        usuario[i].cor=15;
    }
    fwrite(usuario,sizeof(USUARIO),10,arq); // Armazena no arquivo de Highscores
    fclose(arq);
}


/*escolheCor:retorna um valor inteiro referente a uma cor. Um menu � imprimido
na tela e existe um cursor que varre as op��es de cima a baixo. Ao apertar enter,
o la�o que define a nova posi��o do cursor � encerrado e a posi��o final do cursor
retorna uma cor espec�fica que tem diferentes aplica��es dentro do c�digo. Como a
cor tema do jogo, cor do cursor e cor do nome no ScoreBoard (caso tenha pontua��o suficiente pra alcan��-lo).*/
int escolheCor(int cor, int status) // Menu para sele��o de cores
{
    system("cls");
    POS cursor; // Faz funcionar o cursor
    molduraJogo(cor);
    int direct;
    cursor.y = 15;
    cursor.x = 43;

    /*-------------------------------*/
    // Imprime na tela op��o de cor:

    gotoxy(32,5);
    if (status == 1)
    {
        gotoxy(29,5);
        printf("Escolha uma cor tematica para o seu jogo: ");
    }
    else
    {
        gotoxy(33,5);
        printf("Escolha uma cor para o seu nome: ");
    }
    gotoxy(cursor.x, cursor.y);
    printf("%c",175);
    SetColor(AZUL);
    gotoxy(47,13);
    printf("%c%c",219,219);
    SetColor(VERDE);
    gotoxy(47,15);
    printf("%c%c",219,219);
    SetColor(VERMELHO);
    gotoxy(47,17);
    printf("%c%c",219,219);
    SetColor(ROSA);
    gotoxy(47,19);
    printf("%c%c",219,219);
    SetColor(CINZA);
    gotoxy(47,21);
    printf("%c%c",219,219);
    SetColor(MAGENTA);
    gotoxy(47,23);
    printf("%c%c",219,219);
    SetColor(AZUL_ESCURO);
    gotoxy(47,25);
    printf("%c%c",219,219);
    SetColor(15); // Branco
    gotoxy(38,28);
    printf("Press 'Enter' to select.");

    /*-------------------------------*/
    // Movimenta��o do cursor

    while(direct!=13) // Continua at� receber input 'Enter'
    {

        direct = getch(); // L� uma tecla
        if (direct == 224) // Caso seja setinha
        {
            direct = getch(); // Colhe o segundo valor
            switch (direct)
            {
            case 72: /* Setinha pra Cima */
                gotoxy(cursor.x, cursor.y);
                printf(" ");
                cursor.y -= 2; // Desce
                break;
            case 80: /* Setinha pra Baixo */
                gotoxy(cursor.x, cursor.y);
                printf(" ");
                cursor.y += 2; // Sobe
                break;
            }
        }
        if(cursor.y < 13) cursor.y = 25; // Se subir al�m da primeira posi��o, vai pra ultima
        else if(cursor.y > 25) cursor.y = 13; // Se descer al�m da �ltima posi��o, vai pra primeira
        gotoxy(cursor.x, cursor.y);
        printf("%c", 175);
        gotoxy(cursor.x+3, cursor.y);
    }

    /*-------------------------------*/
    // Devolve uma cor de acordo com a posi��o do cursor quando recebeu 'Enter'

    if(cursor.y == 13) return AZUL;
    else if(cursor.y == 15) return VERDE;
    else if(cursor.y == 17) return VERMELHO;
    else if(cursor.y == 19) return ROSA;
    else if(cursor.y == 21) return CINZA;
    else if(cursor.y == 23) return MAGENTA;
    else return AZUL_ESCURO;
}


/*SetColor:altera a cor de todos printf subsequentes.*/
void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

/*hidecursor: Remove o �_� que representa o cursor no prompt de comando.*/
void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 10;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


/*molduraJogo:realiza a primeira impress�o da borda inferior do jogo.*/
void molduraJogo(int cor)
{
    int i;

    SetColor(cor);

    //--------------MOLDURA--------------
    gotoxy(0,0);
    for(i=0; i<100; i++)
    {
        printf("%c",219);
    }


    gotoxy(0,30);
    for(i=0; i<100; i++)
    {
        printf("%c",220);
    }

    gotoxy(31,31);
    for(i=0; i<3; i++)
    {
        printf("%c",219);
    }

    gotoxy(63,31);
    for(i=0; i<3; i++)
    {
        printf("%c",219);
    }


    gotoxy(0,32);
    for(i=0; i<100; i++)
        printf("%c",219);

    gotoxy(0,38);
    for(i=0; i<100; i++)
    {
        printf("%c",219);
    }

    for(i=0; i<39; i++)
    {
        gotoxy(0,i);
        printf("%c",219);
        gotoxy(99,i);
        printf("%c",219);
    }

    for(i=0; i<6; i++)
    {
        gotoxy(44,32+i);
        printf("%c%c%c",219,219,219);
    }
    //------------------CONTROLES-----------------------------
    gotoxy(13,33);
    printf("%c      MOVIMENTO     %c  PAUSA",186,186);
    gotoxy(13,34);
    printf("%c   %cW%c         %c%c%c  %c    -",186,179,179,179,30,179,186);

    gotoxy(2,35);
    printf("|Controls| %c%cA%c%cS%c%cD%c ou %c%c%c%c%c%c%c%c   %cP%C",186,179,179,179,179,179,179,179,17,179,219,179,16,179,186,179,179);

    gotoxy(13,36);
    printf("%c   %cX%c         %c%c%c  %c    -",186,179,179,179,31,179,186);
    gotoxy(13,37);
    printf("%c                    %c",186,186);


    //----------------------ARTWORK-------------------------
    //Fantasma 1
    SetColor(11);
    gotoxy(48,33);
    printf("  %c%c%c%c%c ",178,178,178,178,178);
    gotoxy(48,34);
    printf(" %c  %c  %c ",178,178,178);
    gotoxy(48,35);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(48,36);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(48,37);
    printf(" %c %c %c %c ",178,178,178,178);

    SetColor(15);
    gotoxy(50,34);
    printf("%c",219);
    gotoxy(53,34);
    printf("%c",219);

    SetColor(1);
    gotoxy(51,34);
    printf("%c",219);
    gotoxy(54,34);
    printf("%c",219);

    //Fantasma 2
    SetColor(13);
    gotoxy(57,33);
    printf("  %c%c%c%c%c ",178,178,178,178,178);
    gotoxy(57,34);
    printf(" %c  %c  %c ",178,178,178);
    gotoxy(57,35);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(57,36);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(57,37);
    printf(" %c %c %c %c ",178,178,178,178);

    SetColor(15);
    gotoxy(59,34);
    printf("%c",219);
    gotoxy(62,34);
    printf("%c",219);

    SetColor(1);
    gotoxy(60,34);
    printf("%c",219);
    gotoxy(63,34);
    printf("%c",219);

    //Fantasma 3
    SetColor(12);
    gotoxy(66,33);
    printf("  %c%c%c%c%c ",178,178,178,178,178);
    gotoxy(66,34);
    printf(" %c  %c  %c ",178,178,178);
    gotoxy(66,35);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(66,36);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(66,37);
    printf(" %c %c %c %c ",178,178,178,178);

    SetColor(15);
    gotoxy(68,34);
    printf("%c",219);
    gotoxy(71,34);
    printf("%c",219);

    SetColor(1);
    gotoxy(69,34);
    printf("%c",219);
    gotoxy(72,34);
    printf("%c",219);

    //Fantasma 4
    SetColor(2);
    gotoxy(75,33);
    printf("  %c%c%c%c%c ",178,178,178,178,178);
    gotoxy(75,34);
    printf(" %c  %c  %c ",178,178,178);
    gotoxy(75,35);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(75,36);
    printf(" %c%c%c%c%c%c%c ",178,178,178,178,178,178,178);
    gotoxy(75,37);
    printf(" %c %c %c %c ",178,178,178,178);

    SetColor(15);
    gotoxy(77,34);
    printf("%c",219);
    gotoxy(80,34);
    printf("%c",219);

    SetColor(1);
    gotoxy(78,34);
    printf("%c",219);
    gotoxy(81,34);
    printf("%c",219);

    //Pacman
    SetColor(14);
    gotoxy(85,33);
    printf("  %c%c%c%c%c%c",220,219,219,219,219,220);
    gotoxy(85,34);
    printf(" %c%c%c%c%c%c",219,219,219,220,219,223);
    gotoxy(85,35);
    printf(" %c%c%c%c",219,219,219,219);
    gotoxy(85,36);
    printf(" %c%c%c%c%c%c",219,219,219,219,219,220);
    gotoxy(85,37);
    printf("  %c%c%c%c%c%c",223,219,219,219,219,223);

    SetColor(15);
    gotoxy(92,35);
    printf(" %c  %c",219,219);
}

/*molduraJogo2:realiza a impress�o da borda que cobre o labirinto durante o jogo*/
void molduraJogo2(char labirinto[LINHASLAB][COLUNASLAB],int cor)
{

    int i;

    SetColor(cor);

    //--------------MOLDURA BORDA LABIRINTO--------------

    for(i=0; i<30; i++)
    {
        if(labirinto[i][0] == '#')
        {
            gotoxy(0,i);
            printf("%c",219);
        }
        else
        {
            gotoxy(0,i);
            printf(" ");
        }
    }


    for(i=0; i<30; i++)
    {
        if(labirinto[i][99] == '#')
        {
            gotoxy(99,i);
            printf("%c",219);
        }
        else
        {
            gotoxy(99,i);
            printf(" ");
        }
    }

    gotoxy(0,0);
    for(i=0; i<100; i++)
    {
        if(labirinto[0][i] == '#')
            printf("%c",219);
        else printf(" ");
    }

    gotoxy(0,29);
    for(i=0; i<100; i++)
    {
        if(labirinto[29][i] == '#')
            printf("%c",219);
        else printf(" ");
    }
}


/*musicaIntro:uma sucess�o de beeps musicais que tocam a ic�nica m�sica do jogo de arcade.*/
void musicaIntro()
{

    Beep(493.88,100);
    Sleep(50);
    Beep(987.77,100);
    Sleep(50);
    Beep(739.99,100);
    Sleep(50);
    Beep(622.25,100);
    Sleep(50);
    Beep(987.77,100);
    Sleep(50);
    Beep(739.99,100);
    Sleep(50);
    Beep(622.25,200);
    Sleep(50);

    Sleep(70);

    Beep(523.25,100);
    Sleep(50);
    Beep(1046.5,100);
    Sleep(50);
    Beep(783.99,100);
    Sleep(50);
    Beep(659.25,100);
    Sleep(50);
    Beep(1046.5,100);
    Sleep(50);
    Beep(793.99,100);
    Sleep(50);
    Beep(659.25,200);
    Sleep(50);

    Sleep(70);

    Beep(493.88,100);
    Sleep(50);
    Beep(987.77,100);
    Sleep(50);
    Beep(739.99,100);
    Sleep(50);
    Beep(622.25,100);
    Sleep(50);
    Beep(987.77,100);
    Sleep(50);
    Beep(739.99,100);
    Sleep(50);
    Beep(622.25,200);
    Sleep(50);

    Sleep(100);

    Beep(622.25,70);
    Sleep(20);
    Beep(659.25,70);
    Sleep(20);
    Beep(698.46,80);
    Sleep(20);

    Sleep(80);

    Beep(698.46,70);
    Sleep(20);
    Beep(739.99,70);
    Sleep(20);
    Beep(793.99,80);
    Sleep(20);

    Sleep(80);

    Beep(793.99,70);
    Sleep(20);
    Beep(830.61,70);
    Sleep(20);
    Beep(880,80);
    Sleep(20);

    Sleep(80);

    Beep(987.77,100);
    Sleep(20);

    Sleep(200);
}
