#include "lexer.h"
#include "sbsexception.h"

#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <ranges>

#ifdef __unix__
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX
#endif

#ifdef _WIN32
#include <windows.h>
#endif

using std::make_unique;
using std::unique_ptr;
using std::string;
using std::string_view;

string findFile(string_view name, string_view origin, bool &outFromLib);

bool isWhitespace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t');
}

bool isVariableChar(char c) {
    return !isWhitespace(c) && (c != '(') && (c != ')') && (c != '/') && (c != '\\');
}

unique_ptr<LexedFile> lexSingleFile(string_view filename, bool fromLib) {
    std::ifstream file(string{filename});
    auto fileText = make_unique<string>(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});
    return lexSingleFile(filename, fileText, fromLib);
}

unique_ptr<LexedFile> lexSingleFile(string_view filename, unique_ptr<string>& fileText, bool fromLib, int startLine, int startPos) {
    auto result = make_unique<LexedFile>();
    result->filename = filename;
    result->fileText = std::move(fileText);

    if (result->fileText->size() == 0)
        return result;

    string& text = *result->fileText;

    int line = startLine;
    int pos = startPos;
    int i = 0; // index of current c
    char c = text[i];

    // the next token could be a directive, if it is at the beginning of a line.
    bool maybeDirective = true;

    // include the '\0' at the end: 'abc\0' (len:3) hasNextChar(2):true, hasNextChar(3):false
    // this is done so that each handler can move i past its token
    auto hasNextChar = [&]() -> bool { return i < static_cast<int>(text.size()); };

    auto nextChar = [&]() -> void {
        assert(hasNextChar());

        if(c == '\n') {
            line++;
            pos = 1;
            maybeDirective = true;
        } else {
            pos++;
            if (!(c == '_' || (c >= 'a' && c <= 'z')))
                maybeDirective = false;
        }

        c = text[++i];
    };

    while (hasNextChar()) {
        if (isWhitespace(c)) {
            nextChar();
        }
        else if(c == '#') {
            while(c != '\n' && hasNextChar())
                nextChar();
        }
        else if (isVariableChar(c)) {
            FileLoc loc(result->filename, line, pos, fromLib); 

            int i_start = i;
            while (isVariableChar(c) && hasNextChar())
                nextChar();
            string_view value{text.begin() + i_start, text.begin() + i};

            if (maybeDirective && (value == "import")) {
                while(isWhitespace(c) && hasNextChar())
                    nextChar();
                
                int i_start_path = i;
                while(c != '\n' && hasNextChar())
                    nextChar();
                string_view path{text.begin() + i_start_path, text.begin() + i};

                bool foundPathFromLib;
                auto foundPath = findFile(path, loc.filename, foundPathFromLib);
                if(foundPath != "")
                    result->importStatements.push_back({ .tokenIndex = result->tokens.size(), .isLib = foundPathFromLib, .filename = foundPath });
            }
            else {
                result->tokens.push_back(Token::makeVar(value, loc));
            }

        }
        else {
            FileLoc loc(result->filename, line, pos, fromLib);

            if (c == '(')       result->tokens.push_back(Token::makeLPar(loc));
            else if (c == ')')  result->tokens.push_back(Token::makeRPar(loc));
            else if (c == '/')  result->tokens.push_back(Token::makeSlash(loc));
            else if (c == '\\') result->tokens.push_back(Token::makeBSlash(loc));

            nextChar();
        }
    }

    return result;
}

LexerResult lexFile(string_view filename) {
    std::ifstream file(string{filename});
    auto fileText = make_unique<string>(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});
    return lexFile(filename, fileText);
}

LexerResult lexFile(string_view filename, unique_ptr<string>& fileText, int startLine, int startPos) {
    LexerResult result;

    auto absPath = [](string_view path) { return std::filesystem::canonical(path).string(); };

    // Read all files seperately

    std::unordered_map<string, unique_ptr<LexedFile>> lexedFiles;
    std::unordered_map<string, bool> filesToLex;

    auto mainFile = lexSingleFile(filename, fileText, false, startLine, startPos);
    for (auto s : mainFile->importStatements)
        filesToLex.insert({absPath(s.filename), s.isLib});
    lexedFiles.insert({string{absPath(filename)}, std::move(mainFile)});

    while (filesToLex.size() > 0) {
        string currentFilename = filesToLex.begin()->first;
        bool isLib = filesToLex.begin()->second;
        filesToLex.erase(currentFilename);

        auto lexedFile = lexSingleFile(currentFilename, isLib);

        for (auto s : lexedFile->importStatements) {
            auto path = absPath(s.filename);
                if (!lexedFiles.contains(path))
                    filesToLex.insert({path, s.isLib});
        }

        lexedFiles.insert({currentFilename, std::move(lexedFile)});
    }

    // Combine tokens to a coherent token list
    auto mergeTokens = [&](string_view filename, const std::vector<string_view>& mergedFiles, auto& mergeTokens) -> void {
        auto& file = *lexedFiles[string{filename}];
        auto it = file.tokens.begin();
        int i = 0;
        int importIdx = 0;
        while (it != file.tokens.end()) {
            if (importIdx < static_cast<int>(file.importStatements.size()) && i == file.importStatements[importIdx].tokenIndex) {
                auto& import = file.importStatements[importIdx++];

                if (std::find(mergedFiles.begin(), mergedFiles.end(), import.filename) == mergedFiles.end()) {
                    std::vector<string_view> nextMergedFiles = mergedFiles;
                    nextMergedFiles.push_back(filename);

                    mergeTokens(import.filename, nextMergedFiles, mergeTokens);
                }
            }
            result.tokens.push_back(*it);
            ++it;
            ++i;
        }
        result.fileData.push_back(std::move(lexedFiles[string{filename}]));
    };
    mergeTokens(absPath(filename), {}, mergeTokens);

    FileLoc lastTokenLoc = result.tokens.size() > 0 ? result.tokens.back().loc : FileLoc{ result.fileData.front()->filename, 1, 1, false };
    result.tokens.push_back(Token::makeEnd(FileLoc{lastTokenLoc.filename, lastTokenLoc.line + 1, 0, lastTokenLoc.fromLib}));

    return result;
}

LexerTestsResult lexTestFile(string_view filename) {
    LexerTestsResult result;

    std::ifstream file(string{filename});
    
    bool beforeFirstTestCase = true;
    string expectedContent;
    string currentSectionContent;
    int contentLineNum = 0;
    bool expectError = false;

    auto lexTest = [](string_view filename, string_view expectedContent, string_view currentSectionContent, bool expectError, int contentLineNum) -> LexerTestCaseResult {
        LexerTestCaseResult result;
        auto expectedText = make_unique<string>(expectedContent);
        auto testText = make_unique<string>(currentSectionContent);
        return {
            .test     = lexFile(filename, testText, contentLineNum + 1, 0),
            .expected = lexFile(filename, expectedText, contentLineNum, 10),
            .expectError = expectError
        };
    };

    int lineNum = 0;
    string line;
    while(std::getline(file, line)) {
        if (line.find("test_case ") == 0) {
            // test_case directive found, split off text

            if (beforeFirstTestCase) {
                auto content = make_unique<string>(currentSectionContent);
                result.commonCode = lexFile(filename, content);
                currentSectionContent = "";

                // completely unefficient weird hack just because we dont have pop_back()
                int tokenCount = result.commonCode.tokens.size() - 1;
                List<Token> tokensWithoutEnd;
                for (int i = 0; i < tokenCount; i++)
                    tokensWithoutEnd.push_back(result.commonCode.tokens[i]);
                result.commonCode.tokens = tokensWithoutEnd;

            } else {
                auto lexerResult = lexTest(filename, expectedContent, currentSectionContent, expectError, contentLineNum);
                currentSectionContent = "";
                lexerResult.expected.tokens.append_front(result.commonCode.tokens);
                lexerResult.test.tokens.append_front(result.commonCode.tokens);
                result.tests.push_back(std::move(lexerResult));
            } 
                
            beforeFirstTestCase = false;

            expectedContent = line.substr(10);
            if (expectedContent == "ERROR") {
                expectError = true;
                expectedContent = "";
            } else {
                expectError = false;
            }

            contentLineNum = lineNum;
        } else {
            currentSectionContent += line + "\n";
        }

        lineNum++;
    }

    if (beforeFirstTestCase)
        throw std::runtime_error("lexTestFile(): file " + string{filename} + " does not contain test cases.");

    auto lexerResult = lexTest(filename, expectedContent, currentSectionContent, expectError, contentLineNum);
    lexerResult.expected.tokens.append_front(result.commonCode.tokens);
    lexerResult.test.tokens.append_front(result.commonCode.tokens);
    result.tests.push_back(std::move(lexerResult));

    return result;
}

string findFile(string_view name, string_view origin, bool &outFromLib) {
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

    throw SBSException(SBSException::Origin::LEXER, "Could not open file '" + string{name} + "' imported from '" + string{origin} + "'.", FileLoc(name, 0, 0, false));
}
