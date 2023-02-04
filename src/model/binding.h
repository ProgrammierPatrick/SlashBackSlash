#ifndef _BINDING_H
#define _BINDING_H

#include <memory>
#include <string>

#include "ast.h"

struct AST;

struct Binding {
    std::string_view name;
    const std::shared_ptr<const AST> value;

    const std::shared_ptr<const AST> origin; // if fromBeta, the app-node which constructed it. else, the let-node

    bool fromBeta; // if binding is from beta reduction. Else, it is from let construct

    Binding(std::string_view name, std::shared_ptr<const AST>& value, std::shared_ptr<const AST> origin, bool fromBeta)
        : name(name), value(value), origin(origin), fromBeta(fromBeta) { }
};

#endif