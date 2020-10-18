#ifndef _EXEC_H
#define _EXEC_H

#include <memory>
#include <string>

#include "model/ast.h"

class Exec {
public:
    Exec(const std::shared_ptr<AST>& ast) {
        root = ast;
    }

    void step(bool skipLibSteps);
    bool isDone();

    std::string printState(bool showLib, bool showBindValues = false);
    std::shared_ptr<AST> getRoot() { return root; }

private:

    std::shared_ptr<AST> root;

    bool running = true;
    int currentStep = 0;

    char io_put = '\0';
};

#endif