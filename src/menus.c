#include<unistd.h>
#include<stdlib.h>
#include<ncurses.h>
#include"ships.h"
#include"board.h"


#define START_MENU 0
#define START_GAME 1
#define CREDITS 2
#define LEAVE 3
#define BACK 4
#define START_GAME_END 5
#define RANDOM_INSERTION 6
#define SELECTED_INSERTION 7
#define ERROR 404

int n_boats,map_size,rand_flag;
int n_for_each_boat[6];


void LEARN_TO_PLAY(){
  //LEARN TO PLAY
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  attron(A_NORMAL|A_BOLD|A_ITALIC);
  mvprintw(3,2,"GAME OBJECTIVE:");
  attroff(A_BOLD|A_ITALIC);
  mvprintw(4,1," _______________________________________________________________________________________________________________________________________________________");
  mvprintw(5,1,"|.The objective of Battleship is to try and sink all of the other player's ships before they sink all of your own.                                      |");
  mvprintw(6,1,"|.All of the other player's ships are somewhere on their board. You try and hit them by calling out the coordinates of one of the squares on the board. |");
  mvprintw(7,1,"|.Neither you nor the other player can see the eachother's board so you must try and guess where boats are.                                             |");
  mvprintw(8,1,"|.If a player loses all their ships the game ends.                                                                                                      |");
  mvprintw(9,1,"|_______________________________________________________________________________________________________________________________________________________|");
  attroff(A_BOLD|A_ITALIC);
}

void STARTING_GAME(){
  //Starting a game
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  attron(A_NORMAL|A_BOLD|A_ITALIC);
  mvprintw(3,2,"Starting a game:");
  attroff(A_BOLD|A_ITALIC);
  mvprintw(4,1," _____________________________________________________________________________________________________________________");
  mvprintw(5,1,"|.Each player places a selected number ships somewhere on their board, or it chooses random selection.                |");
  mvprintw(6,1,"|.The ships can only be placed vertically or horizontally. Diagonal placement is not allowed.                         |");
  mvprintw(7,1,"|.No part of a ship may hang off the edge of the board. Ships may not overlap. No ships may be placed on another ship.|");
  mvprintw(8,1,"|.Once the guessing begins, the players may not move the ships.                                                       |");
  mvprintw(9,1,"|.There are 6 different ships: Pickaxe (9), Sigma (7), Carrier (5), Battleship (4), Cruiser (3), Destroyer (2).       |");
  mvprintw(10,1,"|_____________________________________________________________________________________________________________________|");
  attroff(A_BOLD|A_ITALIC);
}

int select_mode(){
  char *Options[]={".Random Insertion", ".Selected Insertion"};
  int highlighted,choice;
  highlighted = choice = 0;
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  mvprintw(1,1,"Select Insertion Mode:");
  while(TRUE){
    //cursor in "Random Insertion"
    if(highlighted == 0){
      (void)attron(A_REVERSE|A_BOLD|A_ITALIC);
      mvprintw(3,1,(char*)Options[0]);
      (void)attroff(A_REVERSE);
      mvprintw(4,1,(char*)Options[1]);
    }
    //cursor in "Selected Insertion"
    else{
      (void)attron(A_REVERSE|A_BOLD|A_ITALIC);
      mvprintw(4,1,(char*)Options[1]);
      (void)attroff(A_REVERSE);
      mvprintw(3,1,(char*)Options[0]);
    }
    //keystroke press(Up or Down)
    choice = getch();
    switch(choice){
      case KEY_UP:  if(--highlighted == -1) highlighted = 1;break;
      case KEY_DOWN: if(++highlighted == 2) highlighted = 0;break;
    }
    //Option selected
    if(choice == 10){
      if(highlighted == 0) return RANDOM_INSERTION;
      else return SELECTED_INSERTION;
    }
  }
  return ERROR;
}

void prints_ships(){
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
    mvprintw(0,18,"Select Ships:");
    mvprintw(2,1,"2-Destroyer");   mvprintw(2,20,"3-Cruiser");       mvprintw(2,40,"4-Battleship");

    mvprintw(4,1,"0 0 0 0 0");     mvprintw(4,21,"0 0 0 0 0");       mvprintw(4,41,"0 0 1 0 0");
    mvprintw(5,1,"0 0 1 0 0");     mvprintw(5,21,"0 0 1 0 0");       mvprintw(5,41,"0 0 1 0 0");
    mvprintw(6,1,"0 0 X 0 0");     mvprintw(6,21,"0 0 X 0 0");       mvprintw(6,41,"0 0 X 0 0");
    mvprintw(7,1,"0 0 0 0 0");     mvprintw(7,21,"0 0 1 0 0");       mvprintw(7,41,"0 0 1 0 0");
    mvprintw(8,1,"0 0 0 0 0");     mvprintw(8,21,"0 0 0 0 0");       mvprintw(8,41,"0 0 0 0 0");


    mvprintw(10,1,"5-Carrier");    mvprintw(10,21,"7-Sigma");        mvprintw(10,41,"9-Pickaxe");

    mvprintw(12,1,"0 0 1 0 0");    mvprintw(12,21,"0 0 1 1 0");      mvprintw(12,41,"0 1 1 1 0");
    mvprintw(13,1,"0 0 1 0 0");    mvprintw(13,21,"0 0 1 0 0");      mvprintw(13,41,"1 0 1 0 1");
    mvprintw(14,1,"0 0 X 0 0");    mvprintw(14,21,"0 0 X 0 0");      mvprintw(14,41,"0 0 X 0 0");
    mvprintw(15,1,"0 0 1 0 0");    mvprintw(15,21,"0 0 1 0 0");      mvprintw(15,41,"0 0 1 0 0");
    mvprintw(16,1,"0 0 1 0 0");    mvprintw(16,21,"0 1 1 0 0");      mvprintw(16,41,"0 0 1 0 0");

    mvprintw(18,1,"X represents the position you choose to insert the boat");
    getch();
}

void get_number_boats(){
  int remaining_boats = n_boats;
  int iterator = 0;
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  mvprintw(20,1,"Now lets select number of boats for each type:");
  echo();
  while(TRUE){
    //missing boat insertions(continue)
    if(remaining_boats > 0){
      switch(iterator){
        case 0 : mvprintw(21,1,"Pickaxe: ");scanw("%d",&n_for_each_boat[iterator]);break;
        case 1 : mvprintw(22,1,"Sigma: ");scanw("%d",&n_for_each_boat[iterator]);break;
        case 2 : mvprintw(23,1,"Carrier: ");scanw("%d",&n_for_each_boat[iterator]);break;
        case 3 : mvprintw(24,1,"Battleship: ");scanw("%d",&n_for_each_boat[iterator]);break;
        case 4 : mvprintw(25,1,"Cruiser: ");scanw("%d",&n_for_each_boat[iterator]);break;
        case 5 : mvprintw(26,1,"Destryoer: ");scanw("%d",&n_for_each_boat[iterator]);break;
        default: clear();mvprintw(28,1,"Error entering numbers");return;
      }
      remaining_boats-=n_for_each_boat[iterator];
      mvprintw(20+iterator,100,"Number of boats still available: %d",remaining_boats);
      iterator++;
    }
    //inserted too many boats or no more boats for remaing types(restart)
    if(remaining_boats < 0 || (remaining_boats == 0 && iterator < 6)){
      clear();
      mvprintw(20,1,"Impossible configuration. Please try again");
      iterator = 0;
      remaining_boats = n_boats;
      prints_ships();
    }
    //insertion of all boats for all types(stop)
    if(remaining_boats == 0 && iterator == 6) break;
  }
}

int Start_menu(){
  int select;
  int highlighted = 8;
  char *Options[]={"Start Game","Credits","Leave"};
  keypad(stdscr,TRUE);
  //Flags
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  //

  mvprintw(0,101,"Welcome to:");
  mvprintw(2,79,"   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  mvprintw(3,79," XX   ___  ____ ___ ___      ____ ____ _  _ _ ___  XX");
  mvprintw(4,79,"XXX   |__] |__|  |   |  |    |___ [__  |__| | |__]  XXX");
  mvprintw(5,79,"XXX   |__] |  |  |   |  |___ |___ ___] |  | | |     XXX");
  mvprintw(6,79," XX                                                XX");
  mvprintw(7,79,"   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");


  while(TRUE){
    for(int i=8;i<11;i++){
      if(i == highlighted)(void)attron(A_REVERSE|A_BOLD|A_ITALIC);
      mvprintw(i+1,100,(char*)Options[i-8]);
      (void)attroff(A_REVERSE);
    }
    select = getch();
    switch(select){
      case KEY_UP: if(--highlighted == 7) highlighted = 10;break;
      case KEY_DOWN: if(++highlighted == 11) highlighted = 8;break;
      default: break;
    }
    if(select == 10) break;
  }
  switch((highlighted-7)){
      case 1: return START_GAME;
      case 2: return CREDITS;
      case 3: return LEAVE;
      default : mvprintw(1,1,"ERROR. No menu found. Internal Error.");break;
  }
  return 0;
}
int Start_game(){
  char *Option[] = {"Back","Next"};
  start_color();
  keypad(stdscr,true);
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  int select,highlighted,menu_iterator;
  highlighted = menu_iterator = 1;

  //explain game
  while(menu_iterator != 3){
    //move between menus
    switch(menu_iterator){
      case 0: return START_MENU;
      case 1: LEARN_TO_PLAY();break;
      case 2: STARTING_GAME();break;
      default : mvprintw(1,1,"Error.");
    }
    while(TRUE){
      //cursor in "Back"
      if(highlighted == 0){
        (void)attron(A_REVERSE|A_BOLD|A_ITALIC);
        mvprintw(1,1,(char*)Option[0]);
        (void)attroff(A_REVERSE);
        mvprintw(1,206,(char*)Option[1]);
      }
      //cursor in "Next"
      else{
        (void)attron(A_REVERSE|A_BOLD|A_ITALIC);
        mvprintw(1,206,(char*)Option[1]);
        (void)attroff(A_REVERSE);
        mvprintw(1,1,(char*)Option[0]);
      }
      //keystroke press(Left or Right)
      select = getch();
      switch(select){
        case KEY_LEFT:  if(--highlighted == -1) highlighted = 1;break;
        case KEY_RIGHT: if(++highlighted == 2) highlighted = 0;break;
      }
      //Option selected(back or next)
      if(select == 10){
        if(highlighted == 1) menu_iterator++;
        else menu_iterator--;
        break;
      }
    }
    clear();
  }

  echo();
  mvprintw(1,1,"So lets start the game.");
  mvprintw(2,1,"Select size of board(between 20 to 40):");
  scanw("%d",&map_size);
  while(map_size<20 || map_size>40){ clear();mvprintw(1,1,"Invalid size. Insert new size:");scanw("%d",&map_size);}
  clear();
  mvprintw(1,1,"Select number of ships:");
  scanw("%d",&n_boats);
  while(n_boats<0 || n_boats>(map_size*map_size)/25){clear();mvprintw(1,1,"Invalid number of boats. Insert new number of ships:");scanw("%d",&n_boats);}
  clear();
  noecho();
  //user selects random insertion
  if( select_mode() == RANDOM_INSERTION){ rand_flag=1; return START_GAME_END;}
  else{
    clear();
    rand_flag = 0;
    prints_ships();
    get_number_boats();
  }
  return START_GAME_END;
}

int Credits(){
  char *Option[] = {"Back"};
  start_color();
  keypad(stdscr,true);
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));

  mvprintw(20,80,"Made By:");
  mvprintw(21,80,"##################################################");
  mvprintw(22,80,"#                Amadeus Marques                 #");
  mvprintw(23,80,"##                     &&                       ##");
  mvprintw(24,80,"#               Francisco Ribeiro                #");
  mvprintw(25,80,"##################################################");
  mvprintw(26,80,"All rights reserved to the developers 2020.");

  while(TRUE){
    (void)attron(A_REVERSE|A_BOLD|A_ITALIC);
    mvprintw(1,1,(char*)Option[0]);
    (void)attroff(A_REVERSE);
    if(getch() == 10) break;
  }
  attroff(COLOR_PAIR(1));
  return 0;
}

int Leave(){
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));
  (void)attron(A_BOLD|A_ITALIC);
  mvprintw(1,1,"Thank you for playing the game. Hope you comeback for more incredible fights.");
  (void)attroff(A_BOLD|A_ITALIC);
  getch();
  return LEAVE;
}

int menu_select(){
  initscr();
  start_color();
  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  (void)attron(COLOR_PAIR(1));

  char state = Start_menu();
  clear();
  for( ; ; ){
    switch (state) {
      case START_MENU: state = Start_menu();break;
      case START_GAME: state = Start_game();break;
      case CREDITS: state = Credits();break;
      case LEAVE: state = Leave();break;
    }
    clear();
    if(state == START_GAME_END){
      (void)attroff(COLOR_PAIR(1));
      keypad(stdscr,false);
      //stop ncurses screen
      endwin();
      return START_GAME_END;
    }
    if(state == LEAVE){
      (void)attroff(COLOR_PAIR(1));
      //stop ncurses screen
      keypad(stdscr,false);
      endwin();
      return LEAVE;
    }
  }
  return ERROR;
}

int main(){
  menu_select();
  system("clear");
  printf("all clear ;)\n");
  getchar();
  return 0;
}
