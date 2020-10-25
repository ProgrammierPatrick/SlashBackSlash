#include "parser.h"
#include "sbsexception.h"

#include "model/to_string.h"

/*
Grammar:

EXPR = APP | LET | ABS
LET  = / VAR VAL EXPR
VAR  = v
VAL  = ( EXPR ) | VAR
APP  = VAL VAL APP' | VAL
APP' = VAL APP' | #
ABS  = \ VAR EXPR

first(EXPR) = ( v / \
first(APP) = ( v
first(LET) = /
first(ABS) = \
first(VAR) = v
first(VAL) = ( v

first(APP') = ( v follow(APP)


follow(EXPR) = ) #
follow(LET)  = ) #
follow(VAR)  = ) # ( v / \
follow(VAL)  = ) # ( v / \
follow(APP)  = ) #
follow(APP') = ) #
follow(ABS)  = ) #

*/

void parseError(const Token& token, const std::string& symbol, const std::string& expected) {
    throw SBSException(SBSException::Origin::PARSER, "Got Token " + toString(token) + " in " + symbol + ", but expected one of " + expected + ".", token.loc);
}

std::shared_ptr<AST> parseVal(const List<Token>& tokens, int& i);
std::shared_ptr<AST> parseExpr(const List<Token>& tokens, int& i);
std::shared_ptr<AST> parseLet(const List<Token>& tokens, int& i);
std::shared_ptr<AST> parseAbs(const List<Token>& tokens, int& i);
std::shared_ptr<AST> parseApp(const List<Token>& tokens, int& i);


std::shared_ptr<AST> parse(const List<Token>& tokens) {
    int i = 0;
    return parseExpr(tokens, i);
}

// VAL  = ( EXPR ) | VAR
std::shared_ptr<AST> parseVal(const List<Token>& tokens, int& i) {
    if (tokens[i].isVar()) {
        Token token = tokens[i];
        i++;
        return std::make_shared<AST>(AST::Var(token.getVar().name), token.loc);
    }
    else if (tokens[i].isLPar()) {
        i++;

        auto ast = parseExpr(tokens, i);

        if(!tokens[i].isRPar())
            parseError(tokens[i], "VAR", "\\");
        i++;

        return ast;
    }
    else parseError(tokens[i], "VAL", "[val] (");
    return nullptr;
}

// EXPR = APP | LET | ABS
std::shared_ptr<AST> parseExpr(const List<Token>& tokens, int& i) {
    if (tokens[i].isLPar() || tokens[i].isVar())
        return parseApp(tokens, i);
    if (tokens[i].isSlash())
        return parseLet(tokens, i);
    if (tokens[i].isBSlash())
        return parseAbs(tokens, i);
    parseError(tokens[i], "EXPR", "[val] ( / \\");
    return nullptr;
}

// LET  = / VAR VAL EXPR
std::shared_ptr<AST> parseLet(const List<Token>& tokens, int& i) {
    Token token = tokens[i];

    if(!tokens[i].isSlash())
        parseError(tokens[i], "LET", "/");
    i++;

    if(!tokens[i].isVar())
        parseError(tokens[i], "LET", "[val]");

    std::shared_ptr<const std::string> name = tokens[i].getVar().name;
    i++;

    std::shared_ptr<AST> value = parseVal(tokens, i);
    std::shared_ptr<AST> next = parseExpr(tokens, i);

    return std::make_shared<AST>(AST::Let(name, value, next), token.loc);
}

// ABS  = \ VAR EXPR
std::shared_ptr<AST> parseAbs(const List<Token>& tokens, int& i) {
    Token token = tokens[i];
    // auto ast = std::make_shared<AST>(AST::Type::ABS, tokens[i].file, tokens[i].line, tokens[i].pos, tokens[i].fromLib);

    if (!tokens[i].isBSlash())
        parseError(tokens[i], "ABS", "\\");
    i++;

    if (!tokens[i].isVar())
        parseError(tokens[i], "ABS", "[val]");
    std::shared_ptr<const std::string> name = tokens[i].getVar().name;
    i++;

    std::shared_ptr<AST> ast = parseExpr(tokens, i);

    return std::make_shared<AST>(AST::Abs(name, ast), token.loc);
}

// APP  = VAL VAL APP' | VAL
// APP' = VAL APP' | #
std::shared_ptr<AST> parseApp2(std::shared_ptr<AST> last, const List<Token>& tokens, int& i) {
    if(tokens[i].isVar() || tokens[i].isLPar()) {
        Token token = tokens[i];
        auto ast = std::make_shared<AST>(AST::App(std::move(last), parseVal(tokens, i)), token.loc);
        return parseApp2(std::move(ast), tokens, i);
    }
    else return last;
}

std::shared_ptr<AST> parseApp(const List<Token>& tokens, int& i) {
    auto first = parseVal(tokens, i);

    return parseApp2(std::move(first), tokens, i);
}

