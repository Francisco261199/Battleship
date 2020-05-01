#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>
#include "ships.h"
#include "board.h"
#include <time.h>
#include "menus.h"

void game(){
  int player = 1;
  //color Cyan
  printf("\033[1;36m");
  if( START_GAME_END != menu()){
    printf("Error!");
    exit(0);
  }
  GAME* gameboard = init_board(map_size);

  gameboard->state1 = n_boats;
  gameboard->state2 = n_boats;

  if(rand_flag==1) rand_insert_ships(gameboard,n_boats);
  else user_insert(gameboard,n_boats);

  system("clear");
  int n;
  while(TRUE){
    n=0;
    int x,y;
    switch (player){
      case 1:
        printf("PLAYER 1's Turn\n");
        printf("Positions hit:\n");
        print_secret_board(gameboard->map2,map_size);
        printf("Select coordinates to attack:\n");
        printf("X: ");scanf("%d",&x);
        printf("Y: ");scanf("%d",&y);
        n = attack(x,y,gameboard->map2,map_size);
        gameboard->state2-=n;
        player=2;
        sleep(1);
        system("clear");
        break;

      case 2:
        printf("PLAYER 2's Turn\n");
        printf("Positions hit:\n");
        print_secret_board(gameboard->map1,map_size);
        printf("Select coordinates to attack:\n");
        printf("X: ");scanf("%d",&x);
        printf("Y: ");scanf("%d",&y);
        n = attack(x,y,gameboard->map1,map_size);
        gameboard->state1-=n;
        player=1;
        sleep(1);
        system("clear");
        break;
      }
      if(gameboard->state1 == 0){
        printf("Congratulations Player 2, you won!\n");
        break;
      }
      else if (gameboard->state2 == 0){
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
