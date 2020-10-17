#ifndef _LEXER_H
#define _LEXER_H

#include <string>
#include <vector>

struct Token {
    enum class Type {
        VAR, LPAR, RPAR, SLASH, BSLASH, END
    };

    Type type;
    std::string value;

    std::string file;
    int line;
    int pos;
    bool fromLib;

    Token(Type type, const std::string& value, const std::string& file, int line, int pos, bool fromLib)
        : type(type), value(value), file(file), line(line), pos(pos), fromLib(fromLib) {}
    Token(Type type, const std::string& file, int line, int pos, bool fromLib)
        : Token(type, "", file, line, pos, fromLib) {}

    std::string toString() const;
};

std::vector<Token> runLexer(const std::string& filename);
void runLexerForTesting(const std::string& filename, std::vector<Token>& fileTokens, std::vector<Token>& testTokens, bool& expectError);


#endif