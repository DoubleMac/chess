#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>
#include <string>
#include <memory>
#include "info.h"
#include "state.h"
#include "piece.h"
#include "emptypiece.h"
#include "pawn.h"
#include "knight.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

template <typename InfoType> class Observer;

struct Move;
class TextDisplay;
class GraphicsDisplay;

class Grid {
        std::vector<std::vector<std::shared_ptr<Piece> > > board;
        std::vector<std::shared_ptr<Move> > smoothMoves;
        std::shared_ptr<Piece> wKing;
        std::shared_ptr<Piece> bKing;
        std::unique_ptr<TextDisplay *> td;
        std::unique_ptr<GraphicsDisplay *> gd;

        void compileMoves(Colour c);
        bool checkCheck(Colour c);
        bool checkCastle(Colour c);
        bool checkEnPassant(Colour c);
        void setObservers(Observer<Info> *ob);
        void attachNew(std::shared_ptr<Piece>);
        void clear();
        //void noMoves(Colour c);
public:
        Grid();
        void init();
        void customSetup();
        void addPiece(PieceType, int, int, Colour);
        void removePiece(int, int);
        bool validPieces();
        void movePiece(int, int, int, int, Colour);
        Piece *getPiece(int, int);
        std::string gameState(Colour);
        std::vector<std::shared_ptr<Move> > returnMoveList();
        friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif
