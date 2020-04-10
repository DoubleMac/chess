#include "graphicsdisplay.h"

using namespace std;


GraphicsDisplay::GraphicsDisplay() {
	//fill entire window
	window.fillRectangle(0, 0, windowSize, windowSize, Xwindow::Yellow);
	int size = windowSize / gridSize;

	char xaxis = 'a';
	int yaxis = 8;
	
	//draw the axis labels
	for (int col = 0; col < 8; ++col) {
		window.drawString(size / 2, col * size + size / 2, to_string(yaxis), Xwindow::Black);
		--yaxis;
	}

	for (int row = 2; row < 10; ++row) {
		window.drawString(row * size + size / 2, 9 * size + size / 2, string(1,xaxis), Xwindow::Black);
		++xaxis;
	}

	//draw the board (starting with white square)
	for(int row = 0; row < 8; ++row) {
		for(int col = 2; col < 10; ++col) {
			if (row % 2 == 0) {
				if (col % 2 == 0) {
					window.fillRectangle(col * size, row * size, size, size, Xwindow::White);
				} 
				else {
					window.fillRectangle(col * size, row * size, size, size, Xwindow::Black);
				}
			}
			else {
				if (col % 2 == 0) {
					window.fillRectangle(col * size, row * size, size, size, Xwindow::Black);
				}
				else {
					window.fillRectangle(col * size, row * size, size, size, Xwindow::White);
				}
			}
		}
	}

	//draw the pieces
	//black pieces
	window.drawString(2 * size + size / 2, size / 2, string(1,rook), Xwindow::Red);
	window.drawString(3 * size + size / 2, size / 2, string(1,knight), Xwindow::Red);
	window.drawString(4 * size + size / 2, size / 2, string(1,bishop), Xwindow::Red);
	window.drawString(5 * size + size / 2, size / 2, string(1,queen), Xwindow::Red);
	window.drawString(6 * size + size / 2, size / 2, string(1,king), Xwindow::Red);
	window.drawString(7 * size + size / 2, size / 2, string(1,bishop), Xwindow::Red);
	window.drawString(8 * size + size / 2, size / 2, string(1,knight), Xwindow::Red);
	window.drawString(9 * size + size / 2, size / 2, string(1,rook), Xwindow::Red);

	for (int row = 2; row < 10; ++row) {
		window.drawString(row * size + size / 2, size + size / 2, "bp", Xwindow::Red);
	}

	//white pieces
	window.drawString(2 * size + size / 2, 7 * size + size / 2, string(1,rook - 'a' + 'A'), Xwindow::Red);
	window.drawString(3 * size + size / 2, 7 * size + size / 2, string(1,knight - 'a' + 'A'), Xwindow::Red);
	window.drawString(4 * size + size / 2, 7 * size + size / 2, string(1,bishop - 'a' + 'A'), Xwindow::Red);
	window.drawString(5 * size + size / 2, 7 * size + size / 2, string(1,queen - 'a' + 'A'), Xwindow::Red);
	window.drawString(6 * size + size / 2, 7 * size + size / 2, string(1,king - 'a' + 'A'), Xwindow::Red);
	window.drawString(7 * size + size / 2, 7 * size + size / 2, string(1,bishop - 'a' + 'A'), Xwindow::Red);
	window.drawString(8 * size + size / 2, 7 * size + size / 2, string(1,knight - 'a' + 'A'), Xwindow::Red);
	window.drawString(9 * size + size / 2, 7 * size + size / 2, string(1,rook - 'a' + 'A'), Xwindow::Red);

	for (int row = 2; row < 10; ++row) {
			window.drawString(row * size + size / 2, 6 * size + size / 2, "WP", Xwindow::Red);
	}
}

void GraphicsDisplay::notify(Subject<Info> &whoNotified) {
	Info info = whoNotified.getInfo();
	int size = windowSize / gridSize;	//size of each square
	int colour = 0;
	
	if (info.x % 2 == 0) {
		if (info.y % 2 == 0) {
			colour = 1;
		}
	} else {
		if (info.y % 2 == 1) {
			colour = 1;
		}
	}	

	if (info.thePiece == PieceType::Empty) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, Xwindow::White);
		}
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, Xwindow::Black);
		}	
	} 
	else if (info.thePiece == PieceType::Pawn) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, "WP", Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, "bp", Xwindow::Red);
		}
	} 
	else if (info.thePiece == PieceType::Knight) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,knight - 'a' + 'A'), Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,knight), Xwindow::Red);
		}
	} 
	else if (info.thePiece == PieceType::Rook) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,rook - 'a' + 'A'), Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,rook), Xwindow::Red);
		}	
	} 
	else if (info.thePiece == PieceType::Bishop) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,bishop - 'a' + 'A'), Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,bishop), Xwindow::Red);
		}	
	} 
	else if (info.thePiece == PieceType::King) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,king - 'a' + 'A'), Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,king), Xwindow::Red);
		}		
	} 
	else if (info.thePiece == PieceType::Queen) {
		if (info.colour == Colour::White) {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,queen - 'a' + 'A'), Xwindow::Red);
		} 
		else {
			window.fillRectangle((info.x + 2) * size, (7 - info.y) * size, size, size, colour);
			window.drawString((info.x + 2) * size + size / 2, (7 - info.y) * size + size / 2, string(1,queen), Xwindow::Red);
		}
	}
}
