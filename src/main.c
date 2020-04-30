#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "ships.h"
#include "board.h"
#include <time.h>

void game(){
  int player = 1;
  printf("\033[1;36m");
  (void)menu();
  GAME* gameboard = init_board(map_size);
  SHIP newship;
  gameboard->state1 = n_boats;
  gameboard->state2 = n_boats;
  if(rand_flag==1) rand_insert_ships(gameboard,n_boats);
  else user_insert(gameboard,n_boats);
  system("clear");
  while(gameboard->state1 != 0 || gameboard->state2 != 0){
    int x,y;
    switch (player){
      case 1:
        printf("PLAYER 1's Turn\n");
        printf("Select coordinates to attack:\n");
        printf("Positions hit:\n");
        print_secret_board(gameboard->map2,map_size);
        printf("X: ");scanf("%d",&x);
        printf("Y: ");scanf("%d",&y);printf("\n");
        attack(x,y,gameboard->map2,map_size,gameboard->state2);
        player=2;
        break;

      case 2:
        printf("PLAYER 2's Turn\n");
        printf("Select coordinates to attack:\n");
        printf("Positions hit:\n");
        print_secret_board(gameboard->map1,map_size);
        printf("X: ");scanf("%d",&x);
        printf("Y: ");scanf("%d",&y);printf("\n");
        attack(x,y,gameboard->map1,map_size,gameboard->state1);
        player=1;
        break;
      }
    }
    if(gameboard->state1==0) printf("Congratulations Player 2, you won!\n");
    else if (gameboard->state2 == 0) printf("Congratulations Player 1,you won!\n");
    gameboard = erase_game(gameboard);

    char l;
    printf("Play again[y/n]: ");
    scanf("%c",&l);
    if( l=='y' | l == 'Y') game();
}


int main(){
srand(time(0));
game();

system("clear");

//SHIP* boat1 = create_ship(1,7);
//SHIP* boat2 = (SHIP*) malloc(sizeof(SHIP));
//SHIP* boat3 = (SHIP*) malloc(sizeof(SHIP));
//create_ship(boat2,1,2);
//create_ship(boat3,0,2);
//SHIP** boat;

//rand_insert_ships(gameboard,20);
//print_boat(boat2);
//print_boat(boat3);

//insert_ship(12,15,boat1,gameboard->map1,gameboard->size,1);
//insert_ship(9,3,boat2,gameboard->map1,gameboard->size);
//insert_ship(10,13,boat3,gameboard->map1,gameboard->size);
//print_game(1,gameboard);
//printf("\n");
//print_game(2,gameboard);
//printf("\n");

//print_secret_board(gameboard->map1,gameboard->size);
//attack(10,3,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//printf("...\n");
//print_game(1,gameboard);

//attack(10,4,gameboard->map1,gameboard->size);
//print_game(1,gameboard);
//attack(13,21,gameboard->map1);
//print_game(1,gameboard);

printf("\n");

//erase_game(gameboard);

return 0;
}
