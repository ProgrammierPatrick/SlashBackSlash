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
            if (b.name == a->getVar().name) {
                return alphaEquivSimplify(b.value);
            }
        }
    }
    return a;
}

bool alphaEquivImpl(const AST& a, const AST& b, const std::vector<std::string_view>& abstractionsA, const std::vector<std::string_view>& abstractionsB) {
    std::shared_ptr<const AST> l = alphaEquivSimplify(std::make_unique<AST>(a));
    std::shared_ptr<const AST> r = alphaEquivSimplify(std::make_unique<AST>(b));

    if (l->isAbs() && r->isAbs()) {
        auto nextAbstractionsA = abstractionsA;
        auto nextAbstractionsB = abstractionsB;
        nextAbstractionsA.push_back(l->getAbs().name);
        nextAbstractionsB.push_back(r->getAbs().name);
        List<Binding> nextABindings(l->getAbs().ast->bindings);
        List<Binding> nextBBindings(r->getAbs().ast->bindings);
        nextABindings.append_front(l->bindings);
        nextBBindings.append_front(r->bindings);
        AST nextA{ *l->getAbs().ast, nextABindings };
        AST nextB{ *r->getAbs().ast, nextBBindings };
        return alphaEquivImpl(nextA, nextB, nextAbstractionsA, nextAbstractionsB);
    } else if (l->isApp() && r->isApp()) {
        List<Binding> nextA1List(l->getApp().first->bindings);
        List<Binding> nextB1List(r->getApp().first->bindings);
        List<Binding> nextA2List(l->getApp().second->bindings);
        List<Binding> nextB2List(r->getApp().second->bindings);
        nextA1List.append_front(l->bindings);
        nextB1List.append_front(r->bindings);
        nextA2List.append_front(l->bindings);
        nextB2List.append_front(r->bindings);
        AST nextA1{ *l->getApp().first,  nextA1List };
        AST nextB1{ *r->getApp().first,  nextB1List };
        AST nextA2{ *l->getApp().second, nextA2List };
        AST nextB2{ *r->getApp().second, nextB2List };
        return alphaEquivImpl(nextA1, nextB1, abstractionsA, abstractionsB)
            && alphaEquivImpl(nextA2, nextB2, abstractionsA, abstractionsB);
    } else if(l->isLet() && r->isLet()) {
        throw std::runtime_error("Let should not appear here.");
    } else if(r->isVar() && r->isVar()) {
        auto getLastIndex = [](const std::vector<std::string_view>& abstractions, std::string_view name) -> int {
            for(int i = static_cast<int>(abstractions.size()) - 1; i >= 0; i--)
                if(abstractions[i] == name) return i;
            return -1;
        };
        int i1 = getLastIndex(abstractionsA, l->getVar().name);
        int i2 = getLastIndex(abstractionsB, r->getVar().name);
        if(i1 != i2) return false;
        if(i1 == -1) return l->getVar().name == r->getVar().name;
        return true;
    }
    return false;
}

bool AST::alphaEquiv(const AST& a, const AST& b) {
    return alphaEquivImpl(a, b, {}, {});
}

std::vector<std::string_view> ASTGetUnboundVars(const AST& ast, const std::vector<std::string_view>& boundVars) {
    if (ast.isAbs()) {
        auto bound { boundVars };
        bound.push_back(ast.getAbs().name);
        return ASTGetUnboundVars(*ast.getAbs().ast, bound);
    }
    if (ast.isApp()) {
        auto ret = ASTGetUnboundVars(*ast.getApp().first, boundVars);
        for (auto& var : ASTGetUnboundVars(*ast.getApp().second, boundVars))
            if (std::find(ret.begin(), ret.end(), var) == ret.end())
                ret.push_back(var);
        return ret;
    }
    if (ast.isLet()) {
        auto bound { boundVars };
        bound.push_back(ast.getAbs().name);
        auto ret = ASTGetUnboundVars(*ast.getLet().value, boundVars);
        for (auto& var : ASTGetUnboundVars(*ast.getLet().next, bound))
            if (std::find(ret.begin(), ret.end(), var) == ret.end())
                ret.push_back(var);
        return ret;
    }
    if (ast.isVar()) {
        if (std::find(boundVars.begin(), boundVars.end(), ast.getVar().name) == boundVars.end())
            return { ast.getVar().name };
        else return { };
    }

    throw std::runtime_error("ASTGetUnboundVars: undefined AST type encountered.");
}

std::vector<std::string_view> AST::getUnboundVars() const {
    return ASTGetUnboundVars(*this, { });
}

bool ASTisPure(const AST& ast, const std::vector<std::string_view>& boundVars) {
    if (ast.isAbs()) {
        auto bound { boundVars };
        bound.push_back(ast.getAbs().name);
        return ASTisPure(*ast.getAbs().ast, bound);
    }
    if (ast.isApp()) {
        return ASTisPure(*ast.getApp().first, boundVars) && ASTisPure(*ast.getApp().second, boundVars);
    }
    if (ast.isLet()) {
        auto bound { boundVars };
        bound.push_back(ast.getAbs().name);
        return ASTisPure(*ast.getLet().value, boundVars) && ASTisPure(*ast.getLet().next, bound);
    }
    if (ast.isVar()) {
        return std::find(boundVars.begin(), boundVars.end(), ast.getVar().name) != boundVars.end();
    }
    throw std::runtime_error("ASTisPure: undefined AST type encountered.");
}

bool AST::isPure() const {
    return ASTisPure(*this, { });
}