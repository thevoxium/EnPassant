#include "src/model.h"
#include "src/parser.h"

int main() {
  const char *fen =
      "r3r1k1/pp1n1ppp/3bp3/3p4/3P1B2/2N1P3/PP3PPP/R2Q1RK1 w - - 0 21";

  Board *board = parseFen(fen);
  if (board == NULL) {
    return 1;
  }

  printBoard(board);
  freeBoard(board);
  return 0;
}
