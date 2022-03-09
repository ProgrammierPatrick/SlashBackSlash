#ifndef _IL_H
#define _IL_H

#include <unordered_map>
#include <string>
#include <memory>
#include <variant>
#include <ostream>
#include <vector>

struct IL {
    struct Section {
        struct JMPVar {
            int targetVar;
            JMPVar(int targetVar) : targetVar(targetVar) { }
        };
        struct JMP {
            std::shared_ptr<Section> target;
            JMP(const std::shared_ptr<Section>& target) : target(target) { }
        };
        struct PUSHVar {
            int targetVar;
            PUSHVar(int targetVar) : targetVar(targetVar) { }
        };
        struct PUSH {
            std::shared_ptr<Section> section;
            PUSH(const std::shared_ptr<Section>& section) : section(section) { }
        };
        struct PUSH_ALLOC {
            std::shared_ptr<Section> section;
            std::vector<int> pushVars;
            PUSH_ALLOC(const std::shared_ptr<Section>& section, const std::vector<int>& pushVars) : section(section), pushVars(pushVars) { }
        };
        struct POP {
            int var;
            POP(int var) : var(var) { }
        };

        std::string name;
        std::vector<std::variant<JMPVar,JMP,PUSHVar,PUSH,PUSH_ALLOC,POP>> sectionData;
        bool isBuildIn = false;
        Section(const std::string& name, bool isBuildIn = false) : name(name), isBuildIn(isBuildIn) { }
    };

    std::vector<std::shared_ptr<Section>> sections;
    std::shared_ptr<Section> mainSection;
};

std::ostream& operator<<(std::ostream& os, const IL& il);
std::ostream& operator<<(std::ostream& os, const IL::Section& section);
std::ostream& operator<<(std::ostream& os, const IL::Section::JMPVar& jmpVar);
std::ostream& operator<<(std::ostream& os, const IL::Section::JMP& jmp);
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSHVar& pushVar);
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSH& push);
std::ostream& operator<<(std::ostream& os, const IL::Section::PUSH_ALLOC& pushAlloc);
std::ostream& operator<<(std::ostream& os, const IL::Section::POP& pop);

#endif