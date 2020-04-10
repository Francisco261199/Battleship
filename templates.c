#include<stdio.h>
#include"templates.h"
#include<stdlib.h>

//Create variables in memory
static SHIP const Pickaxe_data =
{
.size = 9,
.bitmap = { {'0', '1', '1', '1', '0'}, //
            {'1', '0', '1', '0', '1'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}
          }
};

static SHIP const Carrier_data =
{
.size = 5,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}
          }
};

static SHIP const Battleship_data =
{
.size = 4,
.bitmap = { {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}
          }
};

static SHIP const Submarine_data =
{
.size = 3,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}
          }
};

static SHIP const Cruiser_data =
{
.size = 3,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}
          }
};

static SHIP const Destroyer_data =
{
.size = 2,
.bitmap = { {'0', '0', '0', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '1', '0', '0'}, //
            {'0', '0', '0', '0', '0'}, //
            {'0', '0', '0', '0', '0'}
          }
};

//assign data to each respective SHIP
const SHIP* const Pickaxe = &Pickaxe_data;
const SHIP* const Carrier = &Carrier_data;
const SHIP* const Battleship = &Battleship_data;
const SHIP* const Submarine = &Submarine_data;
const SHIP* const Cruiser = &Cruiser_data;
const SHIP* const Destroyer = &Destroyer_data;
