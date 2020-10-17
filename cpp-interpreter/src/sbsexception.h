#ifndef _UTIL_H
#define _UTIL_H

#include <exception>
#include <string>

class SBSException : public std::exception {
public:

    enum class Origin {
        LEXER,
        PARSER,
        RUNTIME
    };

    SBSException(Origin origin, const std::string& msg, const std::string& filename, int line, int pos)
        : origin(origin), msg(msg), filename(filename), line(line), pos(pos) {
            std::string errorName = "UnknownError";
            if(origin == Origin::LEXER) errorName = "LexerError";
            if(origin == Origin::PARSER) errorName = "ParserError";
            if(origin == Origin::RUNTIME) errorName = "RuntimeError";

            fullMessage = filename + ":" + std::to_string(line) + ":" + std::to_string(pos) + " " + errorName + ": " + msg;
        }

    const char* what() const throw() {
        return fullMessage.c_str();
    }

    Origin origin;
    std::string msg;
    std::string filename;
    int line;
    int pos;

private:
    std::string fullMessage;
};

#endif