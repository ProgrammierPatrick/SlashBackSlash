#include "il.h"

std::ostream& operator<<(std::ostream& os, const IL& il) {
    for (auto& section : il.sections)
        os << *section;
    return os;
}

std::ostream& operator<<(std::ostream& os, const IL::Section& section) {
    os << section.name << ":\n";
    for (auto& line : section.sectionData) {
        std::visit([&os](auto& arg) { os << "  " << arg << "\n"; }, line);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const IL::Section::JMPVar& jmpVar) {
    os << "JMP " << jmpVar.targetVar;
    return os;
}
std::ostream& operator<<(std::ostream& os, const IL::Section::JMP& jmp) {
    os << "JMP " << jmp.target->name;
    return os;
}
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSHVar& pushVar) {
    os << "PUSH " << pushVar.targetVar;
    return os;
}
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSH& push) {
    os << "PUSH " << push.section->name;
    return os;
}
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSH_ALLOC& pushAlloc) {
    os << "PUSH_ALLOC " << pushAlloc.section->name;
    for(int arg : pushAlloc.pushVars)
        os << ", " << arg;
    return os;
}
std::ostream& operator<<(std::ostream& os, const IL::Section::POP& pop) {
    os << "POP " << pop.var;
    return os;
}