#ifndef _LEXER_H
#define _LEXER_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "model/list.h"
#include "model/token.h"


// Result of tokenization of a single file. Please keep fileText in memory since the tokens use its data
struct LexedFile {
    List<Token> tokens;
    std::string filename;
    std::unique_ptr<std::string> fileText;

    struct ImportStatement { int tokenIndex; bool isLib; std::string_view filename; };
    std::vector<ImportStatement> importStatements;
};

std::unique_ptr<LexedFile> lexSingleFile(std::string_view filename, bool fromLib);
std::unique_ptr<LexedFile> lexSingleFile(std::string_view filename, std::unique_ptr<std::string>& fileText, bool fromLib, int startLine = 0, int startPos = 0);

// Result of tokenization of a program including all imports
struct LexerResult {
    List<Token> tokens;
    std::vector<std::unique_ptr<LexedFile>> fileData;
};

LexerResult lexFile(std::string_view filename);
LexerResult lexFile(std::string_view filename, std::unique_ptr<std::string>& fileText, int startLine = 0, int startPos = 0);

struct LexerTestCaseResult {
    LexerResult test;
    LexerResult expected;
    bool expectError;
};

std::vector<LexerTestCaseResult> lexTestFile(std::string_view filename);

#endif