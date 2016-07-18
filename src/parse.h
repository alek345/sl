#ifndef PARSE_H
#define PARSE_H

#include "lex.h"
#include "ast.h"

void parser_init(TokenArray *tokens);

NodeArray* parser_parse();

#endif /* PARSE_H */