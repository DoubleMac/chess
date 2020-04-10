#ifndef BISHOP_H
#define BISHOP_H
#include "piece.h"

class Bishop : public Piece {
public:
	Bishop(int x, int y, Colour colour);
	Info getInfo() override;
	std::pair<bool, bool> validMove(int x, int y, Grid *gd, Colour whosTurn) override;
	void setCoords(int x, int y, bool realMove) override;
	bool getInDanger() override {return false;}
	void setInDanger(bool inDanger) override {}
};

#endif
