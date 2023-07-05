#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

// my sleep
#if defined(_WIN32)
#define WINDOWS_SYSTEM
#include <Windows.h>
#else
#define UNIX_SYSTEM
#include <unistd.h>
#endif

typedef struct Player
{
    char name[100];
    int vitorias;
    int derrotas;
}Player;

void my_sleep(int sleepms);

void sobre(void);

//Essa funcao cria uma matriz com os pesos das cartas
void criando_baralho(int mtr[10][4]);

//Distribui as cartas(colunas se referem a face e linhas a naipes)
void distribuido_baralho(int maralho[10][4], int *jogadorcol, int *jogadorlin, int *jogador);

//desicao da maquina
int maquina_joga(int jogador, int *cartas_marquina, int *coluna, int *linha, int turno);

//leitura da desicao do jogador
int jogador_joga(int *cartas_jogador, int *coluna, int *linha, int turno, int rodada_truco, int partidas_tento);

//analisa quem ganha a mao
int vencedor(int jogador, int maquina);

//move as cartas na mesa para ultima posicao, para descarte
void carta_atu(int *coluna, int *linha, int *valores, int turno, int escolha);

//Analisa partida da mao
int anali_situ(int *ganhador, int *tento, int pontos);

//funcao de truca quando jogador pede
void trucando(int *cartas_maquina, int *pontu_mesa, int turno, int *cont_turno);

//Informa dados necessario para a partida
void infos(int *ganhador, int pontos_turno, int *tento, int *string, int turno);

//Define para maquina quantas cartas de cada peso ela tem
void potencia(int *cartas_maquina, int *alta, int *alta_media, int *media, int *media_baixa, int *baixa, int turno);

//Funcao para maquina decidir se vai trucar
int maquina_truca(int alta, int alta_media, int media, int media_baixa, int baixa, int pontos_adversario);

//Praticamente onde ocorre todo o jogo, funcao que chama as outras
void game(void);

// Anexa o usuario no historico de jogadas
void anexarPlayer();

// Funcao que sera chamada quando o usuario for o vencedor
void indexarVitorias(char *pontVit);

// Funcao que sera chamada quando a maquina vencer
void indexarDerrota(char *pontDer);

// Funcao que vai criar o arquivo ranking depois do resultado parcial
void criarRanking();

// Chamada pelo qsort para fazer as comaparacoes devidas
int comparativo(const void *a, const void *b);

// Funcao que organiza o RC de forma descrecente para printar no ranking
void organizarRanking();

// printar o ranking
void ranking();

//========================================================================

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil"); // infelizmente, nao ta funcionando

    int opcoes;

    do
    {
        printf("\n==============================================");
        printf("\n%c %c %c %c %c %c %c %c TRUCO DOIDO %c %c %c %c %c %c %c %c", 6, 5, 3, 4, 6, 6, 5, 3, 4, 6, 5, 3, 4, 6, 5, 3);

        printf("\nOpcoes:\n");
        printf("[1] - Jogar truco\n[2] - Ver o rank de jogadores\n[3] - Sobre\n[4] - Sair\n");
        printf("==============================================");
        printf("\n>>>");
        scanf("%d", &opcoes);

        if (opcoes < 1 || opcoes > 4)
        {
            printf("Digite novamente!\n");
            printf("\n");
        }

        else if (opcoes == 1)
        {
            anexarPlayer ();
            game();
        }

        else if (opcoes == 2)
        {
            criarRanking ();
            organizarRanking ();
            ranking();
        }

        else if (opcoes == 3)
        {
            sobre();
        }

        else if (opcoes == 4)
        {
            return 0;
        }
    } while (opcoes != 4);
}

void my_sleep(int sleepms)
{
#ifdef WINDOWS_SYSTEM
    Sleep(sleepms);
#else
    usleep(sleepms * 1000);
#endif
}

void sobre(void)
{
    printf("\n==============================================");
    printf("\n%c %c %c %c %c %c %c %c %c TRUCO DOIDO %c %c %c %c %c %c %c %c", 6, 5, 3, 4, 6, 6, 5, 3, 4, 6, 5, 3, 4, 6, 5, 3, 4);

    printf("\n");

    printf("\nVersao do jogo: 1.0.1\n");

    printf("\n");

    printf("Autores do jogo:\n");
    printf("Arthur Alves Assuncao\n");
    printf("Odelmo Ferreira Neto\n");
    printf("Sthephanny Caroline da Silva Santos\n");
    printf("Vitor Martins Rocha\n");

    printf("\n");

    printf("Consideracoes:\n");

    printf("\n");

    printf("Trabalho realizado durantes os meses de Dezembro e Janeiro do ano de 2022 e 2023\n");
    printf("para concluir a materia de Programacao Procedimental ministrada pelo docente 'Claudiney Ramos'\n");
    printf("que atua na Universidade Federal de Uberlandia (UFU).\n");
    printf("O intuito do Trabalho de Conclusao de Disciplina (TCD) e criar um jogo de truco entre a\n");
    printf("maquina e o usuario.\n");

    printf("\n");
}

void criando_baralho(int mtr[10][4])
{
    int peso = 11;  //variavel para preencher matriz de acordo com o valor de cada carta

    // criando matriz completa
    for (int lin = 0; lin < 10; lin++)
    {
        peso--;
        for (int col = 0; col < 4; col++)
        {
            mtr[lin][col] = peso;
        }
    }

    // defiinindo manilhas
    mtr[9][1] = 50; // zap
    mtr[6][2] = 40; // copas
    mtr[2][0] = 30; // espadilha
    mtr[6][3] = 20; // ouros
}

void distribuido_baralho(int maralho[10][4], int *jogadorcol, int *jogadorlin, int *jogador)
{

    int linha, coluna;
    srand(time(NULL));

    for (int i = 0; i < 3; i++)
    {
        // sorteando carta
        linha = rand() % 4;
        coluna = rand() % 10;

        // vendo se eh uma carta valida
        if (maralho[coluna][linha] == 0)
        {
            i--;
        }

        else
        {
            jogador[i] = maralho[coluna][linha]; // quardando forca da carta

            jogadorlin[i] = linha;  // quardando linha correspondente(naipe)
            jogadorcol[i] = coluna; // quardando coluna correspondente (face)

            maralho[coluna][linha] = 0; // limpando espaco
        }
    }
}

int maquina_joga(int jogador, int *cartas_marquina, int *coluna, int *linha, int turno)
{
    int *ganhar, ganha = 0;
    int maquina;
    int *ponteiro;
    int empate = -1;

    char naipe[4] = {6, 5, 3, 4};
    char face[10] = {'3', '2', 'A', 'K', 'J', 'Q', '7', '6', '5', '4'};

    ponteiro = (int *)malloc(turno * sizeof(int *));

    for (int i = 0; i < turno; i++)
    {
        ponteiro[i] = cartas_marquina[i];
    }

    ganhar = (int *)malloc(turno * sizeof(int *));

    for (int i = 0; i < turno; i++)
    {
        if (cartas_marquina[i] > jogador)
        {
            ganhar[ganha] = cartas_marquina[i];
            ganha++;
        }
    }

    // CASO: nao ha possibilidade de ganhar
    if (ganha == 0)
    {
        // EMPATE
        for (int i = 0; i < turno; i++)
        {
            if (cartas_marquina[i] == jogador)
            {
                empate = i;
            }
        }
        if (empate >= 0)
        {
            maquina = empate;
        }

        // nao existe chance de empate
        else
        {
            for (int i = 0; i < turno; i++)
            {
                for (int j = i; j < turno; j++)
                {
                    if (ponteiro[i] > ponteiro[j])
                    {
                        int temp = ponteiro[i];
                        ponteiro[i] = ponteiro[j];
                        ponteiro[j] = temp;
                    }
                }
            }

            for (int i = 0; i < turno; i++)
            {
                if (ponteiro[0] == cartas_marquina[i])
                {
                    maquina = i;
                }
            }
        }
    }

    // CASO: ha possibilidade de ganhar
    else
    {
        ganhar = realloc(ganhar, ganha * sizeof(int));

        for (int i = 0; i < ganha; i++)
        {
            for (int j = i; j < ganha; j++)
            {
                if (ganhar[i] > ganhar[j])
                {
                    int temp = ganhar[i];
                    ganhar[i] = ganhar[j];
                    ganhar[j] = temp;
                }
            }
        }

        for (int i = 0; i < turno; i++)
        {
            if (ganhar[0] == cartas_marquina[i])
            {
                maquina = i;
            }
        }
    }
    printf("\nMAQUINA JOGA: ");
    printf("%c%c\t", face[*(coluna + maquina)], naipe[*(linha + maquina)]);

    return maquina;
}

int jogador_joga(int *cartas_jogador, int *coluna, int *linha, int turno, int rodada_truco, int partidas_tento)
{

    int escolha, choice;
    int j = 1;

    printf("\n %c %c %c %c %c  QUAL CARTA DESEJA JOGAR %c %c %c %c %c", 5, 3, 4, 6, 4, 5, 3, 6, 5, 4);

    char naipe[4] = {6, 5, 3, 4};
    char face[10] = {'3', '2', 'A', 'K', 'J', 'Q', '7', '6', '5', '4'};

    printf("\nSuas cartas sao:\n");

    for (int i = 0; i < turno; i++)
    {
        if (partidas_tento == 1)
        {
            printf("[*]\t");
            j++;
        }

        else
        {
            printf("[%d]%c%c\t", j, face[*(coluna + i)], naipe[*(linha + i)]);
            j++;
        }
    }

    if (rodada_truco != 1)
    {
        printf("[0] PEDIR TRUCO");
    }

    for (int i = 0; i < 1; i++)
    {
        printf("\n>>>");
        scanf("%d", &choice);
        if (choice > turno || choice < 0)
        {
            printf("Digite uma opcao valida!!");
            i--;
        }
    }

    if (rodada_truco == 1 && choice == 0)
    {
        while (choice == 0)
        {
            printf("\nVOCE JA TRUCOU!\nfaca uma escolha valida!");
            printf("\n>>>");
            scanf("%d", &choice);
        }
    }

    if (choice != 0)
    {
        choice = choice - 1;

        escolha = *(cartas_jogador + choice);

        printf("\nJOGADOR JOGA: ");
        printf("%c%c\t", face[*(coluna + choice)], naipe[*(linha + choice)]);
    }
    else if (choice == 0)
    {
        choice = choice - 1;
    }

    return choice;
}

int vencedor(int jogador, int maquina)
{
    if (jogador > maquina)
    {
        printf("\nJOGADOR LEVOU ESSA MAO!!\n");

        return -1;
    }
    else if (jogador < maquina)
    {
        printf("\nMAQUINA LEVOU ESSA MAO!!\n");

        return 1;
    }

    else if (jogador == maquina)
    {
        printf("\nEMPATE NESSA MAO!!\n");

        return 0;
    }
}

void carta_atu(int *coluna, int *linha, int *valores, int turno, int escolha)
{

    turno--;

    int ultimoC = *(coluna + turno);
    *(coluna + turno) = *(coluna + escolha);
    *(coluna + escolha) = ultimoC;

    int ultimoL = *(linha + turno);
    *(linha + turno) = *(linha + escolha);
    *(linha + escolha) = ultimoL;

    int ultimo = *(valores + turno);
    *(valores + turno) = *(valores + escolha);
    *(valores + escolha) = ultimo;
}

int anali_situ(int *ganhador, int *tento, int pontos)
{
    // EMPATE NA PRIMEIRA RODA
    if (ganhador[0] == 0 && ganhador[1] == 1)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == 0 && ganhador[1] == -1)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // EMPATE NA SEGUNDA RODADA
    if (ganhador[0] == 1 && ganhador[1] == 0)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == -1 && ganhador[1] == 0)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // EMPATE NAS 2 PRIMEIRAS RODADAS
    if (ganhador[0] == 0 && ganhador[1] == 0 && ganhador[2] == 1)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == 0 && ganhador[1] == 0 && ganhador[2] == -1)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // EMPATE NA 2 e 3 RODADA
    if (ganhador[0] == 1 && ganhador[1] == 0 && ganhador[2] == 0)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == -1 && ganhador[1] == 0 && ganhador[2] == 0)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // EMPATE NA 3 RODADA
    if (ganhador[0] == 1 && ganhador[1] == -1 && ganhador[2] == 0)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == -1 && ganhador[1] == 1 && ganhador[2] == 0)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // EMPATE NAS 3 RODAS
    if (ganhador[0] == 0 && ganhador[1] == 0 && ganhador[2] == 0)
    {
        printf("\nESSA RODADA DEU IMPATE NINGUEM GANHA");
        return 4;
    }

    // CASO MAQUINA OU JOGADOR GANHE AS 2 PRIMEIRAS PARTIDAS
    if (ganhador[0] == -1 && ganhador[1] == -1)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    if (ganhador[0] == 1 && ganhador[1] == 1)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }
    // CASO MAQUINA OU JOGADOR GANHE AS 2 ULTIMAS PARTIDAS
    if (ganhador[0] == -1 && ganhador[1] == 1 && ganhador[2] == 1)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == 1 && ganhador[1] == -1 && ganhador[2] == -1)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    // CASO O JOGADOR GANHE AS PARTIDAS DO EXTREMO
    if (ganhador[0] == 1 && ganhador[1] == -1 && ganhador[2] == 1)
    {
        printf("\nMAQUINA LEVOU ESSA RODADA");
        tento[1] = tento[1] + pontos;
        return 4;
    }

    if (ganhador[0] == -1 && ganhador[1] == 1 && ganhador[2] == -1)
    {
        printf("\nJOGADOR LEVOU ESSA RODADA");
        tento[0] = tento[0] + pontos;
        return 4;
    }

    if (ganhador[0] == 1 && ganhador[1] == 3 && ganhador[2] == 3)
    {
        return 0;
    }
}

void trucando(int *cartas_maquina, int *pontu_mesa, int turno, int *cont_turno)
{

    int alta = 0, alta_media = 0, medio = 0;
    int escolha;
    int escolha1;

    /*
    alta - 50, 40, 30 e 20
    alta media - 10 e 9
    medio - 8 e 7
    baixa - 6, 5, 4, 3, 2, 1
    */

    // verificando se tenho carta alta
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 50 || cartas_maquina[i] == 40 || cartas_maquina[i] == 30 || cartas_maquina[i] == 20)
        {
            alta++;
        }
    }

    // verificando se tenho carta alta media
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 10 || cartas_maquina[i] == 9)
        {
            alta_media++;
        }
    }

    // verificando se tenho carta media
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 8 || cartas_maquina[i] == 7 || cartas_maquina[i] == 6)
        {
            medio++;
        }
    }
    /*
        printf("\n%d",alta);
        printf("\n%d",alta_media);
        printf("\n%d",medio);
    */
    if (turno == 3)
    // analisando truco 3 cartas
    {
        if ((alta >= 2) || (alta >= 1 && alta_media >= 1) || (alta_media >= 1 && medio >= 1) || (alta >= 1 && medio >= 1) || (alta_media >= 2))
        {
            printf("\nMAQUINA ACEITA TRUCO!");
            // PEDINDO 6
            if ((alta >= 2) || (alta >= 1 && alta_media >= 1) || (alta_media >= 2))
            {
                printf("\n6 LADRAO!");
                *pontu_mesa = 6;
            }
            else
            {
                printf("\nMAQUINA NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 3;
            }
        }

        else
        {
            printf("\nMAQUINA CORREU!");
            printf("\nJOGADOR RECEBE 1 PONTO");
            *pontu_mesa = 1;
            *cont_turno = 5;
        }

        // analisando truco 6 pontos
        if (*pontu_mesa == 6)
        {
            // MAQUINA PEDIU 6
            do
            {
                printf("\nAceita?");
                printf("\n[1]MANDAR CAIR\t[2]FUGIR\t[3]PEDIR 9");
                printf("\n>>");
                scanf("%d", &escolha);
            } while (escolha != 1 && escolha != 2 && escolha != 3);

            if (escolha == 3)
            {
                printf("\nNOVE SUA FAJUTA!");
                *pontu_mesa = 9;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 6;
            }

            else if (escolha == 2)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 3 PONTOS");
                *pontu_mesa = 3;
                *cont_turno = 6;
            }
        }

        // analisando truco 9 pontos
        if (*pontu_mesa == 9)
        {
            if ((alta >= 2) || (alta >= 1 && alta_media >= 1) || (alta_media >= 2))
            {
                printf("\nMAQUINA ACEITA 9!");
                // PEDINDO 12
                if ((alta >= 2) || (alta >= 1 && medio >= 1))
                {
                    printf("\n12 PRA ACABAR!");
                    *pontu_mesa = 12;
                }

                else
                {
                    printf("\nMAQUINA NAO QUER AUMENTAR OS TENTO!");
                    *pontu_mesa = 9;
                    // finaliza a partida
                }
            }
            else
            {
                printf("\nMAQUINA CORREU!");
                printf("\nJOGADOR RECEBE 6 PONTO");
                *pontu_mesa = 6;
                *cont_turno = 5;
            }
        }

        // analisando truco 12 pontos
        if (*pontu_mesa == 12)
        {
            // MAQUINA PEDIU 12
            printf("\nAceita?");
            printf("\n[0]MANDA DESCER\t[1]FUGIR");
            printf("\n>>");
            scanf("%d", &escolha);
            if (escolha == 0)
            {
                printf("\nJOGADOR ACEITOU 12!");
                *pontu_mesa = 12;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 9 PONTOS");
                *pontu_mesa = 9;
                *cont_turno = 6;
            }
        }
    }

    if (turno == 2)
    // analisando truco 3 pontos
    {
        if ((alta >= 1) || (alta >= 1 && alta_media >= 1) || (alta_media >= 2) || (medio >= 2) || (alta_media >= 1 && medio >= 1))
        {
            printf("\nMAQUINA ACEITOU O TRUCO!");
            // PEDINDO 6
            if ((alta >= 1) || (alta >= 1 && alta_media >= 1))
            {
                printf("\nSEIS!");
                ;
                *pontu_mesa = 6;
            }
            else
            {
                printf("\nMAQUINA NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 3;
            }
        }

        else
        {
            printf("\nMAQUINA CORREU!");
            printf("\nJOGADOR RECEBE 1 TENTOS");
            *pontu_mesa = 1;
            *cont_turno = 5;
        }

        // analisando truco 6 pontos
        if (*pontu_mesa == 6)
        {
            // MAQUINA PEDIU 6
            do
            {
                printf("\nAceita??");
                printf("\n[1]MANDAR CAIR\t[2]FUGIR\t[3]PEDIR 9");
                printf("\n>>");
                scanf("%d", &escolha);
            } while (escolha != 1 && escolha != 2 && escolha != 3);

            if (escolha == 3)
            {
                printf("\nJOGADOR PEDIU 9!");
                *pontu_mesa = 9;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 6;
            }

            else if (escolha == 2)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 3 TENTOS");
                *pontu_mesa = 3;
                *cont_turno = 6;
            }
        }

        // analisando truco 9 pontos
        if (*pontu_mesa == 9)
        {
            if ((alta >= 2) || (alta >= 1 && alta_media >= 1) || (alta_media >= 2))
            {
                printf("\nMAQUINA ACEITA 9!");
                // PEDINDO 12
                if ((alta >= 2) || (alta >= 1 && alta_media >= 1))
                {
                    printf("\nDOZE!");
                    *pontu_mesa = 12;
                }

                else
                {
                    printf("\nMAQUINA NAO QUER AUMENTAR TENTO!");
                    *pontu_mesa = 9;
                    // finaliza a partida
                }
            }
            else
            {
                printf("\nMAQUINA CORREU!");
                printf("\nJOGADOR RECEBE 6 TENTOS");
                *pontu_mesa = 6;
                *cont_turno = 5;
            }
        }

        // analisando truco 12 pontos
        if (*pontu_mesa == 12)
        {
            // MAQUINA PEDIU 12
            printf("\nAceita?");
            printf("\n[0]MANDAR CAIR\t[1]FUGIR");
            printf("\n>>");
            scanf("%d", &escolha);
            if (escolha == 0)
            {
                printf("\nEH DOZE!");
                *pontu_mesa = 12;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 9 TENTOS");
                *pontu_mesa = 9;
                *cont_turno = 6;
            }
        }
    }

    if (turno == 1)
    // analisando truco 3 pontos
    {
        if ((alta == 1) || (alta_media == 1))
        {
            printf("\nMAQUINA ACEITOU TRUCO!");
            // PEDINDO 6
            if ((alta >= 1))
            {
                printf("\nSEIS!");
                *pontu_mesa = 6;
            }
            else
            {
                printf("\nMAQUINA NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 3;
            }
        }

        else
        {
            printf("\nMAQUINA CORREU!");
            printf("\nJOGADOR RECEBE 1 TENTOS");
            *pontu_mesa = 1;
            *cont_turno = 5;
        }

        // analisando truco 6 pontos
        if (*pontu_mesa == 6)
        {
            // MAQUINA PEDIU 6
            do
            {
                printf("\nAceita?");
                printf("\n[1]MANDAR CAIR\t[2]FUGIR\t[3]PEDIR 9");
                printf("\n>>");
                scanf("%d", &escolha);
            } while (escolha != 1 && escolha != 2 && escolha != 3);

            if (escolha == 3)
            {
                printf("\nNOVE!");
                *pontu_mesa = 9;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR NAO QUER AUMENTAR TENTO!");
                *pontu_mesa = 6;
            }

            else if (escolha == 2)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 3 TENTOS");
                *pontu_mesa = 3;
                *cont_turno = 6;
            }
        }

        // analisando truco 9 pontos
        if (*pontu_mesa == 9)
        {
            if ((alta == 1))
            {
                printf("\nMAQUINA ACEITA 9!");
                // PEDINDO 12
                if (alta == 1)
                {
                    printf("\nDOZE!");
                    *pontu_mesa = 12;
                }

                else
                {
                    printf("\nMAQUINA NAO QUER AUMENTAR TENTO!");
                    *pontu_mesa = 9;
                }
            }
            else
            {
                printf("\nMAQUINA CORREU!");
                printf("\nJOGADOR RECEBE 6 TENTOS");
                *pontu_mesa = 6;
                *cont_turno = 5;
            }
        }

        // analisando truco 12 pontos
        if (*pontu_mesa == 12)
        {
            // MAQUINA PEDIU 12
            printf("\nAceita?");
            printf("\n[0]MANDAR DESCER\t[1]FUGIR");
            printf("\n>>");
            scanf("%d", &escolha);
            if (escolha == 0)
            {
                printf("\nJOGADOR ACEITOU 12!");
                *pontu_mesa = 12;
            }
            else if (escolha == 1)
            {
                printf("\nJOGADOR CORREU!");
                printf("\nMAQUINA RECEBE 9 TENTOS");
                *pontu_mesa = 9;
                *cont_turno = 6;
            }
        }
    }
}

void infos(int *ganhador, int pontos_turno, int *tento, int *string, int turno)
{
    system("cls");
    char naipe[4] = {6, 5, 3, 4};
    char face[10] = {'3', '2', 'A', 'K', 'J', 'Q', '7', '6', '5', '4'};

    printf("\n==============================================");

    printf("\nPLACAR PARCIAL: JOGADOR[%d] X [%d]MAQUINA", tento[0], tento[1]);
    printf("\nRODADAS VENCIDAS:");

    // primeiro ganhador
    if (ganhador[0] == 1)
        printf("\n[1] VENCEDOR: MAQUINA");
    else if (ganhador[0] == -1)
        printf("\n[1] VENCEDOR: JOGADOR");
    else
        printf("\n[1] VENCEDOR:");

    // segundo ganhador
    if (ganhador[1] == 1)
        printf("\n[2] VENCEDOR: MAQUINA");
    else if (ganhador[1] == -1)
        printf("\n[2] VENCEDOR: JOGADOR");
    else
        printf("\n[2] VENCEDOR:");

    // terceiro ganhador
    if (ganhador[2] == 1)
        printf("\n[3] VENCEDOR: MAQUINA");
    else if (ganhador[2] == -1)
        printf("\n[3] VENCEDOR: JOGADOR");
    else
        printf("\n[3] VENCEDOR:");

    printf("\nVALOR DA MAO ATUAL: %d", pontos_turno);

    printf("\n==============================================");
    printf("\n-------------- CARTAS NA MESAS ---------------");

    if (turno == 3)
    {
        printf("\nMAQUINA:  [] [] []");
        printf("\n\nJOGADOR:  [] [] []");
    }
    if (turno == 2)
    {
        printf("\nMAQUINA:  %c%c  [] []", face[string[0]], naipe[string[1]]);
        printf("\n\nJOGADOR:  %c%c  [] []", face[string[2]], naipe[string[3]]);
    }
    if (turno == 1)
    {
        printf("\nMAQUINA:  %c%c  %c%c  [] ", face[string[0]], naipe[string[1]], face[string[4]], naipe[string[5]]);
        printf("\n\nJOGADOR:  %c%c  %c%c  []", face[string[2]], naipe[string[3]], face[string[6]], naipe[string[7]]);
    }

    printf("\n==============================================");
}

void potencia(int *cartas_maquina, int *alta, int *alta_media, int *media, int *media_baixa, int *baixa, int turno)
{

    /*
    alta - 50, 40, 30 e 20
    alta media - 10 e 9
    medio - 8 e 7
    baixa - 6, 5, 4, 3, 2, 1
    */

    // verificando se tenho carta alta
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 50 || cartas_maquina[i] == 40 || cartas_maquina[i] == 30 || cartas_maquina[i] == 20)
        {
            *alta = *alta + 1;
        }
    }

    // verificando se tenho carta alta media
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 10 || cartas_maquina[i] == 9)
        {
            *alta_media = *alta_media + 1;
        }
    }

    // verificando se tenho carta media
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 8 || cartas_maquina[i] == 7)
        {
            *media = *media + 1;
        }
    }

    // verificando se tenho carta media baixa
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 6 || cartas_maquina[i] == 5)
        {
            *media_baixa = *media_baixa + 1;
        }
    }

    // verificando se tenho carta baixa
    for (int i = 0; i < turno; i++)
    {
        if (cartas_maquina[i] == 4 || cartas_maquina[i] == 3 || cartas_maquina[i] == 2 || cartas_maquina[i] == 1)
        {
            *baixa = *baixa + 1;
        }
    }
}

int maquina_truca(int alta, int alta_media, int media, int media_baixa, int baixa, int pontos_adversario)
{

    /*
        VARIAVEIS
        alta - Zap (50), 7 Copas (40), Espadilha (30) e 7 Ouro (20)
        alta_media - Tres (10) e Dois (9)
        media - Ás (8) e Rei (7)
        media_baixa - Valete (6) e Dama (5)
        baixa - Sete (4), Seis (3), Cinco (2), Quatro (1)
    */

    /*
        RETORNO
        0 - nao truca
        1 - truca
        2 - aumenta a mao
    */

    // Baseado nos pontos do oponente, nao nos pontos da mesa

    // alto risco na primeira rodada
    if (pontos_adversario == 0)
    {
        if ((alta >= 1) || (alta == 1 && alta_media == 2) || (media >= 1 && media_baixa == 1) || (baixa == 1 && alta_media >= 1) || (alta_media >= 1 && media == 1))
        {
            return 1;
        }
        else if ((alta >= 1) || (alta >= 1 && alta_media >= 1) || (media >= 1 && baixa >= 1) || (alta_media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // ainda no alto risco, porem conservadora ao aumentar a mao
    if (pontos_adversario == 1)
    {
        if ((alta == 1 && alta_media == 2) || (media >= 1) || (alta_media == 1 && baixa >= 1))
        {
            return 1;
        }
        else if ((alta >= 1 && alta_media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // conservador, e nao truca
    if (pontos_adversario == 2)
    {
        if ((alta == 2 && alta_media == 1) || (alta_media == 3))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // risco, porém conservador no momento de aumentar aposta
    if (pontos_adversario == 3)
    {
        if ((alta >= 1) || (alta == 1 && alta_media == 2) || (alta_media == 1 && media >= 1) || (media_baixa >= 1 && baixa >= 1))
        {
            return 1;
        }
        else if ((alta >= 1) || (alta >= 1 && alta_media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // conservado ao trucar, mas alto risco em aumentar a aposta
    if (pontos_adversario == 4)
    {
        if ((alta >= 1) || (alta >= 1 && alta_media == 1) || (alta_media == 3))
        {
            return 1;
        }
        else if ((alta >= 1) || (alta == 1 && alta_media == 1 && media == 1) || (alta_media == 1 && media >= 1) || (alta == 1 && baixa >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // medio risco, sem aumentar
    if (pontos_adversario == 5)
    {
        if ((alta == 1 && media_baixa >= 1) || (alta_media >= 1 && media == 1) || (alta_media >= 2))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    // alto risco no truco e medio risco no aumento de mao
    if (pontos_adversario == 6)
    {
        if ((alta >= 1) || (alta_media == 1 && baixa == 2) || (media == 1 && alta_media >= 1) || (media >= 1 && media_baixa == 1))
        {
            return 1;
        }
        else if ((alta == 1 && media_baixa >= 1) || (alta == 1 && media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // medio risco
    if (pontos_adversario == 7)
    {
        if ((alta == 2 && baixa == 1) || (alta_media >= 1 && alta == 1) || (media == 1 && alta_media >= 1))
        {
            return 1;
        }
        else if ((alta_media == 2 && alta == 1) || (media >= 1 && alta_media == 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // só vai trucar se tiver duas manilhas
    if (pontos_adversario == 8)
    {
        if (alta > 2)
        {
            return 1;
        }
        else if ((alta >= 1 && alta_media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    // extremamente conservador
    if (pontos_adversario == 9)
    {
        if (alta >= 2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    // muito conservador
    if (pontos_adversario == 10)
    {
        if ((alta >= 2 && alta_media == 1))
        {
            return 1;
        }
        else if ((alta >= 2) || (alta == 2 && alta_media >= 1))
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
}

void game(void)
{

    char naipe[4] = {6, 5, 3, 4};
    char face[10] = {'3', '2', 'A', 'K', 'J', 'Q', '7', '6', '5', '4'};

    struct Player Player[100];

    srand(time(NULL));
    int acabou = 0; // usado para finaliza a partida
    int tento[2] = {0, 0}; // posicao 0 - jogador, posicao 1 - maquina

    int quem_joga = rand() % 2;//chama de forma aleatoria quem começa na primeira mao

    // RODADA TENTO(TENTO)
    for (int i = 0; acabou < 12; i++)
    {
        my_sleep(1000);
        int mtr[10][4];

        int *p_jogc, *p_jogl, *p_jog; // CORDENADAS NA MAQUINA
        int *p_magc, *p_magl, *p_mag; // CORDENADAS NA JOGADOR

        int escolha_jog, escolha_mag; // ESCOLHAS A SEREM ANALISADAS
        char continuar;

        int *ganhador, resultado, contador_ganhador = 0;
        /*
        ganhador - EQUIVALE A QUEM GANHOU CADA TURNO (-1 - JOGADOR GANHOU, 0 - IMPATE, 1 - MAQUINA GANHOU)
        resultado - variavel para retorno da funcao vencedor
        contador_ganhador - variavel para contar em qual posicao/turno deve guardar quem ganhou
        */

        // DEFININDO TAMANHO DOS PONTEIROS
        p_jogc = (int *)malloc(3 * sizeof(int *));
        p_jogl = (int *)malloc(3 * sizeof(int *));
        p_jog = (int *)malloc(3 * sizeof(int *));

        p_magc = (int *)malloc(3 * sizeof(int *));
        p_magl = (int *)malloc(3 * sizeof(int *));
        p_mag = (int *)malloc(3 * sizeof(int *));

        ganhador = (int *)malloc(3 * sizeof(int *));
        for (int i = 0; i < 3; i++)
        {
            ganhador[i] = 3;
        }

        criando_baralho(mtr);

        // DISTRIBUINDO PARA O JOGADOR
        distribuido_baralho(mtr, p_jogc, p_jogl, p_jog);

        my_sleep(1000);
        // DISTRIBUINDO PARA A MAQUINA
        distribuido_baralho(mtr, p_magc, p_magl, p_mag);

        int turno = 3, escolha, ESCOLHA = 0;
        /*
        turno - EQUIVALE A QUANTIDADE DE CARTAS NA MAO DE CADA JOGADOR
        escolha - EQUIVALE A POSIÇÃO NO VETOR CARTAS JOGADOR ONDE SE ENCONTTRA A CARTA ESCOLHIDA
        ESCOLHA - EQUIVALE A POSIÇÃO NO VETOR CARTAS JOGADOR ONDE SE ENCONTTRA A CARTA ESCOLHIDA
        */

        int cont_turno; //conta
        int pontos_turno = 1; //quanta vale a mao
        int rodada_truco = 0; // saber se eh uma partida de truco ou nao

        int string[12];
        int cont_string = 0;
        int escolha_jog_mag;

        // RODADA CARTAS(MAO)
        for (cont_turno = 0; cont_turno < 3; cont_turno++)
        {
            int partidas_tento = 0;

            if (quem_joga == 1)
            {
                if (tento[0] == 11 && tento[1] == 11)
                {
                    partidas_tento = 1;
                }
                infos(ganhador, pontos_turno, tento, string, turno);
                printf("\n           MAQUINA JOGA PRIMEIRO");
                printf("\n==============================================");

                int alta = 0, alta_media = 0, media = 0, media_baixa = 0, baixa = 0;

                if (partidas_tento == 1)
                {
                    ESCOLHA = rand() % 3;
                }

                else
                {

                    potencia(p_mag, &alta, &alta_media, &media, &media_baixa, &baixa, turno);

                    ESCOLHA = maquina_truca(alta, alta_media, media, media_baixa, baixa, tento[0]);

                    if (rodada_truco == 0)
                    {
                        if (ESCOLHA == 1 || ESCOLHA == 2)
                        {
                            printf("\nMAQUINA TRUCOU");
                            do
                            {
                                printf("\nvoce aceita??");
                                printf("\n[1]MANDAR CAIR\t[2]FUGIR\t[3]PEDIR 6");
                                printf("\n>>");
                                scanf("%d", &escolha_jog_mag);
                            } while (escolha_jog_mag != 1 && escolha_jog_mag != 2 && escolha_jog_mag != 3);

                            if (escolha_jog_mag == 3)
                            {
                                printf("\nJOGADOR PEDIU 6");
                                rodada_truco = 1;
                                pontos_turno = 6;
                                printf("\nMAQUINA MANDOU CAIR");
                            }

                            if (escolha_jog_mag == 2)
                            {
                                printf("\nJOGADOR FUGIU");
                                cont_turno = 6;
                                pontos_turno = 3;
                            }

                            if (escolha_jog_mag == 1)
                            {
                                printf("\nJOGADOR MADOU CAIR");
                                rodada_truco = 1;
                                pontos_turno = 3;
                            }

                            rodada_truco = 1;
                        }
                    }

                    escolha_jog = rand() % 30;

                    printf("\n==============================================\n");

                    ESCOLHA = maquina_joga(escolha_jog, p_mag, p_magc, p_magl, turno);
                }

                escolha_mag = *(p_mag + ESCOLHA);

                printf("\n==============================================\n");

                escolha = jogador_joga(p_jog, p_jogc, p_jogl, turno, rodada_truco, partidas_tento);
                if (escolha == -1)
                {
                    trucando(p_mag, &pontos_turno, turno, &cont_turno);
                    printf("\n==============================================");
                    printf("\nJOGADOR PEDIU TRUCO");

                    rodada_truco = 1;
                    if (cont_turno == 5 || cont_turno == 6)
                    {
                        if (cont_turno == 5)
                        {
                            printf("\nJOGADOR LEVOU ESSE TURNO");
                        }
                        if (cont_turno == 6)
                        {
                            printf("\nMAQUINA LEVOU ESSE TURNO");
                        }
                    }

                    else
                    {
                        escolha = jogador_joga(p_jog, p_jogc, p_jogl, turno, rodada_truco, partidas_tento);
                    }
                }

                else
                {
                    escolha_jog = *(p_jog + escolha);
                }

                if (cont_turno == 5 || cont_turno == 6)
                {
                    if (cont_turno == 5)
                    {
                        tento[0] = tento[0] + pontos_turno;
                    }
                    if (cont_turno == 6)
                    {
                        tento[1] = tento[1] + pontos_turno;
                    }
                }

                else
                {
                    escolha_jog = *(p_jog + escolha);

                    // analisando vencedo

                    resultado = vencedor(escolha_jog, escolha_mag);

                    ganhador[contador_ganhador] = resultado;

                    cont_turno = anali_situ(ganhador, tento, pontos_turno);

                    // APENAS FRUFRU PARA PRINTAR CARTAZ NA MESA
                    string[cont_string] = p_magc[ESCOLHA];
                    cont_string++;
                    string[cont_string] = p_magl[ESCOLHA];
                    cont_string++;
                    string[cont_string] = p_jogc[escolha];
                    cont_string++;
                    string[cont_string] = p_jogl[escolha];
                    cont_string++;

                    carta_atu(p_jogc, p_jogl, p_jog, turno, escolha);
                    carta_atu(p_magc, p_magl, p_mag, turno, ESCOLHA);

                    my_sleep(1000);

                    turno--;
                    contador_ganhador++;

                    p_mag = realloc(p_mag, turno * sizeof(int));
                    p_magc = realloc(p_magc, turno * sizeof(int));
                    p_magl = realloc(p_magl, turno * sizeof(int));
                }

                printf("\n==============================================");
                printf("\nTAB para continuar");
                continuar = getch();
                // system("cls");
            }

            if (quem_joga == 0)
            {

                int j=0;

                if (tento[0] == 11 && tento[1] == 11)
                {
                    partidas_tento = 1;
                }

                if(tento[0]==11) {
                    printf("\n==============================================");
                    printf("\n %c %c %c %c %c  VOCE ESTA NA MAO DE ONZE %c %c %c %c %c ", 6, 5, 3, 4, 6, 5, 3, 4, 6, 5);
                    printf("\nSuas cartas sao:\n");

                    for(int i = 0; i < turno; i++) {
                        printf("[%d]%c%c\t",j, face[*(p_jogc+i)], naipe[*(p_jogl+i)] );
                        j++;

                    }
                    int escolha1;
                    printf("\n==============================================");
                    printf("\nDESEJA JOGAR MAO DE ONZE?\n[1]-Jogar [2]-Fugir");
                    printf("\n>>>");
                    fflush(stdin);
                    scanf("%d", &escolha1);

                    if (escolha1==1) {
                        pontos_turno=3;
                    }
                    if (escolha1==2) {
                        cont_turno= 11;
                     }
                }


                infos(ganhador, pontos_turno, tento, string, turno);
                printf("\n           JOGADOR JOGA PRIMEIRO");
                printf("\n==============================================");

                escolha = jogador_joga(p_jog, p_jogc, p_jogl, turno, rodada_truco, partidas_tento);

                if (escolha== -1 && tento[0]==11) {
                    printf("\nVOCE ESTA NA MAO DE ONZE!! VOCE TRUCOU. MAQUINA GANHOU");
                    cont_turno=6;
                    pontos_turno= 12;
                    tento[1]=12;
                    escolha=0;
                }

                if (escolha== -1 && tento[0]==11 && tento[0]==11) {
                    printf("\nVOCE ESTA NA MAO DE FERRO!! VOCE TRUCOU. MAQUINA GANHOU");
                    cont_turno=6;
                    pontos_turno= 12;
                    tento[1]=12;
                    escolha=0;
                }

                if (escolha == -1)
                {
                    trucando(p_mag, &pontos_turno, turno, &cont_turno);
                    printf("\n==============================================");
                    printf("\nJOGADOR PEDIU TRUCO");

                    rodada_truco = 1;
                    if (cont_turno == 5 || cont_turno == 6)
                    {
                        if (cont_turno == 5)
                        {
                            printf("\nJOGADOR LEVOU ESSE MAO");
                        }
                        if (cont_turno == 6)
                        {
                            printf("\nMAQUINA LEVOU ESSE MAO");
                        }
                    }

                    else
                    {
                        escolha = jogador_joga(p_jog, p_jogc, p_jogl, turno, rodada_truco, partidas_tento);
                    }
                }

                else
                {
                    escolha_jog = *(p_jog + escolha);
                }

                if (cont_turno == 5 || cont_turno == 6)
                {
                    if (cont_turno == 5)
                    {
                        tento[0] = tento[0] + pontos_turno;
                    }
                    if (cont_turno == 6)
                    {
                        tento[1] = tento[1] + pontos_turno;
                    }
                }

                else
                {
                    escolha_jog = *(p_jog + escolha);

                    printf("\n==============================================\n");

                    if (partidas_tento == 1)
                    {
                        ESCOLHA = rand() % 3;
                    }

                    else
                    {
                        ESCOLHA = maquina_joga(escolha_jog, p_mag, p_magc, p_magl, turno);
                    }

                    escolha_mag = *(p_mag + ESCOLHA);

                    if (partidas_tento == 1)
                    {
                        printf("\nMAQUINA JOGA: ");
                        printf("%c%c\t", face[*(p_magc + ESCOLHA)], naipe[*(p_magl + ESCOLHA)]);
                    }

                    printf("\n==============================================\n");

                    // analisando vencedo

                    resultado = vencedor(escolha_jog, escolha_mag);

                    ganhador[contador_ganhador] = resultado;

                    cont_turno = anali_situ(ganhador, tento, pontos_turno);

                    // APENAS FRUFRU PARA PRINTAR CARTAZ NA MESA
                    string[cont_string] = p_magc[ESCOLHA];
                    cont_string++;
                    string[cont_string] = p_magl[ESCOLHA];
                    cont_string++;
                    string[cont_string] = p_jogc[escolha];
                    cont_string++;
                    string[cont_string] = p_jogl[escolha];
                    cont_string++;

                    carta_atu(p_jogc, p_jogl, p_jog, turno, escolha);
                    carta_atu(p_magc, p_magl, p_mag, turno, ESCOLHA);

                    my_sleep(1000);

                    turno--;
                    contador_ganhador++;

                    p_mag = realloc(p_mag, turno * sizeof(int));
                    p_magc = realloc(p_magc, turno * sizeof(int));
                    p_magl = realloc(p_magl, turno * sizeof(int));
                }
                printf("\n==============================================");
                printf("\nTAB para continuar");
                continuar = getch();
                // system("cls");
            }
        }

        if (quem_joga == 0)
        {
            quem_joga = 1;
        }
        else if (quem_joga == 1)
        {
            quem_joga = 0;
        }
        // PLACAR
        printf("\n==============================================\n");

        if (tento[0] >= 12 || tento[1] >= 12)
        {
            acabou = 12;
        }
    }

    if (tento[0] >= 12)
    {
        printf("\nJOGADOR GANHOU!!!");
        indexarVitorias (Player->name);
        criarRanking ()     ;
        organizarRanking () ;
    }
    if (tento[1] >= 12)
    {
        printf("\nMAQUINA GANHOU!!!");
        indexarDerrota (Player->name);
        criarRanking ()     ;
        organizarRanking () ;
    }
}

void ranking()
{
    FILE *fp;                   // ponteiro arquivo
    char line[1000];            // linhas

    fp = fopen("ranking.txt", "r");
    if (fp == NULL) { printf("Erro ao abrir arquivo");}


    printf("\n\t RANKING DOS JOGADORES:\n\n");
    while (fgets(line, 1000, fp) != NULL) {
        printf("%s", line);
    }

    printf("\n");

    fclose(fp);
}

void anexarPlayer ()
{
    struct
        Player Player[100];             // valor alto para conseguir lidar com muitas entradas

    FILE *fp;                           // chamada do arquivo


    // Abertura, modo apenas escrita:
    fp = fopen("player.txt", "a+");
    if (fp == NULL) {printf("Erro ao abrir o arquivo people.txt");}

    printf("- Digite seu nomezinho: ");
    fflush(stdin);  scanf("%100[^\n]", Player->name);

    fclose(fp);

    printf("[ HEHE! Voce agora faz parte do jogo :) Agradcemos!\n");

}

void indexarVitorias (char *pontVit)
{

    struct
        Player Player[100];             // valor alto para conseguir lidar com muitas entradas

    FILE *fp;                           // chamada arquivo

    // Abertura, modo apenas escrita, seguida de verificacao:
    fp = fopen("player.txt", "a");
    if (fp == NULL) {printf("Erro ao abrir o arquivo player.txt");}

    // Magica de insercao da vitoria em caso de vitoria:
    Player->vitorias = Player->vitorias + 1;

    // Indexando no final do arquivo o novo valor:
    fprintf(fp, "%s, %d, %d\n", (char*)pontVit, Player->vitorias, Player->derrotas);

    fclose(fp);
}

void indexarDerrota (char *pontDer)
{
    struct
        Player Player[100];         // valor alto para conseguir lidar com muitas entradas

    FILE *fp;                       // chamada do arquivo

    // Abertura, modo apenas escrita, seguida de verificacao:
    fp = fopen("player.txt", "a");
    if (fp == NULL) {printf("Erro ao abrir o arquivo people.txt");}

    // Magica de insercao da derrota em caso de derrota:
    Player->derrotas = Player->derrotas + 1;

    // Indexando no final do arquivo o novo valor:
    fprintf(fp, "%s, %d, %d\n", (char*)pontDer, Player->vitorias, Player->derrotas);

    fclose(fp);
}

void criarRanking ()
{
    FILE    *txtEntrada, *txtSaida;             // ponteiros que irao manipular os arquivos
    char    linha_atual[200];                   // linha atual
    char    ultima_linha[200];                  // ultima linha

    txtEntrada    = fopen("player.txt", "r");
    if (txtEntrada == NULL) {printf("Erro ao abrir o arquivo player.txt");}

    txtSaida      = fopen("RC.txt", "a");
    if (txtSaida == NULL) {printf("Erro ao abrir o arquivo RC.txt");}

    while (fgets(linha_atual, 200, txtEntrada) != NULL)
    {
        strcpy(ultima_linha, linha_atual);
    }

    fprintf(txtSaida, "%s", ultima_linha);

    fclose(txtEntrada);
    fclose(txtSaida);
}

int comparativo(const void *a, const void *b)
{
    Player *p1 = (Player *)a;               // comparativo player um
    Player *p2 = (Player *)b;               // comparitivo player dois
    return p2->vitorias - p1->vitorias;     // retorno do comparativo
}

void organizarRanking ()
{
    struct
            Player PlayerStr[100];             // valor alto lidar com muitas entradas

    FILE *txtEntrada, *txtSaida;               // ponteiro dos dois arquivos

    int tamanho = 0;                           // qtd de linhas
    int i = 0;                                 // indice padrao


    txtEntrada    = fopen("RC.txt", "r+");
    if (txtEntrada == NULL) {printf("Erro ao abrir o arquivo RC.txt");}

    //while (fgets(linha, 1024, txtEntrada) != NULL)      { tamanho++; }

    while (fscanf(txtEntrada, "%[^,],%d,%d\n", PlayerStr[tamanho].name, &PlayerStr[tamanho].vitorias, &PlayerStr[tamanho].derrotas) == 3)
    {
        tamanho++;
    }

    fclose(txtEntrada);

    qsort(PlayerStr, tamanho, sizeof(Player), comparativo);

    txtSaida      = fopen("ranking.txt", "w");
    if (txtSaida == NULL) {printf("Erro ao abrir o arquivo ROD.txt");}

    for (i = 0; i < tamanho; i++)
    {
        fprintf(txtSaida, "%s | Vitoria: %d | Derrota: %d\n", PlayerStr[i].name, PlayerStr[i].vitorias, PlayerStr[i].derrotas);
    }

    fclose(txtSaida);
}
