# Components:
1. Preprocessor
2. Compiler
3. Assembler
4. Linker

# 1. Mini Preprocessor: File Reading Interface

* works
1. handling # sentences
2. Macros
3. #if, #else, ... (checking conditions)
4. removing the code from either #if or #else after checking the condition.
 
* reader.h   :



* reader.c   :

contains these header files
- #include <stdio.h>    -> FILE, fopen, fseek, ftell, fread, fclose, perror
- #include <stdlib.h>    -> malloc
- #include "reader.h"    -> functions declarations

- It has pointer to file in heap 
input reader of preprocessor reads it.

- perror prints OS-level error
- ftell -> tells the no. of bytes means file size
- rewind -> it tells go back to start 

so basically it reads full source once

- allocating memory 
heap memory survives the function return 

* main.c   :

- Responsibility: CLI + pipeline entry


* Done : 
CLI program
File I/O
Heap memory handling
Clear module separation




