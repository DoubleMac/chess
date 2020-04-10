#include "rook.h"
#include "grid.h"
using namespace std;


Rook::Rook(int x, int y, Colour colour): Piece{x, y, true, colour} {};

Info Rook::getInfo(){
	return {getx(), gety(), getColour(), PieceType::Rook, getFirstMove()};
}

void Rook::checkVertical(int x, int y, int i, bool &valid, bool &capture, bool &endloop, Grid *gd){
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

void Rook::checkHorizontal(int x, int y, int i, bool &valid, bool &capture,  bool &endloop, Grid *gd){
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


pair<bool,bool> Rook::validMove(int x, int y, Grid *gd, Colour whosTurn){
	bool valid = false;
	bool capture = false;
	bool endloop = false;

	if (whosTurn != getColour()) return pair<bool,bool>(valid, capture); //invalid move if accessing colour different from turn


	if (x == getx() && y != gety()){ 
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
				checkHorizontal(x, y, i, valid, capture, endloop, gd);
				if (endloop) break;
			}
		}
	}// else coord is outside of legal move range => not valid and no capture

	return make_pair(valid, capture);
	
}

void Rook::setCoords(int x, int y, bool realMove){
	if (realMove){
		setPiece(x, y, false);
	} else {
		setPiece(x, y, getFirstMove());
	}
}

