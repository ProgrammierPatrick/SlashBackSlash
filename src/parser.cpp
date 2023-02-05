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

std::shared_ptr<AST> parseVal(List<Token>::const_iterator& it);
std::shared_ptr<AST> parseExpr(List<Token>::const_iterator& it);
std::shared_ptr<AST> parseLet(List<Token>::const_iterator& it);
std::shared_ptr<AST> parseAbs(List<Token>::const_iterator& it);
std::shared_ptr<AST> parseApp(List<Token>::const_iterator& it);


std::shared_ptr<AST> parse(const List<Token>& tokens) {
    auto it = tokens.begin();
    return parseExpr(it);
}

// VAL  = ( EXPR ) | VAR
std::shared_ptr<AST> parseVal(List<Token>::const_iterator& it) {
    if (it->isVar()) {
        Token token = *it;
        ++it;
        return std::make_shared<AST>(AST::Var(token.getVar().name), token.loc);
    }
    else if (it->isLPar()) {
        ++it;

        auto ast = parseExpr(it);

        if(!it->isRPar())
            parseError(*it, "VAL", ")");
        ++it;

        return ast;
    }
    else parseError(*it, "VAL", "[var] (");
    return nullptr;
}

// EXPR = APP | LET | ABS
std::shared_ptr<AST> parseExpr(List<Token>::const_iterator& it) {
    if (it->isLPar() || it->isVar())
        return parseApp(it);
    if (it->isSlash())
        return parseLet(it);
    if (it->isBSlash())
        return parseAbs(it);
    parseError(*it, "EXPR", "[var] ( / \\");
    return nullptr;
}

// LET  = / VAR VAL EXPR
std::shared_ptr<AST> parseLet(List<Token>::const_iterator& it) {
    Token token = *it;

    if(!it->isSlash())
        parseError(*it, "LET", "/");
    ++it;

    if(!it->isVar())
        parseError(*it, "LET", "[var]");

    auto name = it->getVar().name;
    ++it;

    std::shared_ptr<AST> value = parseVal(it);
    std::shared_ptr<AST> next = parseExpr(it);

    return std::make_shared<AST>(AST::Let(name, value, next), token.loc);
}

// ABS  = \ VAR EXPR
std::shared_ptr<AST> parseAbs(List<Token>::const_iterator& it) {
    Token token = *it;
    // auto ast = std::make_shared<AST>(AST::Type::ABS, it->file, it->line, it->pos, it->fromLib);

    if (!it->isBSlash())
        parseError(*it, "ABS", "\\");
    ++it;

    if (!it->isVar())
        parseError(*it, "ABS", "[var]");
    auto name = it->getVar().name;
    ++it;

    std::shared_ptr<AST> ast = parseExpr(it);

    return std::make_shared<AST>(AST::Abs(name, ast), token.loc);
}

// APP  = VAL VAL APP' | VAL
// APP' = VAL APP' | #
std::shared_ptr<AST> parseApp2(std::shared_ptr<AST> last, List<Token>::const_iterator& it) {
    if(it->isVar() || it->isLPar()) {
        Token token = *it;
        auto ast = std::make_shared<AST>(AST::App(std::move(last), parseVal(it)), token.loc);
        return parseApp2(std::move(ast), it);
    }
    else return last;
}

std::shared_ptr<AST> parseApp(List<Token>::const_iterator& it) {
    auto first = parseVal(it);

    return parseApp2(std::move(first), it);
}

