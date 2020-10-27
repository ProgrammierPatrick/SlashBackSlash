#ifndef _TO_STRING_H
#define _TO_STRING_H

#include <string>

#include "ast.h"
#include "token.h"

std::string toString(const Token& token);
std::string toString(const AST& ast, bool showLib);
std::string toString(const Binding& binding);

#endif