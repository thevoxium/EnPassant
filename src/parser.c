#include "parser.h"
#include "string.h"

void buildBoardFromFen(const char *position, Board *b) {}

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

  buildBoardFromFen(tokens[0], b);

  free(fenCopy);
  return b;
}
