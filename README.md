# Lexical Analyzer and Recursive Descent Parser in C

**Author:** Halmuhammet Muhamedorazov 

---

## 📄 Description

This project implements a **Lexical Analyzer** and a **Recursive Descent Parser** in C. It tokenizes an input source file and parses it according to a defined grammar for a simplified C-like language.

🛠️ **Key Feature:**  
The parser is designed to **recover from syntax errors** rather than stopping at the first issue. This allows it to identify and report multiple syntax errors in a single run, continuing to parse the remaining code for better diagnostics and robustness.

---

## 🧠 Grammar (BNF Rules)

```
P    ::= S
S    ::= V = E 
       | read ( V ) 
       | write ( V ) 
       | do { S } while ( C ) 
       | S ; S

C    ::= E < E | E > E | E == E | E != E | E <= E | E >= E

E    ::= T E'
E'   ::= + T E' | - T E' | ε

T    ::= F T'
T'   ::= * F T' | / F T' | ε

F    ::= ( E ) | O | N | V

O    ::= V++ | V--

V    ::= a | b | ... | z | aV | bV | ... | zV

N    ::= 0 | 1 | ... | 9 | 0N | 1N | ... | 9N
```

---

## 📁 Files

- `main.c` - Entry point, handles file I/O and initial token scanning
- `parser.c` - Recursive descent parser implementation with error recovery
- `parser.h` - Function declarations for parser
- `front.h` - Token types and global declarations

---

## 🧪 Compilation

```bash
gcc main.c parser.c -o analyzer
```

---

## ▶️ Usage

```bash
./analyzer path_to_input_file.txt
```

- If no file is provided, prints: `No input file given`
- If file can't be opened, prints: `File not found`

---

## 💬 Output

- **Lexical Tokens:**

  ```
  <lexeme>   <token>
  ```

- **After Parsing:**

  ```
  Syntax Validated       # if successful
  Invalid Syntax         # if errors found (with recovery)
  ```

  ❗ Even when syntax errors occur, the parser continues checking the rest of the input.

---

## 🪄 Tokens Recognized

| Token Name  | Examples          |
|-------------|-------------------|
| KEY_READ    | `read`            |
| KEY_WRITE   | `write`           |
| KEY_DO      | `do`              |
| KEY_WHILE   | `while`           |
| IDENT       | `x`, `myVar`      |
| INT_LIT     | `42`, `123`       |
| ASSIGN_OP   | `=`               |
| REL_OP      | `<`, `!=`, `==`   |
| INC_OP      | `++`, `--`        |
| ARITH_OP    | `+`, `-`, `*`, `/`|
| PAREN       | `(`, `)`          |
| BRACE       | `{`, `}`          |
| SEMICOLON   | `;`               |

---

## ❌ Exit Codes

| Code | Description             |
|------|-------------------------|
| 0    | Syntax Validated        |
| 1    | Invalid Syntax Found    |
| 2    | No input file provided  |
| 3    | File could not be opened|

---

## 📚 Example Input

```c
read(x);
x = 4 + 5;
do { write(x); } while (x < 10);
```

---

## ✅ Example Output

```
read     KEY_READ
(        LEFT_PAREN
x        IDENT
)        RIGHT_PAREN
;        SEMICOLON
x        IDENT
=        ASSIGN_OP
4        INT_LIT
+        ADD_OP
5        INT_LIT
;        SEMICOLON
do       KEY_DO
{        LEFT_BRACE
write    KEY_WRITE
(        LEFT_PAREN
x        IDENT
)        RIGHT_PAREN
;        SEMICOLON
}        RIGHT_BRACE
while    KEY_WHILE
(        LEFT_PAREN
x        IDENT
<        LESS_THAN
10       INT_LIT
)        RIGHT_PAREN
;        SEMICOLON
Syntax Validated
```

---

## 🧠 Notes

- Implements **recursive descent parsing** with functions per grammar rule.
- Uses `ungetc()` for character lookahead in token scanning.
- Errors during parsing trigger messages but do **not** halt parsing—allowing recovery and continued analysis.

---
