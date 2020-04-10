#include "human.h"
using namespace std;

Human::Human(Colour colour, Grid *theGrid): colour{colour}, theGrid{theGrid} {}

void Human::move(int startX, int startY, int endX, int endY){
	theGrid->attemptMove(startX, startY, endX, endY, colour);
}

void Human::pawnPromotion(int startX, int startY, int endX, int endY, PieceType pt){
	theGrid->attemptMove(startX, startY, endX, endY, colour);
	theGrid->pawnPromote(endX, endY, pt, colour);
}
