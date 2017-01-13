//
//  interpreter.hpp
//  BF_Compiler
//
//  Created by nemotiger on 2017/01/13.
//  Copyright © 2017 Hsia Yu. All rights reserved.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

enum class Result_Type {ASM, VM, ALL};

void interpreter(const char* file, Result_Type result_t, bool optimize);


#endif /* INTERPRETER_HPP */
