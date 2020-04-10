#include "pawn.h"
#include "grid.h"
#include <cmath>
using namespace std;

Pawn::Pawn(int x, int y, Colour colour): Piece{x, y, true, colour}, inDanger{false} {}

Info Pawn::getInfo() {
	return {getx(), gety(), getColour(), PieceType::Pawn, getFirstMove()};
}

pair<bool,bool> Pawn::checkPawn(int x, int y, Grid *gd, int space1, int space2) {
	if (getFirstMove() && 
		(y == (gety() + space1) || y == (gety() + space2)) && 
		x == getx()) {	//first move, going straight by 1 or 2 squares
		
		Piece *p1 = nullptr;
		Piece *p2 = nullptr;
		Info p1Info;
		Info p2Info;
		
		//check if there are pieces infront of us or not
		if (getColour() == Colour::White) {
			if (gety() == 6) {
				p1 = gd->getPiece(x, gety() + space1);
				p1Info = p1->getInfo();
			} else {
				p1 = gd->getPiece(x, gety() + space1);
				p1Info = p1->getInfo();
				p2 = gd->getPiece(x, gety() + space2);
				p2Info = p2->getInfo();
			}
		}
		else {
			if (gety() == 1) {
				p1 = gd->getPiece(x, gety() + space1);
				p1Info = p1->getInfo();
			} else {
				p1 = gd->getPiece(x, gety() + space1);
				p1Info = p1->getInfo();
				p2 = gd->getPiece(x, gety() + space2);
				p2Info = p2->getInfo();
			}
		}

		if (y == gety() + space1) {	//trying to move one sqaure forward
			return make_pair((p1Info.thePiece == PieceType::Empty), false);
		}
		else {	//trying to move 2 squares forward
			return make_pair((p1Info.thePiece == PieceType::Empty
				&& p2Info.thePiece == PieceType::Empty), false);
		}
	}
	else if (y == gety() + space1 && x == getx()) { //not first move, moving straight
		Piece *p1 = gd->getPiece(x, y);
		Info p1Info = p1->getInfo();

		return make_pair(p1Info.thePiece == PieceType::Empty, false);
	}
	else if ((x == getx() - 1 || x == getx() + 1) && y == gety() + space1) {	//moving diagonal 

		// capture by enPassant 
		Piece *pawn = gd->getPiece(x, gety());
		Info pawnInfo = pawn->getInfo();
		//cout << "pawn we r capturing in danger? " << pawn->getInDanger() << endl;
		if (pawnInfo.thePiece == PieceType::Pawn && pawn->getInDanger() && pawnInfo.colour !=getColour()) {
			return make_pair(true,true);
		}

		// normal capture
		Piece *p1 = gd->getPiece(x, y);
		Info p1Info = p1->getInfo();
		// a piece that we can capture and isnt our colour
		return make_pair((p1Info.thePiece != PieceType::Empty && p1Info.colour != getColour()), 
			(p1Info.thePiece != PieceType::Empty && p1Info.colour != getColour())); 
	}
	else {
		return make_pair(false,false);
	}	
}

pair<bool,bool> Pawn::validMove(int x, int y, Grid *gd, Colour whosTurn) {
	if (whosTurn != getColour()) {	//not your turn
		return make_pair(false, false);
	}
	
	if (x == getx() && y == gety()) {	//destination is the piece's current location
		return make_pair(false, false);
	}

	if (getColour() == Colour::White) {
		return this->checkPawn(x, y, gd, 1, 2);
	}
	else {
		return this->checkPawn(x, y, gd, -1, -2);
	}
}

void Pawn::setCoords(int x, int y, bool realMove){
	if (realMove) {
		// moving by 2 spaces, in danger for en passant
		if (getx() == x && abs(gety() - y) == 2) {
			//cout << "setting piece to inDanger" << endl;
			setInDanger(true);
		}
		setPiece(x, y, false);
	} else {
		setPiece(x, y, getFirstMove());
	}
}
