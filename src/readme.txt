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

Instalação QUADTREE:
Depois de extrair o zip, correr o comando "make quadtree" e executar com ./battleship

Desisntalar: 
1-Correr o comando "make clean" na pasta de instalação.

Controlos:
Os menus de jogo são controlados com as setas do teclado.

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

7-No caso de um barco ser destruido a mensagem "Ship Destroyed!" aparecerá no terminal;
8-O jogo termina quando um dos jogadores não tiver mais barcos;



Divisão de ficheiros:

menus.c -- gestão da interface do jogo.
main.c --  gestão do i/o do jogo e execução do ciclo de jogo.
board.c -- estrutura do tabuleiro, inserção e inserção random, print board e attack.
ships.c -- estruturas de navios, criação de navios e rotações.
makefile -- makefile do jogo, resulta no executavel Battleship.

Estruturas:

Estrutura SHIP : 
Tem 2 inteiros (x e y), um array bidimensional de chars(bitmap) e um inteiro size

As variaveis x e y guardam as coordenadas do centro do navio no mapa depois de inserido. Isto é feito
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
LEARN_TO_PLAY -- Imprime informações sobre os objetivos básicos do jogo.

STARTING_GAME -- Imprime as regras de inserção no tabuleiro.

Select_mode -- seleciona o modo de inserção (manual ou aleatorio).

prints_ships -- Imprime uma representação do bitmap dos navios, o x representa o centro do navio.

get_number_boats -- lê input do jogador referente ao numero de barcos e quantos barcos de cada tipo e armazena os no array n_for_each_boat;

start_menu -- menu inicial de jogo , permite ao utilizador navegar para o inicio do jogo, creditos ou sair do jogo.

start_game -- menu de inicio de jogo, recolhe o input referente ao tamanho do mapa,e permite aceder as regras e objetivos de jogo

credits -- informações sobre a criação do jogo.

leave -- sai do jogo.

menu -- permite circular entre os menus do jogo.

Funções do main.c:

game -- função que junta todas as componentes do menus.c e board.c para criar o jogo em si, inicializa o jogo e entra num ciclo que permite aos jogadores atacar alternadamente até que todos os navios de um deles sejam destruidos, quando sai desse ciclo executa a função erase_game;


Funções do board.c:
init_board -- Inicializa os mapas de cada jogador, alocando a memoria necessária e preenchendo os campos shot do SHIP com 0 (não atacado).
              Todos os apontadores para ship do array são inicializados em NULL para segurança.

Verify_Insert -- Verifica se é possivel inserir o navio na posição indicada.

insert_ship -- insere o navio no mapa de jogo, se não for possivel inserir pede novas coordenadas.

user_insert -- função utilizada para criar todos os navios, aplicar rotações e inseri-los no mapa.

generate_number -- gera um numero aleatório entre a e b, é usada apenas na função rand_insert_ships.

rand_insert_ships -- cria e insere um dado numero de navios aleatoriamente no mapa.

attack -- função principal do jogo, recebe coordenadas para atacar, verifica se existe algum navio nessa posição e altera o mapa dada a existencia ou não de um navio na posição, modifica também o estado do jogo permitindo que este termine quando não existem barcos.

print_game -- imprime o tabuleiro de jogo no estado atual com todos os barcos revelados.

print_secret_board -- imprime o tabuleiro apenas com água e posições já atingidas, é usada na função attack para dar uma referência visual ao jogador.

erase_game -- limpa o mapa de jogo da memória e altera todos os apontadores para null, é apenas usada após terminar o ciclo de jogo.


funções do Ships.c: 
No Ships.c criamos as variaveis com a informação dos navios e atribuimos a cada navio um apontador.

rotation_90 -- rotação de 90 graus.
rotation_180 -- rotação de 180 graus.
rotation_270 -- rotação de 270 graus.

Todas as rotações são feitas por camadas em vez de por posições individuais por ser mais eficiente.

create_ship -- recebendo uma estrutura ship "vazia" atribui-lhe um dos navios existente com base no tamanho e aplica as rotações se necessário.

print_boat -- imprime o bitmap do navio.

read_buffer -- lê input de forma a evitar erros relacionados com tipos diferentes.
                    Ex: é pedido um inteiro e o jogador escreve uma string.

----------------------------------------------------------------QUADTREE----------------------------------------------------------------------


ESTRUTURAS:

POINT : guarda duas coordenadas, x e y, que representam um ponto.
QD_TNODE : ENUM para distinguir os tipos de nó (leaf(QDLEAF) ou interno(QDNODE)).
QD_Node_ : nó da árvore, contém o tipo de nó(type), quantos nós estão na arvore(nodes_inside), os floats cx e cy guardam as coordenadas do centro do quadrante
a que este nó pertence e level a profundidade do nó na árvore(decrescente). Contém também um apontador para o nó pai e contém um ponto e apontador para navio
se o nó for do tipo QDLEAF ou apontadores para os 4 nós filhos (NW,NE,SW,SE) no caso do nó ser do tipo QDNODE
GAME: guarda as raízes das árvores do jogador 1 e 2;

info1 = matriz que guarda informações dos pontos(com e sem navio) para o mapa do jogador 1.
info2 = matriz que guarda informações dos pontos(com e sem navio) para o mapa do jogador 2.

Funções da QUADTREE
make_point : guarda um valor x e y numa estrutura point.
print_tree : imprime os endereços da árvore, usada para debug.
get_parent : devolve o nó pai
get_last_child : devolve o primeiro filho de um nó(no contexto em que é aplicada devolve o único filho).
create_node : cria um nó do tipo QD_LEAF e inicializa todos os argumentos necessários.
node_clean : liberta os apontadores e informação que contida.
expand_tree : cria uma subdivisão do mapa expandindo o nó atual para 4 filhos e faz os cálculos dos centros para cada nó.
get_next : devolve o nó seguinte na árvore durante a procura(usada apenas na inserção de nós).
get_subdivision : Encontra a posição em que as coordenadas repetidas deverão ser inseridas.
divide_insert : expande árvore sempre que ao verificar a posição de dois nós obtem a mesma.
node_insert : insere um nó na árvore.
node_delete : remove um nó da árvore.
search_point : procura um ponto na árvore.
wallhack : equivalente à print_game da versão com matrizes.


Funções da Boardq:
No geral estas funções funcionam da mesma maneira que as funções do board.c mas existem algumas alteraçoes:

init_board: cria dois nós para servirem de raiz para a arvore do jogador 1 e 2. tambem inicializa dois arrays unidimensionais (info1 e info2)
com tamanho igual a size*size e valores a 0.

verify_insert: recebendo o ponto do meio est função agora cria um array com todos os pontos a inserir na árvore.

insert_ship = insere os nós com os pontos e apontadores para navios na árvore e atualiza o array info do jogador respectivo.

attack: funciona da mesma forma que a da board.c mas a verificação para ataques repetidos é feita em O(1) com info[x*size + y].







