#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "compiler_il.h"
#include "compiler_c.h"
#include "sbsexception.h"
#include "cli.h"
#include "model/list.h"
#include "model/to_string.h"

int main(int argc, char **argv) {
    Cli cli;
    int ret = cli.parse(argc, argv);
    if(ret != 0 || cli.help)
        return ret;

    if(cli.test) {
        if (cli.verbose) std::cerr << "run lexer" << std::endl;
        auto tests = lexTestFile(cli.filename);

        if(cli.printLexer) {
            std::cerr << "common: ";
            for(auto t : tests.commonCode.tokens) {
                if(cli.showLib || !t.loc.fromLib)
                    std::cerr << toString(t);
            }
            std::cerr << std::endl;
            for(auto& test : tests.tests) {
                if(!test.expectError) {
                    std::cerr << "expected: ";
                    for(auto t : test.expected.tokens)
                        if(cli.showLib || !t.loc.fromLib)
                            std::cerr << toString(t);
                    std::cerr << std::endl;
                }
                std::cerr << "test: ";
                for(auto t : test.test.tokens) {
                    if(cli.showLib || !t.loc.fromLib)
                        std::cerr << toString(t);
                }
                std::cerr << std::endl;
            }
        }

        bool failedAny = false;
        for (size_t i = 0; i < tests.tests.size(); i++) {
            auto& test = tests.tests[i];
            std::string msg;
            bool failed = false;

            if(test.expectError) {
                bool receivedException = true;
                try {
                    if (cli.verbose) std::cerr << "run parser" << std::endl;

                    auto ast = parse(test.test.tokens);

                    if(cli.printAST)
                        std::cerr << toString(*ast, cli.showLib) << std::endl; 

                    Exec exec(ast);
                    while(!exec.isDone()) {
                        exec.step(false);
                        if(cli.trace && !exec.isDone()) std::cerr << exec.printState(cli.showLib, cli.showBindValues) << std::endl;
                    }
                    receivedException = false;
                } catch(SBSException&) { }

                if(!receivedException) {
                    msg = "no error in '" + cli.filename + "':" + std::to_string(test.test.tokens.front().loc.line) + ".";
                    failed = true;
                }
            } else {
                try {
                    if (cli.verbose) std::cerr << "run parser for expected value" << std::endl;

                    auto expectedAST = parse(test.expected.tokens);
                    auto testAST = parse(test.test.tokens);

                    if(cli.printAST) {
                        std::cout << "Expected AST:\n";
                        std::cerr << toString(*expectedAST, cli.showLib) << "\n";
                        std::cout << "Test AST:\n";
                        std::cerr << toString(*testAST, cli.showLib) << std::endl; 
                    }

                    Exec expectedExec(expectedAST);
                    while(!expectedExec.isDone()) expectedExec.step(false);
                    std::string expectedState = expectedExec.printState(false);

                    if (cli.verbose) std::cerr << "run main parser" << std::endl;
                    Exec exec(testAST);
                    while(!exec.isDone()) {
                        exec.step(false);
                        if(cli.trace && !exec.isDone()) std::cerr << exec.printState(cli.showLib, cli.showBindValues) << std::endl;
                    }
                    std::string resultState = exec.printState(false);

                    if (cli.verbose) std::cerr << "check alpha equivalence" << std::endl;
                    if(!AST::alphaEquiv(*expectedExec.getRoot(), *exec.getRoot())) {
                        msg = "result of '" + cli.filename + "':" + std::to_string(test.test.tokens.front().loc.line) + " does not match expected result.";
                        msg += "\n  expected: " + expectedState;
                        msg += "\n  result: " + resultState;
                        failed = true;
                    }
                }
                catch(std::exception& e) {
                    msg = e.what();
                    failed = true;
                }
            }

            std::cerr << "test " << (i + 1) << " / " << tests.tests.size() << ": ";
            if (failed)
                std::cerr << "failed! " << msg << "\n";
            else
                std::cerr << "Ok.\n";

            if (failed) failedAny = true;
        }

        return failedAny ? -1 : 0;
    }
    else {
        LexerResult lexed;
        try {
            if (cli.verbose) std::cerr << "run lexer" << std::endl;
            lexed = lexFile(cli.filename);
        } catch(std::exception& e) {
            std::cerr << "Exception in runLexer(" << cli.filename << "): " << e.what() << std::endl;
            return -1;
        }

        if(cli.printLexer) {
            for(auto t : lexed.tokens)
                if(cli.showLib || !t.loc.fromLib)
                    std::cerr << toString(t);
            std::cerr << std::endl;
        }

        std::shared_ptr<AST> ast;
        try {
            if (cli.verbose) std::cerr << "run parser" << std::endl;
            ast = parse(lexed.tokens);
        } catch(std::exception& e) {
            std::cerr << "Exception in parse(tokens): " << e.what() << std::endl;
            return -1;
        }

        if(cli.printAST)
            std::cerr << toString(*ast, cli.showLib) << std::endl;

        IL il;
        if (cli.compile) {
            try {
                if (cli.verbose) std::cerr << "compile to intermediate language" << std::endl;
                il = compileToIL(ast);
            } catch (std::exception& e) {
                std::cerr << "Exception in compileToIL(ast): " << e.what() << std::endl;
            }

            if (cli.printIL)
                std::cerr << il << std::endl;
            
            if (cli.verbose) std::cerr << "compile to C" << std::endl;
            std::cout << compileToC(il, cli.trace) << std::endl;
        }

        if (!cli.compile) {
            if (cli.verbose) std::cerr << "run interpreter" << std::endl;
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
        }

        return 0;
    }
}