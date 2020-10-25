#include "CLI11.hpp"

#include "cli.h"

int Cli::parse(int argc, char** argv) {
    CLI::App app{"SlashBackSlash"};

    app.add_flag("-l,--print-lexer", printLexer, "print tokens after lexing");
    app.add_flag("-a,--print-ast", printAST, "print AST after parsing");

    app.add_flag("-t,--trace", trace, "print complete state after each instruction");

    app.add_flag("-v,--print-bind-values", showBindValues, "print values of bound variables in trace output");

    app.add_flag("-L,--show-lib", showLib, "include actions from lib/ in all outputs");

    app.add_flag("-T,--test", test, "run file as .sbst test case");

    app.add_option("filename", filename, "the file to execute")->required();

    CLI11_PARSE(app, argc, argv);
    return 0;
}