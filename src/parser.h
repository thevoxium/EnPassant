#ifndef PARSER_H
#define PARSER_H

#include "model.h"

Board *parseFen(const char *fen);
void positionFromFen(char *position, Board *b);

#endif // !PARSER_H
