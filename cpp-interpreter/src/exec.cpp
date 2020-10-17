
#include "exec.h"
#include "sbsexception.h"

#include <iostream>
#include <algorithm>

using namespace std::string_literals;

bool Exec::isDone() {
    return !running;
}

void Exec::step(bool skipLibSteps) {
    currentStep++;

    std::vector<std::shared_ptr<AST>> stack { root };

    auto getBindings = [&]() -> std::vector<AST::Binding>& { return stack.back()->bindings; };
    auto setNewNode = [&](const std::shared_ptr<AST>& ast) {
        if (stack.size() == 1)
            root = ast;
        else
            stack[stack.size() - 2]->app.first = ast;
    };

    bool done = false;

    while (!done) {

        if (stack.back()->type == AST::Type::LET) {
            for(auto b : getBindings()) {
                stack.back()->let.next->bindings.push_back(b);
                stack.back()->let.value->bindings.push_back(b);
            }
            stack.back()->let.next->bindings.push_back(AST::Binding(stack.back()->let.name, stack.back()->let.value, stack.back()->fromLib, false));
            setNewNode(stack.back()->let.next);
            done = true;

            if (skipLibSteps && stack.back()->fromLib) {
                step(skipLibSteps);
            }
        }
        else if (stack.back()->type == AST::Type::ABS) {
            if (stack.back() != root)
                throw SBSException(SBSException::Origin::RUNTIME, "encountered ABS not at root in execution.", stack.back()->file, stack.back()->line, stack.back()->pos);
            running = false;
            done = true;
        }
        else if (stack.back()->type == AST::Type::VAR) {
            bool found = false;
            // for(int i = stack.size()-1; !found && (i >= -1); i--) {
            for(int i = stack.size()-1; !found && (i >= 0); i--) {
                std::vector<AST::Binding>& bindings = stack[i]->bindings;
                auto it = std::find_if(bindings.begin(), bindings.end(), [&](auto p) {
                    return p.name == stack.back()->var.name;
                });
                if(it != bindings.end()) {
                    setNewNode(it->value);
                    found = true;
                }
            }
            if(!found) {
                throw SBSException(SBSException::Origin::RUNTIME, "unbounded variable '"s + stack.back()->var.name + "' found.", stack.back()->file, stack.back()->line, stack.back()->pos);
                running = false;
            }

            done = true;
        }
        else if (stack.back()->type == AST::Type::APP) {
            if (stack.back()->app.first->type == AST::Type::ABS) {
                // this is beta-reduction
                auto& absBindings = stack.back()->app.first->bindings;
                auto& absAstBindings = stack.back()->app.first->abs.ast->bindings;
                absAstBindings.insert(absAstBindings.end(), absBindings.begin(), absBindings.end());
                absAstBindings.push_back(AST::Binding(stack.back()->app.first->abs.name, stack.back()->app.second, stack.back()->fromLib, true));
                setNewNode(stack.back()->app.first->abs.ast);
                done = true;
            } else {
                stack.push_back(stack.back()->app.first);
                done = false;
            }
        }
    }
}

std::string printStateImpl(const AST& node, bool showLib, bool showBindValues, bool parens = false);

std::string printBindings(const std::vector<AST::Binding>& bindings, bool showLib, bool showBindValues) {
    std::vector<AST::Binding> validBindings;
    for(auto b : bindings) {
        if(showLib || !b.fromLib)
            validBindings.push_back(b);
    }
    if(validBindings.size() > 0) {
        std::string s("[");
        for(auto b : validBindings) {
            s += (b.fromApp ? '\\' : '/') + b.name;
            if(showBindValues) {
                s += ":(";
                s += printStateImpl(*b.value, showLib, showBindValues);
                s += ")";
            }
        }
        return s + "] ";
    } else return "";
}

std::string printStateImpl(const AST& node, bool showLib, bool showBindValues, bool parens) {
    auto p = [parens]() -> std::string { return parens ? "(" : ""; };
    auto q = [parens]() -> std::string { return parens ? ")" : ""; };

    std::string s = printBindings(node.bindings, showLib, showBindValues);

    if (node.type == AST::Type::VAR)
        return s + node.var.name;
    if (node.type == AST::Type::ABS)
        return p() + s + "\\" + node.abs.name + " " + printStateImpl(*node.abs.ast, showLib, showBindValues) + q();
    if (node.type == AST::Type::LET) {
        if(showLib || !node.fromLib)
            return p() + s + "/" + node.let.name + " " + printStateImpl(*node.let.value, showLib, showBindValues, true) + " " + printStateImpl(*node.let.next, showLib, showBindValues) + q();
        else return s + printStateImpl(*node.let.next, showLib, showBindValues, parens);
    }
    if (node.type == AST::Type::APP)
        return p() + s + printStateImpl(*node.app.first, showLib, showBindValues, true) + " " + printStateImpl(*node.app.second, showLib, showBindValues, true) + q();
    
    return "ERROR_TYPE";
}

std::string Exec::printState(bool showLib, bool showBindValues) {
    // std::string s = printBindings(globalBindings, showLib);
    return "[" + std::to_string(currentStep) + "] " + printStateImpl(*root, showLib, showBindValues);
}