#include "queen.h"
#include "grid.h"
#include <cmath>
using namespace std;

Queen::Queen(int x, int y, Colour colour): Piece{x, y, true, colour} {};

Info Queen::getInfo(){
	return {getx(), gety(), getColour(), PieceType::Queen, getFirstMove()};
}

void Queen::checkVertical(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid * gd){
	Piece *temp = gd->getPiece(x, i);
	Info info = temp->getInfo();

	if (info.y == y){ //checks destination
		if (info.thePiece == PieceType::Empty) { //move to empty square
			valid = true;
		} else if (info.colour != getColour()) { //if capture
			valid = true;
			capture = true;
		}
	} else {
		if (info.thePiece != PieceType::Empty){ //move blocked on the way
			endloop = true;
		}
	}
}

void Queen::checkHorizontal(int x, int y, int i, bool &valid, bool &capture,  bool &endloop, Grid *gd){
	Piece *temp = gd->getPiece(i, y);
	Info info = temp->getInfo();

	if (info.x == x){ //checks destination
		if (info.thePiece == PieceType::Empty) { //move to empty square
			valid = true;
		} else if (info.colour != getColour()) { //if capture
			valid = true;
			capture = true;
		}
	} else {
		if (info.thePiece != PieceType::Empty){ //move blocked on the way
			endloop = true;
		}
	}
}


void Queen::checkDiagonal(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid *gd) {
	int tmpx = getx() + i;
	int tmpy = gety() + i;

	if (x < getx()) tmpx = getx() - i;
	if (y < gety()) tmpy = gety() - i; 

	Piece *temp = gd->getPiece(tmpx, tmpy);
	Info info = temp->getInfo();

	if (info.x == x && info.y == y){ //checks destination
		if (info.thePiece == PieceType::Empty) { //move to empty square
			valid = true;
		} else if (info.colour != getColour()) { //if capture
			valid = true;
			capture = true;
		}
	} else {
		if (info.thePiece != PieceType::Empty){ //move blocked on the way
			endloop = true;
		}
	}
}

pair<bool,bool> Queen::validMove(int x, int y, Grid *gd, Colour whosTurn){
	bool valid = false;
	bool capture = false;
	bool endloop = false;

	if (whosTurn != getColour()) return make_pair(valid, capture); //invalid move if accessing colour different from turn

	if (x == getx() && y != gety()) {
		if (y > gety()){ // move front 
			for(int i = gety() + 1; i <= y; ++i){
				checkVertical(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;	
			}
		} else { //move back
			for(int i = gety() - 1; i >= y; --i){
				checkVertical(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;
			}
		}

	} else if (y == gety() && x != getx()){
		if (x > getx()){ // move right
			for(int i = getx() + 1; i <= x; ++i){
				checkHorizontal(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;	
			}
		} else { // move left 
			for(int i = getx() - 1; i >= x; --i){
				checkHorizontal(x, y, i, valid, capture, endloop,gd);
				if (endloop) break;
			}
		}

	} else if (abs(x - getx()) == abs(y - gety())){
		if (x > getx()) { // move NE or SE
			for(int i = 1; i + getx() <= x; ++i){
				checkDiagonal(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;
			}
		} else { // move NW or SW
			for(int i = 1; getx() - i >= x; ++i){
				checkDiagonal(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;
			}

		}

	} // else coord is outside of legal move range => not valid and no capture

	return make_pair(valid, capture);

}

void Queen::setCoords(int x, int y, bool realMove){
	setPiece(x, y, false);
}
