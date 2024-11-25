# CMSC 124 - Propositional Logic Evaluator

A command-line tool that evaluates propositional logic expressions and generates truth tables using the concept of recursive descent parsing.

## Technical Overview

The evaluator is built using a multi-stage processing pipeline:

### Tokens
The program recognizes the following token types:
- **Variables**: `P`, `Q` (Boolean variables)
- **Operators**:
  - `AND`: Logical conjunction
  - `OR`: Logical disjunction
  - `NOT`: Logical negation
  - `->`: Logical implication
- **Parentheses**: `(`, `)` for grouping expressions
- **Special**: End-of-line markers, whitespace

### Scanner (Lexical Analysis)
The Scanner breaks down the input string into a sequence of tokens:
- Identifies and validates individual tokens
- Handles whitespace and invalid characters
- Reports lexical errors (invalid variables, unknown operators)
- Maintains position information for error reporting

### Parser (Syntax Analysis)
The Parser constructs an Abstract Syntax Tree (AST) from the token stream:
- Implements recursive descent parsing
- Enforces operator precedence: NOT > AND > OR > IMPLIES
- Validates expression structure
- Handles parenthesized expressions
- Reports syntax errors (malformed expressions, mismatched parentheses)

### Evaluator
The Evaluator processes the AST to:
- Generate truth tables for all possible input combinations
- Evaluate complex expressions recursively
- Format and display results in a tabular format
- Handle both single expressions and batch processing

## Features

- Interactive mode for evaluating single expressions
- Batch processing mode through input files
- Supports common logical operators (AND, OR, NOT, etc.)
- Generates complete truth tables for expressions
- Handles both command-line and file input modes

## Prerequisites

- C++ compiler (g++ or equivalent)
- Make sure you have one of the following installed:
  - **Windows**: MinGW-w64 or Visual Studio with C++ support
  - **macOS**: Xcode Command Line Tools or Homebrew's GCC
  - **Linux**: GCC/G++

## Installation

```bash
# Clone the repository
git clone https://github.com/nbaquino/simple-interpreter.git
cd simple-interpreter
```

## Building the Program

### Windows
```bash
cd src
g++ -o LOGIC Logic.cpp Scanner.cpp Parser.cpp Evaluator.cpp
```

### macOS/Linux
```bash
cd src
g++ -o LOGIC Logic.cpp Scanner.cpp Parser.cpp Evaluator.cpp
# Make the executable runnable (macOS/Linux only)
chmod +x LOGIC
# If the above command fails, you can try using clang++ instead of g++
g++ -o LOGIC Logic.cpp Scanner.cpp Parser.cpp Evaluator.cpp
```

## Usage

### Interactive Mode
```bash
# Windows
.\LOGIC.exe

# macOS/Linux
./LOGIC
```

### File Input Mode
```bash
# Windows
.\LOGIC.exe input_file.txt

# macOS/Linux
./LOGIC input_file.txt
```

### Input Format
- Enter propositional logic expressions using the following operators:
  - AND: `AND`
  - OR: `OR`
  - NOT: `NOT`
  - IMPLIES: `->`
- Variables: Only `P` and `Q` are accepted
- Example: `P AND Q`, `NOT P`, `P -> Q`

### Input File Format
- One expression per line
- Lines starting with '#' are treated as comments
- Empty lines are ignored

## Example Usage

### Interactive Mode
```bash
$ ./LOGIC.exe
Propositional Logic Evaluator
Enter 'exit' or 'quit' to terminate the program.

Enter a propositional logic statement: P AND Q
Truth Table:
P       Q       (P AND Q)
F       F       F
T       F       F
F       T       F
T       T       T

Enter a propositional logic statement: P OR Q
Truth Table:
P       Q       (P OR Q)
F       F       F
T       F       T
F       T       T
T       T       T

Enter a propositional logic statement: NOT P
Truth Table:
P       (NOT P)
F       T
T       F

Enter a propositional logic statement: exit
```

### File Input Mode
Create a file named `input.txt` with the following content:
```text
# Sample expressions
P AND Q
P OR (Q AND R)
NOT (P AND Q)
```

Run the program:
```bash
$ ./LOGIC.exe input.txt

Truth Table:
P       Q       (P AND Q)
F       F       F
T       F       F
F       T       F
T       T       T

Truth Table:
P       Q       R       (P OR (Q AND R))
F       F       F       F
F       F       T       F
F       T       F       F
F       T       T       T
T       F       F       T
T       F       T       T
T       T       F       T
T       T       T       T

Truth Table:
P       Q       (NOT (P AND Q))
F       F       T
T       F       T
F       T       T
T       T       F
```

### Error Handling
The program includes error handling for:
- Invalid tokens
- Syntax errors
- Unmatched parentheses
- Invalid variables (any letter other than P or Q)
- Malformed expressions

Example error messages:
```bash
Enter a propositional logic statement: A AND B
Error processing 'A AND B': There is an error at position 0. Invalid token: 'A'

Enter a propositional logic statement: P AND R
Error processing 'P AND R': There is an error at position 6. Invalid token: 'R'

Enter a propositional logic statement: P AND (Q OR
Error processing 'P AND (Q OR': There is an error at position 11. Expected ')'

Enter a propositional logic statement: P AND AND Q
Error processing 'P AND AND Q': There is an error at position 6. Unexpected token: 'AND'
```

## Contributing
1. Fork the repository
2. Create a new branch (`git checkout -b feature/improvement`)
3. Make your changes
4. Push to the branch (`git push origin feature/improvement`)
5. Open a Pull Request

## License
MIT License

Copyright (c) 2024 Nathan Aquino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
