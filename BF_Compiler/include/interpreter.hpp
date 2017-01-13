//
//  interpreter.hpp
//  BF_Compiler
//
//  Created by Nemo Tiger on 2016/12/30.
//  Copyright © 2016年 Nemotiger Home. All rights reserved.
//

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

enum class Result_Type {ASM, VM, ALL};

void interpreter(const char* file, Result_Type result_t, bool optimize);


#endif /* INTERPRETER_HPP */
