#include "king.h"
#include "grid.h"
#include <cmath>
using namespace std;

King::King(int x, int y, Colour colour): Piece{x, y, true, colour} {};

Info King::getInfo(){
        return {getx(), gety(), getColour(), PieceType::King, getFirstMove()};
}

pair<bool,bool> King::validMove(int x, int y, Grid *gd, Colour whosTurn){
        bool valid = false;
        bool capture = false;

        if (whosTurn != getColour()) return make_pair(valid, capture); //invalid move if accessing colour different from turn

        if ((x == getx() && abs(y - gety()) == 1) ||
                (y == gety() && abs(x - getx()) == 1) ||
                (abs(x - getx()) == 1 && abs(y - gety()) == 1)) {

                Piece *temp = gd->getPiece(x, y);
                Info info = temp->getInfo();

                if (info.thePiece == PieceType::Empty){
                        valid = true;
                } else if (info.colour != getColour()) {
                        valid = true;
                        capture = true;
                } // else move blocked

        } else if (getFirstMove() && abs(x - getx()) == 2 && y == gety()) { //castling
                int rookX = 0;
                int spaces = 3;
                if (x > getx()){
                        rookX = 7;
                        spaces = 2;
                }
                Piece *rook = gd->getPiece(rookX, y);
                
                if (rook->getInfo().firstMove) {              
                        for (int i = 1 ; i <= spaces; i++){
                                int tempx = getx() + i;
                                if (x < getx()) {
                                        tempx = getx() - i;
                                }
                               
                                Piece *temp = gd->getPiece(tempx, y);
                                Info info = temp->getInfo();

                                if (tempx == x) {
                                        if (info.thePiece == PieceType::Empty){
                                                valid = true;
                                        } // else , piece in the way (castling cannot capture)
                                } else {
                                        if (info.thePiece != PieceType::Empty) break; //move blocked
                                }
                        }
                }
        }// else coord is outside of legal move range => not valid and no capture

        return make_pair(valid,capture);
}

void King::setCoords(int x, int y, bool realMove){
        if (realMove) {
                setPiece(x, y, false);
        } else {
                setPiece(x, y, getFirstMove());
        }
}
