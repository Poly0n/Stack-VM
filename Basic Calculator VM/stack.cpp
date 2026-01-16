#include "stack.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

bool stack::isNumber(const std::string& str) const {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c) && c != '-') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> stack::split(const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool stack::parseToken(const std::string& token) {
    // Check if token contains comma
    size_t commaPos = token.find(',');

    if (commaPos != std::string::npos) {
        std::string opcodeStr = token.substr(0, commaPos);
        std::string valueStr = token.substr(commaPos + 1);

        if (!isNumber(opcodeStr) || !isNumber(valueStr)) {
            std::cerr << "Error: Invalid number in token: " << token << std::endl;
            return false;
        }
        
        if (valueStr > "9999") {
            std::cerr << "Dataset is too large.";
            return false;
        }

        int opcode = std::stoi(opcodeStr);
        int value = std::stoi(valueStr);

        if (opcode == 1) {  // PUSH
            instructions.push_back(Instruction(opcode, value));
            return true;
        }
        else {
            std::cerr << "Error: Only PUSH (1) should have comma: " << token << std::endl;
            return false;
        }
    }
    else {
        if (!isNumber(token)) {
            std::cerr << "Error: Invalid token (not a number): " << token << std::endl;
            return false;
        }

        int opcode = std::stoi(token);

        if (opcode >= 2 && opcode <= 5) {
            instructions.push_back(Instruction(opcode));
            return true;
        }
        else if (opcode == 1) {
            std::cerr << "Error: PUSH (1) missing value. Use format: 1,value" << std::endl;
            return false;
        }
        else {
            std::cerr << "Error: Invalid opcode: " << opcode << std::endl;
            return false;
        }
    }
}

bool stack::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    instructions.clear();
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        std::istringstream lineStream(line);
        std::string token;

        while (lineStream >> token) {
            if (!parseToken(token)) {
                std::cerr << "  at line " << lineNum << std::endl;
                instructions.clear();
                return false;
            }
        }
    }

    std::cout << "Successfully parsed " << instructions.size() << " instructions" << std::endl;
    return true;
}

void stack::printInstructions() const {
    std::cout << "\nInstructions:" << std::endl;
    for (size_t i = 0; i < instructions.size(); i++) {
        const auto& instr = instructions[i];
        std::cout << i << ": ";

        switch (instr.opcode) {
        case 1: std::cout << "PUSH " << instr.value; break;
        case 2: std::cout << "ADD"; break;
        case 3: std::cout << "SUB"; break;
        case 4: std::cout << "PRINT"; break;
        case 5: std::cout << "HALT"; break;
        default: std::cout << "UNKNOWN";
        }
        std::cout << std::endl;
    }
}

void stack::execute() {
    std::vector<int> stack;

    std::cout << "\nExecuting:" << std::endl;

    for (size_t i = 0; i < instructions.size(); i++) {
        const auto& instr = instructions[i];

        switch (instr.opcode) {
        case 1:  // PUSH
            stack.push_back(instr.value);
            std::cout << "  PUSH " << instr.value << std::endl;
            break;

        case 2:  // ADD
            if (stack.size() < 2) {
                std::cerr << "  ERROR: Stack underflow on ADD" << std::endl;
                return;
            }
            {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();

                stack.push_back(a + b);
                std::cout << "  ADD (" << a << " + " << b << " = " << a + b << ")" << std::endl;
            }
            break;

        case 3:  // SUB
            if (stack.size() < 2) {
                std::cerr << "  ERROR: Stack underflow on SUB" << std::endl;
                return;
            }
            {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();

                stack.push_back(a - b);
                std::cout << "  SUB (" << a << " - " << b << " = " << a - b << ")" << std::endl;
            }
            break;

        case 4:  // PRINT
            if (stack.empty()) {
                std::cerr << "  ERROR: Stack empty on PRINT" << std::endl;
                return;
            }
            std::cout << "  PRINT: " << stack.back() << std::endl;
            stack.pop_back();
            break;

        case 5:  // HALT
            std::cout << "  HALT" << std::endl;
            return;
        }

        // Show stack after each operation
        std::cout << "    Stack: [";
        for (size_t j = 0; j < stack.size(); j++) {
            if (j > 0) std::cout << ", ";
            std::cout << stack[j];
        }
        std::cout << "]" << std::endl;
    }
}