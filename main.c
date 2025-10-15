
#include "src/model.h"
#include "src/move.h"
#include "src/parser.h"

int main() {
  const char *fen = "3nq2k/2r1r3/4PR1p/p1p5/P1Bp1Q1P/1P6/6P1/6K1 b - - 4 41";

  Board *board = parseFen(fen);
  if (board == NULL)
    return 1;

  printBoard(board);

  PossibleMoves possibleMoves;
  generateAllMoves(board, BLACK, &possibleMoves);

  for (int i = 0; i < possibleMoves.count; i++) {
    Move m = possibleMoves.moves[i];
    printf("%d -> %d | moved: %d | captured: %d | promoted: %d | type: %d\n",
           m.fromSquare, m.toSquare, m.pieceMoved, m.pieceCaptured,
           m.piecePromoted, m.moveMade);
  }

  freeBoard(board);
  return 0;
}
