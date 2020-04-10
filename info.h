#ifndef __INFO_H__
#define __INFO_H__
#include <cstddef>

enum class Colour { Black, White };
enum class PieceType { Empty, Pawn, Knight, Rook, Bishop, King, Queen };

struct Info {
  int x,y;
  Colour colour;
  PieceType thePiece;
  bool firstMove;
};

#endif
