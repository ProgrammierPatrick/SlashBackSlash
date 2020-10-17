#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <variant>
#include <memory>

#include "lexer.h"
#include "ast.h"

std::shared_ptr<AST> parse(const std::vector<Token>& tokens);
std::string ASTToString(const AST& ast, bool showLib, int i = 0);

#endif