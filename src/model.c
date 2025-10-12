#include "model.h"

Board *initBoard(void) {
  Board *b = (Board *)malloc(sizeof(Board));
  if (!b) {
    return NULL;
  }

  for (int i = 0; i < 64; ++i) {
    int row = i / 8;
    b->grid[i].type = EMPTY;
    b->grid[i].color = NONE;
    if (row == 1) {
      b->grid[i].type = PAWN;
      b->grid[i].color = BLACK;
    } else if (row == 6) {
      b->grid[i].type = PAWN;
      b->grid[i].color = WHITE;
    } else if (row == 0 || row == 7) {
      b->grid[i].color = (row == 0) ? BLACK : WHITE;
      int col = i % 8;
      if (col == 0 || col == 7)
        b->grid[i].type = ROOK;
      else if (col == 1 || col == 6)
        b->grid[i].type = KNIGHT;
      else if (col == 2 || col == 5)
        b->grid[i].type = BISHOP;
      else if (col == 3)
        b->grid[i].type = QUEEN;
      else if (col == 4)
        b->grid[i].type = KING;
    }
  }

  return b;
}

void freeBoard(Board *b) {
  if (b == NULL) {
    return;
  }
  free(b);
}

char pieceChar(PieceType type, Color color) {
  if (type == EMPTY)
    return '.';
  char c;
  switch (type) {
  case PAWN:
    c = 'P';
    break;
  case KNIGHT:
    c = 'N';
    break;
  case BISHOP:
    c = 'B';
    break;
  case ROOK:
    c = 'R';
    break;
  case QUEEN:
    c = 'Q';
    break;
  case KING:
    c = 'K';
    break;
  default:
    c = '?';
    break;
  }
  return (color == WHITE) ? c : c + 32; // lowercase for black
}

void printBoard(const Board *b) {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      printf("%c ",
             pieceChar(b->grid[i * 8 + j].type, b->grid[i * 8 + j].color));
    }
    printf("\n");
  }
}
