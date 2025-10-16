
#include "src/model.h"
#include "src/move.h"
#include "src/parser.h"
#include "src/search.h"
#include <stdio.h>

int main() {
  // const char *fen = "rnb1kbnr/ppppqppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0
  // 1";
  const char *fen = "3nq2k/2r1r3/4PR1p/p1p5/P1Bp1Q1P/1P6/6P1/6K1 b - - 4 41";
  // const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0
  // 1";

  Board *board = parseFen(fen);
  if (board == NULL)
    return 1;

  printBoard(board);

  PossibleMoves possibleMoves;
  generateAllMoves(board, WHITE, &possibleMoves);

  printf("Possible Moves at this position: %d\n", possibleMoves.count);
  for (int i = 0; i < possibleMoves.count; i++) {
    Move m = possibleMoves.moves[i];
    printf("%d -> %d | moved: %d | captured: %d | promoted: %d | type: %d\n",
           m.fromSquare, m.toSquare, m.pieceMoved, m.pieceCaptured,
           m.piecePromoted, m.moveMade);
  }

  for (int i = 0; i < possibleMoves.count; i++) {
    if (isMoveLegal(board, &possibleMoves.moves[i], WHITE)) {
      doMove(board, &possibleMoves.moves[i]);
      printBoard(board);
      undoMove(board, &possibleMoves.moves[i]);
    }
  }
  printf("Evaluation Score: %d\n", evaluatePosition(board));
  freeBoard(board);
  return 0;
}
