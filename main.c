#include "src/model.h"
#include "src/move.h"
#include "src/search.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int SEARCH_DEPTH = 4;

void printMove(Move m) {
  int from_file = m.fromSquare % 8;
  int from_rank = 7 - (m.fromSquare / 8);
  int to_file = m.toSquare % 8;
  int to_rank = 7 - (m.toSquare / 8);

  printf("%c%d%c%d", 'a' + from_file, from_rank + 1, 'a' + to_file,
         to_rank + 1);

  if (m.piecePromoted != EMPTY) {
    switch (m.piecePromoted) {
    case QUEEN:
      printf("q");
      break;
    case ROOK:
      printf("r");
      break;
    case BISHOP:
      printf("b");
      break;
    case KNIGHT:
      printf("n");
      break;
    default:
      break;
    }
  }
}

void applyFullMove(Board *b, Move *m) {
  b->grid[m->toSquare].color = b->grid[m->fromSquare].color;
  b->grid[m->toSquare].type = m->pieceMoved;
  b->grid[m->fromSquare].color = NONE;
  b->grid[m->fromSquare].type = EMPTY;

  if (m->moveMade >= PROMOTION_QUEEN && m->moveMade <= PROMOTION_KNIGHT) {
    b->grid[m->toSquare].type = m->piecePromoted;
  }
}

Move parseMove(Board *b, Color player, const char *moveStr) {
  Move invalidMove = {-1};
  if (strlen(moveStr) < 4 || strlen(moveStr) > 5) {
    printf("Invalid move format. Use format like 'e2e4'.\n");
    return invalidMove;
  }

  int from_file = tolower(moveStr[0]) - 'a';
  int from_rank = moveStr[1] - '1';
  int to_file = tolower(moveStr[2]) - 'a';
  int to_rank = moveStr[3] - '1';

  PieceType promotion_piece = EMPTY;
  if (strlen(moveStr) == 5) {
    switch (tolower(moveStr[4])) {
    case 'q':
      promotion_piece = QUEEN;
      break;
    case 'r':
      promotion_piece = ROOK;
      break;
    case 'b':
      promotion_piece = BISHOP;
      break;
    case 'n':
      promotion_piece = KNIGHT;
      break;
    default:
      printf("Invalid promotion piece.\n");
      return invalidMove;
    }
  }

  if (from_file < 0 || from_file > 7 || from_rank < 0 || from_rank > 7 ||
      to_file < 0 || to_file > 7 || to_rank < 0 || to_rank > 7) {
    printf("Invalid square in move.\n");
    return invalidMove;
  }

  int from_square = (7 - from_rank) * 8 + from_file;
  int to_square = (7 - to_rank) * 8 + to_file;

  PossibleMoves possibleMoves;
  generateAllMoves(b, player, &possibleMoves);

  for (int i = 0; i < possibleMoves.count; i++) {
    Move currentMove = possibleMoves.moves[i];
    if (currentMove.fromSquare == from_square &&
        currentMove.toSquare == to_square) {
      if (promotion_piece != EMPTY) {
        if (currentMove.piecePromoted == promotion_piece) {
          if (isMoveLegal(b, &currentMove, player))
            return currentMove;
        }
      } else {
        if (isMoveLegal(b, &currentMove, player))
          return currentMove;
      }
    }
  }

  printf("Illegal move.\n");
  return invalidMove;
}

int main() {
  Board *board = initBoard();
  Color playerColor;
  char colorChoice;

  printf("Welcome to your Chess Engine!\n");
  printf("Choose your color (W for White, B for Black): ");
  scanf(" %c", &colorChoice);

  if (tolower(colorChoice) == 'w') {
    playerColor = WHITE;
    printf("You are playing as White.\n");
  } else {
    playerColor = BLACK;
    printf("You are playing as Black.\n");
  }

  while (1) {
    printBoard(board);

    PossibleMoves legalMoves;
    generateAllMoves(board, board->activeColor, &legalMoves);
    int legalMoveCount = 0;
    for (int i = 0; i < legalMoves.count; i++) {
      if (isMoveLegal(board, &legalMoves.moves[i], board->activeColor)) {
        legalMoveCount++;
      }
    }

    if (legalMoveCount == 0) {
      if (isInCheck(board, board->activeColor)) {
        printf("Checkmate! %s wins.\n",
               board->activeColor == WHITE ? "Black" : "White");
      } else {
        printf("Stalemate! The game is a draw.\n");
      }
      break;
    }

    if (board->activeColor == playerColor) {
      char moveStr[10];
      Move playerMove = {-1};
      while (playerMove.fromSquare == -1) {
        printf("Your move (e.g., e2e4): ");
        scanf("%s", moveStr);
        playerMove = parseMove(board, playerColor, moveStr);
      }
      applyFullMove(board, &playerMove);
    } else {
      printf("Computer is thinking...\n");
      BestMove computerMove = alphaBetaOrderedSearch(
          board, SEARCH_DEPTH, true, board->activeColor, -9999999, 9999999);

      if (computerMove.bestScore == -999999) {
        printf("Error: Engine failed to find a move.\n");
        break;
      }

      printf("Computer plays: ");
      printMove(computerMove.bestMove);
      printf(" (Score: %d)\n", computerMove.bestScore);
      applyFullMove(board, &computerMove.bestMove);
    }

    board->activeColor = (board->activeColor == WHITE) ? BLACK : WHITE;
  }

  freeBoard(board);
  return 0;
}
