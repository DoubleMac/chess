#ifndef PAWN_H
#define PAWN_H
#include "piece.h"

class Pawn : public Piece {
	bool inDanger;
	
	std::pair<bool,bool> checkPawn(int x, int y, Grid *gd, int space1, int space2);
public:
	bool getInDanger() override { return inDanger; }
	void setInDanger(bool inDanger) override { this->inDanger = inDanger; }
	Pawn(int x, int y, Colour colour);
	Info getInfo() override;
	std::pair<bool, bool> validMove(int x, int y, Grid *gd, Colour whosTurn) override;
	void setCoords(int x, int y, bool realMove) override;
};

#endif
