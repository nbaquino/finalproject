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
- To check if you have gcc installed in you device, please run this in your terminal
```
gcc -version
```
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
g++ -o LOGIC .\Main.cpp .\Scanner.cpp .\Parser.cpp .\Evaluator.cpp
```

### macOS/Linux
```bash
cd src
g++ -o LOGIC .\Main.cpp .\Scanner.cpp .\Parser.cpp .\Evaluator.cpp
```

## Usage

### Interactive Mode
```bash
# Windows
.\LOGIC.exe

# macOS/Linux
# Make the executable runnable (macOS/Linux only)
chmod +x ./LOGIC #or
chmod +x LOGIC   #or
chmod +x .\LOGIC #or
chmod +x ./LOGIC.exe
chmod +x LOGIC.exe
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
P       Q       P AND Q
F       F       F
T       F       F
F       T       F
T       T       T

Enter a propositional logic statement: P OR Q
Truth Table:
P       Q       P OR Q
F       F       F
T       F       T
F       T       T
T       T       T

Enter a propositional logic statement: NOT P
Truth Table:
P       NOT P
F       T
T       F

Enter a propositional logic statement: exit
```

### File Input Mode
Create a file named `input.txt` with the following content. In this case, we have sentence.pl which contains:
```text
# Sample expressions
P
Q
TRUE
FALSE
NOT P
NOT Q
P AND Q
P OR Q
P IMPLIES Q
P EQUIVALENT Q
(P AND Q) IMPLIES R
(P OR Q) EQUIVALENT R
(P IMPLIES Q) AND (Q IMPLIES P)
(P EQUIVALENT Q) OR (NOT P AND NOT Q)
NOT NOT (P EQUIVALENT Q) OR (P IMPLIES Q)

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


...etc
```

### Error Handling
The program includes comprehensive error handling across multiple stages:

#### Lexical Errors (Scanner)
- Invalid characters in expressions
- Unrecognized tokens/operators
- Invalid variable names (only P and Q are allowed)

Example:
```bash
Error: Invalid character: '$' at position 3
Error: Invalid token: 'X' at position 0
```

#### Syntax Errors (Parser)
- Missing operands for operators
- Mismatched parentheses
- Invalid operator placement
- Unexpected tokens or expressions

Example:
```bash
Error: Binary operator 'AND' must have both left and right operands
Error: Missing closing parenthesis
Error: NOT operator can only appear before its operand
Error: Unexpected tokens after valid expression
```

#### Evaluation Errors (Evaluator)
- Invalid expression structures
- Empty nodes in syntax tree
- Unknown operators

Example:
```bash
Error: Invalid expression: Empty node encountered
Error: Unknown operator: 'XOR'
Error: Failed to generate truth table: [specific error message]
```

#### File Handling Errors
- File not found
- File access permission issues

Example:
```bash
Error: Could not open input file: input.txt
```

The program provides detailed error messages that include:
- The type of error encountered
- The position in the input where the error occurred (when applicable)
- A description of what went wrong
- The specific token or expression causing the error

All errors are caught and handled gracefully, allowing the program to:
- Continue execution in interactive mode after an error
- Process subsequent expressions in batch mode
- Provide clear feedback to help users correct their input

## Contributing
- Fork the repository
- Create a new branch (`git checkout -b feature/improvement`)
- Make your changes
- Push to the branch (`git push origin feature/improvement`)
- Open a Pull Request
