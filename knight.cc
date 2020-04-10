#include "knight.h"
#include "grid.h"
using namespace std;

Knight::Knight(int x, int y, Colour colour): Piece{x, y, true, colour} {}

Info Knight::getInfo() {
	return {getx(), gety(), getColour(), PieceType::Knight, getFirstMove()};
}


pair<bool,bool> isVaild(Info p1, Info p2) {
	if (p1.thePiece != PieceType::Empty) {	//there is a piece here
		if (p1.colour != p2.colour) {	//piece is opponent piece
			return make_pair(true, true);	//vaild and capture
		}
		else {
			return make_pair(false, false); //not vaild, no capture, bc its our piece
		}	
	}
	else {	//empty piece
		return make_pair(true, false); //vaild, no capture	
	}
}

pair<bool,bool> Knight::validMove(int x, int y, Grid *gd, Colour whosTurn) {
	if (whosTurn != getColour()) {	//not your turn
		return make_pair(false, false);
	}
	
	if (x == getx() + 2 || x == getx() - 2) {
		Piece *p1 = gd->getPiece(x, y);
		Info p1Info = p1->getInfo();
		Info myPieceInfo = this->getInfo();
		
		if (y == gety() + 1 || y == gety() - 1) {
			return isVaild(p1Info, myPieceInfo);
		}
		else {
			return make_pair(false, false);
		}
	}
	else if (x == getx() + 1 || x == getx() - 1) {
		Piece *p1 = gd->getPiece(x, y);
		Info p1Info = p1->getInfo();
		Info myPieceInfo = this->getInfo();

		if (y == gety() + 2 || y == gety() - 2) {
			return isVaild(p1Info, myPieceInfo);
		}
		else {
			return make_pair(false, false);
		}		
	}
	else {
		return make_pair(false, false);
	}
}

void Knight::setCoords(int x, int y, bool realMove){
	if (realMove){
		setPiece(x, y, false);
	} else {
		setPiece(x, y, true);
	}
}
