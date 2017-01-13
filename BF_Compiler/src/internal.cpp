//
//  internal.cpp
//  BF_Compiler
//
//  Created by nemotiger on 2017/01/13.
//  Copyright © 2017 Hsia Yu. All rights reserved.
//

#include <string>
#include <regex>
#include "internal.hpp"
using namespace std;

bool isValid_file_extension(const char* file) noexcept {
    const regex file_pattern(R"(^\S+\.bf$)", regex::optimize);
    return regex_match(file, file_pattern);
}

string output_file_ASM(const char* file) noexcept {
    const regex file_pattern(R"(^(\S+\.)bf$)", regex::optimize);
    const string fmt = "$1asm";
    return regex_replace(file, file_pattern, fmt);
}

std::string output_file_VM(const char* file) noexcept {
    const regex filename_pattern(R"(^\w+\.bf$)", regex::optimize);
    const regex win_file_pattern(R"(^(\S*\\)\w+\.bf$)", regex::optimize);
    const regex unix_file_pattern(R"(^(\S*/)\w+\.bf$)", regex::optimize);
    const string win_fmt = "$1Main.vm", unix_fmt = "$1Main.vm";
    if(regex_match(file, filename_pattern))
        return "Main.vm";
    else if(regex_match(file, unix_file_pattern))
        return regex_replace(file, unix_file_pattern, unix_fmt);
    else
        return regex_replace(file, win_file_pattern, win_fmt);
}

string init_asm() noexcept {
    return "@20\nD=A\n@Ptr\nM=D\n(INIT_LOOP)\n@Ptr\nD=M\n@SCREEN\nD=D-A\n@END_INIT_LOOP\nD;JEQ\n@Ptr\nA=M\nM=0\n@Ptr\nM=M+1\n@INIT_LOOP\n0;JMP\n(END_INIT_LOOP)\n@20\nD=A\n@Ptr\nM=D\n";
}

string translate_asm(const char command, const unsigned _count, const unsigned loop_index) noexcept {
    static unsigned input_index = 0;
    switch(command) {
        case '+' :
            return _count == 1 ? "@Ptr\nA=M\nM=M+1\n" : "@" + to_string(_count) + "\nD=A\n@Ptr\nA=M\nM=M+D\n";
        case '-' :
            return _count == 1 ? "@Ptr\nA=M\nM=M-1\n" : "@" + to_string(_count) + "\nD=A\n@Ptr\nA=M\nM=M-D\n";
        case '>' :
            return _count == 1 ? "@Ptr\nM=M+1\n" : "@" + to_string(_count) + "\nD=A\n@Ptr\nM=M+D\n";
        case '<' :
            return _count == 1 ? "@Ptr\nM=M-1\n" : "@" + to_string(_count) + "\nD=A\n@Ptr\nM=M-D\n";
        case '[' :
            return "(LOOP_" + to_string(loop_index) + ")\n@Ptr\nA=M\nD=M\n@END_LOOP_" + to_string(loop_index) + "\nD;JEQ\n";
        case ']' :
            return "@Ptr\nA=M\nD=M\n@LOOP_" + to_string(loop_index) + "\nD;JNE\n(END_LOOP_" + to_string(loop_index) + ")\n";
        case ',' :
            ++input_index;
            return "(WAIT_INPUT_" + to_string(input_index) + ")\n@KBD\nD=M\n@WAIT_INPUT_" + to_string(input_index)
            + "\nD;JEQ\n@Ptr\nA=M\nM=D\n";
        default :
            return "";
    }
}

string halt_asm() noexcept {
    return "(END)\n@END\n0;JMP\n";
}

string init_vm() noexcept {
    return string("function Main.main 0\n") + "push constant 0\n" + "pop static 1\n" + "push constant 5000\n"
    + "call Array.new 1\n" + "pop static 0\n";
}

string translate_vm(const char command, const unsigned _count, const unsigned loop_index) noexcept {
    switch(command) {
        case '+' :
            return string("push static 1\n") + "push static 0\n" + "add\n" + "push static 1\n" + "push static 0\n" + "add\n"
            + "pop pointer 1\n" + "push that 0\n" + "push constant " + to_string(_count) + "\nadd\n" + "pop temp 0\n" + "pop pointer 1\n" + "push temp 0\n" + "pop that 0\n";
        case '-' :
            return string("push static 1\n") + "push static 0\n" + "add\n" + "push static 1\n" + "push static 0\n" + "add\n"
            + "pop pointer 1\n" + "push that 0\n" + "push constant " + to_string(_count) + "\nsub\n" + "pop temp 0\n" + "pop pointer 1\n" + "push temp 0\n" + "pop that 0\n";
        case '>' :
            return string("push static 1\n") + "push constant " + to_string(_count) + "\nadd\n" + "pop static 1\n";
        case '<' :
            return string("push static 1\n") + "push constant " + to_string(_count) + "\nsub\n" + "pop static 1\n";
        case '[' :
            return "label WHILE_EXP" + to_string(loop_index) + "\npush static 1\n" + "push static 0\n" + "add\n" + "pop pointer 1\n" + "push that 0\n" + "push constant 0\n"
            + "eq\nnot\nnot\n" + "if-goto WHILE_END" + to_string(loop_index) + "\n";
        case ']' :
            return "goto WHILE_EXP" + to_string(loop_index) + "\nlabel WHILE_END" + to_string(loop_index) + "\n";
        case ',' :
            return string("push static 1\n") + "push static 0\n" + "add\n" + "call Keyboard.readChar 0\n" + "pop temp 0\n"
            + "pop pointer 1\n" + "push temp 0\n" + "pop that 0\n";
        case '.' :
            return string("push static 1\n") + "push static 0\n" + "add\n" + "pop pointer 1\n" + "push that 0\n"
            + "call Output.printChar 1\n" + "pop temp 0\n";
        default :
            return "";
    }
}

string halt_vm() noexcept {
    return string("push constant 0\n") + "return\n";
}

