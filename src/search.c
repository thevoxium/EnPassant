#include "search.h"
#include "model.h"
#include "move.h"

int pieceSquareTable[6][64] = {
    // PAWN
    {0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
     10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
     0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
     5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0},
    // KNIGHT
    {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
     0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
     15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
     -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
     5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50},
    // BISHOP
    {-20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
     0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
     10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
     -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
     0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20},
    // ROOK
    {0,  0,  0,  5,  5,  0,  0,  0,  -5, 0, 0, 0, 0, 0, 0, -5,
     -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
     -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
     5,  10, 10, 10, 10, 10, 10, 5,  0,  0, 0, 0, 0, 0, 0, 0},
    // QUEEN
    {-20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,  0,
     0,   0,   -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5, 0,
     5,   5,   5,   5,   0,   -5,  0,   0,   5,   5,   5,   5,  0,
     -5,  -10, 5,   5,   5,   5,   5,   0,   -10, -10, 0,   5,  0,
     0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20},
    // KING (middle game)
    {20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,
     0,   20,  20,  -10, -20, -20, -20, -20, -20, -20, -10, -20, -30,
     -30, -40, -40, -30, -30, -20, -30, -40, -40, -50, -50, -40, -40,
     -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50,
     -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30}};

int pieceValue[6] = {100, 320, 330, 500, 900, 20000};

int evaluatePawnStructure(Board *b, Color color);
int evaluateKingSafety(Board *b, Color color);
int evaluateMobility(Board *b, Color color);
int countLegalMoves(Board *b, Color color);
bool isPassedPawn(Board *b, int index, Color color);
bool isIsolatedPawn(Board *b, int index, Color color);
bool isDoubledPawn(Board *b, int index, Color color);

int evaluatePosition(Board *b) {
  int score = 0;
  int whiteMaterial = 0, blackMaterial = 0;
  int whiteBishopCount = 0, blackBishopCount = 0;

  for (int i = 0; i < 64; i++) {
    Square sq = b->grid[i];
    if (sq.type == EMPTY)
      continue;

    int typeIndex = sq.type - 1;
    int pieceScore = pieceValue[typeIndex];
    int pst = pieceSquareTable[typeIndex][(sq.color == WHITE) ? i : 63 - i];

    if (sq.color == WHITE) {
      whiteMaterial += pieceScore;
      if (sq.type == BISHOP)
        whiteBishopCount++;
      score += pieceScore + pst;
    } else {
      blackMaterial += pieceScore;
      if (sq.type == BISHOP)
        blackBishopCount++;
      score -= pieceScore + pst;
    }
  }

  if (whiteBishopCount >= 2)
    score += 50;
  if (blackBishopCount >= 2)
    score -= 50;

  score += evaluatePawnStructure(b, WHITE);
  score -= evaluatePawnStructure(b, BLACK);

  score += evaluateKingSafety(b, WHITE);
  score -= evaluateKingSafety(b, BLACK);

  score += evaluateMobility(b, WHITE);
  score -= evaluateMobility(b, BLACK);

  return score;
}

int evaluatePawnStructure(Board *b, Color color) {
  int score = 0;
  for (int i = 0; i < 64; i++) {
    Square sq = b->grid[i];
    if (sq.type != PAWN || sq.color != color)
      continue;

    if (isIsolatedPawn(b, i, color))
      score -= 15;
    if (isDoubledPawn(b, i, color))
      score -= 20;
    if (isPassedPawn(b, i, color))
      score += 40;
  }
  return score;
}

bool isIsolatedPawn(Board *b, int idx, Color color) {
  int file = idx % 8;
  for (int i = 0; i < 64; i++) {
    if (b->grid[i].type == PAWN && b->grid[i].color == color) {
      int f2 = i % 8;
      if (abs(f2 - file) == 1)
        return false;
    }
  }
  return true;
}

bool isDoubledPawn(Board *b, int idx, Color color) {
  int file = idx % 8;
  for (int i = 0; i < 64; i++) {
    if (i == idx)
      continue;
    if (b->grid[i].type == PAWN && b->grid[i].color == color && i % 8 == file)
      return true;
  }
  return false;
}

bool isPassedPawn(Board *b, int idx, Color color) {
  int rank = idx / 8, file = idx % 8;
  int dir = (color == WHITE) ? 1 : -1;
  for (int r = rank + dir; (color == WHITE) ? r < 8 : r >= 0; r += dir) {
    for (int f = file - 1; f <= file + 1; f++) {
      if (f < 0 || f > 7)
        continue;
      int i2 = r * 8 + f;
      if (b->grid[i2].type == PAWN && b->grid[i2].color != color)
        return false;
    }
  }
  return true;
}

int evaluateKingSafety(Board *b, Color color) {
  int score = 0;
  int kingIndex = -1;
  for (int i = 0; i < 64; i++)
    if (b->grid[i].type == KING && b->grid[i].color == color)
      kingIndex = i;

  int rank = kingIndex / 8, file = kingIndex % 8;
  if (color == WHITE) {
    if (rank <= 1)
      score += 30; // castled king
  } else {
    if (rank >= 6)
      score += 30;
  }

  int pawnShield = 0;
  int startRank = rank + ((color == WHITE) ? 1 : -1);
  for (int f = file - 1; f <= file + 1; f++) {
    int i2 = startRank * 8 + f;
    if (f >= 0 && f < 8 && i2 >= 0 && i2 < 64 && b->grid[i2].type == PAWN &&
        b->grid[i2].color == color)
      pawnShield++;
  }
  score += pawnShield * 10;
  return score;
}

int countLegalMoves(Board *b, Color color) {
  PossibleMoves moves;
  generateAllMoves(b, color, &moves);
  return moves.count;
}
int evaluateMobility(Board *b, Color color) {
  int moves = countLegalMoves(b, color);
  return moves * 2; // mobility coefficient
}

BestMove miniMaxSearch(Board *b, int depth, bool isMaxPlayer,
                       Color colorToMove) {
  if (depth == 0) {
    BestMove bestMove;
    bestMove.bestScore = evaluatePosition(b);
    return bestMove;
  }

  BestMove bestMove;
  bestMove.bestScore = isMaxPlayer ? -99999 : 99999;

  PossibleMoves possibleMoves;
  generateAllMoves(b, colorToMove, &possibleMoves);

  for (int i = 0; i < possibleMoves.count; i++) {
    Move move = possibleMoves.moves[i];

    doMove(b, &move);
    BestMove child = miniMaxSearch(b, depth - 1, !isMaxPlayer,
                                   colorToMove == WHITE ? BLACK : WHITE);
    undoMove(b, &move);

    if (isMaxPlayer) {
      if (child.bestScore > bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
    } else {
      if (child.bestScore < bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
    }
  }

  return bestMove;
}
