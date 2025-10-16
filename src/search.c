#include "search.h"
#include "model.h"
#include "move.h"
#include <stdbool.h>
#include <stdlib.h>

const int mg_piece_value[6] = {82, 337, 365, 477, 1025, 20000};
const int eg_piece_value[6] = {94, 281, 297, 512, 936, 20000};

const int mg_pawn_table[64] = {
    0,   0,  0,  0,  0,  0,  0,   0,   98,  134, 61,  95,  68,  126, 34,  -11,
    -6,  7,  26, 31, 65, 56, 25,  -20, -14, 13,  6,   21,  23,  12,  17,  -23,
    -27, -2, -5, 12, 17, 6,  10,  -25, -26, -4,  -4,  -10, 3,   3,   -22, -9,
    -5,  2,  -3, -1, 4,  -2, -22, -23, -15, -2,  -18, -31, -33, -53};

const int eg_pawn_table[64] = {
    0,   0,   0,   0,   0,   0,   0,  0,  178, 173, 158, 134, 147, 132, 165,
    187, 94,  100, 85,  67,  56,  53, 82, 84,  32,  24,  13,  5,   -2,  4,
    17,  17,  4,   -9,  -17, -11, -2, -9, -15, -4,  -13, -25, -20, -18, -20,
    -24, -28, -35, -24, 0,   0,   0,  0,  0,   0,   0,   0};

const int mg_knight_table[64] = {
    -167, -89, -34, -49, 61,   -97, -15, -107, -73, -41, 72,  36,  23,
    62,   7,   -17, -47, 60,   37,  65,  84,   129, 73,  44,  -9,  17,
    19,   53,  37,  69,  18,   22,  -13, 4,    16,  13,  28,  19,  21,
    -8,   -23, -9,  12,  10,   19,  17,  25,   -16, -29, -53, -12, -3,
    -1,   18,  -14, -19, -105, -21, -58, -33,  -17, -28, -19, -23};

const int eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99, -25, -8,  -25, -2,  -9,
    -25, -24, -52, -24, -20, 10,  9,   -1,  -9,  -19, -41, -17, 3,
    22,  22,  22,  11,  8,   -18, -18, -6,  16,  25,  16,  17,  4,
    -18, -23, -9,  -1,  11,  13,  22,  18,  -20, -47, -29, -25, -9,
    -25, -16, -29, -53, -32, -26, -22, -8,  -50, -64, -81};

const int mg_bishop_table[64] = {
    -29, 4,   -82, -37, -25, -42, 7,   -8,  -26, 16,  -18, -13, 30,  59,  18,
    -47, -16, 37,  43,  40,  35,  50,  37,  -2,  -9,  -1,  4,   -1,  15,  44,
    26,  7,   -10, 13,  13,  10,  22,  12,  -1,  -9,  -33, -3,  -14, -12, -13,
    -2,  -16, -26, -46, -6,  -69, -26, -27, -41, -21, -21, -34, -46};

const int eg_bishop_table[64] = {
    -14, -21, -11, -8, -7,  -9, -17, -24, -8, -4,  7,   -12, -3,  -13, -4,  -14,
    2,   -8,  0,   -1, -2,  6,  0,   4,   -3, -6,  13,  10,  -14, 12,  -8,  -7,
    -9,  -4,  -5,  15, -8,  -7, -1,  -1,  -6, -7,  -10, 4,   -1,  1,   -14, -12,
    -8,  -14, -25, -8, -25, -1, -1,  -2,  -6, -11, -8,  -16, -26, -30, -33};

const int mg_rook_table[64] = {
    32,  42,  32,  51, 63, 9,  31,  43,  27,  32,  58,  62,  80, 67, 26, 44,
    -5,  19,  26,  36, 17, 45, 61,  16,  -24, -11, 7,   26,  24, 35, -8, -20,
    -36, -26, -12, -1, 9,  -7, 6,   -23, -45, -25, -16, -17, 3,  0,  -5, -33,
    -44, -16, -20, -9, -1, 11, -30, -52, -37, -26, -42, -52, -40};

const int eg_rook_table[64] = {
    13,  10, 18,  15, 12, 12, 8,  5,  11, 13,  13, 11, -3, 3,  8,
    3,   7,  7,   7,  5,  4,  -3, -5, -3, 4,   3,  13, 1,  2,  1,
    -1,  2,  3,   5,  8,  4,  -5, -6, -8, -11, -4, 0,  -5, -1, -7,
    -12, -8, -16, -6, -6, 0,  2,  -9, -9, -11, -3, -9};

const int mg_queen_table[64] = {
    -28, 0,   29, 12,  59, 44,  43,  45,  -24, -39, -5,  1,   -16, 57, 28, 54,
    -13, -17, 7,  8,   29, 56,  47,  57,  -27, -27, -16, -16, -1,  17, -7, -1,
    -9,  -26, -9, -10, -2, -4,  3,   -3,  -14, 2,   -11, -2,  -5,  2,  14, 5,
    -35, -5,  1,  13,  5,  -22, -71, -13, -22, -16, -16, -30, -36};

const int eg_queen_table[64] = {
    -9,  22,  22,  27,  27,  19, 10,  20, -17, 20,  32,  41,  58,  25,  30,
    0,   -20, 6,   9,   49,  47, 35,  19, 9,   3,   22,  24,  45,  57,  40,
    57,  36,  -18, 28,  19,  47, 31,  34, 39,  23,  6,   -2,  -10, -6,  1,
    -16, -27, 0,   -15, -15, -1, -18, -9, -11, -19, -15, -16, -30, -36, -46};

const int mg_king_table[64] = {
    -65, 23,  16,  -15, -56, -34, 2,   13,  29,  -1,  -20, -7,  -8,  -4,
    -38, -29, -9,  24,  2,   -16, -20, 6,   22,  -22, -17, -20, -12, -27,
    -30, -25, -14, -36, -49, -1,  -27, -39, -46, -44, -33, -51, -21, -21,
    -34, -49, -45, -37, -3,  -14, -50, -57, -39, -26, -32, -43, -53, -64};

const int eg_king_table[64] = {
    -74, -35, -18, -18, -11, 15,  4,   -17, -12, 17,  14,  17,  17,  38,
    23,  11,  10,  17,  23,  15,  20,  45,  47,  13,  -8,  22,  24,  27,
    26,  33,  26,  3,   -18, -4,  21,  24,  27,  23,  9,   -11, -19, -13,
    10,  -15, -14, -18, -27, -39, -46, -44, -38, -28, -32, -43, -53, -64};

static const int *mg_psts[6] = {mg_pawn_table, mg_knight_table, mg_bishop_table,
                                mg_rook_table, mg_queen_table,  mg_king_table};
static const int *eg_psts[6] = {eg_pawn_table, eg_knight_table, eg_bishop_table,
                                eg_rook_table, eg_queen_table,  eg_king_table};

static const int passed_pawn_bonus[8] = {0, 5, 20, 45, 75, 115, 165, 250};
static const int bishop_pair_bonus_mg = 30;
static const int bishop_pair_bonus_eg = 60;
static const int rook_on_open_file_bonus = 25;
static const int rook_on_semi_open_file_bonus = 15;
static const int doubled_pawn_penalty_mg = -10;
static const int doubled_pawn_penalty_eg = -20;
static const int isolated_pawn_penalty_mg = -12;
static const int isolated_pawn_penalty_eg = -25;
static const int mobility_bonus_mg = 1;
static const int mobility_bonus_eg = 2;

typedef struct {
  int mg;
  int eg;
} TaperedScore;

static int countLegalMoves(Board *b, Color color) {
  PossibleMoves moves;
  generateAllMoves(b, color, &moves);
  int legalMoveCount = 0;
  for (int i = 0; i < moves.count; i++) {
    if (isMoveLegal(b, &moves.moves[i], color)) {
      legalMoveCount++;
    }
  }
  return legalMoveCount;
}

int evaluatePosition(Board *b) {
  int game_phase = 0;
  TaperedScore score = {0, 0};
  int white_bishops = 0;
  int black_bishops = 0;
  int white_pawns_on_file[8] = {0};
  int black_pawns_on_file[8] = {0};

  for (int i = 0; i < 64; i++) {
    Square sq = b->grid[i];
    if (sq.type == EMPTY)
      continue;

    int type_idx = sq.type - 1;
    int pst_idx = (sq.color == WHITE) ? i : (i ^ 56);

    int mg_val = mg_piece_value[type_idx] + mg_psts[type_idx][pst_idx];
    int eg_val = eg_piece_value[type_idx] + eg_psts[type_idx][pst_idx];

    if (sq.color == WHITE) {
      score.mg += mg_val;
      score.eg += eg_val;
      if (sq.type == BISHOP)
        white_bishops++;
      if (sq.type == PAWN)
        white_pawns_on_file[i % 8]++;
    } else {
      score.mg -= mg_val;
      score.eg -= eg_val;
      if (sq.type == BISHOP)
        black_bishops++;
      if (sq.type == PAWN)
        black_pawns_on_file[i % 8]++;
    }

    switch (sq.type) {
    case KNIGHT:
      game_phase += 1;
      break;
    case BISHOP:
      game_phase += 1;
      break;
    case ROOK:
      game_phase += 2;
      break;
    case QUEEN:
      game_phase += 4;
      break;
    default:
      break;
    }
  }

  if (white_bishops >= 2) {
    score.mg += bishop_pair_bonus_mg;
    score.eg += bishop_pair_bonus_eg;
  }
  if (black_bishops >= 2) {
    score.mg -= bishop_pair_bonus_mg;
    score.eg -= bishop_pair_bonus_eg;
  }

  for (int i = 0; i < 64; i++) {
    Square sq = b->grid[i];
    if (sq.type == EMPTY)
      continue;
    int rank = i / 8;
    int file = i % 8;

    if (sq.type == PAWN) {
      bool is_passed = true;
      if (sq.color == WHITE) {
        if (white_pawns_on_file[file] > 1) {
          score.mg += doubled_pawn_penalty_mg;
          score.eg += doubled_pawn_penalty_eg;
        }
        if ((file > 0 && white_pawns_on_file[file - 1] == 0) &&
            (file < 7 && white_pawns_on_file[file + 1] == 0)) {
          score.mg += isolated_pawn_penalty_mg;
          score.eg += isolated_pawn_penalty_eg;
        }
        for (int f = file - 1; f <= file + 1; f++) {
          if (f < 0 || f > 7)
            continue;
          for (int r = rank - 1; r >= 0; r--) {
            if (b->grid[BOARD_INDEX(r, f)].type == PAWN &&
                b->grid[BOARD_INDEX(r, f)].color == BLACK) {
              is_passed = false;
              break;
            }
          }
          if (!is_passed)
            break;
        }
        if (is_passed) {
          int bonus_rank = 7 - rank;
          score.mg += passed_pawn_bonus[bonus_rank];
          score.eg += passed_pawn_bonus[bonus_rank] * 1.5;
        }
      } else { // Black Pawn
        if (black_pawns_on_file[file] > 1) {
          score.mg -= doubled_pawn_penalty_mg;
          score.eg -= doubled_pawn_penalty_eg;
        }
        if ((file > 0 && black_pawns_on_file[file - 1] == 0) &&
            (file < 7 && black_pawns_on_file[file + 1] == 0)) {
          score.mg -= isolated_pawn_penalty_mg;
          score.eg -= isolated_pawn_penalty_eg;
        }
        for (int f = file - 1; f <= file + 1; f++) {
          if (f < 0 || f > 7)
            continue;
          for (int r = rank + 1; r < 8; r++) {
            if (b->grid[BOARD_INDEX(r, f)].type == PAWN &&
                b->grid[BOARD_INDEX(r, f)].color == WHITE) {
              is_passed = false;
              break;
            }
          }
          if (!is_passed)
            break;
        }
        if (is_passed) {
          int bonus_rank = rank;
          score.mg -= passed_pawn_bonus[bonus_rank];
          score.eg -= passed_pawn_bonus[bonus_rank] * 1.5;
        }
      }
    } else if (sq.type == ROOK) {
      if (sq.color == WHITE) {
        if (white_pawns_on_file[file] == 0) {
          if (black_pawns_on_file[file] == 0) {
            score.mg += rook_on_open_file_bonus;
          } else {
            score.mg += rook_on_semi_open_file_bonus;
          }
        }
      } else {
        if (black_pawns_on_file[file] == 0) {
          if (white_pawns_on_file[file] == 0) {
            score.mg -= rook_on_open_file_bonus;
          } else {
            score.mg -= rook_on_semi_open_file_bonus;
          }
        }
      }
    }
  }

  int white_mobility = countLegalMoves(b, WHITE);
  int black_mobility = countLegalMoves(b, BLACK);
  score.mg += (white_mobility - black_mobility) * mobility_bonus_mg;
  score.eg += (white_mobility - black_mobility) * mobility_bonus_eg;

  int phase = (game_phase * 256 + 12) / 24;
  if (phase > 256)
    phase = 256;

  int final_score = (score.mg * phase + score.eg * (256 - phase)) / 256;

  return (b->activeColor == WHITE) ? final_score : -final_score;
}

BestMove miniMaxSearch(Board *b, int depth, bool isMaxPlayer,
                       Color colorToMove) {
  if (depth == 0) {
    BestMove bestMove;
    bestMove.bestScore = evaluatePosition(b);
    return bestMove;
  }

  BestMove bestMove;
  bestMove.bestScore = isMaxPlayer ? -999999 : 999999;

  PossibleMoves possibleMoves;
  generateAllMoves(b, colorToMove, &possibleMoves);

  if (possibleMoves.count == 0) {
    if (isInCheck(b, colorToMove)) {
      bestMove.bestScore = isMaxPlayer ? -200000 - depth : 200000 + depth;
    } else {
      bestMove.bestScore = 0;
    }
    return bestMove;
  }

  for (int i = 0; i < possibleMoves.count; i++) {
    Move move = possibleMoves.moves[i];
    if (!isMoveLegal(b, &move, colorToMove))
      continue;

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

BestMove alphaBetaSearch(Board *b, int depth, bool isMaxPlayer,
                         Color colorToMove, int alpha, int beta) {
  BestMove bestMove;

  if (depth == 0) {
    bestMove.bestScore = evaluatePosition(b);
    return bestMove;
  }

  PossibleMoves possibleMoves;
  generateAllMoves(b, colorToMove, &possibleMoves);

  if (possibleMoves.count == 0) {
    if (isInCheck(b, colorToMove)) {
      bestMove.bestScore = isMaxPlayer ? -200000 - depth : 200000 + depth;
    } else {
      bestMove.bestScore = 0;
    }
    return bestMove;
  }

  if (isMaxPlayer) {
    bestMove.bestScore = -999999;
    for (int i = 0; i < possibleMoves.count; i++) {
      Move move = possibleMoves.moves[i];
      if (!isMoveLegal(b, &move, colorToMove))
        continue;

      doMove(b, &move);
      BestMove child =
          alphaBetaSearch(b, depth - 1, false,
                          colorToMove == WHITE ? BLACK : WHITE, alpha, beta);
      undoMove(b, &move);

      if (child.bestScore > bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }

      alpha = (alpha > bestMove.bestScore) ? alpha : bestMove.bestScore;
      if (beta <= alpha) {
        break;
      }
    }
  } else {
    bestMove.bestScore = 999999;
    for (int i = 0; i < possibleMoves.count; i++) {
      Move move = possibleMoves.moves[i];
      if (!isMoveLegal(b, &move, colorToMove))
        continue;

      doMove(b, &move);
      BestMove child =
          alphaBetaSearch(b, depth - 1, true,
                          colorToMove == WHITE ? BLACK : WHITE, alpha, beta);
      undoMove(b, &move);

      if (child.bestScore < bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }

      beta = (beta < bestMove.bestScore) ? beta : bestMove.bestScore;
      if (beta <= alpha) {
        break;
      }
    }
  }

  return bestMove;
}

static int moveScore(Move m) {
  int score = 0;
  int piece_values[6] = {100, 320, 330, 500, 900, 0};
  if (m.moveMade == CAPTURE || m.moveMade == EN_PASSANT) {
    score = 1000 + piece_values[m.pieceCaptured - 1] -
            piece_values[m.pieceMoved - 1];
  }
  if (m.moveMade >= PROMOTION_QUEEN && m.moveMade <= PROMOTION_KNIGHT) {
    score += 9000;
  }
  return score;
}

static int compareMoves(const void *a, const void *b) {
  Move *m1 = (Move *)a;
  Move *m2 = (Move *)b;
  return moveScore(*m2) - moveScore(*m1);
}

BestMove alphaBetaOrderedSearch(Board *b, int depth, bool isMaxPlayer,
                                Color colorToMove, int alpha, int beta) {
  BestMove bestMove;

  if (depth == 0) {
    bestMove.bestScore = evaluatePosition(b);
    return bestMove;
  }

  PossibleMoves possibleMoves;
  generateAllMoves(b, colorToMove, &possibleMoves);

  if (possibleMoves.count == 0) {
    if (isInCheck(b, colorToMove)) {
      bestMove.bestScore = isMaxPlayer ? -200000 - depth : 200000 + depth;
    } else {
      bestMove.bestScore = 0;
    }
    return bestMove;
  }

  qsort(possibleMoves.moves, possibleMoves.count, sizeof(Move), compareMoves);

  if (isMaxPlayer) {
    bestMove.bestScore = -999999;
    for (int i = 0; i < possibleMoves.count; i++) {
      Move move = possibleMoves.moves[i];
      if (!isMoveLegal(b, &move, colorToMove))
        continue;

      doMove(b, &move);
      BestMove child = alphaBetaOrderedSearch(
          b, depth - 1, false, colorToMove == WHITE ? BLACK : WHITE, alpha,
          beta);
      undoMove(b, &move);
      if (child.bestScore > bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
      alpha = (alpha > bestMove.bestScore) ? alpha : bestMove.bestScore;
      if (beta <= alpha) {
        break;
      }
    }
  } else {
    bestMove.bestScore = 999999;
    for (int i = 0; i < possibleMoves.count; i++) {
      Move move = possibleMoves.moves[i];
      if (!isMoveLegal(b, &move, colorToMove))
        continue;

      doMove(b, &move);
      BestMove child = alphaBetaOrderedSearch(
          b, depth - 1, true, colorToMove == WHITE ? BLACK : WHITE, alpha,
          beta);
      undoMove(b, &move);
      if (child.bestScore < bestMove.bestScore) {
        bestMove.bestScore = child.bestScore;
        bestMove.bestMove = move;
      }
      beta = (beta < bestMove.bestScore) ? beta : bestMove.bestScore;
      if (beta <= alpha) {
        break;
      }
    }
  }

  return bestMove;
}
