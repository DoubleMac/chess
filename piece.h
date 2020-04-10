#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include "observer.h"
#include "subject.h"
#include "info.h"
#include <utility>

class Grid;

class Piece : public Subject<Info> {
	int x, y;
	bool firstMove;
	Colour colour;
public:
	void setPiece(int x, int y, bool firstMove) {
		this->x = x;
		this->y = y;
		this->firstMove = firstMove;
	}
	int getx() {
		return x;
	}
	int gety() {
		return y;
	}
	bool getFirstMove() {
		return firstMove;
	}
	Colour getColour() {
		return colour;
	}
	
	virtual bool getInDanger() = 0;
	virtual void setInDanger(bool inDanger) = 0;
	Piece(int x, int y, bool firstMove, Colour colour) : x{x}, y{y}, firstMove{firstMove}, colour{colour} {}
	Info getInfo() override { return Info{x,y,colour, PieceType::Empty, true}; }
	virtual std::pair<bool,bool> validMove(int x, int y, Grid *gd, Colour whosTurn) = 0;
	virtual void setCoords(int x, int y, bool realMove) = 0;
};

#endif
