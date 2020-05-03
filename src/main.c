#include <stdio.h>
#include <stdlib.h>
#include "ships.h"
#include "board.h"
#include <time.h>
#include "menus.h"

void game(){
  int x,y,player,n;
  player = 1;
  //color Cyan
  printf("\033[1;36m");

  if( START_GAME_END != menu()){
    printf("Error!");
    exit(1);
  }
  GAME* gameboard = init_board(map_size);

  gameboard->state1 = n_boats;
  gameboard->state2 = n_boats;

  if(rand_flag==1) rand_insert_ships(gameboard);
  else user_insert(gameboard);

  system("clear");
  printf("Boats inserted succsefully! When you're both ready press ENTER to start!\n");
  getchar();

  while(TRUE){
    n=0;
    sleep(1);
    system("clear");
    printf("\033[1;36m");
    printf("PLAYER %d's Turn\n",player);
    printf("Positions hit:\n");

    //print boats in map
    if(player == 1) print_secret_board(gameboard->map2,map_size);
    else print_secret_board(gameboard->map1,map_size);

    //get coordinates
    printf("Select coordinates to attack:\n");
    printf("X: ");
    x = read_buffer();
    printf("Y: ");
    y = read_buffer();

    //player x attacks selected position
    switch (player){
      case 1:
        n = attack(x,y,gameboard->map2,map_size);
        gameboard->state2-=n;
        player=2;
        break;

      case 2:
        n = attack(x,y,gameboard->map1,map_size);
        gameboard->state1-=n;
        player=1;
        break;
      }

      if(gameboard->state1 == 0){
        printf("Congratulations Player 2, you won!\n");
        break;
      }
      else if(gameboard->state2 == 0){
        printf("Congratulations Player 1,you won!\n");
        break;
      }
    }
    sleep(2);
    gameboard = erase_game(gameboard);
}


int main(){
  srand(time(0));
  while(TRUE) game();
  system("clear");
  return 0;
}
