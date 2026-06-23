---

# Mini Preprocessor

The **Preprocessor** is the first stage of the C compilation pipeline.

Before the compiler analyzes variables, functions, loops, or syntax, the preprocessor scans the source file and performs a series of textual transformations.

It does **not** understand the C language.

<!--Instead, it treats the source file as plain text and processes special directives that begin with the ('#') symbol. -->

Examples include:

```c
#include <stdio.h>

#define PI 3.14159

#ifdef DEBUG

#ifndef MAX_SIZE

#if VERSION >= 2
```

After preprocessing is complete, the resulting file contains expanded source code that is then passed to the compiler.

---

# Why Does the Preprocessor Exist?

Imagine writing a large C project with hundreds of source files.

Without a preprocessor, every file would need to manually contain thousands of lines of library code.

For example:

```c
printf();
scanf();
malloc();
free();
fopen();
fclose();
```

Where do these functions come from?

They are declared inside header files such as:

```c
stdio.h
stdlib.h
string.h
math.h
```

Instead of copying these declarations into every file manually, programmers simply write:

```c
#include <stdio.h>
```

The preprocessor replaces this directive with the contents of the header file before compilation begins.

This dramatically improves code organization, maintainability, and reusability.

---

# Responsibilities of the Preprocessor

A C preprocessor performs several important tasks.

| Feature                 | Purpose                                        |
| ----------------------- | ---------------------------------------------- |
| Header Inclusion        | Inserts the contents of another file           |
| Macro Expansion         | Replaces symbolic names with values            |
| Conditional Compilation | Includes or skips code depending on conditions |
| Comment Removal         | Removes comments before compilation            |
| File Inclusion          | Combines multiple source files                 |
| Line Control            | Maintains line mappings for debugging          |

In this project, these features will be implemented progressively so that each stage can be understood independently.

---

# Position of the Preprocessor in the Toolchain

```
Source Code (.c)
        │
        ▼
+----------------+
| Preprocessor   |
+----------------+
        │
        ▼
Preprocessed File (.i)
        │
        ▼
Compiler
```

The compiler never receives the original `.c` file.

Instead, it receives the **preprocessed source**, where directives have already been handled.

---

# Example

Original source file:

```c
#include <stdio.h>

#define PI 3.14

int main()
{
    printf("%f", PI);
}
```

After preprocessing, it becomes conceptually similar to:

```c
/* thousands of lines from stdio.h */

int main()
{
    printf("%f", 3.14);
}
```

Notice that:

* `#include` disappeared.
* `#define PI` disappeared.
* `PI` was replaced with `3.14`.

The compiler only sees the transformed version.

---

# High-Level Workflow

The preprocessing stage follows a sequence of operations.

```
Read Source File
        │
        ▼
Scan Line by Line
        │
        ▼
Identify Preprocessor Directives
        │
        ▼
Process Directives
        │
        ▼
Generate New Source
        │
        ▼
Pass Output to Compiler
```

Every directive is processed independently before normal compilation begins.

---

# Internal Architecture of the Mini Preprocessor

The implementation in this repository is intentionally modular.

```
                +----------------+
                | Command Line   |
                +-------+--------+
                        |
                        ▼
                +----------------+
                | File Reader    |
                +-------+--------+
                        |
                        ▼
                +----------------+
                | Line Scanner   |
                +-------+--------+
                        |
                        ▼
                +----------------+
                | Directive      |
                | Analyzer       |
                +-------+--------+
                        |
                        ▼
                +----------------+
                | Output Writer  |
                +----------------+
```

Each module has a single responsibility.

This makes the project easier to understand and extend.

---

# Module Overview

## 1. File Reader

The first component is responsible for loading the source file from disk.

Responsibilities:

* Open the file.
* Read its contents.
* Handle file errors.
* Store the source as raw text.
* Pass the data to the scanner.

Input:

```
main.c
```

Output:

```
Raw source code stored in memory
```

The reader does **not** interpret the contents of the file.

It simply loads bytes into memory.

---

## 2. Line Scanner

Once the file is loaded, the scanner processes it one line at a time.

Responsibilities:

* Read individual lines.
* Detect directives beginning with `#`.
* Separate normal C code from preprocessing directives.

Example:

```
#include <stdio.h>

↓

Directive Detected
```

```
int x = 10;

↓

Normal C Code
```

The scanner acts as a traffic controller, deciding how each line should be handled.

---

## 3. Directive Analyzer

This module examines every preprocessing directive and determines its meaning.

Examples:

```
#include
```

↓

Header inclusion

---

```
#define
```

↓

Macro definition

---

```
#ifdef
```

↓

Conditional compilation

Each directive is forwarded to the appropriate handler.

---

## 4. Output Writer

After all transformations are complete, the resulting source is written to a new file.

```
Input

↓

main.c

↓

Preprocessor

↓

main.i
```

The `.i` file is the input for the next stage of the toolchain.

---

# Reader Module (Current Implementation)

At the current stage of development, the repository focuses on implementing the **Reader Module**.

The reader provides the foundation for all future preprocessing operations.

Current capabilities include:

* Reading a source file.
* Loading the file into memory.
* Displaying its contents.
* Error handling for missing files.
* Basic command-line interface.

Although simple, this component is critical because every later stage depends on correctly loading the source program.

---

# Data Flow

```
Source File
      │
      ▼
Reader
      │
      ▼
Memory Buffer
      │
      ▼
Scanner
      │
      ▼
Directive Processor
      │
      ▼
Output File
```

---

# Header File Inclusion (`#include`)

Header inclusion is one of the most frequently used features in C programming.

Almost every C program begins with one or more `#include` directives:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

Although these directives look simple, they perform an essential role in the compilation process.

The preprocessor replaces each `#include` directive with the **contents of the referenced header file** before the compiler sees the source code.

This process is called **header file inclusion**.

---

# Why Do We Need Header Files?

Imagine writing a program that uses the `printf()` function.

```c
#include <stdio.h>

int main()
{
    printf("Hello World");
}
```

Where is `printf()` defined?

It is **not** built into the C language.

Instead, `printf()` is declared inside the **Standard Input/Output Library** header:

```c
stdio.h
```

That header contains declarations similar to:

```c
int printf(const char *format, ...);
```

Without this declaration, the compiler would not know:

* The function exists.
* What parameters it accepts.
* What value it returns.

Header files provide this information.

---

# What Exactly Is a Header File?

A header file is simply another source file with the extension:

```text
.h
```

Unlike `.c` files, header files usually contain:

* Function declarations
* Structure declarations
* Type definitions
* Constants
* Macro definitions
* External variable declarations

Example:

```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

int add(int a, int b);
int subtract(int a, int b);

#endif
```

Notice that there are **no function implementations** here—only declarations.

The actual function definitions are placed inside corresponding `.c` files.

---

# How `#include` Works

Suppose the source file is:

```c
#include "math.h"

int main()
{
    return 0;
}
```

Assume `math.h` contains:

```c
int add(int a, int b);
int sub(int a, int b);
```

The preprocessor performs a simple text replacement.

Result:

```c
int add(int a, int b);
int sub(int a, int b);

int main()
{
    return 0;
}
```

The compiler never sees the original `#include` directive.

It only sees the expanded source code.

---

# Textual Substitution

One important concept is that the preprocessor performs **textual substitution**, not compilation.

Think of it as copying and pasting the contents of one file into another.

Conceptually:

```text
main.c

↓

#include "math.h"

↓

Copy Contents of math.h

↓

Paste Into main.c

↓

Continue Processing
```

The preprocessor does not understand:

* Variables
* Functions
* Classes
* Loops
* Syntax

It only manipulates text.

---

# Include Processing Flow

```text
Read Source File
        │
        ▼
Find '#include'
        │
        ▼
Extract File Name
        │
        ▼
Locate Header File
        │
        ▼
Open Header File
        │
        ▼
Read Contents
        │
        ▼
Insert Contents
        │
        ▼
Continue Scanning
```

Every `#include` directive follows this same sequence.

---

# Types of Include Directives

C supports two forms of header inclusion.

## System Headers

```c
#include <stdio.h>
```

Angle brackets tell the preprocessor:

> Search only the system include directories.

Typical examples:

* stdio.h
* stdlib.h
* string.h
* math.h
* time.h

---

## User Headers

```c
#include "student.h"
```

Quotation marks tell the preprocessor:

1. Search the current project directory.
2. If not found, search the system include directories.

These are commonly used for project-specific code.

Example:

```text
project/

main.c

student.h

student.c
```

---

# Difference Between `<...>` and `"..."`

| Syntax       | Search Order                                     |
| ------------ | ------------------------------------------------ |
| `<header.h>` | System include directories only                  |
| `"header.h"` | Current directory first, then system directories |

Example:

```c
#include <stdio.h>
```

↓

Search:

```text
/usr/include/

or

Compiler Include Directory
```

---

```c
#include "student.h"
```

↓

Search:

```text
Current Project Folder

↓

System Include Folder
```

---

# Include Search Paths

When processing an include directive, the preprocessor searches multiple directories.

Conceptually:

```text
Source File

↓

Current Directory

↓

Additional Include Paths

↓

Compiler Include Directory

↓

Standard Library
```

If the file cannot be found:

```text
Fatal Error:

header file not found
```

Compilation stops immediately.

---

# Nested Includes

Header files may include other header files.

Example:

```text
main.c

↓

#include "A.h"
```

A.h

```c
#include "B.h"
```

B.h

```c
#include "C.h"
```

The preprocessor expands them recursively.

Flow:

```text
main.c

↓

A.h

↓

B.h

↓

C.h
```

Nested inclusion is extremely common in large software projects.

---

# Recursive Expansion

The preprocessor continues expanding headers until no include directives remain.

Example:

```text
main.c

↓

graphics.h

↓

window.h

↓

system.h

↓

config.h
```

Each included file may include many others.

Large projects can process **hundreds or thousands** of header files during compilation.

---

# The Multiple Inclusion Problem

Suppose:

```text
main.c
```

includes

```text
student.h
```

twice.

```c
#include "student.h"

#include "student.h"
```

Without protection, the preprocessor expands the file twice.

Result:

```c
struct Student
{
    int id;
};

struct Student
{
    int id;
};
```

Now the compiler encounters duplicate definitions and produces an error.

---

# Include Guards

To prevent duplicate inclusion, programmers use **Include Guards**.

Example:

```c
#ifndef STUDENT_H

#define STUDENT_H

struct Student
{
    int id;
};

#endif
```

Explanation:

```text
Has STUDENT_H been defined?

↓

NO

↓

Define It

↓

Include File

↓

Future Includes Skip Entire File
```

Include guards ensure that each header is processed only once during compilation.

---

# Include Guard Workflow

```text
Open Header

↓

Check Macro

↓

Already Defined?

↓

YES ─────────► Skip File

↓

NO

↓

Define Macro

↓

Process Header

↓

Continue
```

This simple technique prevents multiple-definition errors in almost every C project.

---

# How GCC Handles `#include`

Although GCC is far more sophisticated, the high-level process is similar:

```text
Read Source

↓

Find #include

↓

Locate Header

↓

Open Header

↓

Expand Header

↓

Expand Nested Headers

↓

Generate Preprocessed File
```

Our implementation follows the same conceptual pipeline, but in a simplified and educational manner.

---

# How This Project Will Implement `#include`

The `Mini Preprocessor` will process header inclusion in several stages.

### Step 1

Read the source file.

↓

### Step 2

Detect lines beginning with:

```text
#include
```

↓

### Step 3

Extract the filename.

↓

### Step 4

Locate the file.

↓

### Step 5

Open the header.

↓

### Step 6

Read its contents.

↓

### Step 7

Replace the directive with the header's contents.

↓

### Step 8

Continue scanning the remaining source.

---

# Architecture of the Include Handler

```text
                 Source File
                      │
                      ▼
             Include Detector
                      │
                      ▼
             Filename Extractor
                      │
                      ▼
             File Locator
                      │
                      ▼
             Header Reader
                      │
                      ▼
            Include Expander
                      │
                      ▼
            Output Generator
```

Each module performs a single task, making the implementation easier to understand and extend.

---

# Macros (`#define`)

Macros are one of the most powerful features provided by the C preprocessor.

Unlike variables or functions, macros are **not part of the C language itself**. They exist only during the preprocessing stage.

A macro instructs the preprocessor to replace one piece of text with another **before the compiler begins compilation**.

The compiler never sees macro definitions—it only sees the expanded source code produced by the preprocessor.

---

# What is a Macro?

A macro is a rule that tells the preprocessor:

> "Whenever you encounter this token, replace it with the following text."

For example:

```c
#define PI 3.141592653589793
```

Here:

* **Macro Name:** `PI`
* **Replacement Text:** `3.141592653589793`

Whenever the preprocessor encounters the token `PI`, it replaces it with the replacement text.

Example:

```c
#define PI 3.141592653589793

float area = PI * r * r;
```

After preprocessing:

```c
float area = 3.141592653589793 * r * r;
```

Notice that the compiler never sees `PI`.

---

# Why Use Macros?

Macros were introduced to improve readability and maintainability.

Imagine writing:

```c
3.141592653589793
```

hundreds of times.

Instead, we define:

```c
#define PI 3.141592653589793
```

Now the source becomes:

```c
float circumference = 2 * PI * r;
```

Benefits:

* Improved readability
* Easy maintenance
* Single point of modification
* Reduced typing errors
* Symbolic representation of constants

---

# Macro Expansion

The process of replacing a macro with its replacement text is called **macro expansion**.

Example:

Input:

```c
#define MAX 100

int array[MAX];
```

Expansion:

```c
int array[100];
```

The macro definition itself disappears from the output.

---

# Internal View of Macro Expansion

Conceptually, the preprocessor performs the following steps:

```text
Read Source File
        │
        ▼
Find #define
        │
        ▼
Store Macro Definition
        │
        ▼
Continue Reading
        │
        ▼
Encounter Macro Name
        │
        ▼
Replace Text
        │
        ▼
Continue Scanning
```

Every occurrence of the macro is replaced before compilation.

---

# Object-like Macros

The simplest form of macro is an **object-like macro**.

Syntax:

```c
#define NAME replacement
```

Examples:

```c
#define MAX_SIZE 256

#define BUFFER_LENGTH 1024

#define VERSION 2
```

Usage:

```c
char buffer[MAX_SIZE];
```

Expansion:

```c
char buffer[256];
```

---

# Function-like Macros

Macros can also accept parameters.

Syntax:

```c
#define NAME(parameters) replacement
```

Example:

```c
#define SQUARE(x) ((x) * (x))
```

Usage:

```c
int value = SQUARE(5);
```

Expansion:

```c
int value = ((5) * (5));
```

Notice that no function call occurs.

The replacement happens entirely during preprocessing.

---

# Function vs Macro

Consider:

Function:

```c
int square(int x)
{
    return x * x;
}
```

Call:

```c
square(5);
```

Execution:

```text
Program Running

↓

Function Call

↓

Return Result
```

Now compare with a macro:

```c
#define SQUARE(x) ((x) * (x))
```

Usage:

```c
SQUARE(5)
```

Expansion:

```c
((5) * (5))
```

The compiler sees only the expanded expression.

No function exists.

No function call occurs.

---

# Macro Expansion Example

Input:

```c
#define LENGTH 20

#define WIDTH 10

int area = LENGTH * WIDTH;
```

Output:

```c
int area = 20 * 10;
```

The macro definitions themselves are removed from the final preprocessed file.

---

# Multiple Macro Expansion

Example:

```c
#define PI 3.14

#define TWO_PI (2 * PI)

float value = TWO_PI;
```

Expansion:

Step 1:

```c
float value = (2 * PI);
```

Step 2:

```c
float value = (2 * 3.14);
```

Macros may expand into other macros.

The preprocessor continues expanding until no expandable macros remain.

---

# Nested Macro Expansion

Example:

```c
#define A B

#define B C

#define C 100

int x = A;
```

Expansion:

```text
A

↓

B

↓

C

↓

100
```

Final source:

```c
int x = 100;
```

---

# Macro Storage

Internally, the preprocessor maintains a table of defined macros.

Conceptually:

| Macro  | Replacement |
| ------ | ----------- |
| PI     | 3.14        |
| MAX    | 100         |
| BUFFER | 1024        |

Whenever a token matches a macro name, the replacement text is substituted.

Our project will implement a simplified version of this macro table.

---

# Macro Processing Workflow

```text
Read Line
      │
      ▼
Contains #define?
      │
      ▼
YES
      │
      ▼
Extract Name
      │
      ▼
Extract Replacement
      │
      ▼
Store in Macro Table
      │
      ▼
Continue Reading
```

Later:

```text
Read Token
      │
      ▼
Exists in Macro Table?
      │
      ▼
YES
      │
      ▼
Replace Token
```

---

# Internal Architecture

```text
                 Source Code
                      │
                      ▼
              Directive Scanner
                      │
                      ▼
              Macro Parser
                      │
                      ▼
              Macro Table
                      │
                      ▼
             Expansion Engine
                      │
                      ▼
             Output Generator
```

Each module has a single responsibility, making the implementation modular and easier to understand.

---

# Common Macro Pitfalls

Although macros are powerful, careless usage can introduce subtle bugs.

## Missing Parentheses

Incorrect:

```c
#define SQUARE(x) x * x
```

Usage:

```c
SQUARE(2 + 3)
```

Expansion:

```c
2 + 3 * 2 + 3
```

Result:

```text
11
```

Expected:

```text
25
```

Correct definition:

```c
#define SQUARE(x) ((x) * (x))
```

Now expansion becomes:

```c
((2 + 3) * (2 + 3))
```

Correct result:

```text
25
```

---

# Side Effects

Consider:

```c
#define DOUBLE(x) ((x) + (x))
```

Usage:

```c
DOUBLE(i++)
```

Expansion:

```c
((i++) + (i++))
```

The variable is incremented **twice**.

This may lead to undefined or unexpected behavior.

Macros should avoid expressions with side effects.

---

# Advantages of Macros

* Faster than function calls for simple substitutions
* Easy to define symbolic constants
* Improve readability
* Eliminate repetitive code
* No runtime overhead
* Useful for conditional compilation
* Widely supported across C compilers

---

# Limitations of Macros

* No type checking
* Harder to debug
* Can produce confusing error messages
* Simple text substitution may introduce bugs
* Expressions require careful parenthesization
* Difficult to trace after expansion

Because of these limitations, modern C often prefers `const`, `inline` functions, or `enum` where appropriate. However, macros remain essential for preprocessing tasks and low-level programming.

---

# Implementation in This Project

The `Mini Preprocessor` will implement macro processing in the following stages:

1. Detect `#define` directives.
2. Parse the macro name.
3. Parse the replacement text.
4. Store the macro in an internal table.
5. Scan subsequent tokens.
6. Replace matching tokens with their corresponding replacement text.
7. Generate the transformed source file.

The initial implementation will support object-like macros first, followed by function-like macros in later iterations.

---
