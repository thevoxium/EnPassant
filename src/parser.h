#ifndef PARSER_H
#define PARSER_H

#include "model.h"

Board *parseFen(const char *fen);
void buildBoardFromFen(const char *position, Board *b);

#endif // !PARSER_H
