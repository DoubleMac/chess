#include "computer.h"
#include "move.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Computer::Computer(Colour c, Grid *g, int diff) : clr{c}, g{g}, difficulty{diff}, turn{1}, end{c == Colour::White ? 7 : 0}, moves{0} {}

void Computer::pawnPromotion(int a, int b, int x, int y, PieceType pt) {
	if (y == end) {
		g->pawnPromote(x, y, pt, clr);
	}
}

void Computer::move(int a, int b, int c, int d) {
	//cerr << "BEEP BOOP, CALCULATING OPTIMAL MOVE, ZZZT" << endl;
	moves = g->returnMoveList();
	pair<int, char> i;
	
	if (difficulty == 1) {
		i = bogoChess();
	} else if (difficulty == 2) {
		i = dieDieDIE();
	} else if (difficulty == 3) {
		i = basic();
	} else if (difficulty == 4) {
		i = alphaOne();
	}
	
	int x1 = moves[i.first]->start.first;
	int y1 = moves[i.first]->start.second;
	int x2 = moves[i.first]->dest.first;
	int y2 = moves[i.first]->dest.second;
	
	g->attemptMove(x1, y1, x2, y2, clr);
	
	PieceType pt;
	if (i.second) {
		if (i.second == 'r') {
			pt = PieceType::Rook;
		} else if (i.second == 'k') {
			pt = PieceType::Knight;
		} else if (i.second == 'b') {
			pt = PieceType::Bishop;
		} else if (i.second == 'q') {
			pt = PieceType::Queen;
		}
		pawnPromotion(x1, y1, x2, y2, pt);
	}
	moves.clear();
	//cerr << "BZZT, MOVE COMPLETE. TREMBLE AT MY SUPERIOR COMPUTATIONAL ABILITIES, ZZZT" << endl;
}

pair<int, char> Computer::bogoChess() {
	srand(time(NULL));
	int i = rand() % moves.size();
	
	char p = false;
	if (moves[i]->dest.second == end) {
		int k = rand() % 4;
		if (k == 0) {
			p = 'r';
		} else if (k == 1) {
			p = 'k';
		} else if (k == 2) {
			p = 'b';
		} else if (k == 3) {
			p = 'q';
		}
	}
	return pair<int, char> (i, p);
}

pair<int, char> Computer::dieDieDIE() {
	int k = 0;
	int l = moves.size();
	for (int i = 1; i < l; i++) {
		if (moves[i]->causesCheck) {
			k = i;
		} else if (!moves[k]->causesCheck && moves[i]->capture) {
			k = i;
		}
	}
	
	char p = false;
	if (moves[k]->dest.second == end) {
		p = 'q';
	}
	return pair<int, char> (k, p);
}

pair<int, char> Computer::basic() {
	srand(time(NULL));
	int i = rand() % 5;
	
	if (i == 0) {
		return bogoChess();
	} else {
		return dieDieDIE();
	}
}

pair<int, char> Computer::alphaOne() {
	srand(time(NULL));
	int i = rand() % 2;
	
	if (i == 0) {
		return bogoChess();
	} else {
		return dieDieDIE();
	}
}








