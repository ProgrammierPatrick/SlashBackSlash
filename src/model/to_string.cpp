#include "to_string.h"

std::string padding(int i) {
    std::string s;
    for(int j = 0; j < i; j++)
        s += "  ";
    return s;
}
std::string toStringASTImpl(const AST& ast, bool showLib, int i) {
    bool show = showLib || !ast.loc.fromLib;

    std::string s;

    std::string b;
    if(!ast.getBindFromParent) {
        b += " *";
    }

    if(ast.bindings.size() > 0) {
        b += " [";
        for(auto bi : ast.bindings) {
            b += std::string{bi.name} + ",";
        }
        b += "]";
    }

    if (ast.isVar()) {
        s += padding(i) + std::string{ast.getVar().name} + b;
    } else if(ast.isLet()) {
        if(show) s += padding(i) + "LET " + std::string{ast.getLet().name} + b + "\n" + toStringASTImpl(*ast.getLet().value, showLib, i + 1) + "\n";
        s += toStringASTImpl(*ast.getLet().next, showLib, i);
    } else if(ast.isApp()) {
        s += padding(i) + "APP " + b + "\n" + toStringASTImpl(*ast.getApp().first, showLib, i + 1) + "\n" + toStringASTImpl(*ast.getApp().second, showLib, i + 1);
    } else if(ast.isAbs()) {
        s += padding(i) + "ABS " + std::string{ast.getAbs().name} + b + "\n" + toStringASTImpl(*ast.getAbs().ast, showLib, i + 1);
    } else s += "NULLPTR";

    return s;
}

std::string toString(const AST& ast, bool showLib) {
    return toStringASTImpl(ast, showLib, 0);
}

std::string toString(const Token& token) {
    if(token.isVar()) {
        std::string varName{ token.getVarName() };
        return "\'" + varName + "\' ";
    }
    else if(token.isLPar()) return "( ";
    else if(token.isRPar()) return ") ";
    else if(token.isSlash()) return "/ ";
    else if(token.isBSlash()) return "\\ ";
    else if(token.isEnd()) return "#";
    return "{UNKNOWN_TOKEN_TYPE} ";
}

std::string toString(const Binding& binding) {
    return "B[" + std::string{binding.name} + (binding.fromBeta ? "*" : "") +"]";
}