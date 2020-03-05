#include <stdio.h>	
#include <stdlib.h>
#include <assert.h>
#include "map.h"


typedef struct{
    char **map1;
    char **map2;
    int size;
    int state1,state2;
} BOARD;

typedef struct Coordinate{
    int x;
    int y;
} coord;

typedef struct{
coord *points;
int size;

} SHIP;



BOARD * init_board(int size){
    BOARD * pGame = (BOARD*)malloc(sizeof(BOARD));

    pGame->size = size;
    pGame->map1 = (char**)malloc(size*sizeof(char*));
    pGame->map2 = (char**)malloc(size*sizeof(char*));

    for(int i=0;i<size;i++){
        pGame->map1[i]=(char*)malloc(size*sizeof(char));
        pGame->map2[i]=(char*)malloc(size*sizeof(char));
    
     for(int j=0;j<size;j++){
        
        pGame->map1[i][j]='+';
        pGame->map2[i][j]='+';
    }
}



return pGame;
}
SHIP * create_ship(coord * start,int orient,int size,BOARD * game){
 SHIP * newship = (SHIP*)malloc 


}
/*void insert_ship(int player,SHIP ship,BOARD * game){

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
    char* map1Pos=game->map1[i];
    char* map2Pos = game->map2[i];
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

//insert_ship(1,2,3,'x',gameboard);
//insert_ship(2,3,4,'*',gameboard);
print_game(1,gameboard);

printf("\n");

print_game(2,gameboard);

//erase_game(gameboard);

//print_game(1,gameboard);

return 0;

}