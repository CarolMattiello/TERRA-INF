#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
//definicao de valores
#define ALTURA 800
#define LARGURA 1200
#define LINHAS 20
#define COLUNAS 30
#define TAMANHO_NOME 20
#define MAX_SCORES 5
// limita tamanho do nome do item
#define TAMITEM 10
#define PONTUACAO_MAXIMA 1000

typedef struct //struct JOGADOR para armazenamento de dados do mesmo
{
    char *nome;
    int col;
    int lin;
    int pontuacao;
    int vidas;
    int energia;
    int escadas;
    int modoMineracao;
    int nivel;
    // ultimo item eh uma char que depende do item minerado ex: se minerar s, ultimo item rece a string prata
    char *ultimo_item;
} JOGADOR;

typedef struct //struct QUADRADO para fazer o mapa
{
    int jogador;
    char tipo;
} QUADRADO;

typedef struct //struct RANKING para montagem do ranking
{
    char *nome;
    int pontos;
} RANKING;

int carregaMapa(char *fileName, QUADRADO mapa[LINHAS][COLUNAS], JOGADOR *jogador) //faz a leitura do arquivo para montagem do mapa
{
    int i, j;
    char linha[COLUNAS + 1];

    FILE *mapaFile = fopen(fileName, "r");
    if (mapaFile == NULL)
    {
        return 0;
    }
    else
    {
        while (!feof(mapaFile))
        {
            for (i = 0; i < LINHAS; i++)
            {
                fscanf(mapaFile, "%[^\n]%*c", &linha);
                for (j = 0; j < COLUNAS; j++)
                {
                    QUADRADO quadrado;
                    char tipo = linha[j];
                    if (tipo == 'J')
                    {
                        quadrado.jogador = 1;
                        quadrado.tipo = ' ';
                        jogador->col = j;
                        jogador->lin = i;
                    }
                    else
                    {
                        quadrado.tipo = tipo;
                        quadrado.jogador = 0;
                    }

                    mapa[i][j] = quadrado;
                }
            }
        }
    }
    fclose(mapaFile);
    return 1;
}

void desenhaMapa(QUADRADO mapa[LINHAS][COLUNAS]) //desenha o mapa, acompanhada dos sprites .png
{
    Texture2D textTerra = LoadTexture("terra.png");
    Texture2D textBorda = LoadTexture("borda.png");
    Texture2D textFundo = LoadTexture("fundo_terra.png");
    Texture2D textMinerio = LoadTexture("minerio.png");
    Texture2D textEscada = LoadTexture("escada.png");

    int i, j;
    for (i = 0; i < LINHAS; i++)
    {
        for (j = 0; j < COLUNAS; j++)
        {
            QUADRADO quadrado = mapa[i][j];
            switch (quadrado.tipo)
            {
            case 'G': //desenha ouro
                DrawTexture(textMinerio, j * 40, i * 40, WHITE);
                break;
            case 'T': //desenha titanio
                DrawTexture(textMinerio, j * 40, i * 40, WHITE);
                break;
            case 'S': //desenha prata
                DrawTexture(textMinerio, j * 40, i * 40, WHITE);
                break;
            case 'U': //desenha uranio
                DrawTexture(textMinerio, j * 40, i * 40, WHITE);
                break;
            case 'C': //desenha cesio
                DrawTexture(textMinerio, j * 40, i * 40, WHITE);
                break;
            case ' ': //desenha fundo
                DrawTexture(textFundo, j * 40, i * 40, WHITE);
                break;
            case 'B': //desenha borda
                DrawTexture(textBorda, j * 40, i * 40, WHITE);
                break;
            case 'X': //desenha terra
                DrawTexture(textTerra, j * 40, i * 40, WHITE);
                break;
            case 'H': //desenha escada
                DrawTexture(textEscada, j * 40, i * 40, WHITE);
                break;
            }
        }
    }
}

void desenhaJogador(JOGADOR *jogador, QUADRADO mapa[LINHAS][COLUNAS]) //faz o desenho do jogador, em todos as possiveis situacoes;
{                                                                     //desenho para modomineracao, modomineracaoescada, jogadorescada e etc;
    int col = jogador->col;
    int lin = jogador->lin;

    Texture2D textJogador = LoadTexture("jogador.png");
    Texture2D textJogadorPicaretaEscada = LoadTexture("jogador_picareta_escada.png");
    Texture2D textJogadorEscada = LoadTexture("jogador_escada.png");
    Texture2D textJogadorPicareta = LoadTexture("jogador_picareta.png");

    if (mapa[lin][col].tipo == 'H') //condicao para jogador em cima de uma escada
    {
        if (jogador->modoMineracao == 1) //condicao para jogador em cima de uma escada COM modomineracao ativado
        {
            DrawTexture(textJogadorPicaretaEscada, col * 40, lin * 40, WHITE); //desenha jogador + picareta + escada
        }
        else
        {
            DrawTexture(textJogadorEscada, col * 40, lin * 40, WHITE); //desenha jogador + escada
        }
    }
    else //caso o jogador nao esteja em cima de uma escada
    {
        if (jogador->modoMineracao == 1) //condicao para jogador com modomineracao ativado
        {
            DrawTexture(textJogadorPicareta, col * 40, lin * 40, WHITE); //desenha jogador + picareta
        }
        else
        {
            DrawTexture(textJogador, col * 40, lin * 40, WHITE); //desenha somente o jogador
        }
    }
}

void move(JOGADOR *jogador, QUADRADO mapa[LINHAS][COLUNAS]) //analisa o movimento do jogador, vendo se eh permitido se mover ou nao;
{                                                           //acompanha a "gravidade", responsavel pela queda do jogador, caso nao haja algum bloco abaixo dele;
    int col = jogador->col;
    int lin = jogador->lin;                                 //a movimentacao consiste em aumento ou diminuicao da matriz;
    int i;

    mapa[lin][col].jogador = 0;
    if (IsKeyPressed(KEY_D))
    {
        if (mapa[lin][col + 1].tipo == ' ' || mapa[lin][col + 1].tipo == 'H') //caso o jogador se mexa para a direita, col é adicionado em 1
        {
            col++;
        }
    }
    else if (IsKeyPressed(KEY_A))
    {
        if (mapa[lin][col - 1].tipo == ' ' || mapa[lin][col - 1].tipo == 'H') //caso o jogador se mexa para a esquerda, col é reduzido em 1
        {
            col--;
        }
    }
    if (mapa[lin][col].tipo == 'H') //condicao para o jogador se mover para cima ou para baixo, SOMENTE se tiver sobre uma escada
    {
        if (IsKeyPressed(KEY_S))
        {
            if (mapa[lin + 1][col].tipo == ' ' || mapa[lin + 1][col].tipo == 'H') //caso o jogador se mexa para baixo, lin é adicionado em 1
            {
                lin++;
            }
        }
        else if (IsKeyPressed(KEY_W))
        {
            if (mapa[lin - 1][col].tipo == 'H') //caso o jogador se mexa para cima, lin é reduzido em 1
            {
                lin--;
            }
        }
    }
    // gravidade
    if (mapa[lin + 1][col].tipo == ' ') //analise de queda e numero de blocos caidos
    {
        for (i = lin; i < LINHAS; i++)
        {
            if (mapa[i][col].tipo != ' ')
            {
                lin = i - 1;
                break;
            }
        }
    }
    if (lin - jogador->lin > 3) //caso o jogador caia mais de 3 blocos, a vida eh reduzida em 1
    {
        jogador->vidas--;
    }

    mapa[lin][col].jogador = 1;
    jogador->col = col;
    jogador->lin = lin;
}

void podeEscada(JOGADOR *jogador, QUADRADO mapa[LINHAS][COLUNAS]) //analise da possibilidade de colocar uma escada
{
    if (IsKeyPressed(KEY_LEFT_SHIFT) && jogador->escadas > 0)
    {
        if (mapa[jogador->lin][jogador->col].tipo == ' ')
        {
            mapa[jogador->lin][jogador->col].tipo = 'H'; //troca do espaco em branco ' ' para 'H', mudando a matriz
            jogador->escadas--; //diminuicao do numero de escadas
        }
        else if (mapa[jogador->lin - 1][jogador->col].tipo == ' ')
        {
            mapa[jogador->lin - 1][jogador->col].tipo = 'H';
            jogador->escadas--;
        }
    }
}

int modoMineracao(JOGADOR *jogador, QUADRADO mapa[LINHAS][COLUNAS]) //ativa ou desativa o modomineracao a partir da tecla 1
{
    if (IsKeyPressed(KEY_ONE))
    {
        if (jogador->modoMineracao == 1)
        {
            jogador->modoMineracao = 0;
        }
        else
        {
            jogador->modoMineracao = 1;
        }
    }

    if (jogador->modoMineracao == 0 || !IsKeyDown(KEY_SPACE)) //quando o jogador SEGURAR a tecla espaco e se mover, o bloco sera minerado
    {
        return 0;
    }

    int lin, col;

    if (IsKeyPressed(KEY_W)) //caso minere para cima, lin eh reduzido em 1
    {
        lin = jogador->lin - 1;
        col = jogador->col;
    }
    else if (IsKeyPressed(KEY_S)) //caso minere para baixo, lin eh aumentado em 1
    {
        lin = jogador->lin + 1;
        col = jogador->col;
    }
    else if (IsKeyPressed(KEY_D)) //caso minere para direita, col eh aumentado em 1
    {
        lin = jogador->lin;
        col = jogador->col + 1;
    }
    else if (IsKeyPressed(KEY_A)) //caso minere para esquerda, col é reduzido em 1
    {
        lin = jogador->lin;
        col = jogador->col - 1;
    }
    else
    {
        return 0;
    }

    QUADRADO alvo = mapa[lin][col];
    int pontuacao;
    int energia;

    if (alvo.tipo != ' ' && alvo.tipo != 'H' && alvo.tipo != 'B') //condicao para nao minerar espacos em branco, espacos ocupados por escadas e espacos ocupados por bordas
    {
        switch (alvo.tipo) //definicao do aumento/perda de pontuacao e energia, alem do uso da jogador->ultimo_item para mostragem na barra de status
        {
        case 'G': //definicoes para o caso ouro
            energia = 20;
            pontuacao = 100;
            jogador->ultimo_item = "Ouro";
            break;
        case 'T': //definicoes para o caso titanio
            energia = 30;
            pontuacao = 150;
            jogador->ultimo_item = "Titanio";
            break;
        case 'S': //definicoes para o caso prata
            energia = 10;
            pontuacao = 50;
            jogador->ultimo_item = "Prata";
            break;
        case 'U': //definicoes para o caso uranio
            energia = -30;
            jogador->ultimo_item = "Uranio";
            break;
        case 'C': //definicoes para o caso cesio
            energia = -20;
            jogador->ultimo_item = "Cesio";
            break;
        default: //definicoes para o caso terra
            energia = -3;
            pontuacao = 0;
            break;
        }
        jogador->pontuacao += pontuacao; //acrescimo/decrescimo de pontuacao
        jogador->energia += energia; //acrescimo/decrescimo de energia

        mapa[lin][col].tipo = ' '; //o espaco minerado agora eh espaco em branco
    }
}

void estadoJogo(JOGADOR *jogador) //auxilio do estado do jogador
{
    if (jogador->energia <= 20) //se a energia for inferior a 20, a vida eh reduzida em 1
    {
        jogador->vidas--;
        if (jogador->vidas > 0) //caso o jogador perca uma vida pela queda da energia, jogador->energia eh adicionado em 100
        {
            jogador->energia += 100;
        }
    }

    if (jogador->vidas < 0) //caso o jogador perca todas as vidas
    {
        while (1)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Game Over :(", ALTURA / 2, ALTURA / 2, 36, RED); //tela de game over eh desenhada
            EndDrawing();
        }
    }
}

void barraStatus(JOGADOR *jogador) //desenha a barra de status na parte superior da tela do jogo
{
    DrawRectangle(0, 0, LARGURA, 40, BLACK);
    //desenha pontuacao
    DrawText(TextFormat("SCORE:"), 10, 10, 20, BROWN);
    DrawText(TextFormat("%01i pontos", jogador->pontuacao), 90, 10, 20, GREEN);
    //desenha vidas
    DrawText(TextFormat("VIDAS:"), 250, 10, 20, BROWN);
    DrawText(TextFormat("%01i", jogador->vidas), 330, 10, 20, GREEN);
    //desenha energia
    DrawText(TextFormat("ENERGIA:"), 400, 10, 20, BROWN);
    DrawText(TextFormat("%03i", jogador->energia), 500, 10, 20, GREEN);
    //deseha escadas
    DrawText(TextFormat("ESCADAS:"), 600, 10, 20, BROWN);
    DrawText(TextFormat("%02i", jogador->escadas), 710, 10, 20, GREEN);
    //desenha fase
    DrawText(TextFormat("FASE:"), 770, 10, 20, BROWN);
    DrawText(TextFormat("%01i", jogador->nivel), 840, 10, 20, GREEN);
    //desenha item
    DrawText(TextFormat("ITEM:"), 890, 10, 20, BROWN);
    if (jogador->ultimo_item[0] == '0') //caso nao tenha minerado nenhum item
    {
        DrawText(TextFormat("Ainda nao minerou"), 960, 10, 20, GREEN);
    }
    else
    {
        DrawText(TextFormat("%s", jogador->ultimo_item), 960, 10, 20, GREEN);
    }
}

int desenhaMenu() //desenha o menu do jogo
{
    BeginDrawing();
    Texture2D menu_capa = LoadTexture("capa.png"); //carrega o png da capa do menu
    // DrawTexture(menu_capa, 0, 0, WHITE);
    DrawTexturePro(menu_capa, (Rectangle){0, 0, LARGURA, ALTURA}, (Rectangle){0, 0, LARGURA, ALTURA}, (Vector2){0, 0}, 0, WHITE);
    DrawText(TextFormat("Novo Jogo [N]"), ((LARGURA / 2) - 110), 450, 30, GREEN);
    DrawText(TextFormat("Ranking de Pontos [R]"), ((LARGURA / 2) - 110), 520, 20, GREEN);
    DrawText(TextFormat("Sair [S]"), ((LARGURA / 2) - 40), 570, 20, GREEN);
    DrawText(TextFormat("Para voltar ao menu, pressione backspace"), ((LARGURA / 2) - 220), 650, 20, WHITE);
    EndDrawing();

    if (IsKeyPressed(KEY_N))
    {
        //se retornar 1, carrega novo jogo

        return 1;
    }
    else if (IsKeyPressed(KEY_R))
    {
        //se retornar 2, mostra o ranking
        return 2;
    }
    else if (IsKeyPressed(KEY_S))
    {
        //se retornar 3, sai do jogo
        return 3;
    }
}

void iniciarJogador(JOGADOR *jogador) //inicializacao de todas as variaveis do jogador;
{
    jogador->modoMineracao = 0;
    jogador->pontuacao = 0;
    jogador->vidas = 3;
    jogador->energia = 100;
    jogador->escadas = 20;
    jogador->ultimo_item = "0000000000";
    jogador->nivel = 1;
}

void telaRanking(FILE *ranking) //desenha a tela de ranking
{
    while (!(IsKeyPressed(KEY_BACKSPACE)))
    {
        fseek(ranking, 0, SEEK_SET);
        RANKING leitura_atual;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("RANKING \n"), 50, 50, 30, WHITE);

        fread(&leitura_atual, sizeof(RANKING), 1, ranking);
        DrawText(TextFormat("Jogador: %s\nPontuacao: %05d\n\n", leitura_atual.nome, leitura_atual.pontos), 50, 125, 20, WHITE);

        fread(&leitura_atual, sizeof(RANKING), 1, ranking);
        DrawText(TextFormat("Jogador: %s\nPontuacao: %05d\n\n", leitura_atual.nome, leitura_atual.pontos), 50, 200, 20, WHITE);

        fread(&leitura_atual, sizeof(RANKING), 1, ranking);
        DrawText(TextFormat("Jogador: %s\nPontuacao: %05d\n\n", leitura_atual.nome, leitura_atual.pontos), 50, 290, 20, WHITE);

        fread(&leitura_atual, sizeof(RANKING), 1, ranking);
        DrawText(TextFormat("Jogador: %s\nPontuacao: %05d\n\n", leitura_atual.nome, leitura_atual.pontos), 50, 370, 20, WHITE);

        fread(&leitura_atual, sizeof(RANKING), 1, ranking);
        DrawText(TextFormat("Jogador: %s\nPontuacao: %05d\n\n", leitura_atual.nome, leitura_atual.pontos), 50, 450, 20, WHITE);

        EndDrawing();
    }
}

/*
void pegaNome(JOGADOR *jogador) //pega o nome do atual jogador
{
    char nome[TAMANHO_NOME + 1] = "\0";
    int num_letra = 0;

    Rectangle caixaTexto = {LARGURA / 2.0f - 275, 180, 450, 50};

    int framesCounter = 0;

    while (!(IsKeyPressed(KEY_ENTER)))
    {
        //pega o caracter pressionado
        int tecla = GetCharPressed();

        while (tecla > 0)
        {
            if ((tecla >= 32) && (tecla <= 125) && (num_letra < TAMANHO_NOME))
            {
                nome[num_letra] = (char)tecla;
                nome[num_letra + 1] = '\0'; //adiciona '\0' no fim da string
                num_letra++;
            }

            tecla = GetCharPressed(); //confere o proximo caracter
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            num_letra--;
            if (num_letra < 0)
                num_letra = 0;
            nome[num_letra] = '\0';
        }
        framesCounter++;
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //desenha a tela de peganome

        DrawText("PARABENS! VOCE ENTROU NO RANKING :) INSIRA SEU NOME:", 240, 140, 20, GRAY);

        DrawRectangleRec(caixaTexto, LIGHTGRAY);

        DrawRectangleLines((int)caixaTexto.x, (int)caixaTexto.y, (int)caixaTexto.width, (int)caixaTexto.height, RED);
        DrawText(nome, (int)caixaTexto.x + 5, (int)caixaTexto.y + 8, 40, MAROON);

        DrawText(TextFormat("NOME: %i/%i", num_letra, TAMANHO_NOME), 315, 250, 20, DARKGRAY);
        DrawText(TextFormat("APERTE ENTER PARA CONFIRMAR:"), 315, 300, 20, DARKGRAY);

        if (num_letra < TAMANHO_NOME)
        {
            if (((framesCounter / 20) % 2) == 0)
                DrawText("_", (int)caixaTexto.x + 8 + MeasureText(nome, 40), (int)caixaTexto.y + 12, 40, MAROON);
        }
        else
            DrawText("Aperte BACKSPACE para apagar caracteres...", 230, 300, 20, GRAY);

        EndDrawing();
    }

    //sai do loop quando aperta enter
    jogador->nome = nome;

}

void geraRanking(JOGADOR *jogador, FILE *ranking)
 {

    //criar objeto com struct ranking -> jogadorRank
    //fread(ranking, sizeof(RANKING), 1, jogadorRank);
    //compara jogador->pontuacao com jogadoratual.nome
    int posicao; posicao++;
    if
    //fseek(ranking, 0, SEEK_SET);
    RANKING rank1, rank2, rank3, rank4;
    int i = 0;
    int posicao = 0;
    RANKING jogadorRank;
    RANKING novo;

    //le o ranking do inicio ate achar alguem de quem eh maior, quando acha, armazena a posicao em que deve ficar
    rewind(ranking);
    do
    {
        fread(&jogadorRank, sizeof(RANKING), 1, ranking);
        posicao++;
    } while (jogadorRank.pontos > jogador->pontuacao && !(feof(ranking)));

    rewind(ranking);

    //armazena atuais posicoes do ranking
    //onde armazeo o que ler, tamanho de, quantidade, leio de onde
    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank1.nome);
    rank1.nome = jogadorRank.nome;
    rank1.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank2.nome);
    rank2.nome = jogadorRank.nome;
    rank2.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank3.nome);
    rank3.nome = jogadorRank.nome;
    rank3.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank4.nome);
    rank4.nome = jogadorRank.nome;
    rank4.pontos = jogadorRank.pontos;

    //se a posicao
    if (posicao < 6 && posicao > 0)
    {
        pegaNome(jogador); //passa o endereço do ponteiro

/*
        //strcpy(jogador->nome, novo.nome);
        novo.nome = jogador->nome;
        novo.pontos = jogador->pontuacao;

        if (posicao == 1)
        {
            //o que escrevo, que tamanho, quantidade, onde escrevo
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 2)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 3)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 4)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 5)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
       }
    }*/

     /*while (!(IsKeyPressed(KEY_K)))
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("GERANDO RANKING COM NOME\n"), 50, 50, 30, WHITE);
        EndDrawing();
    }
}*/


void geraRanking(JOGADOR *jogador, FILE *ranking)
 {

    /* criar objeto com struct ranking -> jogadorRank
    fread(ranking, sizeof(RANKING), 1, jogadorRank);
    compara jogador->pontuacao com jogadoratual.nome
    int posicao; posicao++;
    if */
    //fseek(ranking, 0, SEEK_SET);
    RANKING rank1, rank2, rank3, rank4;
    int i = 0;
    int posicao = 0;
    RANKING jogadorRank;
    RANKING novo;

    //le o ranking do inicio ate achar alguem de quem eh maior, quando acha, armazena a posicao em que deve ficar
    rewind(ranking);
    do
    {
        fread(&jogadorRank, sizeof(RANKING), 1, ranking);
        posicao++;
    } while (jogadorRank.pontos > jogador->pontuacao && !(feof(ranking)));

    rewind(ranking);

    //armazena atuais posicoes do ranking
    //onde armazeo o que ler, tamanho de, quantidade, leio de onde
    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank1.nome);
    rank1.nome = jogadorRank.nome;
    rank1.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank2.nome);
    rank2.nome = jogadorRank.nome;
    rank2.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank3.nome);
    rank3.nome = jogadorRank.nome;
    rank3.pontos = jogadorRank.pontos;

    fread(&jogadorRank, sizeof(RANKING), 1, ranking);
    //strcpy(jogadorRank.nome, rank4.nome);
    rank4.nome = jogadorRank.nome;
    rank4.pontos = jogadorRank.pontos;

    //se a posicao
    if (posicao < 6 && posicao > 0)
    {

        char nome[TAMANHO_NOME + 1] = "\0";
        int num_letra = 0;

        Rectangle caixaTexto = {LARGURA / 2.0f - 275, 180, 450, 50};

        int framesCounter = 0;

        while (!(IsKeyPressed(KEY_ENTER)))
        {
            //pega o caracter pressionado
            int tecla = GetCharPressed();

            while (tecla > 0)
            {
                if ((tecla >= 32) && (tecla <= 125) && (num_letra < TAMANHO_NOME))
                {
                    nome[num_letra] = (char)tecla;
                    nome[num_letra + 1] = '\0'; //adiciona '\0' no fim da string
                    num_letra++;
                }

                tecla = GetCharPressed(); //confere o proximo caracter
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                num_letra--;
                if (num_letra < 0)
                    num_letra = 0;
                nome[num_letra] = '\0';
            }
            framesCounter++;
            BeginDrawing();
            ClearBackground(RAYWHITE);

            //desenha a tela de peganome

            DrawText("PARABENS! VOCE ENTROU NO RANKING :) INSIRA SEU NOME:", 240, 140, 20, GRAY);

            DrawRectangleRec(caixaTexto, LIGHTGRAY);

            DrawRectangleLines((int)caixaTexto.x, (int)caixaTexto.y, (int)caixaTexto.width, (int)caixaTexto.height, RED);
            DrawText(nome, (int)caixaTexto.x + 5, (int)caixaTexto.y + 8, 40, MAROON);

            DrawText(TextFormat("NOME: %i/%i", num_letra, TAMANHO_NOME), 315, 250, 20, DARKGRAY);
            DrawText(TextFormat("APERTE ENTER PARA CONFIRMAR:"), 315, 300, 20, DARKGRAY);

            if (num_letra < TAMANHO_NOME)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)caixaTexto.x + 8 + MeasureText(nome, 40), (int)caixaTexto.y + 12, 40, MAROON);
            }
            else
                DrawText("Aperte BACKSPACE para apagar caracteres...", 230, 300, 20, GRAY);

            EndDrawing();
        }

        //sai do loop quando aperta enter
        jogador->nome = nome;

        //strcpy(novo.nome, jogador->nome);
        novo.nome = jogador->nome;
        novo.pontos = jogador->pontuacao;

        /*while (!(IsKeyPressed(KEY_K)))
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("NOME %s\n", novo.nome), 50, 50, 30, WHITE);
            EndDrawing();
        }*/

        rewind(ranking);

        if (posicao == 1)
        {
            //o que escrevo, que tamanho, quantidade, onde escrevo
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 2)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 3)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 4)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
        }
        else if (posicao == 5)
        {
            fwrite(&rank1, sizeof(RANKING), 1, ranking);
            fwrite(&rank2, sizeof(RANKING), 1, ranking);
            fwrite(&rank3, sizeof(RANKING), 1, ranking);
            fwrite(&rank4, sizeof(RANKING), 1, ranking);
            fwrite(&novo, sizeof(RANKING), 1, ranking);
       }
    }
}

 void inicializaRanking(FILE *ranking)
 {
    //inicializa a partir do que ja esta escrito no arquivo, no fim
    fseek(ranking, 0, SEEK_END);
    RANKING temp1, temp2, temp3, temp4, temp5;
    temp1.nome = "Amanda";
    temp1.pontos = 50;
    temp2.nome = "Bruno";
    temp2.pontos = 60;
    temp3.nome = "Caio";
    temp3.pontos = 70;
    temp4.nome = "Daniela";
    temp4.pontos = 80;
    temp5.nome = "Eduarda";
    temp5.pontos = 0;
    //o que escrevo, que tamanho, quantidade, onde escreve
    fwrite(&temp1, sizeof(RANKING), 1, ranking);
    fwrite(&temp2, sizeof(RANKING), 1, ranking);
    fwrite(&temp3, sizeof(RANKING), 1, ranking);
    fwrite(&temp4, sizeof(RANKING), 1, ranking);
    fwrite(&temp5, sizeof(RANKING), 1, ranking);
 }

int main()
{
    // inicializa valores do jogador
    JOGADOR jogador;
    int opcao_menu;

    FILE *ranking = fopen("ranking.bin", "w+b");

    QUADRADO mapa[LINHAS][COLUNAS];
    // inicializa janela
    InitWindow(LARGURA, ALTURA, "TERRAINF");
    // define FPS
    SetTargetFPS(60);

    inicializaRanking(ranking);

    while (!WindowShouldClose())
    {
        //chamada de desenho do menu
        do{
            opcao_menu = desenhaMenu();
        }
        while (opcao_menu<1 || opcao_menu>3);

        if (opcao_menu == 1)
        {
            //chamada da funcao de carregamapa e inicializa jogador
            iniciarJogador(&jogador);
            carregaMapa("nivel1.txt", mapa, &jogador);
            while (!(IsKeyPressed(KEY_ENTER)))
            {
                BeginDrawing();
                desenhaMapa(mapa);
                desenhaJogador(&jogador, mapa);
                barraStatus(&jogador);
                ClearBackground(WHITE);
                EndDrawing();
                move(&jogador, mapa);
                podeEscada(&jogador, mapa);
                modoMineracao(&jogador, mapa);
                estadoJogo(&jogador);

                if (jogador.nivel == 1 && jogador.pontuacao >= PONTUACAO_MAXIMA)
                {
                    iniciarJogador(&jogador);
                    carregaMapa("nivel2.txt", mapa, &jogador);
                    jogador.nivel = 2;
                }
                else if (jogador.nivel == 2 && jogador.pontuacao >= PONTUACAO_MAXIMA * 2)
                {
                    geraRanking(&jogador, ranking);
                }
            }
        }
        else if (opcao_menu == 2)
        {
            telaRanking(ranking);
        }
        else if (opcao_menu == 3)
        {
            return 0;
        }
        //sempre zerar para voltar ao menu
        opcao_menu = 0;

    }

    CloseWindow();

    fclose(ranking);

    return 0;
}
