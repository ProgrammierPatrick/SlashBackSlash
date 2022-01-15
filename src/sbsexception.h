#ifndef _UTIL_H
#define _UTIL_H

#include <exception>
#include <string>

#include "model/file_loc.h"

class SBSException : public std::exception {
public:

    enum class Origin {
        LEXER,
        PARSER,
        RUNTIME,
        IL_COMPILER,
        C_COMPILER
    };

    SBSException(Origin origin, const std::string& msg, const FileLoc& loc)
        : origin(origin), msg(msg), loc(loc) {
            std::string errorName = "UnknownError";
            if(origin == Origin::LEXER) errorName = "LexerError";
            if(origin == Origin::PARSER) errorName = "ParserError";
            if(origin == Origin::RUNTIME) errorName = "RuntimeError";
            if(origin == Origin::IL_COMPILER) errorName = "ILCompilerError";
            if(origin == Origin::C_COMPILER) errorName = "CCompilerError";

            fullMessage = *loc.filename + ":" + std::to_string(loc.line) + ":" + std::to_string(loc.pos) + " " + errorName + ": " + msg;
        }

    const char* what() const throw() {
        return fullMessage.c_str();
    }

    Origin origin;
    std::string msg;
    FileLoc loc;

private:
    std::string fullMessage;
};

#endif