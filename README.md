Overview
This project is a 2-pass compiler for a mock assembler language. The compiler performs the following tasks in two passes:

First Pass: All function calls are replaced with the actual function code in the assembler language.
Second Pass: All assembler commands are translated to machine language, using '.' and '/' instead of '0' and '1'.
The compiler is written in C and was developed and tested on Ubuntu (Unix). Compatibility with Windows may vary depending on IDE settings.

Features
Function Inlining: Replaces function calls with the function definitions in the first pass.
Assembler to Machine Code Translation: Converts assembler language commands into machine code using a unique '.' and '/' notation.
Prerequisites
GCC (GNU Compiler Collection)
Ubuntu (Unix-based environment)
