#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H
#include <vector>
#include "observer.h"
#include "subject.h"
#include "info.h"
#include "window.h"


class GraphicsDisplay: public Observer<Info> {
	char pawn = 'p';
	char rook = 'r';
	char knight = 'n';
	char bishop = 'b';
	char king = 'k';
	char queen = 'q';
	int windowSize = 500;
	int gridSize = 10;
	Xwindow window;

public:
	GraphicsDisplay();
	void notify(Subject<Info> &whoNotified) override;	
};

#endif
