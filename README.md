# BF_Compiler
A brainfuck compiler that can make the BF programs run on the Hack machine

## Usage
	BF_Compiler <file> (--asm | --vm | --all) [-o | --optimize]  
		Result type options:  
			--asm			Compile to Hack machine’s assembly code  
			--vm            Compile to Hack machine’s virtual machine code  
			--all           Compile to both assembly and VM code  
		Optimize:  
			-o, --optimize  Optimize the result code if it is specified  
The source file must use .bf as the file extension, and the compiled file will be put in the same directory as the source file. The result file’s name will be original_filename.asm (assembly code) or Main.vm (virtual machine code).

## Building
The BF_Compiler is a cross-platform software and was implemented in C++ (ISO-C++11). All the source files should be built into a single target. It had been built successfully with clang++/LLVM8.0 and LLVM C++ standard library with c++11 support, and tested in macOS 10.12.

## Requirements
The Hack machine is a virtual computer provides by the textbook The Elements of Computing Systems, MIT Press, by Noam Nisan and Shimon Schocken. The compiled codes can only run on CPUEmulator and VMEmulator supplied by the book, and the softwares can be downloaded at [HERE](http://www.nand2tetris.org/software.php).

## LICENSE
[MIT License](https://github.com/nemotiger/BF_Compiler/blob/master/LICENSE)

## Limitations
This compiler won’t follow the language’s convention. Due to the Hack machine’s structure, each cell would be 2 bytes. The array will start at RAM[20] and extend to the end of Hack machine’s RAM when compiling to assembly code. The length of the array will be limited to 5000 when compiling to VM code. Because CPUEmulator doesn’t have the built-in operating system, and it is too complicate to directly implement the output operation in Hack’s assembly under BF’s memory structure, the output command will be discarded when compiling to assembly code. But the result can still be seen in the CPUEmulator. In addition, the compiler will automatically add the initialization code at the start of the .asm file, so it will need some time to prepare for the environment before actually running the BF program.

## Warning
Compared with other operating systems, Hack machine uses different character set and encoding in I/O operations. So the codes that can work correctly in other operating system might not work as intended on the Hack machine. It is the programmer’s responsibility to make sure the codes can work as intended, the compiler will not deal with these kind of situations.

