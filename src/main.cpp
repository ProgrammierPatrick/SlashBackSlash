#include <iostream>
#include <string>


#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "sbsexception.h"
#include "cli.h"
#include "model/list.h"
#include "model/to_string.h"

int main(int argc, char **argv) {
    Cli cli;
    int ret = cli.parse(argc, argv);
    if(ret != 0) return ret;

    if(cli.test) {
        List<Token> tokens;
        List<Token> expectedTokens;
        bool expectError;
        runLexerForTesting(cli.filename, tokens, expectedTokens, expectError);

        if(cli.printLexer) {
            if(!expectError){
                std::cerr << "expected: ";
                for(auto t : expectedTokens)
                    if(cli.showLib || !t.loc.fromLib)
                        std::cerr << toString(t);
                std::cerr << std::endl;
            }
            for(auto t : tokens)
                if(cli.showLib || !t.loc.fromLib)
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
                std::cerr << "Test failed: no error in '" << cli.filename << "'." << std::endl;
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
                    std::cerr << "Test failed: result of '" << cli.filename << "' does not match expected result." << std::endl
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
            tokens = runLexer(cli.filename);
        } catch(std::exception& e) {
            std::cerr << "Exception in runLexer(" << cli.filename << "): " << e.what() << std::endl;
            return -1;
        }

        if(cli.printLexer) {
            for(auto t : tokens)
                if(cli.showLib || !t.loc.fromLib)
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

        if(cli.printAST)
            std::cerr << toString(*ast, cli.showLib) << std::endl;

        Exec exec(ast);
            
        if(cli.trace) {
            try {
                std::cerr << exec.printState(cli.showLib, cli.showBindValues) << std::endl;
            } catch(std::exception& e) {
                std::cerr << "Exception in Exec::printState(): " << e.what() << std::endl;
                return -1;
            }
        }
            
        while(!exec.isDone()) {

            try {
                exec.step(!cli.showLib);
            } catch(std::exception& e) {
                std::cerr << "Exception in Exec::step(): " << e.what() << std::endl;
                return -1;
            }

            if(cli.trace && !exec.isDone()) {
                try {
                    std::cerr << exec.printState(cli.showLib, cli.showBindValues) << std::endl;
                } catch(std::exception& e) {
                    std::cerr << "Exception in Exec::printState(): " << e.what() << std::endl;
                    return -1;
                }
            }
        }

        return 0;
    }
}