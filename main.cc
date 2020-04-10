#include <iostream>
#include <string>
#include "grid.h"
#include "human.h"
#include "computer.h"
#include <memory>
using namespace std;

class BadCommand{};

void setCoords(bool isHuman, int &startX, int &startY, int &endX, int &endY){
  if (isHuman) {
    string start, end; // start and end coordinates
    cin >> start >> end;

    startX = start[0] - 'a';
    endX = end[0] - 'a';

    if (start[0] < 'a' || start[0] > 'h') { //BAD INPUT EXEPTION
      throw InvalidMove{};
    }

    const char *startYchar = &start[1];
    const char *endYchar = &end[1];

    startY = atoi(startYchar) - 1;
    endY = atoi(endYchar) - 1;

    if (startY >= 8 || endY >= 8) { //BAD INPUT EXCEPTION
      throw InvalidMove{};
    }
  }
}

void checkPromote(int endY, string promotePiece, Colour turn){
  if (!(turn == Colour::White && endY == 7 && (promotePiece == "R" || promotePiece == "N" || promotePiece == "B" || promotePiece == "Q" )) &&
      !(turn == Colour::Black && endY == 0 && (promotePiece == "r" || promotePiece == "n" || promotePiece == "b" || promotePiece == "q" ))){
    throw InvalidMove{}; // INVALID PAWN PROMOTION
  }
}

void setPT(PieceType &pt, string promotePiece){ // Sets PieceType of promotion piece
  if (promotePiece == "R" || promotePiece == "r") {
    pt = PieceType::Rook;
  } else if (promotePiece == "N" || promotePiece == "n" ) {
    pt = PieceType::Knight;
  } else if (promotePiece == "B" || promotePiece == "b") {
    pt = PieceType::Bishop;
  } else if (promotePiece == "Q" || promotePiece == "q" ) {
    pt = PieceType::Queen;
  }
}

void setLVL(int &playerLVL, string level){ // sets CPU Player level
  if (level == "computer[1]") {
    playerLVL = 1;
  } else if (level == "computer[2]") {
    playerLVL = 2;
  } else if (level == "computer[3]") {
    playerLVL = 3;
  } else if (level == "computer[4]") {
    playerLVL = 4;
  } else {
    throw BadCommand{};
  }
}

int main() {
  string cmd;
  Grid g;
  unique_ptr<Player> p1(nullptr);
  unique_ptr<Player> p2(nullptr);
  bool p1Human = false;
  bool p2Human = false;
  int p1LVL = 0;
  int p2LVL = 0;

  Colour turn = Colour::White;
  int whiteScore = 0;
  int blackScore = 0;

  cin.exceptions(ios::failbit|ios::eofbit);
  bool done = false;

  bool inGame = false;

  try{
  while (!done){
     g.init(); //initializes chessboard

    // THE GAME LOOP
    while (true) {
      cin >> cmd;
      if (cmd == "game") {
        try{
          string white, black; //determines human/computer player
          cin >> white >> black;

          // delete old players for new game
          p1.reset();
          p2.reset();

          if (white == "human" && black == "human") {
            p1 = unique_ptr<Player> (new Human{Colour::White, &g});
            p2 = unique_ptr<Player> (new Human{Colour::Black, &g});
            
            p1Human = true;
            p2Human = true;

          } else if (white == "human") {
            p1 = unique_ptr<Player> (new Human{Colour::White, &g});
       
            setLVL(p2LVL, black);
            p2 = unique_ptr<Player> (new Computer{Colour::Black, &g, p2LVL});
     
            p1Human = true;
          } else if (black == "human") {
            setLVL(p1LVL, white);
            p1 = unique_ptr<Player> (new Computer{Colour::White, &g, p1LVL});
                      
            p2 = unique_ptr<Player> (new Human{Colour::Black, &g});          
            p2Human = true;
          } else {
            setLVL(p1LVL, white);
            setLVL(p2LVL, black);
            p1 = unique_ptr<Player> (new Computer{Colour::White, &g, p1LVL});
            p2 = unique_ptr<Player> (new Computer{Colour::Black, &g, p2LVL});
          } 

          cout << g; //outputs chessboard
          inGame = true;
          if (turn == Colour::White) {
            cout << "White's turn" << endl;
          } else {
            cout << "Black's turn" << endl;
          }
          g.compileMoves(turn);
        }
        catch (BadCommand &) {continue;}
      } else if (cmd == "move") {
        if (inGame){       
          int startX = 0;
          int endX = 0;
          int startY = 0;
          int endY = 0;

          if (turn == Colour::White){ // White player's turn
            try{
              setCoords(p1Human, startX, startY, endX, endY);

              if (p1Human){
                  string promotePiece;

                if (cin.peek() != '\n'){
                  cin >> promotePiece;
                  checkPromote(endY, promotePiece, turn);

                  PieceType pt;
                  setPT(pt, promotePiece);
 
                  p1->pawnPromotion(startX, startY, endX, endY, pt);

                } else {
                  p1->move(startX, startY, endX, endY); //regular move
                }
              } else {
                p1->move(startX, startY, endX, endY); //CPU move
              }

              cout << g;
              turn = Colour::Black;
              cout << "Black's Turn" << endl; //the next turn's colour
            }
            catch (InvalidMove &) {continue;}
          }
          else { // Black player's turn
            try{
              setCoords(p2Human, startX, startY, endX, endY);

              if (p2Human){
                  string promotePiece;

                if (cin.peek() != '\n'){
                  cin >> promotePiece;
                  checkPromote(endY, promotePiece, turn);

                  PieceType pt;
                  setPT(pt, promotePiece);

                  p2->pawnPromotion(startX, startY, endX, endY, pt);

                } else {
                  p2->move(startX, startY, endX, endY); //regular move
                }
              } else {
                p2->move(startX, startY, endX, endY); //CPU move
              }

              cout << g;
              turn = Colour::White;
              cout << "White's turn" << endl; //the next turn's colour
            }
            catch (InvalidMove &) {continue;}
          }

          g.compileMoves(turn);
          string currState = g.gameState(turn);

          if (currState != "neutral") {
            if (currState == "check"){
              if (turn == Colour::White){
                cout << "White is in check" << endl;
              } else {
                cout << "Black is in check" << endl;
              }

            } else if (currState == "checkmate"){
              cout << "Checkmate! ";

              if (turn == Colour::White){
                cout << "Black wins!" << endl;
              } else {
                cout << "White wins!" << endl;
              }

              inGame = false;
              turn = Colour::White;
              break; //end game

            } else if (currState == "stalemate"){
              cout << "Stalemate!" << endl;

              inGame = false;
              turn = Colour::White;
              break; //end game
            }
          }
        }

      } else if (cmd == "resign") {
        if (inGame){
          string surrender;

          if (turn == Colour::White) {
            surrender = "Black";
            ++blackScore;
          } else {
            surrender = "White";
            ++whiteScore;
          }

          cout << surrender << " wins!" << endl;

          inGame = false;
          turn = Colour::White;
          break; //end current game
        }
      } else if (cmd == "setup") {
        if (!inGame){
          string op;
          g.customSetup();

          while(cin >> op){
            if (op == "done"){
              //cannot exit setup mode unless pieces are valid
              if (g.validPieces()){
                break;
              } else {
                cout << "Invalid pieces. Setup exit failed." << endl;
              } 
            } else if (op == "="){ //set turn colour
              string newcolour;
              cin >> newcolour;

              if (newcolour == "white"){
                turn = Colour::White;
              } else if (newcolour == "black") {
                turn = Colour::Black;
              }

            } else if (op == "+"){ //add piece
              try{
                string piece, coord;
                cin >> piece  >> coord;
                Colour colour;
                PieceType pt;

                //MUST CHECK IF CAPITAL LETTER FOR PIECE (white) or LOWERCASE LETTER (black)
                if (piece >= "A" && piece <= "Z"){
                  colour = Colour::White;
                } else if  (piece >= "a" && piece <= "z"){
                  colour = Colour::Black;
                } else {// BAD INPUT EXCEPTION
                  throw InvalidMove{};
                }

                //FIND PIECETYPE
                if (piece == "P" || piece == "p"){
                  pt = PieceType::Pawn;

                } else if (piece == "N" || piece == "n"){
                  pt = PieceType::Knight;

                } else if (piece == "B" || piece == "b"){
                  pt = PieceType::Bishop;

                } else if (piece == "R" || piece == "r"){
                  pt = PieceType::Rook;

                } else if (piece == "Q" || piece == "q"){
                  pt = PieceType::Queen;

                } else if (piece == "K" || piece == "k"){
                  pt = PieceType::King;

                } else { // BAD INPUT EXCEPTION
                  throw InvalidMove{};
                }

                const char *y = &coord[1];

                g.addPiece(pt, coord[0] - 'a', atoi(y) - 1 , colour);
                cout << g;
              }
              catch (checkLOL &) {
                cout << "Don't put kings in check." << endl;
                continue;
              } catch (InvalidMove &) {continue;}

            } else if (op == "-"){ //remove piece
              try{
                string coord;
                cin >> coord;
                const char *y = &coord[1];

                g.removePiece(coord[0] - 'a', atoi(y) - 1 );
                cout << g;
              }
              catch (InvalidMove &) {continue;}

            }
          }
        } else { // game is in session so cant setup
          cout << "Game is in session. Can't use Setup" << endl;
        }
      }
    }
  }
  } catch (ios::failure &) {
    cout << "Final Score:" << endl;
    cout << "White: " << whiteScore << endl;
    cout << "Black: " << blackScore << endl;
  }
}
