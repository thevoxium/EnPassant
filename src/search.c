#include "search.h"
#include "model.h"
#include "move.h"

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

BestMove miniMaxSearch(Board *b, int depth, bool isMaxPlayer,
                       Color colorToMove) {
  if (depth == 0) {
    BestMove bestMove;
    bestMove.bestScore = evaluatePosition(b);
    return bestMove;
  }

  BestMove bestMove;
  bestMove.bestScore = isMaxPlayer ? -99999 : 99999;

  PossibleMoves possibleMoves;
  generateAllMoves(b, colorToMove, &possibleMoves);

  for (int i = 0; i < possibleMoves.count; i++) {
    Move move = possibleMoves.moves[i];

    doMove(b, &move);
    BestMove child = miniMaxSearch(b, depth - 1, !isMaxPlayer,
                                   colorToMove == WHITE ? BLACK : WHITE);
    undoMove(b, &move);

    if (isMaxPlayer) {
      if (child.bestScore > bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
    } else {
      if (child.bestScore < bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
    }
  }

  return bestMove;
}
