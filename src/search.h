
#ifndef SEARCH_H
#define SEARCH_H

#include "move.h"

typedef struct BestMove {
  Move bestMove;
  int bestScore;
} BestMove;

int pieceSquareTable[6][64];
int pieceValue[6];

int evaluatePosition(Board *b);
int evaluatePawnStructure(Board *b, Color color);
int evaluateKingSafety(Board *b, Color color);
int evaluateMobility(Board *b, Color color);
bool isPassedPawn(Board *b, int index, Color color);
bool isIsolatedPawn(Board *b, int index, Color color);
bool isDoubledPawn(Board *b, int index, Color color);
int countLegalMoves(Board *b, Color color);

BestMove miniMaxSearch(Board *b, int depth, bool isMaxPlayer,
                       Color colorToMove);

#endif
