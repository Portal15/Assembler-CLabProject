# Assembler in C

**A two-pass assembler with macro expansion, symbol resolution, and 24-bit machine-code generation.**

Developed by **Yonatan Ben Yehuda** as a university C programming laboratory project, this assembler translates a custom assembly language into hexadecimal object code. It implements the translation pipeline in C: expanding macros, parsing and validating statements, building symbol tables, encoding instructions, resolving references, and writing object, entry, and external-reference files.

The project demonstrates systems programming through explicit memory management, linked data structures, bitwise instruction encoding, and a modular compilation pipeline. Its output targets the project's educational instruction set; it is not a native executable for the host computer.

## At a glance

| Component | Implementation |
| --- | --- |
| Language | C; CMake requests C11 |
| Architecture | Macro preprocessing followed by two assembly passes |
| Machine word | 24 bits, written as six hexadecimal digits |
| Starting address | Decimal `100` |
| Instruction set | 16 instructions |
| Registers | `r0`–`r7` |
| Addressing modes | Immediate, direct, relative, and register |
| Directives | `.data`, `.string`, `.entry`, `.extern` |
| Macros | `mcro` / `mcroend` |
| Output | `.am`, `.ob`, and conditional `.ent` / `.ext` files |
| Dependencies | C standard library; CMake is optional |

## Build and run

### With GCC or Clang

From the repository root:

```sh
cc -std=gnu11 -Wall -Wextra -I. \
  main.c preProcessor.c inputAnalyzer.c validationsChecker.c \
  objectTableAnalyzer.c binaryConvertor.c secondPass.c \
  createFiles.c errorHandler.c utils.c \
  -o assembler
```

The `-I.` option is required by the current header layout. GNU C11 mode accommodates the current `strdup` declarations on typical GCC/Clang environments. The build may report warnings.

Run the assembler with the **complete source filename, including `.as`**:

```sh
./assembler program.as
```

Multiple inputs can be assembled in one invocation:

```sh
./assembler first.as second.as
```

Each file is assembled independently, with its own symbol tables and output files. The current driver processes arguments from right to left; it does not link the inputs together.

### With CMake

Requires **CMake 3.30 or later** and a C compiler.

```sh
cmake -S . -B build
cmake --build build
./build/Maman14Project program.as
```

Use a fresh `build/` directory. The checked-in `cmake-build-debug*` directories contain machine-specific build artifacts.

## Example: from assembly to object code

Save the following as `demo.as`. It combines a macro, an exported entry point, an external reference, register and immediate operands, and numeric and string data.

```asm
; Macro expansion and symbol resolution
.entry MAIN
.extern EXT
mcro PRINT
prn r3
mcroend
MAIN: mov #5, r3
PRINT
jsr EXT
stop
VALUES: .data 6, -9
TEXT: .string "abc"
; End
```

**Current implementation note:** save this example without a newline after the final `; End` comment. The first-pass reader can read beyond the last line when a file ends with a newline; see [Implementation notes](#implementation-notes).

Run:

```sh
./assembler demo.as
```

The macro definition is removed from `demo.am`, and `PRINT` is replaced with `prn r3`. Successful assembly creates:

| File | Contents |
| --- | --- |
| `demo.am` | Assembly source after macro expansion |
| `demo.ob` | Six instruction/operand words followed by six data words |
| `demo.ent` | The exported `MAIN` symbol and its address |
| `demo.ext` | The address of the operand word that references `EXT` |

The resulting `demo.ob` is:

```text
     6 6
0000100 001b04
0000101 00002c
0000102 341b04
0000103 24081c
0000104 000001
0000105 3c0004
0000106 000006
0000107 fffff7
0000108 000061
0000109 000062
0000110 000063
0000111 000000
```

The accompanying symbol files are:

```text
# demo.ent
MAIN 0000100

# demo.ext
EXT 0000104
```

The `#` lines above identify the files; they are not part of their contents. An external-reference address identifies the **use of the symbol**, not the location of its eventual definition.

## Assembly language

### Statements and labels

A statement contains an optional label followed by an instruction or directive:

```asm
MAIN: mov #5, r3
COUNT: .data 10
```

Use labels beginning with a letter and no longer than **30 characters**. Avoid instruction names, register names, and macro names. Keywords are lowercase and comparisons are case-sensitive.

Separate two operands with a comma. Put comments on their own lines, with `;` in the first column. Keep source lines below 80 visible characters to accommodate the implementation's line-length check, which includes the newline.

### Addressing modes

| Mode | Code | Syntax | Example |
| --- | --- | --- | --- |
| Immediate | `0` | `#number` | `prn #48` |
| Direct | `1` | `LABEL` | `mov COUNT, r1` |
| Relative | `2` | `&LABEL` | `bne &LOOP` |
| Register | `3` | `r0`–`r7` | `add r1, r4` |

Relative addressing is available for `jmp`, `bne`, and `jsr`. Use local labels for relative targets.

### Instruction set

The operand combinations below follow the validation table in [validationsChecker.c](validationsChecker.c). For a one-operand instruction, the operand appears in the destination column.

| Instruction | Operation | Source modes | Destination modes |
| --- | --- | --- | --- |
| `mov` | Move | Immediate, direct, register | Direct, register |
| `cmp` | Compare | Immediate, direct, register | Immediate, direct, register |
| `add` | Add | Immediate, direct, register | Direct, register |
| `sub` | Subtract | Immediate, direct, register | Direct, register |
| `lea` | Load effective address | Direct | Direct, register |
| `clr` | Clear | — | Direct, register |
| `not` | Bitwise NOT | — | Direct, register |
| `inc` | Increment | — | Direct, register |
| `dec` | Decrement | — | Direct, register |
| `jmp` | Jump | — | Direct, relative |
| `bne` | Branch if not equal | — | Direct, relative |
| `jsr` | Jump to subroutine | — | Direct, relative |
| `red` | Read input | — | Direct, register |
| `prn` | Print | — | Immediate, direct, register |
| `rts` | Return from subroutine | — | — |
| `stop` | Stop | — | — |

These are the target machine's operations. The assembler encodes them; it does not execute them.

### Directives

| Directive | Purpose | Example |
| --- | --- | --- |
| `.data` | Define comma-separated integer data | `VALUES: .data 6, -9, 31` |
| `.string` | Define character data followed by a zero word | `TEXT: .string "abc"` |
| `.entry` | Export a locally defined symbol | `.entry MAIN` |
| `.extern` | Declare a symbol defined outside this source file | `.extern EXT` |

### Macros

```asm
mcro PRINT
prn r3
mcroend
PRINT
```

Define a macro before invoking it, and place its name alone on the invocation line. Macro bodies are expanded as text into the `.am` file. Parameterized macros and recursive expansion are not implemented.

## How it works

### 1. Macro preprocessing

[preProcessor.c](preProcessor.c) reads the source, collects macro definitions in a linked list, and expands subsequent invocations. It writes the intermediate `.am` file and returns the macro names so they can also be treated as reserved names during validation.

### 2. First pass: parse, validate, and encode

The first-pass loop in [main.c](main.c) coordinates four responsibilities:

- **Parsing:** [inputAnalyzer.c](inputAnalyzer.c) converts source lines into structured labels, commands, operands, and directive values.
- **Validation:** [validationsChecker.c](validationsChecker.c) checks instruction/operand combinations, registers, numeric operands, reserved names, and duplicate labels.
- **Symbol collection:** [objectTableAnalyzer.c](objectTableAnalyzer.c) maintains separate linked lists for code labels, data labels, entries, and externals, assigning addresses as the source is processed.
- **Encoding:** [binaryConvertor.c](binaryConvertor.c) packs instruction fields and appends words to the intermediate output list. Symbol operands remain placeholders until their addresses can be resolved.

The current implementation emits code and data in source order. Place data after instructions when you want the conventional code-then-data layout.

### 3. Second pass: resolve symbols

[secondPass.c](secondPass.c) replaces symbolic placeholders with encoded addresses or relative displacements, records external references, and converts word values to six-digit hexadecimal strings. This allows instructions to reference labels defined later in the source.

### 4. Output generation

[createFiles.c](createFiles.c) writes the final files when no assembly error has been recorded.

| Extension | Format | Creation condition |
| --- | --- | --- |
| `.am` | Expanded assembly text | Produced during preprocessing |
| `.ob` | Code/data word counts, then address–word pairs | Successful assembly with encoded words |
| `.ent` | Entry symbol and decimal address | Successful assembly with entry declarations |
| `.ext` | External symbol and decimal reference address | Successful assembly with external references |

Final output files are written beside the input, using the same filename stem. Addresses are printed as seven decimal digits; object words use six lowercase hexadecimal digits.

## Machine-word encoding

The first word of an instruction uses the following 24-bit layout:

| Bits | Width | Field |
| --- | --- | --- |
| 23–18 | 6 | Opcode |
| 17–16 | 2 | Source addressing mode |
| 15–13 | 3 | Source register |
| 12–11 | 2 | Destination addressing mode |
| 10–8 | 3 | Destination register |
| 7–3 | 5 | Function code |
| 2–0 | 3 | A/R/E flags |

Register operands are packed into the instruction word. Immediate, direct, and relative operands require additional words.

The low three bits distinguish **absolute** (`100`), **relocatable** (`010`), and **external** (`001`) values. Data words use the full 24-bit word; negative values are rendered in two's-complement form.

## Source map

| File | Responsibility |
| --- | --- |
| [main.c](main.c) | CLI, per-file pipeline, first-pass orchestration, cleanup |
| [preProcessor.c](preProcessor.c) | Macro definitions and expansion |
| [inputAnalyzer.c](inputAnalyzer.c) | Statement parsing and parsed-object cleanup |
| [validationsChecker.c](validationsChecker.c) | Operand, instruction, and label validation |
| [objectTableAnalyzer.c](objectTableAnalyzer.c) | Symbol tables and address assignment |
| [binaryConvertor.c](binaryConvertor.c) | Instruction encoding and unresolved references |
| [secondPass.c](secondPass.c) | Symbol resolution and hexadecimal conversion |
| [createFiles.c](createFiles.c) | Output filenames and file serialization |
| [errorHandler.c](errorHandler.c) | Centralized diagnostic messages |
| [utils.c](utils.c) | String, line-counting, and input helpers |
| [headerFiles/dataTypes.h](headerFiles/dataTypes.h) | Shared structures and linked-list definitions |
| [headerFiles/](headerFiles/) | Module interfaces and constants |
| [CMakeLists.txt](CMakeLists.txt) | CMake executable target and source list |

## Examples and verification

The repository includes assembly examples and saved output under [cmake-build-debug/](cmake-build-debug/):

- **`bla.as`** exercises direct and relative addressing, entries, externals, strings, and numeric data. Its `.ob`, `.ent`, and `.ext` files provide reference output.
- **`cefli.as`** contains a malformed macro declaration.
- **`trash.as`** contains an overlong label.

To try the main example without overwriting its checked-in outputs:

```sh
cp cmake-build-debug/bla.as sample.as
./assembler sample.as
```

There is no automated test suite configured in `CMakeLists.txt`. The direct compiler build and the complete example in this README were checked on Linux with GCC. The main repository example also reproduced its saved output, disregarding trailing blank lines.

## Implementation notes

This is an educational implementation. A few details matter when running the current version:

- **Input filenames and permissions:** include the `.as` extension, use short relative paths (under 40 characters), and ensure the source is writable. The preprocessor currently opens it with `r+`.
- **End-of-file handling:** the first pass uses a counted-line loop without checking the result of `fgets`. A trailing newline can therefore cause an extra read and spurious errors. The verified example ends with a comment and no final newline.
- **Diagnostics and exit status:** detected assembly errors normally suppress final output generation, but the driver still returns `0`. Inspect diagnostic messages rather than relying on the exit code. First/second-pass line numbers refer to expanded input and are currently zero-based.
- **Existing output files:** failed assembly does not remove output from a previous run. Use a fresh filename or remove stale output before comparing results.
- **Parser and encoding edges:** use integer numeric literals and alphabetic strings for the demonstrated workflow. General punctuation/digit string encoding and numeric range validation need further work. Operand names beginning with lowercase `r` are parsed as registers.

## Academic context

Built as a university project to explore how assembly source becomes machine code, with emphasis on modular C programming, symbol management, multi-pass translation, and manual memory ownership.

**Author:** [Yonatan Ben Yehuda](https://github.com/Portal15)
            [Yonatan Shmuel](https://github.com/Yonatan1387)
