#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <stdexcept>

constexpr size_t TAPE_SIZE = 30000;
constexpr size_t MAX_PROGRAM_SIZE = 1024;

class Interpreter {
private:
    std::vector<unsigned char> tape;
    unsigned char* ptr;
    std::unordered_map<size_t, size_t> jump_table;

    void build_jump_table(const char* program) {
        std::stack<size_t> stack;
        for (size_t i = 0; program[i]; ++i) {
            if (program[i] == '[') {
                stack.push(i);
            } else if (program[i] == ']') {
                if (!stack.empty()) {
                    size_t start = stack.top();
                    stack.pop();
                    jump_table[start] = i;
                    jump_table[i] = start;
                }
            }
        }
    }

public:
    Interpreter() : tape(TAPE_SIZE, 0), ptr(tape.data()) {}

    void interpret(const char* program) {
        build_jump_table(program);
        for (size_t i = 0; program[i]; ++i) {
            switch (program[i]) {
                case '>': ++ptr; break;
                case '<': --ptr; break;
                case '+': ++*ptr; break;
                case '-': --*ptr; break;
                case '.': std::putchar(*ptr); break;
                case ',': *ptr = std::getchar(); break;
                case '[':
                    if (*ptr == 0) {
                        i = jump_table[i];
                    }
                    break;
                case ']':
                    if (*ptr != 0) {
                        i = jump_table[i];
                    }
                    break;
                default:
                    throw std::runtime_error("syntax error lmao");
            }
        }
    }
};

int main() {
    char program[MAX_PROGRAM_SIZE];
    std::cin.getline(program, MAX_PROGRAM_SIZE);

    Interpreter interpreter;
    interpreter.interpret(program);

    std::cout << std::endl;

    return 0;
}