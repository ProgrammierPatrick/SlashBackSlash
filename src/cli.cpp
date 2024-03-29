#include "CLI11.hpp"

#include "cli.h"

int Cli::parse(int argc, char** argv) {
    CLI::App app{"SlashBackSlash"};

    app.add_flag("-l,--print-lexer", printLexer, "print tokens after lexing");
    app.add_flag("-a,--print-ast", printAST, "print AST after parsing");
    app.add_flag("-i,--print-il", printIL, "print IL assembly after compiling");

    app.add_flag("-c,--compile", compile, "compile to IL file for native (fast) execution");
    app.add_flag("-t,--trace", trace, "print complete state after each instruction");

    app.add_flag("-v,--print-bind-values", showBindValues, "print values of bound variables in trace output");

    app.add_flag("-L,--show-lib", showLib, "include actions from lib/ in all outputs");

    app.add_flag("-V,--verbose", verbose, "show processing steps");

    app.add_flag("-T,--test", test, "run file as .sbst test case");

    app.add_option("filename", filename, "the file to execute")->required();

    try {
        app.parse(argc, argv);
        return 0;
    } catch(const CLI::CallForHelp &e) {
        help = true;
        return app.exit(e);
    } catch(const CLI::ParseError &e) {
        return app.exit(e);
    }
}