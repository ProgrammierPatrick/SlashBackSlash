#ifndef _CLI_H
#define _CLI_H

#include <string>

struct Cli {
    bool printLexer = false;
    bool printAST = false;
    bool trace = false;
    bool showBindValues = false;
    bool showLib = false;
    bool test = false;
    std::string filename;

    int parse(int argc, char** argv);
};

#endif