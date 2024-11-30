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

## Building the Program
Skip this step if you want because there is already a build LOGIC.exe when you clone this repo. So you may proceed with the Usage section.
### Windows
```bash
cd src
g++ -o LOGIC .\Main.cpp .\Scanner.cpp .\Parser.cpp .\Evaluator.cpp
```

### macOS

1. Navigate to the source directory:
```bash
cd propositional-logic-interpreter/src
```

2. Install Homebrew (skip if already installed):
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Check if the Homebrew package manager was sucessfully installed by entering this in the terminal
```
brew 
```

3. In the terminal, install LLVM using Homebrew or typing this command:
```bash
brew install llvm
```

4. After llvm installation, configure environment variables by entering the following command:
```bash
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export CC="/opt/homebrew/opt/llvm/bin/clang"' >> ~/.zshrc
echo 'export CXX="/opt/homebrew/opt/llvm/bin/clang++"' >> ~/.zshrc
source ~/.zshrc
```

5. Compile the program:
```bash
clang++ -o LOGIC Main.cpp Scanner.cpp Parser.cpp Evaluator.cpp
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
$ ./LOGIC or ./LOGIC.exe
Enter a propositional logic statement: P AND Q
Truth Table:
P       Q       P AND Q
F       F       F
T       F       F
F       T       F
T       T       T

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
$ ./LOGIC.exe sentence.pl

Truth Table:
P       Q       P AND Q
F       F       F
T       F       F
F       T       F
T       T       T

Truth Table:
P       Q       R       P OR (Q AND R)
F       F       F       F
F       F       T       F
F       T       F       F
F       T       T       T
T       F       F       T
T       F       T       T
T       T       F       T
T       T       T       T

Truth Table:
P       Q       NOT (P AND Q)
F       F       T
T       F       T
F       T       T
T       T       F
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
