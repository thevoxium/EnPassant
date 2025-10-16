#include "search.h"
#include "model.h"

int evaluatePosition(Board *b) {
  int score = 0;
  for (int i = 0; i < 64; i++) {
    int squareScore = 0;
    switch (b->grid[i].type) {
    case PAWN:
      squareScore = 100;
      break;
    case ROOK:
      squareScore = 500;
      break;
    case BISHOP:
      squareScore = 350;
      break;
    case KNIGHT:
      squareScore = 300;
      break;
    case QUEEN:
      squareScore = 900;
      break;
    case KING:
      squareScore = 1500;
      break;
    default:
      break;
    }
    score += ((b->grid[i].color == WHITE) ? 1 : -1) * squareScore;
  }
  return score;
}
