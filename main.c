#include "src/model.h"
#include "src/parser.h"

int main() {
  const char *fen = "r2qkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Board *board = parseFen(fen);
  if (board == NULL) {
    return 1;
  }

  printBoard(board);
  freeBoard(board);
  return 0;
}
