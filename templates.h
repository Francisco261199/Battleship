typedef struct{
    //center coordinates of the boat in CELL
    int x;
    int y;
    char bitmap[5][5];
    int size;
}SHIP;


extern  SHIP*  Pickaxe;
extern  SHIP*  Carrier;
extern  SHIP*  Battleship;
extern  SHIP*  Sigma;
extern  SHIP*  Cruiser;
extern  SHIP*  Destroyer;
