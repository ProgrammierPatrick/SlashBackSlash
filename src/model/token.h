#ifndef _TOKEN_H
#define _TOKEN_H

#include <variant>
#include <memory>
#include <string>

#include "file_loc.h"

// immutable
struct Token {

    struct Var {
        const std::shared_ptr<const std::string> name;
        Var(const std::shared_ptr<const std::string>& name) : name(name) { }
    };
    struct LPar { };
    struct RPar { };
    struct Slash { };
    struct BSlash { };
    struct End { };

    const std::variant<Var, LPar, RPar, Slash, BSlash, End> data;
    const FileLoc loc;

    template<typename T>
    Token(const T& value, const FileLoc& loc) : data(value), loc(loc) { }

    bool isVar()    const { return std::holds_alternative<Var>(data); }
    bool isLPar()   const { return std::holds_alternative<LPar>(data); }
    bool isRPar()   const { return std::holds_alternative<RPar>(data); }
    bool isSlash()  const { return std::holds_alternative<Slash>(data); }
    bool isBSlash() const { return std::holds_alternative<BSlash>(data); }
    bool isEnd()    const { return std::holds_alternative<End>(data); }

    std::string getVarName() const { if(!isVar()) throw std::runtime_error("Token::getVarName() called on non-var token."); return *std::get<Var>(data).name; }
    const Var& getVar() const { if(!isVar()) throw std::runtime_error("Token::getVar() called on non-var token."); return std::get<Var>(data); }

    static Token makeVar(const std::shared_ptr<const std::string>& name, const FileLoc& fileLoc) {
        return Token(Var(name), fileLoc);
    }
    static Token makeLPar(const FileLoc& fileLoc) { return Token(LPar(), fileLoc); }
    static Token makeRPar(const FileLoc& fileLoc) { return Token(RPar(), fileLoc); }
    static Token makeSlash(const FileLoc& fileLoc) { return Token(Slash(), fileLoc); }
    static Token makeBSlash(const FileLoc& fileLoc) { return Token(BSlash(), fileLoc); }
    static Token makeEnd(const FileLoc& fileLoc) { return Token(End(), fileLoc); }
};

#endif