#ifndef PLAYER_H
#define PLAYER_H
#include "info.h"

class Player{
  public:
  virtual void move(int startX, int startY, int endX, int endY) = 0;  
  virtual void pawnPromotion(int startX, int startY, int endX, int endY, PieceType pt) = 0;
  virtual ~Player() = default;
};


#endif 
