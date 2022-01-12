#ifndef _AST_H
#define _AST_H

#include <memory>
#include <string>
#include <variant>

#include "binding.h"
#include "file_loc.h"
#include "list.h"

struct AST {
    struct Var {
        using str_ptr = std::shared_ptr<const std::string>;
        const str_ptr name;
        Var(const str_ptr& name) : name(name) { }
    };
    struct Let {
        using str_ptr = std::shared_ptr<const std::string>;
        using ast_ptr = std::shared_ptr<const AST>;
        const str_ptr name;
        const ast_ptr value;
        const ast_ptr next;
        Let(const str_ptr& name, const ast_ptr& value, const ast_ptr& next) : name(name), value(value), next(next) { }
    };
    struct App {
        using ast_ptr = std::shared_ptr<const AST>;
        const std::shared_ptr<const AST> first;
        const std::shared_ptr<const AST> second;
        App(const ast_ptr& first, const ast_ptr& second) : first(first), second(second) { }
    };
    struct Abs {
        using str_ptr = std::shared_ptr<const std::string>;
        using ast_ptr = std::shared_ptr<const AST>;
        const std::shared_ptr<const std::string> name;
        const std::shared_ptr<const AST> ast;
        Abs(const str_ptr& name, const ast_ptr& ast) : name(name), ast(ast) { }
    };

    const std::variant<Var,Let,App,Abs> data;
    const FileLoc loc;
    const bool getBindFromParent = true;

    // ordering: first binding is strongest
    const List<Binding> bindings;

    static bool alphaEquiv(const AST& a, const AST& b);

    bool isPure() const;
    std::vector<std::shared_ptr<const std::string>> getUnboundVars() const;

    bool isVar() const { return std::holds_alternative<Var>(data); }
    bool isLet() const { return std::holds_alternative<Let>(data); }
    bool isApp() const { return std::holds_alternative<App>(data); }
    bool isAbs() const { return std::holds_alternative<Abs>(data); }

    const Var& getVar() const { if(!isVar()) throw std::runtime_error("AST::getVar() called on non-var node."); return std::get<Var>(data); }
    const Let& getLet() const { if(!isLet()) throw std::runtime_error("AST::getLet() called on non-let node."); return std::get<Let>(data); }
    const App& getApp() const { if(!isApp()) throw std::runtime_error("AST::getApp() called on non-app node."); return std::get<App>(data); }
    const Abs& getAbs() const { if(!isAbs()) throw std::runtime_error("AST::getAbs() called on non-abs node."); return std::get<Abs>(data); }

    AST(const Var& val, FileLoc loc, const List<Binding>& bindings = { }) : data(val), loc(loc), bindings(bindings) { }
    AST(const Let& val, FileLoc loc, const List<Binding>& bindings = { }) : data(val), loc(loc), bindings(bindings) { }
    AST(const App& val, FileLoc loc, const List<Binding>& bindings = { }) : data(val), loc(loc), bindings(bindings) { }
    AST(const Abs& val, FileLoc loc, const List<Binding>& bindings = { }) : data(val), loc(loc), bindings(bindings) { }

    AST(const AST& other, const List<Binding>& bindings)
        : data(other.data), loc(other.loc), getBindFromParent(other.getBindFromParent), bindings(bindings) { }

    AST(const AST& other, bool getBindFromParent)
        : data(other.data), loc(other.loc), getBindFromParent(getBindFromParent), bindings(other.bindings) { }


};

#endif