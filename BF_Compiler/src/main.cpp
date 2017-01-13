//
//  main.cpp
//  BF_Compiler
//
//  Created by Nemo Tiger on 2016/12/30.
//  Copyright © 2016年 Nemotiger Home. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <exception>
#include "interpreter.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    
    if(argc < 3 || argc > 4) {
        cerr <<"<!> Invalid arguments" <<endl;
        return EXIT_FAILURE;
    }
    else {
        Result_Type result_flag;
        bool optimize = false;
        
        if(strcmp(argv[2], "--asm") == 0) {
            result_flag = Result_Type::ASM;
            cout <<"Warning : Compile to hack's assembly code will discard the character '.'" <<endl;
        }
        else if(strcmp(argv[2], "--vm") == 0)
            result_flag = Result_Type::VM;
        else if(strcmp(argv[2], "--all") == 0) {
            result_flag = Result_Type::ALL;
            cout <<"Warning : Compile to hack's assembly code will discard the character '.'" <<endl;
        }
        else {
            cerr <<"<!> Invalid result type" <<endl;
            return EXIT_FAILURE;
        }
        
        if(argc == 4) {
            if(strcmp(argv[3], "-o") == 0 || strcmp(argv[3], "--optimize") == 0)
                optimize = true;
            else {
                cerr <<"<!> Obscure flag" <<endl;
                return EXIT_FAILURE;
            }
        }
        
        try {
            interpreter(argv[1], result_flag, optimize);
            
        } catch(invalid_argument err) {
            cerr <<"<!> " << err.what() <<endl;
            return EXIT_FAILURE;
            
        } catch(runtime_error err) {
            cerr <<"<!> " << err.what() <<endl;
            return EXIT_FAILURE;
        }
    }
    
    
    return EXIT_SUCCESS;
}
