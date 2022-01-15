#include "compiler_c.h"

#include "sbsexception.h"

#include <sstream>

// src: https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

std::string toSafeStr(const std::string& name) {
    std::string newName = name;
    auto replace = [&newName](const std::string& before, const std::string& after) {
        auto pos = newName.find(before);
        while(pos != std::string::npos) {
            newName.replace(pos, before.size(), after);
            pos = newName.find(before);
        }
    };
    replace(">", "_gt_");
    replace("<", "_lt_");
    replace("[", "_lb_");
    replace("]", "_rb_");
    replace("=", "_eq_");
    return newName;
}

std::string compileToC(const IL& il, bool traceExecution) {
    std::stringstream ss;

    ss << 
        "#include <stdint.h>\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "\n"
        "typedef struct {\n"
        "    void* func;\n"
        "    void* arg;\n"
        "} data_t;\n"
        "typedef data_t (*func_t)(void*);\n"
        "\n"
        "void stop() {\n"
        "    exit(0);\n"
        "}\n"
        "\n"
        "#define stack_size (1024 * 1024) // 128MB stack (at 64bit) for 1M entries \n"
        "data_t stack[stack_size];\n"
        "size_t stack_ptr = 0;\n"
        "data_t pop() {\n"
        "    if (stack_ptr == 0) stop();\n"
        "    return stack[--stack_ptr];\n"
        "}\n"
        "void push(func_t func, void* arg) {\n"
        "    stack[stack_ptr].func = func;\n"
        "    stack[stack_ptr++].arg = arg;\n"
        "}\n"
        "\n"
        "#define heap_size (16 * 1024 * 1024) // 128MB heap (at 64bit) for 16M entries (for 2 binds per closure: 4M closures)\n"
        "func_t heap[heap_size];\n"
        "size_t heap_ptr = 0;\n"
        "func_t* heap_alloc(size_t size) {\n"
        "    int largeEnough = 0;\n"
        "    while (!largeEnough) {\n"
        "        largeEnough = 1;\n"
        "        for (size_t i = 0; i < size; i++) {\n"
        "            if (heap[heap_ptr + i] != 0) {\n"
        "                largeEnough = 0;\n"
        "                heap_ptr += i + 1;\n"
        "                break;\n"
        "            }\n"
        "        }\n"
        "        while (heap[heap_ptr] != 0)\n"
        "            heap_ptr++;\n"
        "    }\n"
        "    func_t* alloced_ptr = &heap[heap_ptr];\n"
        "    heap_ptr += size;\n"
        "    return alloced_ptr;\n"
        "}\n"
        "\n"
        "data_t __call_closure(void* arg) {\n"
        "    func_t binded_func = ((func_t)arg) + 1;\n"
        "    while (binded_func)\n"
        "        push(binded_func++, 0);\n"
        "    data_t ret;\n"
        "    ret.func = (func_t)arg;\n"
        "    return ret;\n"
        "}\n\n";

    if (   std::find_if(il.sections.begin(), il.sections.end(), [](auto& s) { return s->name == "__IO_PUT_BEGIN"; }) != il.sections.end()
        || std::find_if(il.sections.begin(), il.sections.end(), [](auto& s) { return s->name == "__IO_PUT_INC"; }) != il.sections.end()
        || std::find_if(il.sections.begin(), il.sections.end(), [](auto& s) { return s->name == "__IO_PUT_DONE"; }) != il.sections.end())
        ss << "int io_put = 0;\n\n";
    
    bool addCallNum = std::find_if(il.sections.begin(), il.sections.end(), [](auto& s) { return s->name == "__IO_GET"; }) != il.sections.end();
    if (addCallNum)
        ss << 
            "data_t __call_num(void* arg) {\n"
            "    size_t num = (size_t)pop().func;\n"
            "    data_t f = pop();\n"
            "    data_t z = pop();\n"
            "    if (num != 0) {\n"
            "        func_t* h = heap_alloc(5);\n"
            "        h[0] = __call_closure;\n"
            "        h[1] = z.func;\n"
            "        h[2] = f.func;\n"
            "        h[3] = (func_t)(num - 1);\n"
            "        h[4] = 0;\n"
            "        push(__call_closure, h);\n"
            "        return f;\n"
            "    } else {\n"
            "        return z;\n"
            "    }\n"
            "}\n\n";

    for (auto& section : il.sections) {
        ss << "data_t _" << toSafeStr(section->name) << "(void* arg) {\n";
        if (section->isBuildIn) {
            if (section->name == "__IO_PUT_BEGIN") {
                ss << "    io_put = 0;\n";
                ss << "    return pop();\n";
            } else if (section->name == "__IO_PUT_INC") {
                ss << "    io_put++;\n";
                ss << "    return pop();\n";
            } else if (section->name == "__IO_PUT_DONE") {
                ss << "    putchar(io_put);\n";
                ss << "    return pop();\n";
            } else if (section->name == "__IO_EOF") {
                ss << "    exit(0);\n";
            } else if (section->name == "__IO_GET") {
                ss <<
                    "    int c = getchar();\n"
                    "    data_t s = pop();\n"
                    "    func_t* h = heap_alloc(5);\n"
                    "    h[0] = __call_num;\n"
                    "    h[1] = (func_t)c;\n"
                    "    h[2] = 0;\n"
                    "    push(__call_num, h);\n"
                    "    return s;\n";
            } else throw SBSException(SBSException::Origin::C_COMPILER, "Could not synthesize buildin function " + section->name, FileLoc(std::make_shared<std::string>("result.c"), 0, 0, false));
        } else {
            size_t nextHeapVar = 0;
            for (auto& line : section->sectionData) {
                std::visit(overloaded {
                    [&](const IL::Section::JMPVar& op)     { ss << "    return v" << op.targetVar << ";\n"; },
                    [&](const IL::Section::JMP& op)        { ss << "    data_t ret; ret.func = _" << toSafeStr(op.target->name) << "; return ret;\n"; },
                    [&](const IL::Section::PUSHVar& op)    { ss << "    push(v" << op.targetVar << ".func, v"<< op.targetVar << ".arg);\n"; },
                    [&](const IL::Section::POP& op)        { ss << "    data_t v" << op.var << " = pop();\n"; },
                    [&](const IL::Section::PUSH& op)       { ss << "    push(_" << toSafeStr(op.section->name) << ", 0);\n"; },
                    [&](const IL::Section::PUSH_ALLOC& op) {
                        auto var = nextHeapVar++;
                        ss << "    func_t* h" << var << " = heap_alloc(" << op.pushVars.size() + 2 << ");\n";
                        ss << "    h" << var << "[0] = _" << toSafeStr(op.section->name) << ";\n";
                        for (int i = 0; i < op.pushVars.size(); i++)
                            ss << "    h" << var << "[" << i + 1 << "] = v" << op.pushVars[i] << ".func;\n";
                        ss << "    h" << var << "[" << op.pushVars.size() + 1 << "] = 0;\n";
                        ss << "    push(__call_closure, h" << var << ");\n";
                    }
                }, line);
            }
        }
        
        ss << "}\n";
    }
    ss << "\n";

    if (traceExecution) {
        ss <<
            "struct func_str_pair {\n"
            "    void* func;\n"
            "    const char* str;\n"
            "};\n"
            "const struct func_str_pair funcToStr[] = {\n";
        for(auto& section : il.sections)
            ss << "    { _" << toSafeStr(section->name) << ", \"" << section->name << "\" },\n";
        ss << "};\n\n";
    }

    if (traceExecution) ss <<
        "void print_str(func_t f, func_t* arg, int depth) {\n"
        "    if (depth == 0) { printf(\"...\"); return; }\n"
        "    if (f == __call_closure) {\n"
        "        printf(\"[\");\n"
        "        if (arg == 0) { printf(\"NULL]\"); print_str(f, 0, depth - 1); return; }\n"
        "        for(func_t *p = arg+1; *p; p++) {\n"
        "            print_str(*p, 0, depth - 1);\n"
        "            if (*(p + 1)) printf(\",\");\n"
        "        }\n"
        "        printf(\"]\");\n"
        "        print_str(*arg, 0, depth - 1);\n"
        "    } else {\n"
        "        for (size_t i = 0; i < sizeof(funcToStr) / sizeof(funcToStr[0]); i++)\n"
        "            if (funcToStr[i].func == f) {\n"
        "                printf(\"%s\", funcToStr[i].str);\n"
        "                return;\n"
        "            }\n"
        "        printf(\"?\");\n"
        "    }"
        "}\n\n";

    ss <<
        "int main() {\n"
        "    data_t next_section;\n"
        "    next_section.func = _main;\n"
        "    while(1) {\n";
    if (traceExecution)
        ss <<
            "        print_str(next_section.func, next_section.arg, 5);\n"
            "        printf(\" \");\n"
            "        for (int i = stack_ptr - 1; i >= 0; i--) { print_str(stack[i].func, stack[i].arg, 4); printf(\" \"); }\n"
            "        printf(\"\\n\");\n";
    ss <<
        "        next_section = ((func_t)next_section.func)(next_section.arg);\n"
        "    }\n"
        "}\n";

    return ss.str();
}