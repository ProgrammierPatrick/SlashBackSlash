#ifndef _AST_H
#define _AST_H

// AST: implemented as labeled union

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

struct AST;
std::string ASTToString(const AST& ast, bool showLib, int i);

struct AST {

    enum class Type {
        VAR, APP, LET, ABS
    };

    Type type;

    union {
        struct {
            std::string name;
        } var;
        struct {
            std::string name;
            std::shared_ptr<AST> value;
            std::shared_ptr<AST> next;
        } let;
        struct {
            std::shared_ptr<AST> first;
            std::shared_ptr<AST> second;
        } app;
        struct {
            std::string name;
            std::shared_ptr<AST> ast;
        } abs;
    };

    std::string file;
    int line;
    int pos;
    bool fromLib;


    struct Binding {
        std::string name;
        std::shared_ptr<AST> value;
        bool fromLib;
        bool fromApp;
        Binding(const std::string& name, const std::shared_ptr<AST>& value, bool fromLib, bool fromApp)
            : name(name), value(value), fromLib(fromLib), fromApp(fromApp) {}
    };

    std::vector<Binding> bindings;

    AST(Type type, std::string file, int line, int pos, bool fromLib)
        : type(type), file(file), line(line), pos(pos), fromLib(fromLib) {
        if(type == Type::VAR) {
            new (&var.name) std::string();
        } else if (type == Type::LET) {
            new (&let.name) std::string();
            new (&let.value) std::shared_ptr<AST>();
            new (&let.next) std::shared_ptr<AST>();
        } else if (type == Type::APP) {
            new (&app.first) std::shared_ptr<AST>();
            new (&app.second) std::shared_ptr<AST>();
        } else if (type == Type::ABS) {
            new (&abs.name) std::string();
            new (&abs.ast) std::shared_ptr<AST>();
        }
    }

    AST(const AST& other) {
        type = other.type;
        file = other.file;
        line = other.line;
        pos = other.pos;
        fromLib = other.fromLib;
        bindings = other.bindings;
        switch (type){
            case Type::ABS:
                new (&abs.name) std::string(other.abs.name);
                new (&abs.ast) std::shared_ptr<AST>(other.abs.ast);
                break;
            case Type::APP:
                new (&app.first) std::shared_ptr<AST>(other.app.first);
                new (&app.second) std::shared_ptr<AST>(other.app.second);
                break;
            case Type::LET:
                new (&let.name) std::string(other.let.name);
                new (&let.value) std::shared_ptr<AST>(other.let.value);
                new (&let.next) std::shared_ptr<AST>(other.let.next);
                break;
            case Type::VAR:
                new (&var.name) std::string(other.var.name);
                break;
        }

    }

    ~AST() {
        using std::string;
        if (type == Type::VAR) {
            var.name.~string();
        } else if (type == Type::LET) {
            let.name.~string();
            let.value.~shared_ptr();
            let.next.~shared_ptr();
        } else if (type == Type::APP) {
            app.first.~shared_ptr();
            app.second.~shared_ptr();
        } else if (type == Type::ABS) {
            abs.name.~string();
            abs.ast.~shared_ptr();
        }
    }
    static bool equals(const AST& a, const AST& b, std::vector<std::string> abstractionsA = {}, std::vector<std::string> abstractionsB = {}) {
        std::unique_ptr<AST> l = std::make_unique<AST>(a);
        std::unique_ptr<AST> r = std::make_unique<AST>(b);

        // reduce let, bound variables
        auto simplify = [](std::unique_ptr<AST>& a) {
            while (a->type == Type::LET || a->type == Type::VAR) {
                if(a->type == Type::LET) {
                    AST a2 = *a->let.next;
                    a2.bindings.push_back(Binding(a->let.name, a->let.value, false, false));
                    for(auto b : a->bindings) a2.bindings.push_back(b);
                    a = std::make_unique<AST>(a2);
                }
                if(a->type == Type::VAR) {
                    auto it = std::find_if(a->bindings.begin(), a->bindings.end(), [&](auto p) {
                        return p.name == a->var.name;
                    });
                    if(it != a->bindings.end()) {
                        a = std::make_unique<AST>(*it->value);
                    } else return;
                }
            }
        };
        simplify(l);
        simplify(r);
        
        if(l->type != r->type) return false;

        switch(l->type) {
            case Type::ABS: {
                abstractionsA.push_back(l->abs.name);
                abstractionsB.push_back(r->abs.name);
                auto nextA = std::make_unique<AST>(*l->abs.ast);
                auto nextB = std::make_unique<AST>(*r->abs.ast);
                for(auto b : l->bindings) nextA->bindings.push_back(b);
                for(auto b : r->bindings) nextB->bindings.push_back(b);
                return equals(*nextA, *nextB, abstractionsA, abstractionsB);
            }
            case Type::APP: {
                auto nextA1 = std::make_unique<AST>(*l->app.first);
                auto nextB1 = std::make_unique<AST>(*r->app.first);
                auto nextA2 = std::make_unique<AST>(*l->app.second);
                auto nextB2 = std::make_unique<AST>(*r->app.second);
                for(auto b : l->bindings) nextA1->bindings.push_back(b);
                for(auto b : r->bindings) nextB1->bindings.push_back(b);
                for(auto b : l->bindings) nextA2->bindings.push_back(b);
                for(auto b : r->bindings) nextB2->bindings.push_back(b);
                return equals(*nextA1, *nextB1, abstractionsA, abstractionsB) && equals(*nextA2, *nextB2, abstractionsA, abstractionsB);
            }
            case Type::LET:
                throw std::runtime_error("let should not appear here.");
            case Type::VAR:
                auto getLastIndex = [](const std::vector<std::string>& abstractions, std::string name) {
                    for(int i = abstractions.size() - 1; i >= 0; i--)
                        if(abstractions[i] == name) return i;
                    return -1;
                };
                int i1 = getLastIndex(abstractionsA, l->var.name);
                int i2 = getLastIndex(abstractionsB, r->var.name);
                if(i1 != i2) return false;
                if(i1 == -1) return l->var.name == r->var.name;
                return true;
        }
        throw std::runtime_error("reached end of AST::equals()");
    }
};

#endif