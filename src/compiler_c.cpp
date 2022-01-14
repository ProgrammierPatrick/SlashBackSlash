#include "compiler_c.h"

#include <sstream>

// src: https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


std::string compileToC(const IL& il, bool traceExecution) {
    std::stringstream ss;

    ss <<
        "#include <stdint.h>\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n\n";

    // #ifdef _WIN32
    // ss << "#include <windows.h>\n";
    // #endif

    ss <<
        "void* stack[1024 * 1024]; // 64MB stack at 64bit\n"
        "size_t stackPtr = 0;\n"
        "void* pop() {if (stackPtr == 0) exit(0); return stack[--stackPtr]; }\n"
        "void push(void* val) { stack[stackPtr++] = val; }\n\n";

    ss <<
        "void** heapPtrStack[64 * 1024]; // 4MB heap ptr stack at 64bit\n"
        "size_t heapStackPtr = 0;\n"
        "\n"
        "void* heap[64 * 1024 * 1024]; // 64MB heap data storage at 64bit (each entry uses num bindings + 2 words) \n"
        "size_t heapPtr = 0;\n";
    //     "void init_heap() {\n";
    // // #ifdef _WIN32
    // // ss <<
    // //     "    // src: https://stackoverflow.com/a/40937610\n"
    // //     "    SYSTEM_INFO system_info;\n"
    // //     "    GetSystemInfo(&system_info);\n"
    // //     "    DWORD page_size = system_info.dwPageSize;\n"
    // //     "    heap = (void**)VirtualAlloc(((void*)0), page_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);\n";
    // // #endif
    // ss << "    heap = malloc(64 * 1024 * 1024);"
    // ss << "}\nvoid free_heap() {\n";
    // #ifdef _WIN32
    // ss << "    VirtualFree(heap, 0, MEM_RELEASE);\n";
    // #endif
    // ss << "}\n\n";

    ss <<
        "void** heap_alloc(size_t size) {\n"
        "    int largeEnough = 0;\n"
        "    while (!largeEnough) {\n"
        "        int largeEnough = 1;\n"
        "        for (size_t i = 0; i < size; i++) {\n"
        "            if (heap[heapPtr + i] != (void*)0) {\n"
        "                largeEnough = 0;\n"
        "                heapPtr += i + 1;\n"
        "                break;\n"
        "            }\n"
        "        }\n"
        "        while (heap[heapPtr] != (void*)0)\n"
        "            heapPtr++;\n"
        "    }\n"
        "    void** alloced_ptr = &heap[heapPtr];\n"
        "    heapPtr += size;\n"
        "    return alloced_ptr;\n"
        "}\n\n";

    ss <<
        "void* __exec_closure() {\n"
        "    void** heapPtr = heapPtrStack[--heapStackPtr];\n"
        "    void* func = *(heapPtr++);\n"
        "    while (heapPtr != (void*)0) {\n"
        "        push(*(heapPtr++));\n"
        "    }\n"
        "    return func;\n"
        "}\n\n";

    for (auto& section : il.sections) {
        ss << "void* _" << section->name << "() {\n";
        size_t nextHeapVar = 0;
        for (auto& line : section->sectionData) {
            std::visit(overloaded {
                [&](const IL::Section::JMPVar& op)     { ss << "    return v" << op.targetVar << ";\n"; },
                [&](const IL::Section::JMP& op)        { ss << "    return (void*)_" << op.target->name << ";\n"; },
                [&](const IL::Section::PUSHVar& op)    { ss << "    push(v" << op.targetVar << ");\n"; },
                [&](const IL::Section::PUSH& op)       { ss << "    push((void*)_" << op.section->name << ");\n"; },
                [&](const IL::Section::POP& op)        { ss << "    void* v" << op.var << " = pop();\n"; },
                [&](const IL::Section::PUSH_ALLOC& op) {
                    auto var = nextHeapVar++;
                    ss << "    void** h" << var << " = heap_alloc(" << op.pushVars.size() + 2 << ");\n";
                    ss << "    h" << var << "[0] = (void*)_" << op.section->name << ";\n";
                    for (int i = 0; i < op.pushVars.size(); i++)
                        ss << "    h" << var << "[" << i + 1 << "] = v" << op.pushVars[i] << ";\n";
                    ss << "    h" << var << "[" << op.pushVars.size() + 1 << "] = (void*)0;\n";
                    ss << "    push((void*)__exec_closure);\n";
                }
            }, line);
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
            ss << "    { _" << section->name << ", \"" << section->name << "\" },\n";
        ss << "};\n\n";
    }

    ss <<
        "int main() {\n"
        "    void* next_section = (void*)_main;\n"
        "    while(1) {\n";
    if (traceExecution)
        ss <<
            "        for (size_t i = 0; i < sizeof(funcToStr) / sizeof(funcToStr[0]); i++)\n"
            "            if (funcToStr[i].func == next_section) printf(\"%s\\n\", funcToStr[i].str);\n";
    ss <<
        "        next_section = ((void*(*)(void))next_section)();\n"
        "    }\n"
        "}\n";

    return ss.str();
}