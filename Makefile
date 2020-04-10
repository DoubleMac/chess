CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla
EXEC = chess
OBJECTS = main.o computer.o emptypiece.o rook.o pawn.o queen.o king.o knight.o bishop.o human.o textdisplay.o graphicsdisplay.o window.o grid.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
        ${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
        rm ${OBJECTS} ${EXEC} ${DEPENDS}

