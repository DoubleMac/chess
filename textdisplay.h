#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <vector>
#include "observer.h"
#include "info.h"

class Piece;

const int gridSize = 10;

class TextDisplay: public Observer<Info> {
  char pawn = 'p';
  char rook = 'r';
  char knight = 'n';
  char bishop = 'b';
  char king = 'k';
  char queen = 'q';
  char white = ' ';
  char black = '_';
  std::vector<std::vector<char>> theDisplay;
 public:
  TextDisplay();
  void notify(Subject<Info> &whoNotified) override;
  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
