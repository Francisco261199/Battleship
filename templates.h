typedef struct{
    //center coordinates of the boat in CELL
    int x;
    int y;
    char bitmap[5][5];
    int size;
}SHIP;


extern const SHIP* const Pickaxe;
extern const SHIP* const Carrier;
extern const SHIP* const Battleship;
extern const SHIP* const Sigma;
extern const SHIP* const Cruiser;
extern const SHIP* const Destroyer;
