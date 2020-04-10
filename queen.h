#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"

class Queen : public Piece {
	void checkVertical(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid *gd);
	void checkHorizontal(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid *gd);
	void checkDiagonal(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid *gd);

	public:
	bool getInDanger() {return false;}
	void setInDanger(bool inDanger) {return;}
	Queen(int x, int y, Colour colour);
	Info getInfo() override;
	std::pair<bool,bool> validMove(int x, int y, Grid *gd, Colour whosTurn) override;
	void setCoords(int x, int y, bool realMove) override;
};

#endif
