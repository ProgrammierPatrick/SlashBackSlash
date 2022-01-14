#ifndef _COMPILER_C_H
#define _COMPILER_C_H

#include "model/il.h"

#include <string>

std::string compileToC(const IL& il, bool traceExecution);

#endif