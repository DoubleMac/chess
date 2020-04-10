#include <iostream>
#include <vector>
#include <string>
#include "observer.h"
#include "subject.h"
#include "info.h"
#include "textdisplay.h"

using namespace std;


TextDisplay::TextDisplay() {
	// Unoccupied squares are denoted by a blank space 
	//   for white squares, and an underscore character for dark squares
	// we need to display the row and column labels
	// alpha for row, numbers for column

	char row = 'a';
	int col = 8;

	for (int i = 0; i < gridSize; ++i) {
		vector<char> temp;
		
		if (col > 0 ) {
			temp.emplace_back(to_string(col)[0]);
			temp.emplace_back(' ');
			--col;
		}
	

		if (i == 0) { //sets black players
			temp.emplace_back(rook);
			temp.emplace_back(knight);
			temp.emplace_back(bishop);
			temp.emplace_back(queen);
			temp.emplace_back(king);
			temp.emplace_back(bishop);
			temp.emplace_back(knight);
			temp.emplace_back(rook);
		} else if ( i == 1) {	//sets black pawns
			for (int j = 2; j < gridSize; ++j){ 
				temp.emplace_back(pawn);
			}
		} else if (i == 6) {
			for (int j = 2; j < gridSize; ++j){  //sets white pawns
				temp.emplace_back(pawn - 'a' + 'A');
			}			
		} else if (i == 7) { //sets white players
			temp.emplace_back(rook - 'a' + 'A');
			temp.emplace_back(knight - 'a' + 'A');
			temp.emplace_back(bishop - 'a' + 'A');
			temp.emplace_back(queen - 'a' + 'A');
			temp.emplace_back(king - 'a' + 'A');
			temp.emplace_back(bishop - 'a' + 'A');
			temp.emplace_back(knight - 'a' + 'A');
			temp.emplace_back(rook - 'a' + 'A');			
		} else if (i == 8) {
			for (int j = 0; j < gridSize; ++j){ 
				temp.emplace_back(' ');
			}
		} else if (i == 9) { //sets alpha row label
			temp.emplace_back(' ');
			temp.emplace_back(' ');
			for (int j = 2; j < gridSize; ++j){ 
				temp.emplace_back(row);
				++row;
			}
		} else if (i % 2 == 0) { //sets blank squares
			for (int j = 2; j < gridSize; ++j){ 
				if (j % 2 == 0){
					temp.emplace_back(white);
				} else {
					temp.emplace_back(black);
				}
			}			
		} else { //sets blank squares
			for (int j = 2; j < gridSize; ++j){ 
				if (j % 2 == 0){
					temp.emplace_back(black);
				} else {
					temp.emplace_back(white);
				}
			}		
		}
		theDisplay.emplace_back(temp);
	}
}

void TextDisplay::notify(Subject<Info> &whoNotified) {
	Info info = whoNotified.getInfo();

	if (info.thePiece == PieceType::Empty) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? black : white;
	} else if (info.thePiece == PieceType::Pawn) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? pawn : pawn - 'a' + 'A' ;
	} else if (info.thePiece == PieceType::Knight) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? knight : knight - 'a' + 'A' ;
	} else if (info.thePiece == PieceType::Rook) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? rook : rook - 'a' + 'A' ;
	} else if (info.thePiece == PieceType::Bishop) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? bishop : bishop - 'a' + 'A' ;
	} else if (info.thePiece == PieceType::King) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? king : king - 'a' + 'A' ;
	} else if (info.thePiece == PieceType::Queen) {
		theDisplay[7 - info.y][info.x + 2] = (info.colour == Colour::Black) ? queen : queen - 'a' + 'A' ;
	}
}

ostream &operator<<(ostream &out, const TextDisplay &td){
	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			out << td.theDisplay[i][j]; 
		}
		out << endl;
	}
	return out;
}
