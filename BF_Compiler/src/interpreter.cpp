//
//  interpreter.cpp
//  BF_Compiler
//
//  Created by nemotiger on 2017/01/13.
//  Copyright © 2017 Hsia Yu. All rights reserved.
//

#include <fstream>
#include <string>
#include <stack>
#include <exception>
#include "interpreter.hpp"
#include "internal.hpp"
using namespace std;

void interpreter_ASM(fstream&, fstream&) noexcept;
void interpreter_optimize_ASM(fstream&, fstream&) noexcept;
void interpreter_VM(fstream&, fstream&) noexcept;
void interpreter_optimize_VM(fstream&, fstream&) noexcept;


void interpreter(const char* file, Result_Type result_t, bool optimize) {
    fstream src;
    
    if(isValid_file_extension(file))
        src.open(file, fstream::in);
    else
        throw invalid_argument(string("Invalid file : ") + file);
    
    if(!src.is_open())
        throw runtime_error(string("Failed to open file : ") + file);
    
    if(result_t == Result_Type::ASM) {
        fstream result(output_file_ASM(file), fstream::out);
        
        if(!result.is_open())
            throw runtime_error("Failed to create output file");
        
        if(optimize)
            interpreter_optimize_ASM(result, src);
        else
            interpreter_ASM(result, src);
        
        result.close();
    }
    else if(result_t == Result_Type::VM) {
        fstream result(output_file_VM(file), fstream::out);
        
        if(!result.is_open())
            throw runtime_error("Failed to create output file");
        
        if(optimize)
            interpreter_optimize_VM(result, src);
        else
            interpreter_VM(result, src);
        
        result.close();
    }
    else {
        fstream result_ASM(output_file_ASM(file), fstream::out);
        fstream result_VM(output_file_VM(file), fstream::out);
        
        if(!result_ASM.is_open() || !result_VM.is_open())
            throw runtime_error("Failed to create output file");
        
        if(optimize) {
            interpreter_optimize_ASM(result_ASM, src);
            src.clear();
            src.seekg(0, ios_base::beg);
            interpreter_optimize_VM(result_VM, src);
        }
        else {
            interpreter_ASM(result_ASM, src);
            src.clear();
            src.seekg(0, ios_base::beg);
            interpreter_VM(result_VM, src);
        }
        
        result_ASM.close();
        result_VM.close();
    }
    
    src.close();
}

void interpreter_ASM(fstream& result, fstream& src) noexcept {
    stack<unsigned> Stack;
    
    result << init_asm() <<flush;
    
    char ch;
    unsigned loop_index = 0;
    while(src >> ch) {
        if(ch == '[') {
            Stack.push(loop_index);
            result << translate_asm(ch, 1, loop_index++) <<flush;
        }
        else if(ch == ']') {
            result << translate_asm(ch, 1, Stack.top()) <<flush;
            Stack.pop();
        }
        else
            result << translate_asm(ch) <<flush;
    }
    
    result << halt_asm() <<flush;
}

void interpreter_optimize_ASM(fstream& result, fstream& src) noexcept {
    stack<unsigned> Stack;
    
    result << init_asm() <<flush;
    
    char ch, com = 'X';
    unsigned loop_index = 0, _count = 1;
    while(src >> ch) {
        if(ch == '[' || ch == ']' || ch == ',' || ch == '.') {
            if(com != 'X') {
                result << translate_asm(com, _count) <<flush;
                com = 'X';
                _count = 1;
            }
            if(ch == '[') {
                Stack.push(loop_index);
                result << translate_asm(ch, 1, loop_index++) <<flush;
            }
            else if(ch == ']') {
                result << translate_asm(ch, 1, Stack.top()) <<flush;
                Stack.pop();
            }
            else
                result << translate_asm(ch) <<flush;
        }
        else {
            if(ch != com) {
                result << translate_asm(com, _count) <<flush;
                com = ch;
                _count = 1;
            }
            else
                ++_count;
        }
    }
    result << translate_asm(com, _count) <<flush;
    
    result << halt_asm() <<flush;
}

void interpreter_VM(fstream& result, fstream& src) noexcept {
    stack<unsigned> Stack;
    
    result << init_vm() <<flush;
    
    char ch;
    unsigned loop_index = 0;
    while(src >> ch) {
        if(ch == '[') {
            Stack.push(loop_index);
            result << translate_vm(ch, 1, loop_index++) <<flush;
        }
        else if(ch == ']') {
            result << translate_vm(ch, 1, Stack.top()) <<flush;
            Stack.pop();
        }
        else
            result << translate_vm(ch) <<flush;
    }
    
    result << halt_vm() <<flush;
}

void interpreter_optimize_VM(fstream& result, fstream& src) noexcept {
    stack<unsigned> Stack;
    
    result << init_vm() <<flush;
    
    char ch, com = 'X';
    unsigned loop_index = 0, _count = 1;
    while(src >> ch) {
        if(ch == '[' || ch == ']' || ch == ',' || ch == '.') {
            if(com != 'X') {
                result << translate_vm(com, _count) <<flush;
                com = 'X';
                _count = 1;
            }
            if(ch == '[') {
                Stack.push(loop_index);
                result << translate_vm(ch, 1, loop_index++) <<flush;
            }
            else if(ch == ']') {
                result << translate_vm(ch, 1, Stack.top()) <<flush;
                Stack.pop();
            }
            else
                result << translate_vm(ch) <<flush;
        }
        else {
            if(ch != com) {
                result << translate_vm(com, _count) <<flush;
                com = ch;
                _count = 1;
            }
            else
                ++_count;
        }
    }
    result << translate_vm(com, _count) <<flush;
    
    result << halt_vm() <<flush;
}
