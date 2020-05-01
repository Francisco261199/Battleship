***********************************************************************BATTLESHIP***********************************************************************
*                                                                                                                                                      *         
*                                                                     TP2 Grupo D                                                                      *                                                           *
*                                                                                                                                                      *
*                                                               Amadeu Marques (up201804974)                                                           *
*                                                              Francisco Ribeiro (up201805092)                                                         *
*                                                                                                                                                      *
********************************************************************************************************************************************************

Instalação(Linux):
1-Extrair o conteudo do zip para uma pasta.
2-Correr o comando "make" no terminal nessa mesma pasta.
3-Correr o comando "./Battleship".


Desisntalar: 
1-Correr o comando "make clean" na pasta de instalação.

Como jogar:
1-Depois de correr "./Battleship" selecionar a opção "1-Start Game";
2-Ler as Regras e objetivos;
3-Escolher o tamanho do mapa(Tamanho recomendado entre 20x20 e 40x40) e o numero de barcos(o numero máximo de barcos é igual ao tamanho do mapa^2 / 25);
4-Escolher o navio e a orientação com base nos navios apresentados ou escolher a aleatorizaçao dos navios;
5-Inserir as coordenadas onde quer colocar o navio;
6-Depois de inseridos todos os navios, o jogo será iniciado, começando pelo jogador 1:
    -Será dada a oportunindade ao jogador de atacar uma posição dentro do mapa;
    -Caso a posição escolhida não esteja no mapa ou tenha sido previamente atacada pelo mesmo jogador será dada a oportunindade de escolher outra posição;
    -No caso do  jogador acertar na água a palavra "MISS" aparecerá no terminal e será revelado o tabuleiro com as posições atacadas;
    -No caso do jogador acertar num navio a palavra "HIT" aparecerá no terminal e será revelado o tabuleiro com as posições atacadas;
    -Apos a jogada, começa o turno do jogador 2;

7-No caso de um barco ser destruido a mensagem "Ship Destroyed!" apareceá no terminal;
8-O jogo termina quando um dos jogadores não tiver mas barcos;



Divisão de ficheiros:

menus.c -- gestão da interface do jogo.
main.c --  gestão do i/o do jogo e execução do ciclo de jogo.
board.c -- estrutura do tabuleiro, inserção e inserção random,print board e attack.
ships.c -- estruturas de navios, criação de navios e rotações.
makefile -- makefile do jogo, resulta no executavel Battleship.

Estruturas:

Estrutura SHIP : 
Tem 2 inteiros (x e y), um array bidimensional de chars(bitmap) e um inteiro size

As variaveis x e y guardam as coordenadas do centro do navio no mapa depois de inserido.Isto é feito
para converter as coordenadas do mapa CELL para coordenadas no bitmap.

O char bitmap guarda a representação do navio, 0 é utilizado para espaço vazio e 1 para peça do navio.

O size guarda o tamanho do navio, ou seja o número de peças.

Estrutura CELL: 
Tem um apontador para um navio e uma variavel shot que indica se aquela posção foi atingida.

Estrutura GAME:
Tem 2 apontadores duplos para mapas de CELL onde é guardado o tabuleiro de cada jogador.
A variavel size guarda o tamanho do mapa;
A variavel state1 e state2 guarda o estado do jogo do jogador 1 e jogador 2, respetivamente,
o estado do jogo é indicado por quantos navios o jogador tem restantes.


Funções do menus.c:
Funções do main.c:


Funções do board.c:
init_board -- Inicializa os mapas de cada jogador, alocando a memoria necessária e preenchendo os campos shot do SHIP com 0 (não atacado).
              Todos os apontadores para ship do array são inicializados em NULL para segurança.

Verify_Insert -- Verifica se é possivel inserir o navio na posição indicada.

insert_ship -- insere o navio no mapa de jogo, se não for possivel inserir pede novas coordenadas.

generate_number -- gera um numero aleatório entre a e b, é usada apenas na função rand_insert_ships.

rand_insert_ships -- cria e insere um dado numero de navios aleatoriamente no mapa.

attack -- função 











