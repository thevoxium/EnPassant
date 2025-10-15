#include "move.h"
#include "model.h"
#include <stddef.h>

void generateKnightMoves(Board *b, Color colorToMove,
                         PossibleMoves *possibleMoves, int rank, int file) {

  int fromIndex = BOARD_INDEX(rank, file);
  int direction[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                         {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
  for (int i = 0; i < 8; i++) {
    int r = rank + direction[i][0];
    int f = file + direction[i][1];
    if (r >= 0 && r < 8 && f >= 0 && f < 8) {
      int toIndex = BOARD_INDEX(r, f);
      Square target = b->grid[toIndex];
      if (target.type == EMPTY || target.color != colorToMove) {
        Move m = {
            .fromSquare = fromIndex,
            .toSquare = toIndex,
            .pieceMoved = KNIGHT,
            .pieceCaptured = target.type,
            .piecePromoted = EMPTY,
            .moveMade = (target.type == EMPTY) ? QUIET : CAPTURE,
        };
        possibleMoves->moves[possibleMoves->count++] = m;
      }
    }
  }
}

void generateBishopMoves(Board *b, Color colorToMove,
                         PossibleMoves *possibleMoves, int rank, int file) {

  int fromIndex = BOARD_INDEX(rank, file);
  int direction[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

  for (int i = 0; i < 4; i++) {
    int r = rank + direction[i][0];
    int f = file + direction[i][1];

    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
      int toIndex = BOARD_INDEX(r, f);
      Square target = b->grid[toIndex];

      if (target.type == EMPTY) {
        Move m = {
            .fromSquare = fromIndex,
            .toSquare = toIndex,
            .pieceMoved = BISHOP,
            .pieceCaptured = EMPTY,
            .moveMade = QUIET,
        };
        possibleMoves->moves[possibleMoves->count++] = m;
      } else {
        if (target.color != colorToMove) {
          Move m = {
              .fromSquare = fromIndex,
              .toSquare = toIndex,
              .pieceMoved = BISHOP,
              .pieceCaptured = target.type,
              .moveMade = CAPTURE,
          };
          possibleMoves->moves[possibleMoves->count++] = m;
        }
        break;
      }
      r += direction[i][0];
      f += direction[i][1];
    }
  }
}

void generateRookMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file) {

  int fromIndex = BOARD_INDEX(rank, file);
  int direction[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (int i = 0; i < 4; i++) {
    int r = rank + direction[i][0];
    int f = file + direction[i][1];

    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
      int toIndex = BOARD_INDEX(r, f);
      Square target = b->grid[toIndex];

      if (target.type == EMPTY) {
        Move m = {
            .fromSquare = fromIndex,
            .toSquare = toIndex,
            .pieceMoved = ROOK,
            .pieceCaptured = EMPTY,
            .moveMade = QUIET,
        };
        possibleMoves->moves[possibleMoves->count++] = m;
      } else {
        if (target.color != colorToMove) {
          Move m = {
              .fromSquare = fromIndex,
              .toSquare = toIndex,
              .pieceMoved = ROOK,
              .pieceCaptured = target.type,
              .moveMade = CAPTURE,
          };
          possibleMoves->moves[possibleMoves->count++] = m;
        }
        break; // stop scanning after hitting any piece
      }
      r += direction[i][0];
      f += direction[i][1];
    }
  }
}

void generateQueenMoves(Board *b, Color colorToMove,
                        PossibleMoves *possibleMoves, int rank, int file) {
  int fromIndex = BOARD_INDEX(rank, file);
  int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                          {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

  for (int i = 0; i < 8; i++) {
    int r = rank + directions[i][0];
    int f = file + directions[i][1];

    while (r >= 0 && r < 8 && f >= 0 && f < 8) {
      int toIndex = BOARD_INDEX(r, f);
      Square target = b->grid[toIndex];

      if (target.type == EMPTY) {
        Move m = {.fromSquare = fromIndex,
                  .toSquare = toIndex,
                  .pieceMoved = QUEEN,
                  .pieceCaptured = EMPTY,
                  .moveMade = QUIET};
        possibleMoves->moves[possibleMoves->count++] = m;
      } else {
        if (target.color != colorToMove) {
          Move m = {.fromSquare = fromIndex,
                    .toSquare = toIndex,
                    .pieceMoved = QUEEN,
                    .pieceCaptured = target.type,
                    .moveMade = CAPTURE};
          possibleMoves->moves[possibleMoves->count++] = m;
        }
        break;
      }

      r += directions[i][0];
      f += directions[i][1];
    }
  }
}

void generatePawnMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file) {
  int direction = (colorToMove == WHITE) ? -1 : 1;
  int startRank = (colorToMove == WHITE) ? 6 : 1;
  int promotionRank = (colorToMove == WHITE) ? 0 : 7;

  int fromIndex = BOARD_INDEX(rank, file);

  int oneAheadRank = rank + direction;
  if (oneAheadRank >= 0 && oneAheadRank < 8) {
    int oneAheadIndex = BOARD_INDEX(oneAheadRank, file);
    if (b->grid[oneAheadIndex].type == EMPTY) {
      Move m = {
          .fromSquare = fromIndex,
          .toSquare = oneAheadIndex,
          .pieceMoved = PAWN,
          .pieceCaptured = EMPTY,
          .moveMade = QUIET,
      };

      if (oneAheadRank == promotionRank) {
        m.moveMade = PROMOTION_QUEEN;
        m.piecePromoted = QUEEN;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_ROOK;
        m.piecePromoted = ROOK;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_BISHOP;
        m.piecePromoted = BISHOP;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_KNIGHT;
        m.piecePromoted = KNIGHT;
        possibleMoves->moves[possibleMoves->count++] = m;
      } else {
        possibleMoves->moves[possibleMoves->count++] = m;
      }

      if (rank == startRank) {
        int twoAheadRank = rank + 2 * direction;
        int twoAheadIndex = BOARD_INDEX(twoAheadRank, file);
        if (b->grid[twoAheadIndex].type == EMPTY) {
          Move dm = {.fromSquare = fromIndex,
                     .toSquare = twoAheadIndex,
                     .pieceMoved = PAWN,
                     .pieceCaptured = EMPTY,
                     .moveMade = DOUBLE_PAWN_PUSH};
          possibleMoves->moves[possibleMoves->count++] = dm;
        }
      }
    }
  }

  int captureFiles[2] = {file - 1, file + 1};
  for (int i = 0; i < 2; i++) {
    int cf = captureFiles[i];
    if (cf < 0 || cf > 7)
      continue; // off board
    int captureRank = rank + direction;
    if (captureRank < 0 || captureRank > 7)
      continue;

    int targetIndex = BOARD_INDEX(captureRank, cf);
    Square target = b->grid[targetIndex];

    if (target.type != EMPTY && target.color != colorToMove) {
      Move m = {.fromSquare = fromIndex,
                .toSquare = targetIndex,
                .pieceMoved = PAWN,
                .pieceCaptured = target.type,
                .moveMade = CAPTURE};

      if (captureRank == promotionRank) {
        m.moveMade = PROMOTION_QUEEN;
        m.piecePromoted = QUEEN;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_ROOK;
        m.piecePromoted = ROOK;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_BISHOP;
        m.piecePromoted = BISHOP;
        possibleMoves->moves[possibleMoves->count++] = m;

        m.moveMade = PROMOTION_KNIGHT;
        m.piecePromoted = KNIGHT;
        possibleMoves->moves[possibleMoves->count++] = m;
      } else {
        possibleMoves->moves[possibleMoves->count++] = m;
      }
    }
  }
}

void generateKingMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file) {
  int fromIndex = BOARD_INDEX(rank, file);
  int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                          {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

  for (int i = 0; i < 8; i++) {
    int r = rank + directions[i][0];
    int f = file + directions[i][1];

    if (r >= 0 && r < 8 && f >= 0 && f < 8) {
      int toIndex = BOARD_INDEX(r, f);
      Square target = b->grid[toIndex];

      if (target.type == EMPTY || target.color != colorToMove) {
        Move m = {.fromSquare = fromIndex,
                  .toSquare = toIndex,
                  .pieceMoved = KING,
                  .pieceCaptured = target.type,
                  .moveMade = (target.type == EMPTY) ? QUIET : CAPTURE};
        possibleMoves->moves[possibleMoves->count++] = m;
      }
    }
  }
}

void generateAllMoves(Board *b, Color colorToMove,
                      PossibleMoves *possibleMoves) {
  possibleMoves->count = 0;
  for (int i = 0; i < 64; i++) {
    Square s = b->grid[i];
    if (s.color != colorToMove)
      continue;
    int rank = i / 8;
    int file = i % 8;

    switch (s.type) {
    case PAWN:
      generatePawnMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    case ROOK:
      generateRookMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    case KNIGHT:
      generateKnightMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    case QUEEN:
      generateQueenMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    case KING:
      generateKingMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    case BISHOP:
      generateBishopMoves(b, colorToMove, possibleMoves, rank, file);
      break;
    default:
      break;
    }
  }
}
