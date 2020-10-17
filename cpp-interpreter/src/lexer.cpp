#include "lexer.h"
#include "sbsexception.h"

#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

#ifdef __unix__
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX
#endif

#ifdef _WIN32
#include <windows.h>
#endif

bool isWhitespace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t');
}

bool isVariableChar(char c) {
    return !isWhitespace(c) && (c != '(') && (c != ')') && (c != '/') && (c != '\\');
}

struct Lexer {
    std::set<std::string> lexedFiles;
    std::vector<Token> tokens;

    std::string testResult;

    void runLexer(const std::string& filename, bool fromLib, std::istream& file) {

        std::string absPath = std::filesystem::absolute(std::filesystem::path(filename)).string();

        if(lexedFiles.find(absPath) != lexedFiles.end())
            return;
        lexedFiles.insert(absPath);

        // std::ifstream file(filename);
        char c;
        int line = 1;
        int pos = 1;

        bool maybeDirective = true;

        auto nextChar = [&]() {
            if(c == '\n') {
                line++;
                pos = 1;
                maybeDirective = true;
            } else {
                pos++;
                if (c != '_' && ((c < 'a') || (c > 'z')))
                    maybeDirective = false;
            }

            return static_cast<bool>(file.get(c));
        };

        if(!file.get(c)) return;

        while (file) {

            if (isWhitespace(c)) {
                nextChar();
            } else if(c == '#') {
                while(c != '\n')
                    nextChar();
            }
            else if (isVariableChar(c)) {
                int var_line = line;
                int var_pos = pos;
                std::string value;
                
                while (isVariableChar(c)) {
                    value += c;
                    if(!nextChar())
                        break;
                }

                if (maybeDirective && (value == "import")) {
                    while(isWhitespace(c))
                        nextChar();
                    
                    std::string path;
                    while(c != '\n') {
                        path += c;
                        nextChar();
                    }

                    bool foundPathFromLib;
                    auto foundPath = findFile(path, filename, foundPathFromLib);
                    if(foundPath != "") {
                        std::ifstream foundFile(foundPath);
                        runLexer(foundPath, fromLib || foundPathFromLib, foundFile);
                    }
                }
                else if (maybeDirective && (value == "test_case")) {
                    while(isWhitespace(c))
                        nextChar();
                    
                    std::string str;
                    while(c != '\n') {
                        str += c;
                        nextChar();
                    }
                    testResult = str;

                } else {
                    tokens.push_back(Token(Token::Type::VAR, value, filename, var_line, var_pos, fromLib));
                }

            }
            else {
                Token::Type type = Token::Type::END;
                if(c == '(') type = Token::Type::LPAR;
                else if(c == ')') type = Token::Type::RPAR;
                else if(c == '/') type = Token::Type::SLASH;
                else if(c == '\\') type = Token::Type::BSLASH;

                tokens.push_back(Token(type, filename, line, pos, fromLib));

                nextChar();
            }
        }
    }

    std::string findFile(const std::string& name, const std::string& origin, bool &outFromLib) {
        std::filesystem::path originPath(origin);

        outFromLib = false;

        if(std::filesystem::exists(originPath.parent_path() / name))
            return (originPath.parent_path() / name).string();
        
        #if defined(_WIN32) || defined(_WIN64)
        // TODO: completely untested code, check this in windows
        HMODULE hModule = GetModuleHandle(NULL);
        if(hModule) {
            char ownPath[MAX_PATH];
            GetModuleFileName(hModule, ownPath, MAX_PATH);
            std::filesystem::path exePath(ownPath);
            if(std::filesystem::exists(exePath.parent_path() / "lib" / name)) {
                outFromLib = true;
                return (exePath.parent_path() / "lib" / name).string();
            }
        }
        #endif
        #ifdef __unix__
        char ownPath[PATH_MAX];
        if (1 != readlink("/proc/self/exe", ownPath, PATH_MAX)) {
            std::filesystem::path exePath(ownPath);
            if(std::filesystem::exists(exePath.parent_path() / "lib" / name)) {
                outFromLib = true;
                return (exePath.parent_path() / "lib" / name).string();
            }
        }
        #endif

        throw SBSException(SBSException::Origin::LEXER, "Could not open file '" + name + "' imported from '" + origin + "'.", name, 0, 0);
        return "";
    }
};

std::vector<Token> runLexer(const std::string& filename) {
    Lexer lexer;
    std::ifstream file(filename);
    lexer.runLexer(filename, false, file);

    lexer.tokens.push_back(Token(Token::Type::END, filename, lexer.tokens.back().line + 1, 0, false));

    return lexer.tokens;
}

void runLexerForTesting(const std::string& filename, std::vector<Token>& fileTokens, std::vector<Token>& testTokens, bool& expectError) {
    Lexer lexer;
    std::ifstream file(filename);
    lexer.runLexer(filename, false, file);

    lexer.tokens.push_back(Token(Token::Type::END, "", -1, -1, false));

    if(lexer.testResult == "") {
        throw SBSException(SBSException::Origin::LEXER, "program called in test mode, but file is missing test_case directive.", filename, 0, 0);
    }

    if(lexer.testResult == "ERROR") {
        expectError = true;
    } else {
        expectError = false;

        Lexer resultLexer;
        std::stringstream sstream(lexer.testResult);
        resultLexer.runLexer(filename, false, sstream);

        fileTokens = lexer.tokens;
        testTokens = resultLexer.tokens;

        fileTokens.push_back(Token(Token::Type::END, filename, fileTokens.back().line + 1, 0, false));
        testTokens.push_back(Token(Token::Type::END, filename, testTokens.back().line + 1, 0, false));
    }
}


std::string Token::toString() const {
    switch (type) {
        case Token::Type::VAR: return "\'" + value + "\' ";
        case Token::Type::LPAR: return "( ";
        case Token::Type::RPAR: return ") ";
        case Token::Type::SLASH: return "/ ";
        case Token::Type::BSLASH: return "\\ ";
        case Token::Type::END: return "#";
        default: return "UNKNOWN_TOKEN_TYPE";
    }
}