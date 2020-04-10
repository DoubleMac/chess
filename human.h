#ifndef HUMAN_H
#define HUMAN_H
#include "player.h"
#include "grid.h"
#include "info.h"


class Human: public Player{
  Colour colour;
  Grid *theGrid;
  public:
  Human(Colour colour, Grid *theGrid);

  void move(int startX, int startY, int endX, int endY);
  void pawnPromotion(int startX, int startY, int endX, int endY, PieceType pt);
};


#endif
