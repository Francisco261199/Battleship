#include<stdio.h>
#include"templates.h"
#include<stdlib.h>

//Create variables in memory
static SHIP  Pickaxe_data =
{
.size = 9,
.bitmap = { {'0', '1', '1', '1', '0'}, //
            {'1', '0', '1', '0', '1'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}  //
          }
};

static SHIP  Sigma_data =
{
.size = 7,
.bitmap = { {'0', '0', '1', '1', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '1', '1', '0', '0'}  //
          }
};

static SHIP  Carrier_data =
{
.size = 5,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}  //
          }
};

static SHIP  Battleship_data =
{
.size = 4,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

static SHIP  Cruiser_data =
{
.size = 3,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

static SHIP  Destroyer_data =
{
.size = 2,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}, //
            {'0', '0', '0', '0', '0'}  //
          }
};

//assign data to each respective SHIP
 SHIP*  Pickaxe = &Pickaxe_data;
 SHIP*  Sigma = &Sigma_data;
 SHIP*  Carrier = &Carrier_data;
 SHIP*  Battleship = &Battleship_data;
 SHIP*  Cruiser = &Cruiser_data;
 SHIP*  Destroyer = &Destroyer_data;
