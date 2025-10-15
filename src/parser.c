#include "parser.h"
#include "model.h"
#include "string.h"
#define debug(x) printf("%s", x)

void positionFromFen(char *position, Board *b) {
  char *s = position;
  int currentRank = 0;
  int currentFile = 0;
  while (*s != '\0') {
    printf("%c", *s);
    if (*s == '/') {
      currentRank++;
      currentFile = 0;
    } else if (*s >= 48 && *s <= 57) {
      int skip = *s - 48;
      for (int i = 0; i < skip; i++) {
        b->grid[currentRank * 8 + currentFile].type = EMPTY;
        b->grid[currentRank * 8 + currentFile].color = NONE;
        currentFile++;
      }
    } else if ((*s >= 97 && *s <= 122) || (*s >= 65 && *s <= 90)) {
      char c = *s;
      switch (c) {
      case 'r':
        b->grid[currentRank * 8 + currentFile].type = ROOK;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'n':
        b->grid[currentRank * 8 + currentFile].type = KNIGHT;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'b':
        b->grid[currentRank * 8 + currentFile].type = BISHOP;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'q':
        b->grid[currentRank * 8 + currentFile].type = QUEEN;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'k':
        b->grid[currentRank * 8 + currentFile].type = KING;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'p':
        b->grid[currentRank * 8 + currentFile].type = PAWN;
        b->grid[currentRank * 8 + currentFile].color = BLACK;
        break;
      case 'R':
        b->grid[currentRank * 8 + currentFile].type = ROOK;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      case 'N':
        b->grid[currentRank * 8 + currentFile].type = KNIGHT;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      case 'B':
        b->grid[currentRank * 8 + currentFile].type = BISHOP;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      case 'Q':
        b->grid[currentRank * 8 + currentFile].type = QUEEN;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      case 'K':
        b->grid[currentRank * 8 + currentFile].type = KING;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      case 'P':
        b->grid[currentRank * 8 + currentFile].type = PAWN;
        b->grid[currentRank * 8 + currentFile].color = WHITE;
        break;
      default:
        b->grid[currentRank * 8 + currentFile].type = EMPTY;
        b->grid[currentRank * 8 + currentFile].color = NONE;
        break;
      }
      currentFile++;
    }
    s++;
  }
  printf("\n");
}

Board *parseFen(const char *fen) {
  char *fenCopy = strdup(fen);
  if (fenCopy == NULL) {
    return NULL;
  }

  char *tokens[6];
  int i = 0;
  char *token = strtok(fenCopy, " ");
  while (i < 6 && token != NULL) {
    tokens[i++] = token;
    token = strtok(NULL, " ");
  }
  for (int j = 0; j < i; j++) {
    printf("Token %d: %s\n", j + 1, tokens[j]);
  }

  Board *b = (Board *)malloc(sizeof(Board));
  if (b == NULL) {
    free(fenCopy);
    return NULL;
  }

  positionFromFen(tokens[0], b);

  free(fenCopy);
  return b;
}
