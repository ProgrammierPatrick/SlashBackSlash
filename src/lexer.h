#ifndef _LEXER_H
#define _LEXER_H

#include <string>

#include "model/list.h"
#include "model/token.h"

struct TestCaseTokens {
    List<Token> expected;
    List<Token> test;
    bool expectError = false;
    FileLoc loc;
    TestCaseTokens(const FileLoc& loc) : loc(loc) { }
};

List<Token> runLexer(const std::string& filename);
std::vector<TestCaseTokens> runLexerForTesting(const std::string& filename);

#endif