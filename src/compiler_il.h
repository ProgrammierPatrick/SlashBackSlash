#ifndef _COMPILER_IL_H
#define _COMPILER_IL_H

#include "model/ast.h"
#include "model/il.h"

#include <memory>

IL compileToIL(const std::shared_ptr<AST>& ast);

#endif