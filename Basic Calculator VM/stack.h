#pragma once
#include <vector>
#include <string>
#include <utility>

class stack {
public:
    struct Instruction {
        int opcode;
        int value;
        Instruction(int op, int val = 0) : opcode(op), value(val) {}
    };

    bool loadFromFile(const std::string& filename);
    void execute();
    void printInstructions() const;

private:
    std::vector<Instruction> instructions;

    bool parseToken(const std::string& token);
    bool isNumber(const std::string& str) const;
    std::vector<std::string> split(const std::string& str, char delimiter) const;
};