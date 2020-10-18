#include <iostream>
#include <string>

#include "CLI11.hpp"

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "sbsexception.h"
#include "model/list.h"
#include "model/to_string.h"

int main(int argc, char **argv) {
    CLI::App app{"SlashBackSlash"};

    bool printLexer = false;
    app.add_flag("-l,--print-lexer", printLexer, "print tokens after lexing");
    bool printAST = false;
    app.add_flag("-a,--print-ast", printAST, "print AST after parsing");

    bool trace = false;
    app.add_flag("-t,--trace", trace, "print complete state after each instruction");

    bool showBindValues = false;
    app.add_flag("-v,--print-bind-values", showBindValues, "print values of bound variables in trace output");

    bool showLib = false;
    app.add_flag("-L,--show-lib", showLib, "include actions from lib/ in all outputs");

    bool test = false;
    app.add_flag("-T,--test", test, "run file as .sbst test case");

    std::string filename;
    app.add_option("filename", filename, "the file to execute")->required();

    CLI11_PARSE(app, argc, argv);

    if(test) {
        List<Token> tokens;
        List<Token> expectedTokens;
        bool expectError;
        runLexerForTesting(filename, tokens, expectedTokens, expectError);

        if(printLexer) {
            if(!expectError){
                std::cerr << "expected: ";
                for(auto t : expectedTokens)
                    if(showLib || !t.loc.fromLib)
                        std::cerr << toString(t);
                std::cerr << std::endl;
            }
            for(auto t : tokens)
                if(showLib || !t.loc.fromLib)
                    std::cerr << toString(t);
            std::cerr << std::endl;
        }

        if(expectError) {
            bool receivedException = true;
            try {
                Exec exec(parse(tokens));
                while(!exec.isDone()) exec.step(false);
                receivedException = false;
            } catch(SBSException& e) { }

            if(!receivedException) {
                std::cerr << "Test failed: no error in '" << filename << "'." << std::endl;
                return -1;
            } else return 0;
        } else {
            try {
                Exec expectedExec(parse(expectedTokens));
                while(!expectedExec.isDone()) expectedExec.step(false);
                std::string expectedState = expectedExec.printState(false);

                Exec exec(parse(tokens));
                while(!exec.isDone()) exec.step(false);
                std::string resultState = exec.printState(false);

                if(!AST::alphaEquiv(*expectedExec.getRoot(), *exec.getRoot())) {
                    std::cerr << "Test failed: result of '" << filename << "' does not match expected result." << std::endl
                        << "expected: " << expectedState << std::endl
                        << "result: " << resultState << std::endl;
                    return -1;
                }


                return 0;
            }
            catch(SBSException& e) {
                std::cerr << "Test failed: " << e.what() << std::endl;
                return -1;
            }
        }

    }
    else {
        List<Token> tokens;
        try {
            tokens = runLexer(filename);
        } catch(std::exception& e) {
            std::cerr << "Exception in runLexer(" << filename << "): " << e.what() << std::endl;
            return -1;
        }

        if(printLexer) {
            for(auto t : tokens)
                if(showLib || !t.loc.fromLib)
                    std::cerr << toString(t);
            std::cerr << std::endl;
        }

        std::shared_ptr<AST> ast;
        try {
            ast = parse(tokens);
        } catch(std::exception& e) {
            std::cerr << "Exception in parse(tokens): " << e.what() << std::endl;
            return -1;
        }

        if(printAST)
            std::cerr << toString(*ast, showLib) << std::endl;

        Exec exec(ast);
            
        if(trace) {
            try {
                std::cerr << exec.printState(showLib, showBindValues) << std::endl;
            } catch(std::exception& e) {
                std::cerr << "Exception in Exec::printState(): " << e.what() << std::endl;
                return -1;
            }
        }
            
        while(!exec.isDone()) {

            try {
                exec.step(!showLib);
            } catch(std::exception& e) {
                std::cerr << "Exception in Exec::step(): " << e.what() << std::endl;
                return -1;
            }

            if(trace && !exec.isDone()) {
                try {
                    std::cerr << exec.printState(showLib, showBindValues) << std::endl;
                } catch(std::exception& e) {
                    std::cerr << "Exception in Exec::printState(): " << e.what() << std::endl;
                    return -1;
                }
            }
        }

        return 0;
    }
}