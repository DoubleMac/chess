#include "emptypiece.h"

using namespace std;

EmptyPiece::EmptyPiece(int x, int y, Colour colour): Piece{x, y, true, colour} {}

Info EmptyPiece::getInfo() {
	return {getx(), gety(), getColour(), PieceType::Empty, getFirstMove()};
}

pair<bool,bool> EmptyPiece::validMove(int x, int y, Grid *gd, Colour whosTurn) {
	return make_pair(false,false);
}

void EmptyPiece::setCoords(int x, int y, bool realMove) {
	setPiece(x, y, false);
}
