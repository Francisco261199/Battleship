#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "templates.h"
 
 
#define EMPTY '0'
#define NOT_HIT '1'
#define HIT '2'
#define MISSED_SHOT '3'
 
#define BOAT 'O'
 
 
typedef struct{
  char state;
  SHIP *ship;
  int shot=0;
} CELL;
 
 
typedef struct{
    CELL **map1;
    CELL **map2;
    int size;
    int state1,state2;
} GAME;
 
 typedef struct{
char bitmap[5][5];
int size;

} SHIP;
/*typedef struct Coordinate{
    int x;
    int y;
} COORD;*/
 
 
GAME * init_board(int size){
    GAME * pGame = (GAME*)malloc(sizeof(GAME));
 
    pGame->size = size;
    pGame->map1 = (CELL**)malloc(size*sizeof(CELL*));
    pGame->map2 = (CELL**)malloc(size*sizeof(CELL*));
 
    for(int i=0;i<size;i++){
        pGame->map1[i]=(CELL*)malloc(size*sizeof(CELL));
        pGame->map2[i]=(CELL*)malloc(size*sizeof(CELL));
 
        for(int j=0;j<size;j++){
          pGame->map1[i][j].state=EMPTY;
          pGame->map2[i][j].state=EMPTY;
        }
      }
  return pGame;
}
 void rotation (SHIP *ship){
    for(int i=0;i<5;i++).{
        for(int j=0;j<5;j++){
            if(ship->bitmap[i][j]=='1'){
                ship->bitmap[i][j]=='0';
                ship-bitmap[j][4-i]=='1';
            }
        }
    }
 }
 
SHIP *create_ship(int x,int y,int orient,int size){
 
 SHIP *newship = (SHIP*)malloc(sizeof(SHIP));
 newship->points= ;
 newship->size = size;
 for(int i=0;i<orient;i++) {
     ship = rotate(ship);
 }
 return newship;
}
 

 
/*void insert_ship(int player,SHIP *ship,BOARD * game){
if (player == 1){
    game->map1[x][y]=ship;
}
else
{
    game->map2[x][y]=ship;
}
}
*/
 
void print_game(int player,BOARD * b){
 
    if(player == 1){
      for(int i=0;i<b->size;i++){
          for(int j=0;j<b->size;j++){
              printf("%c ",b->map1[i][j]);
            }
            printf("\n");
          }
        }
else if(player == 2){
    for(int i=0;i<b->size;i++){
        for(int j=0;j<b->size;j++){
            printf("%c ",b->map2[i][j]);
        }
 
        printf("\n");
    }
}
 
else
 printf("Player does not exist");
 
 
 
}
 
void * erase_game(BOARD * game){
 
for(int i =0;i<game->size;i++){
    cell* map1Pos=game->map1[i];
    cell* map2Pos = game->map2[i];
    free(map1Pos);
    free(map2Pos);
}
 
 
//free(game->map1);
//free(game->map2);
 
free(game);
 
game = NULL;
game->map1 = NULL;
game->map2 = NULL;
 
assert(game == NULL);
assert(game->map2 == NULL);
}
 
 //inicializar um tabuleiro;
///void board_insert(ship ships[],BOARD board,int player)
 
 
 
 
/*char **init_map(){
char **map= (char **)malloc(n*sizesizeof(char*));
for(int i=0;i<size;i++){
    map[i]=(char*)malloc(size*sizeof(char));
for(int j=0;j<size;j++){
    map[i][j]='+';
}
}
S
return map;
    }
*/
 
int main(){
    int n;
 
 
    printf("Selecione o tamanho do mapa\n");
    scanf("%d",&n);
 
    system("clear");
 
BOARD *gameboard=init_board(n);
coord start;
start.x=3;
start.y=4;
SHIP *s =create_ship(start,3);
//insert_ship(1,2,3,'x',gameboard);
//insert_ship(2,3,4,'*',gameboard);
print_game(1,gameboard);
 
printf("\n");
 
print_game(2,gameboard);
 
//erase_game(gameboard);
 
//print_game(1,gameboard);
 
return 0;
 

    }

