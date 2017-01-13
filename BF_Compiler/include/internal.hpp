//
//  internal.hpp
//  BF_Compiler
//
//  Created by nemotiger on 2017/01/13.
//  Copyright © 2017 Hsia Yu. All rights reserved.
//

#ifndef INTERNAL_HPP
#define INTERNAL_HPP

#include <string>

// validate input file's name
bool isValid_file_extension(const char* file) noexcept;

// generate assembly output file's name
std::string output_file_ASM(const char* file) noexcept;

// generate Hack VM output file's name
std::string output_file_VM(const char* file) noexcept;

// initialize cells and pointer (asm)
std::string init_asm() noexcept;

// translate BF to Hack Assembly code
std::string translate_asm(const char command, const unsigned _count = 1, const unsigned loop_index = 0) noexcept;

// halt program (asm)
std::string halt_asm() noexcept;

// initialize cells and pointer (vm)
std::string init_vm() noexcept;

// translate BF to Hack Virtual Machine code
std::string translate_vm(const char command, const unsigned _count = 1, const unsigned loop_index = 0) noexcept;

// halt program (vm)
std::string halt_vm() noexcept;


#endif /* INTERNAL_HPP */
