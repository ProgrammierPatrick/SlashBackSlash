#ifndef _EXEC_H
#define _EXEC_H

#include "ast.h"

#include <variant>
#include <memory>
#include <vector>
#include <string>

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
    // std::vector<AST::Binding> globalBindings;

    bool running = true;
    int currentStep = 0;
};

#endif



/*
/ true (\t \t t)
/ false (\t \f f)

false (\x x x) (\x x x x)
:: insert binding (because no app with abs to the left)
(\t \f f) (\x x x) (\x x x x)
*/