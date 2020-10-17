#include "parser.h"
#include "sbsexception.h"

#include <iostream>

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
    throw SBSException(SBSException::Origin::PARSER, "Got Token " + token.toString() + " in " + symbol + ", but expected one of " + expected + ".", token.file, token.line, token.pos);
}


std::shared_ptr<AST> parseVal(const std::vector<Token>& tokens, int& i);
std::shared_ptr<AST> parseExpr(const std::vector<Token>& tokens, int& i);
std::shared_ptr<AST> parseLet(const std::vector<Token>& tokens, int& i);
std::shared_ptr<AST> parseAbs(const std::vector<Token>& tokens, int& i);
std::shared_ptr<AST> parseApp(const std::vector<Token>& tokens, int& i);


std::shared_ptr<AST> parse(const std::vector<Token>& tokens) {
    int i = 0;
    return parseExpr(tokens, i);
}


// VAL  = ( EXPR ) | VAR
std::shared_ptr<AST> parseVal(const std::vector<Token>& tokens, int& i) {
    if (tokens[i].type == Token::Type::VAR) {
        auto ast = std::make_shared<AST>(AST::Type::VAR, tokens[i].file, tokens[i].line, tokens[i].pos, tokens[i].fromLib);
        ast->var.name = tokens[i].value;

        i++;
        return ast;
    }
    else if (tokens[i].type == Token::Type::LPAR) {
        i++;

        auto ast = parseExpr(tokens, i);

        if(tokens[i].type != Token::Type::RPAR)
            parseError(tokens[i], "VAR", "\\");
        i++;

        return ast;
    }
    else parseError(tokens[i], "VAL", "[val] (");
    return nullptr;
}

// EXPR = APP | LET | ABS
std::shared_ptr<AST> parseExpr(const std::vector<Token>& tokens, int& i) {
    switch (tokens[i].type) {
        case Token::Type::LPAR:
        case Token::Type::VAR:
            return parseApp(tokens, i);
        case Token::Type::SLASH:
            return parseLet(tokens, i);
        case Token::Type::BSLASH:
            return parseAbs(tokens, i);
        case Token::Type::RPAR:
        case Token::Type::END:
            throw SBSException(SBSException::Origin::PARSER, "Got " + tokens[i].toString() + " in EXPR context.", tokens[i].file, tokens[i].line, tokens[i].pos);
    }
    parseError(tokens[i], "EXPR", "[val] ( / \\");
    return nullptr;
}

// LET  = / VAR VAL EXPR
std::shared_ptr<AST> parseLet(const std::vector<Token>& tokens, int& i) {
    auto ast = std::make_shared<AST>(AST::Type::LET, tokens[i].file, tokens[i].line, tokens[i].pos, tokens[i].fromLib);

    if(tokens[i].type != Token::Type::SLASH)
        parseError(tokens[i], "LET", "/");
    i++;

    if(tokens[i].type != Token::Type::VAR)
        parseError(tokens[i], "LET", "[val]");
    ast->let.name = tokens[i].value;
    i++;

    ast->let.value = parseVal(tokens, i);
    ast->let.next = parseExpr(tokens, i);

    return ast;
}

// ABS  = \ VAR EXPR
std::shared_ptr<AST> parseAbs(const std::vector<Token>& tokens, int& i) {
    auto ast = std::make_shared<AST>(AST::Type::ABS, tokens[i].file, tokens[i].line, tokens[i].pos, tokens[i].fromLib);

    if(tokens[i].type != Token::Type::BSLASH)
        parseError(tokens[i], "ABS", "\\");
    i++;

    if(tokens[i].type != Token::Type::VAR)
        parseError(tokens[i], "ABS", "[val]");
    ast->abs.name = tokens[i].value;
    i++;

    ast->abs.ast = parseExpr(tokens, i);

    return ast;
}

// APP  = VAL VAL APP' | VAL
// APP' = VAL APP' | #
std::shared_ptr<AST> parseApp2(std::shared_ptr<AST> last, const std::vector<Token>& tokens, int& i) {
    if((tokens[i].type == Token::Type::VAR) || (tokens[i].type == Token::Type::LPAR)) {
        auto ast = std::make_shared<AST>(AST::Type::APP, tokens[i].file, tokens[i].line, tokens[i].pos, tokens[i].fromLib);
        ast->app.first = std::move(last);
        ast->app.second = parseVal(tokens, i);
        return parseApp2(std::move(ast), tokens, i);
    }
    else return last;
}

std::shared_ptr<AST> parseApp(const std::vector<Token>& tokens, int& i) {
    auto first = parseVal(tokens, i);

    return parseApp2(std::move(first), tokens, i);
}

std::string padding(int i) {
    std::string s;
    for(int j = 0; j < i; j++)
        s += "  ";
    return s;
}

std::string ASTToString(const AST& ast, bool showLib, int i) {
    bool show = showLib || !ast.fromLib;

    if (ast.type == AST::Type::VAR)
        return padding(i) + ast.var.name;
    else if (ast.type == AST::Type::LET)
        return (show ? (padding(i) + "LET " + ast.let.name + "\n" + ASTToString(*ast.let.value, showLib, i+1) + "\n") : "") + ASTToString(*ast.let.next, showLib, i);
    else if (ast.type == AST::Type::APP)
        return padding(i) + "APP\n" + ASTToString(*ast.app.first, showLib, i+1) + "\n" + ASTToString(*ast.app.second, showLib, i+1);
    else if (ast.type == AST::Type::ABS)
        return padding(i) + "ABS " + ast.abs.name + "\n" + ASTToString(*ast.abs.ast, showLib, i+1);
    else return "NULLPTR";
}