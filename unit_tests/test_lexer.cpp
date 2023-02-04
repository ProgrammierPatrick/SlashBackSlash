#include <iostream>

#include <lexer.h>
#include <model/to_string.h>

#include "testing.h"

using namespace std;

void lexer_single_file() {
    string filename = "test.sbs";
    string content =
        "/a a (\\1 123 123)\n"
        "# this is a comment\n"
        "\t\t  \t a\t b\t # comment\n"
        "x y abcdefg";
    // 0 1 2 3 4 5 6   7   8 9 0 1 2 13
    // / a a ( \ 1 123 123 ) a b x y abcdefg
    auto contentMemory = make_unique<string>(content);

    unique_ptr<LexedFile> lexResult = lexSingleFile(filename, contentMemory, false);

    for (auto t : lexResult->tokens)
        cout << toString(t) << " ";
    cout << endl;

    ASSERT_EQ(lexResult->filename, filename);
    ASSERT_EQ(*lexResult->fileText, content);
    ASSERT_EQ(lexResult->tokens.size(), 14);
    ASSERT(lexResult->tokens[0].isSlash());
    ASSERT(lexResult->tokens[1].isVar());
    ASSERT(lexResult->tokens[3].isLPar());
    ASSERT(lexResult->tokens[4].isBSlash());
    ASSERT(lexResult->tokens[8].isRPar());
    ASSERT(lexResult->tokens[13].isVar());
    ASSERT_EQ(lexResult->tokens[1].getVarName(), "a");
    ASSERT_EQ(lexResult->tokens[2].getVarName(), "a");
    ASSERT_EQ(lexResult->tokens[5].getVarName(), "1");
    ASSERT_EQ(lexResult->tokens[6].getVarName(), "123");
    ASSERT_EQ(lexResult->tokens[13].getVarName(), "abcdefg");
}


int main() {
    lexer_single_file();

    return GET_ASSERT_RETURN;
}
