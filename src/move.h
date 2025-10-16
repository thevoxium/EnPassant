#ifndef MOVE_H
#define MOVE_H
#include "model.h"

typedef enum MoveType {
  QUIET,
  CAPTURE,
  DOUBLE_PAWN_PUSH,
  KING_CASTLE,
  QUEEN_CASTLE,
  EN_PASSANT,
  PROMOTION_QUEEN,
  PROMOTION_ROOK,
  PROMOTION_BISHOP,
  PROMOTION_KNIGHT
} MoveType;

typedef struct Move {
  int fromSquare;
  int toSquare;
  PieceType pieceMoved;
  PieceType pieceCaptured;
  PieceType piecePromoted;
  MoveType moveMade;
} Move;

typedef struct PossibleMoves {
  Move moves[256];
  int count;
} PossibleMoves;

void generateAllMoves(Board *b, Color colorToMove,
                      PossibleMoves *possibleMoves);
void generatePawnMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file);
void generateRookMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file);
void generateKingMoves(Board *b, Color colorToMove,
                       PossibleMoves *possibleMoves, int rank, int file);
void generateQueenMoves(Board *b, Color colorToMove,
                        PossibleMoves *possibleMoves, int rank, int file);
void generateBishopMoves(Board *b, Color colorToMove,
                         PossibleMoves *possibleMoves, int rank, int file);
void generateKnightMoves(Board *b, Color colorToMove,
                         PossibleMoves *possibleMoves, int rank, int file);

bool isInCheck(Board *b, Color colorToMove);
void doMove(Board *b, Move *m);

#endif // !MOVE_H
