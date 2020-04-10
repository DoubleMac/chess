#include "grid.h"
#include "move.h"


using namespace std;

// Checks if the <c> team is in check
bool Grid::checkCheck(Colour c) {
	int x;
	int y;
	Colour opposite = Colour::White;	/* since we're checking if the other player would be able to capture the current
						player's king, we call validMove with the opposite colour */
	if (c == Colour::White) {	// Set the appropiate team's king's coords and correct turn colour
		if (!wKing) return false;
		x = wKing->getx();
		y = wKing->gety();
		opposite = Colour::Black;
	} else {
		if (!bKing) return false;
		x = bKing->getx();
		y = bKing->gety();
	}
	
	// Call validMove for each enemy piece on the ally king
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			if (board[i][k]->validMove(x, y, this, opposite).first) {
				return true; 	// If one instance of check occurs, then they are in check
			}
		}
	}
	return false;	// If no instances of check occur, not in check
}

// Makes a list of valid moves
void Grid::compileMoves(Colour c) {
	//cerr << "START OF COMPILE MOVES" << endl;
	
	smoothMoves.clear();	// Clears any moves from last turn
	//cerr << "CLEAR WORKS" << endl;
	
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 8; x++) {
					if (((board[i][k] == wKing || board[i][k] == bKing) && (x - k == 2 || x - k == -2))
					   && checkCheck(c)) {
						//cerr << "no castling we in check lmaooooooo" << endl;
						continue;
					}
					
					pair<bool, bool> vm = board[i][k]->validMove(x, y, this, c);
					//cerr << "VALID MOVE HAS RUN FOR PIECE (" << k << ", " << i << ") AT LOCATION ("
					//	 << x << ", " << y << ")" << endl;
					
					if (vm.first) {
						//cerr << "VALID MOVE HAS RUN FOR PIECE (" << k << ", " << i << ") AT LOCATION ("
						// << x << ", " << y << ")" << endl;
						//cerr << "...AND RETURNED TRUE" << endl;
						
						/* make backups for the board and the kings as they will be temporarily updated for
						this move to check if it will cause the player to be in check */
						vector<vector<shared_ptr<Piece> > > backup(board);
						//shared_ptr<Piece> backupWking = wKing;
						//shared_ptr<Piece> backupBking = bKing;
						
						//cout << "made backups" << endl;
						
						/* update the board and kings for this hypothetical move and determine if it will put 
						the player in check */
						shared_ptr<Move> mv(new Move{pair<int, int> (k, i), pair<int, int> (x, y), vm.second,
									    false});
						board[i][k]->setCoords(x, y, false);
						int typeofmove = movePiece(mv);
						
						if (typeofmove == 0) {	// Normal move
						} else if (typeofmove == 1) {	// Castling
							if (mv->start.first - mv->dest.first < 0) {	// To the right
								board[mv->start.second][5]->setCoords(5, mv->start.second, false);
							} else {					// To the left
								board[mv->start.second][3]->setCoords(3, mv->start.second, false);
							}
						}
						
						Colour opp = Colour::White;
						if (c == Colour::White) opp = Colour::Black;
						shared_ptr<Move> vm(new Move{mv->start, mv->dest, mv->capture, checkCheck(opp)});
						
						//cerr << "INITIATING CHECK CHECK" << endl;
						if (!checkCheck(c)) {	// if no check, add it to the valid move list
						//	cerr << "(" << k << ", " << i << ") to (" 
						//		<< x << ", " << y << "): no check, good move" << endl;
							smoothMoves.push_back(vm);
						}
						
						//cout << "checked for check" << endl;
						
						// dispose of unwanted pointers
						board.clear();
						//wKing.reset();
						//bKing.reset();
						
						// restore the board and king data
						board = backup;
						for (int i = 0; i < 8; i++) {
							for (int k = 0; k < 8; k++) {
								board[i][k]->setCoords(k, i, false);
							}
						}
						//wKing = backupWking;
						//bKing = backupBking;
					}
				}
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			if (board[i][k]->getInfo().thePiece == PieceType::Pawn) board[i][k]->setInDanger(false);
		}
	}
	//cerr << "COMPILE MOVES HAS COMPILED THE MOVES" << endl;
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
		bool white = i % 2 == 1;	// determines the colour of the empty square
		for (int k = 0; k < 8; k++) {
			shared_ptr<Piece> dead = board[i][k];	// a coffin for the discarded piece ptr
			
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
	
			// notify displays
			gd->notify(*board[i][k]);
			td->notify(*board[i][k]);
		}
	}
}

// attaches observers when a new piece is added
void Grid::attachNew(shared_ptr<Piece> p) {
	// attach the displays
	p->attach(td.get());
	p->attach(gd.get());
}

// default constructor
Grid::Grid() : board{vector<vector<shared_ptr<Piece> > >(8, vector<shared_ptr<Piece> >(8))}, smoothMoves{0}, 
	wKing{nullptr}, bKing{nullptr}, td{nullptr}, gd{nullptr} {}

void Grid::init() {
	td.reset(new TextDisplay);
	gd.reset(new GraphicsDisplay);
	
	// graveyard for dead piece pointers of a previous game
	vector<shared_ptr<Piece> > graveyard;
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			graveyard.push_back(board[i][k]);
		}
	}
	
	// sets up white pieces
	board[0][0] = shared_ptr<Piece> (new Rook{0, 0, Colour::White});
	board[0][1] = shared_ptr<Piece> (new Knight{1, 0, Colour::White});
	board[0][2] = shared_ptr<Piece> (new Bishop{2, 0, Colour::White});
	board[0][3] = shared_ptr<Piece> (new Queen{3, 0, Colour::White});
	board[0][4] = shared_ptr<Piece> (new King{4, 0, Colour::White});
	board[0][5] = shared_ptr<Piece> (new Bishop{5, 0, Colour::White});
	board[0][6] = shared_ptr<Piece> (new Knight{6, 0, Colour::White});
	board[0][7] = shared_ptr<Piece> (new Rook{7, 0, Colour::White});
	
	//cerr << "WHITE NON PAWNS" << endl;
	
	for (int i = 0; i < 8; i++) {
		board[1][i] = shared_ptr<Piece> (new Pawn{i, 1, Colour::White});
	}
	
	//cerr << "WHITE PAWNS" << endl;
			
	// sets up empty pieces
	for (int i = 2; i < 6; i++) {
		bool white = i % 2 == 1;
		for (int k = 0; k < 8; k++) {
			if (white) {
				board[i][k] = shared_ptr<Piece> (new EmptyPiece{k, i, Colour::White});
				white = false;
			} else {
				board[i][k] = shared_ptr<Piece> (new EmptyPiece{k, i, Colour::Black});
				white = true;
			}
		}
	}
	
	//cerr << "EMPTY PIECES" << endl;
			
	// sets up black pieces
	for (int i = 0; i < 8; i++) {
		board[6][i] = shared_ptr<Piece> (new Pawn{i, 6, Colour::Black});
	}
	
	//cerr << "BLACK PAWNS" << endl;
	
	board[7][0] = shared_ptr<Piece> (new Rook{0, 7, Colour::Black});
	board[7][1] = shared_ptr<Piece> (new Knight{1, 7, Colour::Black});
	board[7][2] = shared_ptr<Piece> (new Bishop{2, 7, Colour::Black});
	board[7][3] = shared_ptr<Piece> (new Queen{3, 7, Colour::Black});
	board[7][4] = shared_ptr<Piece> (new King{4, 7, Colour::Black});
	board[7][5] = shared_ptr<Piece> (new Bishop{5, 7, Colour::Black});
	board[7][6] = shared_ptr<Piece> (new Knight{6, 7, Colour::Black});
	board[7][7] = shared_ptr<Piece> (new Rook{7, 7, Colour::Black});
	
	//cerr << "BLACK NON PAWNS" << endl;
			
	// assign king data
	wKing = board[0][4];
	bKing = board[7][4];
	
	//cerr << "KINGS ASSIGNED" << endl;
	
	// attach the observers
	setObservers(td.get());
	setObservers(gd.get());
	
	//cerr << "DISPLAYS SET" << endl;
	
	//cerr << "ALL OBSERVERS SET" << endl;
}

// uhh yeah, wrapper lmao
void Grid::customSetup() {
	clear();
}

// replaces an empty piece with a non empty piece
void Grid::addPiece(PieceType pt, int x, int y, Colour c) {
	if (x >= 0 && x < 8 && y >= 0 && y < 8) {
		shared_ptr<Piece> dead = board[y][x];	// coffin for old piece pointer
		
		//cerr << "currently have " << wKing << " white king and " << bKing << " black king" << endl;
		//cerr << "the address of the current position, which is (" << x << ", " << y << "), is " << board[y][x] << endl;
		
		// determine type of piece and then sets that position to a new piece of that type
		if (pt == PieceType::Rook) {
			if (board[y][x] == wKing) {
				wKing = nullptr;
			} else if (board[y][x] == bKing) {
				bKing = nullptr;
			}
			board[y][x] = shared_ptr<Piece> (new Rook{x, y, c});
		} else if (pt == PieceType::Bishop) {
			if (board[y][x] == wKing) {
				wKing = nullptr;
			} else if (board[y][x] == bKing) {
				bKing = nullptr;
			}
			board[y][x] = shared_ptr<Piece> (new Bishop{x, y, c});
		} else if (pt == PieceType::Queen) {
			if (board[y][x] == wKing) {
				wKing = nullptr;
			} else if (board[y][x] == bKing) {
				bKing = nullptr;
			}
			board[y][x] = shared_ptr<Piece> (new Queen{x, y, c});
		} else if (pt == PieceType::King) {
			//cerr << "adding a king" << endl;
			if ((c == Colour::Black && !bKing) || (c == Colour::White && !wKing)) {
				//cerr << "king is add?" << endl;
				board[y][x] = shared_ptr<Piece> (new King{x, y, c});
				//cerr << "king was add" << endl;
				// assign data to appropiate king
				if (c == Colour::White) {	
					wKing = board[y][x];
				} else if (c == Colour::Black) {
					bKing = board[y][x];
				}
			} else {
				//cerr << "can't have 2 kings of one colour u bumbo" << endl;
				throw InvalidMove{};
			}
			//cerr << "we assigned dem kangs" << endl;
		} else if (pt == PieceType::Knight) {
			if (board[y][x] == wKing) {
				//cerr << "this is not supposed to run" << endl;
				wKing = nullptr;
			} else if (board[y][x] == bKing) {
				//cerr << "this is not supposed to run" << endl;
				bKing = nullptr;
			}
			board[y][x] = shared_ptr<Piece> (new Knight{x, y, c});
		} else {
			if (y > 0 && y < 7) {
				if (board[y][x] == wKing) {
					//cerr << "this is not supposed to run" << endl;
					wKing = nullptr;
				} else if (board[y][x] == bKing) {
					//cerr << "this is not supposed to run" << endl;
					bKing = nullptr;
				}
				board[y][x] = shared_ptr<Piece> (new Pawn{x, y, c});
			} else {
				//cerr << "bad pawn" << endl;
				throw InvalidMove{};
			}
		}
		
		Colour opp = (c == Colour::White ? Colour::Black : Colour::White);
		//cerr << "ok we about to do dat check check" << endl;
		
		bool checkally = checkCheck(c);
		//cerr << "check for our king passed and was " << checkally << endl;
		bool checkopp = checkCheck(opp);
		//cerr << "check for the enemy king passed and was " << checkopp << endl;
		
		if (checkally || checkopp) {
			//cerr << "yo wtf" << endl;
			shared_ptr<Piece> infantMortality = board[y][x];	// baby-sized coffin for dead piece
			if (board[y][x] == wKing) {
				wKing = nullptr;
			} else if (board[y][x] == bKing) {
				bKing = nullptr;
			}
			board[y][x] = dead;
			throw checkLOL{};
		}
		//cerr << "ok we passed dat check check" << endl;
		
		// attach stuff
		attachNew(board[y][x]);
		
		// notify displays
		gd->notify(*board[y][x]);
		td->notify(*board[y][x]);
		
		//cerr << "currently have " << wKing << " white king and " << bKing << " black king" << endl;
	} else {
		throw InvalidMove{};
	}
	//cerr << "ok so its not something in the scope of the if" << endl;
}


// removes a piece and replaces it with an empty piece
void Grid::removePiece(int x, int y){
	if (x >= 0 && x < 8 && y >= 0 && y < 8) {
		shared_ptr<Piece> dead = board[y][x]; // coffin for discarded piece pointer
		
		//cerr << "currently have " << wKing << " white king and " << bKing << " black king" << endl;
		
		if (board[y][x] == wKing) {
			//cerr << "removing a king" << endl;
			wKing = nullptr;
		} else if (board[y][x] == bKing) {
			//cerr << "removing a king" << endl;
			bKing = nullptr;
		}
		
		// determine colour of the empty square
		Colour c;
		if ((x + y) % 2 == 0) {
			c = Colour::Black;
		} else {
			c = Colour::White;
		}
		
		// make the new piece and attach stuff
		board[y][x] = shared_ptr<Piece> (new EmptyPiece{x, y, c});
		attachNew(board[y][x]);
		
		// notify displays
		gd->notify(*board[y][x]);
		td->notify(*board[y][x]);
		
		
		//cerr << "currently have " << wKing << " white king and " << bKing << " black king" << endl;
	} else {
		throw InvalidMove{};
	}
}

// if both kings are on the board, it valid
bool Grid::validPieces() {
	return wKing && bKing;
	
	//for (int i = 0; i < 8; i++) {
	//	for (int k = 0; k < 8; k++) {
	//		cerr << "address of piece (" << k << ", " << i << "): " << board[i][k] << endl;
	//	}
	//}
}

// moves a piece to a spot on the board
void Grid::attemptMove(int x1, int y1, int x2, int y2, Colour clr) {
	//cerr << "we attempting dat move" << endl;
	shared_ptr<Move> mv = nullptr;		// variable for holding the desired move
	
	// go thru move list and check if one of them matches the desired move
	for (auto m : smoothMoves) {
		if (m->start.first == x1 && m->start.second == y1 && m->dest.first == x2 && m->dest.second == y2) {
			//cerr << "THIS MOVE IS IN THIS LIST" << endl;
			// if it matches, set mv to it and break out of the loop
			mv = m;
			break;
		}
	}
	
	// if not null, move the pieces
	if (mv) {
		//cerr << "LET US MOVE THE PIECE" << endl;
		int typeofmove = movePiece(mv);
		//cerr << "we doing a type " << typeofmove << " move" << endl;
		
		// Update/notify start and end positions
		board[mv->dest.second][mv->dest.first]->setCoords(mv->dest.first, mv->dest.second, true);
		gd->notify(*board[mv->dest.second][mv->dest.first]);
		gd->notify(*board[mv->start.second][mv->start.first]);
		td->notify(*board[mv->dest.second][mv->dest.first]);
		td->notify(*board[mv->start.second][mv->start.first]);
		
		//if (board[mv->dest.second][mv->dest.first]->getInfo().thePiece == PieceType::Pawn) {
			//cerr << "moveAttempt says the this pawn is in danger: " << 
				//board[mv->dest.second][mv->dest.first]->getInDanger() << endl;
		//}
		
		if (typeofmove == 0) {	// Normal move
		} else if (typeofmove == 1) {	// Castling
			//cerr << "notify for castle" << endl;
			if (mv->start.first - mv->dest.first < 0) {	// To the right
				
				//cerr << "right notify" << endl;
				if (board[mv->start.second][5]->getInfo().thePiece == PieceType::Rook) {
					//cerr << "the piece at (5, " << mv->start.second << ") is a rook" << endl;
				}
				
				board[mv->start.second][5]->setCoords(5, mv->start.second, true);
				gd->notify(*board[mv->start.second][7]);
				gd->notify(*board[mv->start.second][5]);
				td->notify(*board[mv->start.second][7]);
				td->notify(*board[mv->start.second][5]);
			} else {					// To the left
				
				//cerr << "left notify" << endl;
				if (board[mv->start.second][3]->getInfo().thePiece == PieceType::Rook) {
					//cerr << "the piece at (3, " << mv->start.second << ") is a rook" << endl;
				}
				
				board[mv->start.second][3]->setCoords(3, mv->start.second, true);
				gd->notify(*board[mv->start.second][0]);
				gd->notify(*board[mv->start.second][3]);
				td->notify(*board[mv->start.second][0]);
				td->notify(*board[mv->start.second][3]);
			}
		} else {	// En Passant
			
			if (mv->start.first - mv->dest.first < 0) {	// To the right
				gd->notify(*board[mv->start.second][mv->start.first + 1]);
				td->notify(*board[mv->start.second][mv->start.first + 1]);
			} else {					// To the left
				gd->notify(*board[mv->start.second][mv->start.first - 1]);
				td->notify(*board[mv->start.second][mv->start.first - 1]);
			}
		}
		//cerr << "NOTIFIED" << endl;
	} else {
		throw InvalidMove{};
	}
}



// moves a piece to a spot on the board
int Grid::movePiece(shared_ptr<Move> mv) {	
	if ((board[mv->start.second][mv->start.first] == wKing || board[mv->start.second][mv->start.first] == bKing) 
	    && (mv->start.first - mv->dest.first == -2 || mv->start.first - mv->dest.first == 2)) {
		//cerr << "lol we castling" << endl;
		
		// move the king
		shared_ptr<Piece> dead = board[mv->dest.second][mv->dest.first];	// coffin for captured piece
		board[mv->dest.second][mv->dest.first] = board[mv->start.second][mv->start.first];	// move piece to destination
		// determine colour of now empty square and put an empty piece of that colour in the old spot
		Colour c;
		if ((mv->start.first + mv->start.second) % 2 == 0) {
			c = Colour::Black;
		} else {
			c = Colour::White;
		}
		board[mv->start.second][mv->start.first] = shared_ptr<Piece> (new EmptyPiece{mv->start.first, mv->start.second, c});
		// attach stuff
		attachNew(board[mv->start.second][mv->start.first]);
		
		// move the rook
		if (mv->start.first - mv->dest.first == -2) {	// right castle
			//cerr << "..to the right" << endl;
			movePiece(shared_ptr<Move> (new Move{pair<int, int> (7, mv->start.second),
							     pair<int, int> (5, mv->start.second), false, false}));
		} else if (mv->start.first - mv->dest.first == 2) {	// left castle
			//cerr << "..to the left" << endl;
			movePiece(shared_ptr<Move> (new Move{pair<int, int> (0, mv->start.second),
							     pair<int, int> (3, mv->start.second), false, false}));
		}
		
		return 1;
	} else if (board[mv->start.second][mv->start.first]->getInfo().thePiece == PieceType::Pawn && mv->capture &&
		  board[mv->dest.second][mv->dest.first]->getInfo().thePiece == PieceType::Empty) {
		//cerr << "lol we en passantng" << endl;
		int offset = mv->dest.first - mv->start.first; 
		//cerr << "the captured piece is at (" << mv->start.first + offset << ", " << mv->start.second << ")" << endl;
		shared_ptr<Piece> dead = board[mv->start.second][mv->start.first + offset];	// coffin for captured piece
		// determine colour of now empty square and put an empty piece of that colour in the old spot
		Colour c;
		if ((mv->start.first + offset + mv->start.second) % 2 == 0) {
			c = Colour::Black;
			//cerr << "the colour for this square is black" << endl;
		} else {
			c = Colour::White;
			//cerr << "the colour for this square is white" << endl;
		}
		board[mv->start.second][mv->start.first + offset] = shared_ptr<Piece> (new EmptyPiece{mv->start.first + offset, 
												      mv->start.second, c});

		// attach stuff
		attachNew(board[mv->start.second][mv->start.first + offset]);
		
		movePiece(shared_ptr<Move> (new Move{pair<int, int> (mv->start.first, mv->start.second), 
						     pair<int, int> (mv->dest.first, mv->dest.second), false, false}));
		return 2;
	} else {
		//cerr << "lol we just moving normally" << endl;
		shared_ptr<Piece> dead = board[mv->dest.second][mv->dest.first];	// coffin for captured piece
		
		board[mv->dest.second][mv->dest.first] = board[mv->start.second][mv->start.first];	// move piece to destination
	
		// determine colour of now empty square and put an empty piece of that colour in the old spot
		Colour c;
		if ((mv->start.first + mv->start.second) % 2 == 0) {
			c = Colour::Black;
		} else {
			c = Colour::White;
		}
		board[mv->start.second][mv->start.first] = shared_ptr<Piece> (new EmptyPiece{mv->start.first, mv->start.second, c});
	
		// attach stuff
		attachNew(board[mv->start.second][mv->start.first]);
		return 0;
	}
}

void Grid::pawnPromote(int x, int y, PieceType pt, Colour c) {
	if (c == Colour::White) {
		//cerr << "The colour is fucking white" << endl;
	} else {
		//cerr << "The colour is fucking black" << endl;
	}
	
	if (board[y][x]->getInfo().thePiece == PieceType::Pawn) {
		// addPiece(pt, x, y, c);
		if (x >= 0 && x < 8 && y >= 0 && y < 8) {
			shared_ptr<Piece> dead = board[y][x];	// coffin for old piece pointer
		
			// determine type of piece and then sets that position to a new piece of that type
			if (pt == PieceType::Rook) {
				board[y][x] = shared_ptr<Piece> (new Rook{x, y, c});
			} else if (pt == PieceType::Bishop) {
				board[y][x] = shared_ptr<Piece> (new Bishop{x, y, c});
			} else if (pt == PieceType::Queen) {
				board[y][x] = shared_ptr<Piece> (new Queen{x, y, c});
			} else if (pt == PieceType::Knight) {
				board[y][x] = shared_ptr<Piece> (new Knight{x, y, c});
			}
			
			// attach stuff
			attachNew(board[y][x]);
			
			// notify displays
			gd->notify(*board[y][x]);
			td->notify(*board[y][x]);
		} else {
			throw InvalidMove{};
		}
	}
}

// returns a string describing the state of the game
string Grid::gameState(Colour c) {
	bool check = checkCheck(c);	// determine if in check
	
	//cerr << "NUMBER OF MOVES: " << smoothMoves.size() << endl;
	// return the appropiate status
	bool twoKingStale = true;
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			PieceType pt = board[i][k]->getInfo().thePiece;
			if (pt != PieceType::Empty && pt != PieceType::King) twoKingStale = false;
		}
	}
	
	if (smoothMoves.size() == 0 && check) {
		return "checkmate";
	} else if (smoothMoves.size() == 0 || twoKingStale) {
		return "stalemate";
	} else if (check) {
		return "check";
	} else {
		return "neutral";
	}
}

// returns the position of one of the kings
pair<int, int> Grid::getKing(Colour c) {
	if (c == Colour::Black) {
		return pair<int, int> (bKing->getx(), bKing->gety());
	} else {
		return pair<int, int> (wKing->getx(), wKing->gety());
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
