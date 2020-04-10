#include "grid.h"
#include "move.h"
#include <iostream>

using namespace std;

// Checks if the <c> team is in check
bool Grid::checkCheck(Colour c) {
        int x;
        int y;
        Colour opposite = Colour::White;        /* since we're checking if the other player would be able to capture the current
                                                player's king, we call validMove with the opposite colour */
        if (c == Colour::White) {       // Set the appropiate team's king's coords and correct turn colour
                x = wKing->getx();
                y = wKing->gety();
                opposite = Colour::Black;
        } else {
                x = bKing->getx();
                y = bKing->gety();
        }

        // Call validMove for each enemy piece on the ally king
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        if (board[i][k]->validMove(x, y, this, opposite).first) {
                                return true;    // If one instance of check occurs, then they are in check
                        }
                }
        }
        return false;   // If no instances of check occur, not in check
}

// Checks if a move is a valid castle
bool Grid::checkCastle(Colour c) {
        return false;
}

// Checks if a move is a valid en passant
bool Grid::checkEnPassant(Colour c) {
        return false;
}

// Makes a list of valid moves
void Grid::compileMoves(Colour c) {
        cerr << "START OF COMPILE MOVES" << endl;

        smoothMoves.clear();    // Clears any moves from last turn

        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        for (int y = 0; y < 8; y++) {
                                for (int x = 0; x < 8; x++) {
                                        if (board[i][k]->validMove(y, x, this, c).first) {
                                                /* make backups for the board and the kings as they will be temporarily updated for
                                                this move to check if it will cause the player to be in check */
                                                vector<vector<shared_ptr<Piece> > > backup(board);
                                                shared_ptr<Piece> backupWking = wKing;
                                                shared_ptr<Piece> backupBking = bKing;

                                                /* update the board and kings for this hypothetical move and determine if it will put
                                                the player in check */
                                                movePiece(k, i, x, y, c);
                                                if (!checkCheck(c)) {   // if no check, add it to the valid move list
                                                        Move *mv = new Move{pair<int, int> (k, i), pair<int, int> (x, y),
                                                                board[i][k]->validMove(y, x, this, c).second, false};
                                                        smoothMoves.push_back(shared_ptr<Move> (mv));
                                                }

                                                // dispose of unwanted pointers
                                                board.clear();
                                                wKing.reset();
                                                bKing.reset();

                                                // restore the board and king data
                                                board = backup;
                                                wKing = backupWking;
                                                bKing = backupBking;
                                        }
                                }
                        }
                }
        }
}

// attaches an observer to every piece
void Grid::setObservers(Observer<Info> *ob) {
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        board[i][k]->attach(ob);
                }
        }
}

// Wipes the board and sets all pieces to empty pieces
void Grid::clear() {
        // wipe king data
        wKing = nullptr;
        bKing = nullptr;

        // set all pieces to empty
        for (int i = 0; i < 8; i++) {
                bool white = i % 2 == 1;        // determines the colour of the empty square

                for (int k = 0; k < 8; k++) {
                        shared_ptr<Piece> dead = board[i][k];   // a coffin for the discarded piece ptr

                        // make a new emptyPiece at this location, then switch the colour indicator to the opposite colour
                        if (white) {
                                board[i][k] = shared_ptr<Piece> (new EmptyPiece{k, i, Colour::White});
                                white = false;
                        } else {
                                board[i][k] = shared_ptr<Piece> (new EmptyPiece{k, i, Colour::Black});
                                white = true;
                        }

                        // attach displays to new piece
                        board[i][k]->attach(td.get());
                        board[i][k]->attach(gd.get());
                }
        }

        // attach new pieces to eachother
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        setObservers(board[i][k].get());
                }
        }
}

// attaches observers when a new piece is added
void Grid::attachNew(shared_ptr<Piece> p) {
        // attach the displays
        p->attach(td.get());
        p->attach(gd.get());

        // attach all pieces to new piece and vice versa
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        board[i][k]->attach(p.get());
                        p->attach(board[i][k].get());
                }
        }
}

// default constructor
Grid::Grid() : board{vector<vector<shared_ptr<Piece> > >(8, vector<shared_ptr<Piece> >(8))}, smoothMoves{0},
        wKing{nullptr}, bKing{nullptr}, td{new TextDisplay}, gd{new GraphicsDisplay} {}

void Grid::init() {
        // graveyard for dead piece pointers of a previous game
        vector<shared_ptr<Piece> > graveyard;
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        graveyard.push_back(board[i][k]);
                }
        }

        cout << "graveyard success" << endl;

        for (int i = 0; i < 8; ++i) {
                vector<shared_ptr<Piece> > row;

                if (i == 0) {   // sets up white pieces
                        row.emplace_back(shared_ptr<Piece> (new Rook{0, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Knight{1, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Bishop{2, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Queen{3, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new King{4, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Bishop{5, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Knight{6, 0, Colour::White}));
                        row.emplace_back(shared_ptr<Piece> (new Rook{7, 0, Colour::White}));

                        cout << "row0" << endl;
                }
                else if (i == 1) {      // sets up white pawns
                        for (int j = 0; j < 8; j++) {
                                row.emplace_back(shared_ptr<Piece> (new Pawn{j, 1, Colour::White}));
                        }
                        cout << "row1" << endl;
                }
                else if (i == 6) { // sets up black pawns
                        for (int j = 0; j < 8; j++) {
                                row.emplace_back(shared_ptr<Piece> (new Pawn{j, 6, Colour::Black}));
                        }
                        cout << "row6" << endl;
                }
                else if ( i == 7) {     // sets up black pieces
                        row.emplace_back(shared_ptr<Piece> (new Rook{0, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Knight{1, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Bishop{2, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Queen{3, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new King{4, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Bishop{5, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Knight{6, 7, Colour::Black}));
                        row.emplace_back(shared_ptr<Piece> (new Rook{7, 7, Colour::Black}));

                        cout << "row7" << endl;
                }
                else {  // set up empty pieces
                        bool white = (i % 2 == 1);
                        for (int j = 0; j < 8; j++) {
                                if (white) {
                                        row.emplace_back(shared_ptr<Piece> (new EmptyPiece{j, i, Colour::White}));
                                        white = false;
                                } else {
                                        row.emplace_back(shared_ptr<Piece> (new EmptyPiece{j, i, Colour::Black}));
                                        white = true;
                                }
                        }
                        cout << "row" << i << endl;
                }
                board.emplace_back(row);
        }


        // assign king data
        wKing = board[0][4];
        bKing = board[7][4];

        cout << "kings set" << endl;

        // attach the observers
        setObservers(td.get());
        setObservers(gd.get());
        for (int i = 0; i < 8; i++) {
                for (int k = 0; k < 8; k++) {
                        setObservers(board[i][k].get());
                }
        }
        cout << "observers set" << endl;
}

// uhh yeah, wrapper lmao
void Grid::customSetup() {
        clear();
}

// replaces an empty piece with a non empty piece
void Grid::addPiece(PieceType pt, int x, int y, Colour c) {
        shared_ptr<Piece> dead = board[y][x];   // coffin for old piece pointer

        // determine type of piece and then sets that position to a new piece of that type
        if (pt == PieceType::Rook) {
                board[y][x] = shared_ptr<Piece> (new Rook{x, y, c});
        } else if (pt == PieceType::Bishop) {
                board[y][x] = shared_ptr<Piece> (new Bishop{x, y, c});
        } else if (pt == PieceType::Queen) {
                board[y][x] = shared_ptr<Piece> (new Queen{x, y, c});
        } else if (pt == PieceType::King) {
                board[y][x] = shared_ptr<Piece> (new King{x, y, c});

                // assign data to appropiate king
                if (c == Colour::White) {
                        wKing = board[y][x];
                } else if (c == Colour::Black) {
                        bKing = board[y][x];
                }
        } else if (pt == PieceType::Knight) {
                board[y][x] = shared_ptr<Piece> (new Knight{x, y, c});
        } else {
                board[y][x] = shared_ptr<Piece> (new Pawn{x, y, c});
        }

        // attach stuff
        attachNew(board[y][x]);
}


// removes a piece and replaces it with an empty piece
void Grid::removePiece(int x, int y){
        shared_ptr<Piece> dead = board[y][x]; // coffin for discarded piece pointer

        // determine colour of the empty square
        Colour c;
        if (x + y % 2 == 0) {
                c = Colour::Black;
        } else {
                c = Colour::White;
        }

        // make the new piece and attach stuff
        board[y][x] = shared_ptr<Piece> (new EmptyPiece{x, y, c});
        attachNew(board[y][x]);
}

// if both kings are on the board, it valid
bool Grid::validPieces() {
        return wKing && bKing;
}


// moves a piece to a spot on the board
void Grid::movePiece(int x1, int y1, int x2, int y2, Colour clr) {
        shared_ptr<Move> mv = nullptr;          // variable for holding the desired move

        // go thru move list and check if one of them matches the desired move
        for (auto m : smoothMoves) {
                if (m->start.first == x1 && m->start.second == y1 && m->dest.first == x2 && m->dest.second == y2) {
                        // if it matches, set mv to it and break out of the loop
                        mv = m;
                        break;
                }
        }

        // if not null, move the pieces
        if (mv) {
                board[y1][x1]->notify(*board[y2][x1]);  // notify the pieces observers

                shared_ptr<Piece> dead = board[y2][x2]; // coffin for captured piece

                board[y2][x2] = board[y1][x1];  // move piece to destination

                // determine colour of now empty square and put an empty piece of that colour in the old spot
                Colour c;
                if (x1 + y1 % 2 == 0) {
                        c = Colour::Black;
                } else {
                        c = Colour::White;
                }
                board[y1][x1] = shared_ptr<Piece> (new EmptyPiece{x1, y1, c});

                // attach stuff
                attachNew(board[y1][x1]);
        }
}

// returns a string describing the state of the game
string Grid::gameState(Colour c) {
        bool check = checkCheck(c);     // determine if in check

        // return the appropiate status
        if (board.size() == 0 && check) {
                return "checkmate";
        } else if (board.size() == 0) {
                return "stalemate";
        } else if (check) {
                return "check";
        } else {
                return "neutral";
        }
}

// return a copy of the move list
vector<shared_ptr<Move> > Grid::returnMoveList() {
        return smoothMoves;
}

// return a pointer to the text display
TextDisplay *Grid::getTD() const {
        return td.get();
}

// return a pointer to the piece at the specified coords
Piece *Grid::getPiece(int x, int y) {
        return board[y][x].get();
}

// output operator for grid
ostream &operator<<(ostream &out, const Grid &g){
        // get output op of text display
        out << *(g.getTD());
        return out;
}

