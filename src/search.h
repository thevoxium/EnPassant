#ifndef SEARCH_H
#define SEARCH_H

#include "move.h"

typedef struct BestMove {
  Move bestMove;
  int bestScore;
} BestMove;

int evaluatePosition(Board *b);

BestMove miniMaxSearch(Board *b, int depth, bool isMaxPlayer,
                       Color colorToMove);

BestMove alphaBetaSearch(Board *b, int depth, bool isMaxPlayer,
                         Color colorToMove, int alpha, int beta);

BestMove alphaBetaOrderedSearch(Board *b, int depth, bool isMaxPlayer,
                                Color colorToMove, int alpha, int beta);

#endif
