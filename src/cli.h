#ifndef _CLI_H
#define _CLI_H

#include <string>

struct Cli {
    bool printLexer = false;
    bool printAST = false;
    bool printIL = false;
    bool trace = false;
    bool showBindValues = false;
    bool compile = false;
    bool showLib = false;
    bool test = false;
    bool verbose = false;
    bool help = false;
    std::string filename;

    int parse(int argc, char** argv);
};

#endif