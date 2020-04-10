#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include "grid.h"
#include "info.h"
#include <memory>
#include <vector>
#include <utility>

struct Move;

class Computer: public Player{
	Colour clr;
	Grid *g;
	int difficulty;
	int turn;
	int end;
	std::vector<std::shared_ptr<Move> > moves;
	
	std::pair<int, char> bogoChess();
	std::pair<int, char> dieDieDIE();
	std::pair<int, char> basic();
	std::pair<int, char> alphaOne();
  public:
	Computer(Colour, Grid *, int);
	void move(int, int, int, int) override;
	void pawnPromotion(int, int, int, int, PieceType) override;
};


#endif
