#ifndef _LEXER_H
#define _LEXER_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "model/token.h"


// Result of tokenization of a single file. Please keep fileText in memory since the tokens use its data
struct LexedFile {
    std::vector<Token> tokens;
    std::string filename;
    std::unique_ptr<std::string> fileText;

    struct ImportStatement { int tokenIndex; bool isLib; std::string filename; };
    std::vector<ImportStatement> importStatements;
};

std::unique_ptr<LexedFile> lexSingleFile(std::string_view filename, bool fromLib);
std::unique_ptr<LexedFile> lexSingleFile(std::string_view filename, std::unique_ptr<std::string>& fileText, bool fromLib, int startLine = 1, int startPos = 1);

// Result of tokenization of a program including all imports
struct LexerResult {
    std::vector<Token> tokens;
    std::vector<std::unique_ptr<LexedFile>> fileData;
};

LexerResult lexFile(std::string_view filename);
LexerResult lexFile(std::string_view filename, std::unique_ptr<std::string>& fileText, int startLine = 1, int startPos = 1);

struct LexerTestCaseResult {
    LexerResult test;
    LexerResult expected;
    bool expectError;
};

struct LexerTestsResult {
    LexerResult commonCode;
    std::vector<LexerTestCaseResult> tests;
};

LexerTestsResult lexTestFile(std::string_view filename);

#endif