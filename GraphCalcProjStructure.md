# Structure

## Brief Summary

The program is made of a few fundamental elements.

### Mode detection

Detects and set the working mode (interactive or automatic).

### The main loop

Each iteration prints a promp (on [interactive](#mode-detection "Mode Detection Stage")) + reads a line from `stdin` or the given file and applies the rest of the elements on it:

1. Parses
2. Function Applier

### Input Sanitizer

Takes the input line (string) and converts it into an array of token

### Parser

Converts the string command into the correct objects inside an ordered tree structure.

Also detects syntax errors..

### ParseTree Validator

## Errors

* Runtime errors
  * Variable first mentioned here
  * 

## Classes

#### Symbol Table

All the defined variables inside a map

#### Operator parse order

Whether operator starts parsing from the left or right (only relevant if mutable operators return a value. e.g. assignments).

### Parser

Handles parsing an input line into a binary tree. Each node is a string token. A leaf would be a literal, other nodes will always be an operator.

?? Will validate return type of children

Will validate syntax.

Will interpret function calls as unary operators.

Commands like `who`, `reset`  and `quit` will be returned as a tree with one node, containing the command string.

### Graph

### 



## Utility Functions



## Config Params

* Errors `ostream` (default to `stdout`)
* Automatic/Interactive modes (detect current mode according to `argv`)



# Timeline

1. Parser
2. Graph
3. 