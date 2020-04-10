#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>
#include <string>
#include <memory>
#include "info.h"
#include "piece.h"
#include "emptypiece.h"
#include "pawn.h"
#include "knight.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

template <typename InfoType> class Observer;

struct Move;

class InvalidMove{}; // Exception handling class
class checkLOL{}; // eXcePtIon HaNdlInG cLasS

class Grid {
	std::vector<std::vector<std::shared_ptr<Piece> > > board;
	std::vector<std::shared_ptr<Move> > smoothMoves;
	std::shared_ptr<Piece> wKing;
	std::shared_ptr<Piece> bKing;
	std::unique_ptr<TextDisplay> td;
	std::unique_ptr<GraphicsDisplay> gd;
	
	bool checkCheck(Colour c);
	void setObservers(Observer<Info> *ob);
	void attachNew(std::shared_ptr<Piece>);
	int movePiece(std::shared_ptr<Move>);
	void clear();
	//void noMoves(Colour c);
public:
	Grid();
	void init();
	void customSetup();
	void addPiece(PieceType, int, int, Colour);
	void removePiece(int, int);
	void pawnPromote(int, int, PieceType, Colour);
	bool validPieces();
	void attemptMove(int, int, int, int, Colour);
	void compileMoves(Colour c);
	std::pair<int, int> getKing(Colour c);
	Piece *getPiece(int, int);
	TextDisplay *getTD() const;
	std::string gameState(Colour);
	std::vector<std::shared_ptr<Move> > returnMoveList();
};

std::ostream &operator<<(std::ostream &out, const Grid &g);

#endif
