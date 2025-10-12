#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>

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
} Board;

Board *initBoard(void);
void freeBoard(Board *b);
void printBoard(const Board *b);

#endif
