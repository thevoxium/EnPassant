#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define BOARD_INDEX(rank, file) ((rank) * 8 + (file))

typedef enum PieceType {
  EMPTY,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING
} PieceType;

typedef enum Color { NONE, WHITE, BLACK } Color;

typedef struct {
  PieceType type;
  Color color;
} Square;

typedef struct Board {
  Square grid[64];
  Color activeColor;
  int halfMoveClock;
  int fullMoveCount;
  bool canWhiteCastleKingside;
  bool canBlackCastleKingside;
  bool canWhiteCastleQueenside;
  bool canBlackCastleQueenside;
} Board;

Board *initBoard(void);
void freeBoard(Board *b);
void printBoard(const Board *b);

#endif
