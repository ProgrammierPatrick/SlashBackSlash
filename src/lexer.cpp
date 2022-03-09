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
    List<Token> tokens;

    void runLexer(std::istream& file, const FileLoc& loc) {
        auto& filename = *loc.filename;
        std::string absPath = std::filesystem::absolute(std::filesystem::path(filename)).string();

        if(lexedFiles.find(absPath) != lexedFiles.end())
            return;
        lexedFiles.insert(absPath);

        char c;
        int line = loc.line;
        int pos = loc.pos;
        bool fromLib = loc.fromLib;

        // the next token could be a directive, if it is at the beginning of a line.
        bool maybeDirective = true;

        auto nextChar = [&]() {
            if(c == '\n') {
                line++;
                pos = 1;
                maybeDirective = true;
            } else {
                pos++;
                if (!(c == '_' || (c >= 'a' && c <= 'z')))
                    maybeDirective = false;
            }

            return static_cast<bool>(file.get(c));
        };

        auto& filenamePtr = loc.filename;

        if(!file.get(c)) return;

        while (file) {

            if (isWhitespace(c)) {
                nextChar();
            }
            else if(c == '#') {
                while(c != '\n' && file)
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
                    auto foundPath = std::make_shared<std::string>(findFile(path, filename, foundPathFromLib));
                    if(*foundPath != "") {
                        std::ifstream foundFile(*foundPath);
                        runLexer(foundFile, FileLoc(foundPath, 0, 0, fromLib || foundPathFromLib));
                    }
                }
                else {
                    tokens.push_back(Token::makeVar(std::make_shared<std::string>(value), FileLoc(filenamePtr, var_line, var_pos, fromLib)));
                }

            }
            else {
                FileLoc loc(filenamePtr, line, pos, fromLib);

                if (c == '(')       tokens.push_back(Token::makeLPar(loc));
                else if (c == ')')  tokens.push_back(Token::makeRPar(loc));
                else if (c == '/')  tokens.push_back(Token::makeSlash(loc));
                else if (c == '\\') tokens.push_back(Token::makeBSlash(loc));

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
        HMODULE hModule = GetModuleHandle(NULL);
        if(hModule) {
            char ownPath[MAX_PATH];
            GetModuleFileName(hModule, ownPath, MAX_PATH);
            std::filesystem::path exePath(ownPath);
            if(std::filesystem::exists(exePath.parent_path() / "lib" / name)) {
                outFromLib = true;
                return (exePath.parent_path() / "lib" / name).string();
            }
            if(std::filesystem::exists(exePath.parent_path().parent_path() / "lib" / name)) {
                // in MSVC, the sbs.exe can be placed inside Debug/ folder in build-dir. So relative path to lib is ../lib/
                outFromLib = true;
                return (exePath.parent_path().parent_path() / "lib" / name).string();
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

        throw SBSException(SBSException::Origin::LEXER, "Could not open file '" + name + "' imported from '" + origin + "'.", FileLoc(std::make_shared<std::string>(name), 0, 0, false));
        return "";
    }
};

List<Token> runLexer(const std::string& filename) {
    auto filenamePtr = std::make_shared<std::string>(filename);
    Lexer lexer;
    std::ifstream file(filename);
    lexer.runLexer(file, FileLoc(filenamePtr, 0, 0, false));

    if(lexer.tokens.size() == 0) throw std::runtime_error("runLexer(): file " + filename + " is empty.");

    lexer.tokens.push_back(Token::makeEnd(FileLoc(filenamePtr, lexer.tokens.back().loc.line + 1, 0, false)));

    return lexer.tokens;
}

std::vector<TestCaseTokens> runLexerForTesting(const std::string& filename) {
    std::ifstream file(filename);
    auto filenamePtr = std::make_shared<std::string>(filename);

    List<Token> common;
    std::vector<TestCaseTokens> tests;
    bool firstTest = true;

    std::stringstream ss;
    std::string line;
    int lineNum = 1;
    int startLineNum = 1; // start of current Data in stream

    auto lex = [](std::istream& ss, const FileLoc& loc) {
        Lexer lexer;
        lexer.runLexer(ss, loc);
        return lexer.tokens;
    };

    auto lexLast = [&]() {
        ss << std::flush;
        if (firstTest)
            common = lex(ss, FileLoc(filenamePtr, startLineNum, 0, false));
        else {
            tests.back().test = List<Token>::append(common, lex(ss, FileLoc(filenamePtr, startLineNum, 0, false)));
            tests.back().test.push_back(Token::makeEnd(FileLoc(filenamePtr, lineNum, 0, false)));
        }
        ss = std::stringstream();
        firstTest = false;
    };

    while(std::getline(file, line)) {
        if (line.find("test_case ") == 0) {
            // test_case directive found, split off text

            lexLast();

            tests.push_back(TestCaseTokens(FileLoc(filenamePtr, lineNum, 0, false)));

            auto expectedStr = line.substr(10);
            if (expectedStr == "ERROR") {
                tests.back().expectError = true;
            } else {
                auto ss = std::stringstream(expectedStr);
                tests.back().expected = List<Token>::append(common, lex(ss, FileLoc(filenamePtr, lineNum, 10, false)));
                tests.back().expected.push_back(Token::makeEnd(FileLoc(filenamePtr, lineNum, line.size(), false)));
            }

            startLineNum = lineNum + 1;
        } else {
            ss << line << "\n";
        }

        lineNum++;
    }
    lexLast();

    if(tests.size() == 0) throw std::runtime_error("runLexerForTesting(): file " + filename + " is empty.");

    return tests;
}