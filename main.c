#include "src/model.h"
#include "src/move.h"
#include "src/parser.h"
#include "src/search.h"
#include <stdio.h>

int main() {
  // const char *fen = "rnb1kbnr/ppppqppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0
  // 1";
  // const char *fen = "3nq2k/2r1r3/4PR1p/p1p5/P1Bp1Q1P/1P6/6P1/6K1 b - - 4 41";
  const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  Board *board = parseFen(fen);
  if (board == NULL)
    return 1;

  printBoard(board);

  Color current = WHITE;
  int depth = 4;
  for (int turn = 0; turn < 200; turn++) {
    // BestMove best = miniMaxSearch(board, 4, current == WHITE, current);
    BestMove best = alphaBetaOrderedSearch(board, depth, current == WHITE,
                                           current, -99999, 99999);
    // BestMove best =
    //     alphaBetaSearch(board, depth, current == WHITE, current, -99999,
    //     99999);
    printf("Turn %d | %s to move\n", turn + 1,
           current == WHITE ? "White" : "Black");
    printf("Best score: %d\n", best.bestScore);
    printf("Best move: from %d to %d\n", best.bestMove.fromSquare,
           best.bestMove.toSquare);

    doMove(board, &best.bestMove);
    printBoard(board);

    current = current == WHITE ? BLACK : WHITE;
  }

  freeBoard(board);
  return 0;
}
