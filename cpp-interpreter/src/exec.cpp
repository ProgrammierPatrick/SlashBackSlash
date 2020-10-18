
#include "exec.h"
#include "sbsexception.h"
#include "model/list.h"
#include "model/to_string.h"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std::string_literals;

bool Exec::isDone() {
    return !running;
}

void Exec::step(bool skipLibSteps) {
    currentStep++;

    std::vector<std::shared_ptr<const AST>> stack { root };

    auto setNewNode = [&](const std::shared_ptr<AST>& ast) {
        if (stack.size() == 1)
            root = ast;
        else {
            auto& appNode = stack[stack.size() - 2];
            std::shared_ptr<AST> tmpAST = std::make_shared<AST>(AST::App(ast, appNode->getApp().second), appNode->loc, appNode->bindings);
            for(int i = stack.size() - 3; i >= 0; i--) {
                auto& currAppNode = stack[i];
                tmpAST = std::make_shared<AST>(AST::App(tmpAST, currAppNode->getApp().second), currAppNode->loc, currAppNode->bindings);
            }
            root = tmpAST;
        }
    };

    bool done = false;

    while (!done) {

        if (stack.back()->isLet()) {
            auto& let = stack.back()->getLet();

            List<Binding> nextList, valueList;
            if(let.next->getBindFromParent) nextList = stack.back()->bindings;
            if(let.value->getBindFromParent) valueList = stack.back()->bindings;
            valueList.append_front(let.value->bindings);

            std::shared_ptr<const AST> valueNode = std::make_shared<AST>(*let.value, valueList);

            nextList.push_front(Binding(let.name, valueNode, stack.back(), false));
            nextList.append_front(let.next->bindings);
            
            auto nextNode = std::make_shared<AST>(*let.next, nextList);
            setNewNode(nextNode);
            done = true;

            if (skipLibSteps && stack.back()->loc.fromLib) {
                step(skipLibSteps);
            }
        }
        else if (stack.back()->isAbs()) {
            if (stack.back() != root)
                throw SBSException(SBSException::Origin::RUNTIME, "encountered ABS not at root in execution.", stack.back()->loc);
            running = false;
            done = true;
        }
        else if (stack.back()->isVar()) {
            bool found = false;
            for(int i = stack.size() - 1; i >= 0; i--) {
                auto it = std::find_if(stack[i]->bindings.begin(), stack[i]->bindings.end(), [&](auto b) {
                    return *b.name == *stack.back()->getVar().name;
                });
                if(it != stack[i]->bindings.end()) {
                    setNewNode(std::make_shared<AST>(*it->value, false));
                    found = true;
                    break;
                }
                if(!stack[i]->getBindFromParent) break;
            }
            
            if(!found) {
                throw SBSException(SBSException::Origin::RUNTIME, "unbounded variable '" + *stack.back()->getVar().name + "' found.", stack.back()->loc);
                running = false;
            }

            done = true;
        }
        else if (stack.back()->isApp()) {
            if (stack.back()->getApp().first->isAbs()) {
                // this is beta-reduction
                auto& app = stack.back()->getApp();
                auto& abs = app.first->getAbs();
                auto& absNext = abs.ast;
                auto& appSecond = app.second;

                // add current bindings to both abstract implementation and appication second
                List<Binding> appSecondBindings, absNextBindings;
                for(int i = stack.size() - 1; i >= 0; i--) {
                    if(app.first->getBindFromParent)
                        absNextBindings.append_back(stack[i]->bindings);
                    if(app.second->getBindFromParent)
                        appSecondBindings.append_back(stack[i]->bindings);
                    if(!stack[i]->getBindFromParent) break;
                }
                appSecondBindings.append_front(appSecond->bindings);

                std::shared_ptr<const AST> bindingNode = std::make_shared<AST>(*appSecond, appSecondBindings);

                absNextBindings.append_front(app.first->bindings);
                absNextBindings.push_front(Binding(abs.name, bindingNode, stack.back(), true));
                absNextBindings.append_front(absNext->bindings);
                
                auto newNode = std::make_shared<AST>(*absNext, absNextBindings);
                setNewNode(newNode);
                done = true;
            } else {
                stack.push_back(stack.back()->getApp().first);
                done = false;
            }
        }
    }
}

std::string printStateImpl(const AST& node, bool showLib, bool showBindValues, bool parens = false);

std::string printBindings(const List<Binding>& bindings, bool showLib, bool showBindValues) {
    std::vector<Binding> validBindings;
    for(auto b : bindings) {
        if(showLib || !b.origin->loc.fromLib)
            validBindings.push_back(b);
    }
    if(validBindings.size() > 0) {
        std::string s("[");
        for(auto b : validBindings) {
            s += (b.fromBeta ? '\\' : '/') + *b.name;
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

    if (node.isVar())
        return s + *node.getVar().name;
    if (node.isAbs())
        return p() + s + "\\" + *node.getAbs().name + " " + printStateImpl(*node.getAbs().ast, showLib, showBindValues) + q();
    if (node.isLet()) {
        if(showLib || !node.loc.fromLib)
            return p() + s + "/" + *node.getLet().name + " " + printStateImpl(*node.getLet().value, showLib, showBindValues, true) + " " + printStateImpl(*node.getLet().next, showLib, showBindValues) + q();
        else return s + printStateImpl(*node.getLet().next, showLib, showBindValues, parens);
    }
    if (node.isApp())
        return p() + s + printStateImpl(*node.getApp().first, showLib, showBindValues, true) + " " + printStateImpl(*node.getApp().second, showLib, showBindValues, true) + q();
    
    return "ERROR_TYPE";
}

std::string Exec::printState(bool showLib, bool showBindValues) {
    // std::string s = printBindings(globalBindings, showLib);
    return "[" + std::to_string(currentStep) + "] " + printStateImpl(*root, showLib, showBindValues) + "\n" + toString(*root, showLib);
}