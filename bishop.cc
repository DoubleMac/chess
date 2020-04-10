#include "bishop.h"
#include "grid.h"
#include <cmath>

using namespace std;

Bishop::Bishop(int x, int y, Colour colour): Piece{x, y, true, colour} {}

Info Bishop::getInfo() {
	return {getx(), gety(), getColour(), PieceType::Bishop, getFirstMove()};
}


bool isDiagonal(int x1, int y1, int x2, int y2) {	//checks if (x2,y2) is in any diagonals from (x1,y1)
	return abs(x1 - x2) == abs(y1 - y2);

}

//Moves in any of the four diagonal directions, any distance, but cannot move past any piece that blocks its path.
pair<bool,bool> Bishop::validMove(int x, int y, Grid *gd, Colour whosTurn) {
	if (whosTurn != getColour()) {	//not your turn
		return make_pair(false, false);
	}
	
	if (x == getx() && y == gety()) {
		return make_pair(false, false);
	}

	if (isDiagonal(x, y, getx(), gety())) {
		bool clearPath = true;

		if (x < getx() && y > gety()) {	//NW
			int i = getx() - 1;
			int j = gety() + 1;
			while (i != x && j !=y) {
				Piece *p = gd->getPiece(i,j);
				Info pInfo = p->getInfo();
				if (pInfo.thePiece != PieceType::Empty) {
					clearPath = false;
					break;
				}
				--i;
				++j;
			}
		}
		else if (x > getx() && y > gety()) {	//NE
			int i = getx() + 1;
			int j = gety() + 1;
			while (i != x && j !=y) {
				Piece *p = gd->getPiece(i,j);
				Info pInfo = p->getInfo();
				if (pInfo.thePiece != PieceType::Empty) {
					clearPath = false;
					break;
				}
				++i;
				++j;
			}			
		}
		else if (x < getx() && y < gety()) {	//SW
			int i = getx() - 1;
			int j = gety() - 1;
			while (i != x && j !=y) {
				Piece *p = gd->getPiece(i,j);
				Info pInfo = p->getInfo();
				if (pInfo.thePiece != PieceType::Empty) {
					clearPath = false;
					break;
				}
				--i;
				--j;
			}
		} 
		else {	//SE
			int i = getx() + 1;
			int j = gety() - 1;
			while (i != x && j !=y) {
				Piece *p = gd->getPiece(i,j);
				Info pInfo = p->getInfo();
				if (pInfo.thePiece != PieceType::Empty) {
					clearPath = false;
					break;
				}
				++i;
				--j;
			}
		}

		if (clearPath) {
			Piece *p = gd->getPiece(x,y);
			Info pInfo = p->getInfo();

			if (pInfo.thePiece != PieceType::Empty && pInfo.colour != getColour()) {
				return make_pair(true, true);
			} 
			else if (pInfo.thePiece == PieceType::Empty) {
				return make_pair(true, false);
			} else {
				return make_pair(false, false);
			}			
		} else {
			return make_pair(false, false);
		}
	}
	else {
		return make_pair(false, false);
	}
}

void Bishop::setCoords(int x, int y, bool realMove) {
	setPiece(x, y, false);
}
