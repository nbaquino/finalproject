# CMSC 124 - Propositional Logic Evaluator

A command-line tool that evaluates propositional logic expressions and generates truth tables using recursive descent parsing.

## Technical Overview

The evaluator uses a multi-stage processing pipeline:

### Tokens
- **Variables**: `P`, `Q` (Boolean variables)
- **Operators**:
  - `AND`: Logical conjunction
  - `OR`: Logical disjunction
  - `NOT`: Logical negation
  - `->`: Logical implication
- **Parentheses**: `(`, `)` for grouping expressions

### Processing Stages
1. **Scanner (Lexical Analysis)**: Breaks down input into tokens
2. **Parser (Syntax Analysis)**: Constructs Abstract Syntax Tree (AST)
3. **Evaluator**: Generates truth tables and evaluates expressions

## Prerequisites

- C++ compiler (g++ or equivalent)
- **Windows**: MinGW-w64 or Visual Studio with C++ support
- **macOS**: Xcode Command Line Tools or Homebrew's GCC
- **Linux**: GCC/G++

To check gcc installation:
```bash
gcc -version
```

## Installation

```bash
# Clone the repository
git clone https://github.com/nbaquino/propositional-logic-interpreter.git
cd propositional-logic-interpreter
```

### Windows
```bash
cd src
g++ -o LOGIC .\Main.cpp .\Scanner.cpp .\Parser.cpp .\Evaluator.cpp
```

### macOS
```bash
cd propositional-logic-interpreter/src
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Install LLVM
brew install llvm
# Add LLVM to your PATH and set compiler variables
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export CC="/opt/homebrew/opt/llvm/bin/clang"' >> ~/.zshrc
echo 'export CXX="/opt/homebrew/opt/llvm/bin/clang++"' >> ~/.zshrc
source ~/.zshrc
# Compile the program
clang++ -o LOGIC Main.cpp Scanner.cpp Parser.cpp Evaluator.cpp
# Run the program (with input file)
./LOGIC sentence.pl
# Or run in interactive mode
./LOGIC
```

### Linux
```bash
cd src
g++ -o LOGIC .\Main.cpp .\Scanner.cpp .\Parser.cpp .\Evaluator.cpp
chmod +x LOGIC
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
.\LOGIC.exe sentence.pl

# macOS/Linux
./LOGIC sentence.pl
```

### Input Format
- Operators: `AND`, `OR`, `NOT`, `->`
- Variables: `P` and `Q`
- Example: `P AND Q`, `NOT P`, `P -> Q`

### Input File Format
- One expression per line
- Lines starting with '#' are comments
- Empty lines are ignored

## Example Usage

### Interactive Mode
```bash
$ ./LOGIC
Enter a propositional logic statement: P AND Q
Truth Table:
P       Q       P AND Q
F       F       F
T       F       F
F       T       F
T       T       T

Enter a propositional logic statement: exit
```

### Error Handling

#### Lexical Errors (Scanner)
```bash
Error: Invalid character: '$' at position 3
Error: Invalid token: 'X' at position 0
```

#### Syntax Errors (Parser)
```bash
Error: Binary operator 'AND' must have both left and right operands
Error: Missing closing parenthesis
Error: NOT operator can only appear before its operand
```

#### Evaluation Errors (Evaluator)
```bash
Error: Invalid expression: Empty node encountered
Error: Unknown operator: 'XOR'
```

#### File Handling Errors
```bash
Error: Could not open input file: input.txt
```

## Contributing
- Fork the repository
- Create a new branch (`git checkout -b feature/improvement`)
- Make your changes
- Push to the branch (`git push origin feature/improvement`)
- Open a Pull Request
