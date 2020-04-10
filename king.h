#ifndef KING_H
#define KING_H
#include "piece.h"

class King : public Piece {
public:
	bool getInDanger() {return false;}
	void setInDanger(bool inDanger) {return;}
	King(int x, int y, Colour colour);
	Info getInfo() override;
	std::pair<bool,bool> validMove(int x, int y, Grid *gd, Colour whosTurn) override;
	void setCoords(int x, int y, bool realMove) override;
};

#endif
