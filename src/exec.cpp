
#include "exec.h"
#include "sbsexception.h"
#include "model/list.h"
#include "model/to_string.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std::string_literals;

List<Binding> simplifyBinding(const List<Binding>& list) {
    std::unordered_set<std::string> addedBindings;
    List<Binding> bindings;
    int numRemoved = 0;
    int size = 0;
    for(auto b : list) {
        if(addedBindings.find(*b.name) == addedBindings.end()) {
            bindings.push_back(b);
            addedBindings.insert(*b.name);
        } else numRemoved++;
        size++;
    }
    std::cerr << "simplify: " << numRemoved << "/" << size << " removed." << std::endl;
    if(true || numRemoved == size) {
        for(auto b : list)
            std::cerr << toString(b) << " ";
        std::cerr << std::endl;
    }
    return bindings;
}

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
            valueList = List<Binding>::append_unique(let.value->bindings, valueList);

            std::shared_ptr<const AST> valueNode = std::make_shared<AST>(*let.value, valueList);

            nextList.push_front(Binding(let.name, valueNode, stack.back(), false));
            nextList = List<Binding>::append_unique(let.next->bindings, nextList);
            
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


            static std::shared_ptr<std::string> str_x = std::make_shared<std::string>("x");
            static std::shared_ptr<std::string> str_f = std::make_shared<std::string>("f");
            static std::shared_ptr<std::string> str_s = std::make_shared<std::string>("s");
            static FileLoc loc(std::make_shared<std::string>("_lib_io"), 0, 0, true);
            static std::shared_ptr<AST> ast_x = std::make_shared<AST>(AST::Var(str_x), loc);
            static std::shared_ptr<AST> ast_f = std::make_shared<AST>(AST::Var(str_f), loc);
            static std::shared_ptr<AST> ast_s = std::make_shared<AST>(AST::Var(str_s), loc);
            static std::shared_ptr<AST> ast_id = std::make_shared<AST>(AST::Abs(str_x, ast_x), loc);
            if(*stack.back()->getVar().name == "__IO_PUT_BEGIN") {
                io_put = 0;
                setNewNode(ast_id);
                found = true;
            } else if(*stack.back()->getVar().name == "__IO_PUT_INC") {
                io_put++;
                setNewNode(ast_id);
                found = true;
            } else if(*stack.back()->getVar().name == "__IO_PUT_DONE") {
                std::cout << io_put << std::flush;
                setNewNode(ast_id);
                found = true;
            } else if(*stack.back()->getVar().name == "__IO_EOF") {
                found = true;
                running = false;
            } else if(*stack.back()->getVar().name == "__IO_GET") {
                char c;
                std::cin.get(c);
                // cn = \f \z f (f (f z))
                auto cn = ast_x;
                for(int i = 0; i < c; i++)
                    cn = std::make_shared<AST>(AST::App(ast_f, cn), loc);
                cn = std::make_shared<AST>(AST::Abs(str_x, cn), loc);
                cn = std::make_shared<AST>(AST::Abs(str_f, cn), loc);
                // (\s s cn)
                auto ast = std::make_shared<AST>(AST::App(ast_s, cn), loc);
                ast = std::make_shared<AST>(AST::Abs(str_s, ast), loc);
                setNewNode(ast);
                found = true;
            }

            for(int i = stack.size() - 1; !found && i >= 0; i--) {
                auto it = std::find_if(stack[i]->bindings.begin(), stack[i]->bindings.end(), [&](auto b) {
                    return *b.name == *stack.back()->getVar().name;
                });
                if(it != stack[i]->bindings.end()) {
                    setNewNode(std::make_shared<AST>(*it->value, false));
                    found = true;
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
                size_t lowestStackIdx = stack.size() - 1;
                while(lowestStackIdx > 0 && stack[lowestStackIdx]->getBindFromParent)
                    lowestStackIdx--;

                List<Binding> appSecondBindings, absNextBindings;
                for(size_t i = lowestStackIdx; i < stack.size(); i++) {
                    if(app.first->getBindFromParent)
                        absNextBindings = List<Binding>::append_unique(stack[i]->bindings, absNextBindings);
                    if(app.second->getBindFromParent)
                        appSecondBindings = List<Binding>::append_unique(stack[i]->bindings, appSecondBindings);
                }

                appSecondBindings = List<Binding>::append_unique(appSecond->bindings, appSecondBindings);

                std::shared_ptr<const AST> bindingNode = std::make_shared<AST>(*appSecond, appSecondBindings);

                absNextBindings = List<Binding>::append_unique(app.first->bindings, absNextBindings);
                absNextBindings.push_front(Binding(abs.name, bindingNode, stack.back(), true));
                absNextBindings = List<Binding>::append_unique(absNext->bindings, absNextBindings);
                
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
    return "[" + std::to_string(currentStep) + "] " + printStateImpl(*root, showLib, showBindValues);// + "\n" + toString(*root, showLib);
}