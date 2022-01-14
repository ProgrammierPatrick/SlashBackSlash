#include "compiler_il.h"

#include "sbsexception.h"

#include <unordered_set>

// #define TRACE_COMPILER

#ifdef TRACE_COMPILER
#include <iostream>
int trace_indent_level = -1;
#define TRACE_INC_INDENT trace_indent_level++;
#define TRACE_DEC_INDENT trace_indent_level--;
#define TRACE(X) for(int trace_i = 0; trace_i < trace_indent_level; trace_i++) std::cerr << "| "; std::cerr << X << std::endl;
#define TRACEOP(X) std::visit([](auto& arg) { for(int trace_i = 0; trace_i < trace_indent_level; trace_i++) std::cerr << "| "; std::cerr << "> " << arg << "\n"; }, X);
#else
#define TRACE_INC_INDENT
#define TRACE_DEC_INDENT
#define TRACE(X)
#define TRACEOP(X)
#endif

using Section = IL::Section;
using std::shared_ptr;
using std::variant;
using std::vector;
using std::string;

struct Lambda {
    int var;
    Lambda(int var) : var(var) { }
};
struct Let {
    shared_ptr<Section> section;
    Let(const shared_ptr<Section>& section) : section(section) { }
};
struct Bind {
    string name;
    variant<Lambda, Let> data;
    Bind(const string& name, variant<Lambda, Let> data) : name(name), data(data) { }
};

// src: https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

vector<shared_ptr<Section>> parseSection(const string& name, shared_ptr<const AST> ast, vector<Bind> bindings);
vector<shared_ptr<Section>> getBuildinSections();
IL removeUnusedSections(const IL& il);

IL compileToIL(const shared_ptr<AST>& ast) {
    IL il;

    il.sections = getBuildinSections();
    vector<Bind> buildinBinds;
    for (auto& s : il.sections)
        buildinBinds.push_back(Bind(s->name, Let(s)));

    auto nonBuildinSections = parseSection("main", ast, buildinBinds);
    il.sections.insert(il.sections.end(), nonBuildinSections.begin(), nonBuildinSections.end());

    il.mainSection = il.sections.back();

    il = removeUnusedSections(il);

    return il;
}

void walkSections(const shared_ptr<Section>& section, std::unordered_set<shared_ptr<Section>>& usedSections) {
    if (usedSections.find(section) != usedSections.end())
        return;
    usedSections.insert(section);

    for (auto& line : section->sectionData) {
        if (std::holds_alternative<Section::JMP>(line)) walkSections(std::get<Section::JMP>(line).target, usedSections);
        if (std::holds_alternative<Section::PUSH>(line)) walkSections(std::get<Section::PUSH>(line).section, usedSections);
        if (std::holds_alternative<Section::PUSH_ALLOC>(line)) walkSections(std::get<Section::PUSH_ALLOC>(line).section, usedSections);
    }
}

IL removeUnusedSections(const IL& il) {
    std::unordered_set<shared_ptr<Section>> usedSections;
    walkSections(il.mainSection, usedSections);

    IL result;
    result.mainSection = il.mainSection;
    for (auto& section : il.sections)
        if (usedSections.find(section) != usedSections.end())
            result.sections.push_back(section);
    return result;
}

vector<shared_ptr<Section>> getBuildinSections() {
    vector<shared_ptr<Section>> sections;
    sections.push_back(std::make_shared<Section>("__IO_PUT_BEGIN", true));
    sections.push_back(std::make_shared<Section>("__IO_PUT_INC", true));
    sections.push_back(std::make_shared<Section>("__IO_PUT_DONE", true));
    sections.push_back(std::make_shared<Section>("__IO_EOF", true));
    sections.push_back(std::make_shared<Section>("__IO_GET", true));
    return sections;
}

vector<shared_ptr<Section>> parseSection(const string& name, shared_ptr<const AST> ast, vector<Bind> bindings) {
    TRACE_INC_INDENT
    TRACE("section " << name)
    shared_ptr<Section> section = std::make_shared<Section>(name);
    vector<shared_ptr<Section>> createdSections;

    int nextVar = 0;
    int nextPureSubsection = 0;
    int nextClosureSubsection = 0;

    bool reachedEnd = false;
    while (!reachedEnd) {
        std::visit( overloaded{
            [&](const AST::Abs& abs) {
                // ABS: POP var
                TRACE("ABS " << *abs.name)
                int var = nextVar++;
                section->sectionData.push_back(Section::POP(var));
                TRACEOP(section->sectionData.back());
                bindings.push_back(Bind(*abs.name, Lambda(var)));
                ast = abs.ast;
            }, [&](const AST::App& app) {
                if (app.second->isVar()) {
                    // APP with VAR: PUSH var number if bound in ABS, else PUSH section
                    auto& var = app.second->getVar();
                    TRACE("APP _ (VAR " << *var.name << ")")
                    auto it = std::find_if(bindings.rbegin(), bindings.rend(), [&](const Bind& bind) { return bind.name == *var.name; });
                    if (it != bindings.rend()) {
                        std::visit(overloaded{
                            [&](const Lambda& l) { section->sectionData.push_back(Section::PUSHVar(l.var)); },
                            [&](const Let& l) { section->sectionData.push_back(Section::PUSH(l.section)); }},
                            it->data);
                        TRACEOP(section->sectionData.back());
                    }
                    else {
                        std::cerr << "bindings:\n";
                        for (const Bind& bind : bindings) std::cerr << bind.name << "\n";
                        throw SBSException(SBSException::Origin::IL_COMPILER, "Target of VAR in APP could not be found: " + *var.name, app.second->loc);
                    }
                }
                else {
                    auto isPureExceptLets = [](const shared_ptr<const AST>& ast, const vector<Bind>& bindings) {
                        for (shared_ptr<const string>& unbound : ast->getUnboundVars()) {
                            auto it = std::find_if(bindings.rbegin(), bindings.rend(), [&](const Bind& bind) { return bind.name == *unbound; });
                            if (it != bindings.rend()) {
                                if (std::holds_alternative<Lambda>(it->data)) return false;
                            } else throw SBSException(SBSException::Origin::IL_COMPILER, "In isPureExceptLets(): unbound variable " + *unbound + " could not be found.", ast->loc);
                        }
                        return true;
                    };

                    if (isPureExceptLets(app.second, bindings)) {
                        // APP with pure lambda: create new section {name}_pure_{i} and PUSH it
                        string newName = name + "_l" + std::to_string(nextPureSubsection++);
                        TRACE("APP _ " << newName)
                        auto newSections = parseSection(newName, app.second, bindings);
                        TRACE("back in section " << name)
                        createdSections.insert(createdSections.end(), newSections.begin(), newSections.end());

                        section->sectionData.push_back(IL::Section::PUSH(newSections.back()));
                        TRACEOP(section->sectionData.back());
                    }
                    else {
                        // APP with non pure lambda: closure needs to be allocated at runtime. new section with lifted lambda is created.
                        shared_ptr<const AST> liftedAST = app.second;
                        vector<int> closureData;
                        for (shared_ptr<const string>& unbound : app.second->getUnboundVars()) {
                            auto it = std::find_if(bindings.rbegin(), bindings.rend(), [&](const Bind& bind) { return bind.name == *unbound; });
                            if (it != bindings.rend()) {
                                std::visit(overloaded{
                                    [&](const Lambda& l) {
                                        liftedAST = std::make_shared<AST>(AST::Abs(unbound, liftedAST), app.second->loc);
                                        closureData.push_back(l.var);
                                    },
                                    [](const Let&) { } },
                                    it->data);
                            } else throw SBSException(SBSException::Origin::IL_COMPILER, "In Closure-Generation: unbound variable " + *unbound + " could not be found", app.second->loc);
                        }
                        string newName = name + "_c" + std::to_string(nextClosureSubsection++);
                        TRACE("APP _ " << newName)
                        auto newSections = parseSection(newName, liftedAST, bindings);
                        TRACE("back in section " << name)
                        createdSections.insert(createdSections.end(), newSections.begin(), newSections.end());

                        section->sectionData.push_back(IL::Section::PUSH_ALLOC(newSections.back(), closureData));
                        TRACEOP(section->sectionData.back());
                    }
                }
                ast = app.first;
            }, [&](const AST::Let& let) {
                TRACE("LET " << *let.name)
                auto newSections = parseSection(*let.name, let.value, bindings);
                TRACE("back in section " << name)
                createdSections.insert(createdSections.end(), newSections.begin(), newSections.end());
                bindings.push_back(Bind(*let.name, Let(newSections.back())));
                ast = let.next;
            }, [&](const AST::Var& var) {
                TRACE("VAR " << *var.name)
                auto it = std::find_if(bindings.rbegin(), bindings.rend(), [&](const Bind& bind) { return bind.name == *var.name; });
                if (it != bindings.rend()) {
                    std::visit(overloaded{
                        [&](const Lambda& l) { section->sectionData.push_back(Section::JMPVar(l.var)); },
                        [&](const Let& l) { section->sectionData.push_back(Section::JMP(l.section)); } },
                        it->data);
                    TRACEOP(section->sectionData.back());
                }
                else throw SBSException(SBSException::Origin::IL_COMPILER, "Could not resolve variable " + *var.name, ast->loc);
                reachedEnd = true;
            }},
            ast->data);
    }
    createdSections.push_back(section);
    TRACE_DEC_INDENT
    return createdSections;
}
