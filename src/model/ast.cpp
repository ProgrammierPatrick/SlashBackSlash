#include "ast.h"

#include <vector>
#include <iostream>

std::shared_ptr<const AST> alphaEquivSimplify(const std::shared_ptr<const AST>& a) {
    if (a->isLet()) {
        List<Binding> nextList = a->getLet().next->bindings;
        List<Binding> valList = a->getLet().value->bindings;
        for(auto b : a->bindings) {
            if(a->getLet().next->getBindFromParent) nextList.push_front(b);
            if(a->getLet().value->getBindFromParent) valList.push_front(b);
        }
        std::shared_ptr<const AST> valNode = std::make_shared<AST>(*a->getLet().value, valList);
        valNode = alphaEquivSimplify(valNode);
        Binding newBind(a->getLet().name, valNode, a, false);
        nextList.push_front(newBind);
        auto nextNode = std::make_shared<AST>(*a->getLet().next, nextList);
        return alphaEquivSimplify(nextNode);
    }
    if (a->isVar()) {
        for(auto b : a->bindings) {
            if (*b.name == *a->getVar().name) {
                return alphaEquivSimplify(b.value);
            }
        }
    }
    return a;
}

bool alphaEquivImpl(const AST& a, const AST& b, std::vector<std::string> abstractionsA, std::vector<std::string> abstractionsB) {
    std::shared_ptr<const AST> l = alphaEquivSimplify(std::make_unique<AST>(a));
    std::shared_ptr<const AST> r = alphaEquivSimplify(std::make_unique<AST>(b));

    if (l->isAbs() && r->isAbs()) {
        abstractionsA.push_back(*l->getAbs().name);
        abstractionsB.push_back(*r->getAbs().name);
        List<Binding> nextABindings(l->getAbs().ast->bindings);
        List<Binding> nextBBindings(r->getAbs().ast->bindings);
        nextABindings.append_front(l->bindings);
        nextBBindings.append_front(r->bindings);
        auto nextA = std::make_shared<AST>(*l->getAbs().ast, nextABindings);
        auto nextB = std::make_shared<AST>(*r->getAbs().ast, nextBBindings);
        return alphaEquivImpl(*nextA, *nextB, abstractionsA, abstractionsB);
    } else if (l->isApp() && r->isApp()) {
        List<Binding> nextA1List(l->getApp().first->bindings);
        List<Binding> nextB1List(r->getApp().first->bindings);
        List<Binding> nextA2List(l->getApp().second->bindings);
        List<Binding> nextB2List(r->getApp().second->bindings);
        nextA1List.append_front(l->bindings);
        nextB1List.append_front(r->bindings);
        nextA2List.append_front(l->bindings);
        nextB2List.append_front(r->bindings);
        auto nextA1 = std::make_shared<AST>(*l->getApp().first,  nextA1List);
        auto nextB1 = std::make_shared<AST>(*r->getApp().first,  nextB1List);
        auto nextA2 = std::make_shared<AST>(*l->getApp().second, nextA2List);
        auto nextB2 = std::make_shared<AST>(*r->getApp().second, nextB2List);
        return alphaEquivImpl(*nextA1, *nextB1, abstractionsA, abstractionsB)
            && alphaEquivImpl(*nextA2, *nextB2, abstractionsA, abstractionsB);
    } else if(l->isLet() && r->isLet()) {
        throw std::runtime_error("Let should not appear here.");
    } else if(r->isVar() && r->isVar()) {
        auto getLastIndex = [](const std::vector<std::string>& abstractions, std::string name) {
            for(int i = abstractions.size() - 1; i >= 0; i--)
                if(abstractions[i] == name) return i;
            return -1;
        };
        int i1 = getLastIndex(abstractionsA, *l->getVar().name);
        int i2 = getLastIndex(abstractionsB, *r->getVar().name);
        if(i1 != i2) return false;
        if(i1 == -1) return *l->getVar().name == *r->getVar().name;
        return true;
    }
    return false;
}

bool AST::alphaEquiv(const AST& a, const AST& b) {
    return alphaEquivImpl(a, b, {}, {});
}