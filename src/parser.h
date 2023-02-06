#ifndef _PARSER_H
#define _PARSER_H

#include "model/list.h"
#include "model/ast.h"
#include "model/token.h"

std::shared_ptr<AST> parse(const std::vector<Token>& tokens);


#endif