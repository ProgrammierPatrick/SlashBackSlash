#ifndef _LEXER_H
#define _LEXER_H

#include <string>

#include "model/list.h"
#include "model/token.h"

List<Token> runLexer(const std::string& filename);
void runLexerForTesting(const std::string& filename, List<Token>& fileTokens, List<Token>& testTokens, bool& expectError);

#endif