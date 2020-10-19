# Imaginary-Language-Tokenizer-and-Symbol-Table
Imaginary Language Tokenizer and Symbol Table
Code for the first pass of a compiler for an imaginary language. During the first pass, a symbol table is constructed from the tokens in the program. The tokens are any line number, variable name, or constant that occur in the program. The symbol table contains every token along with its type and location in the final imaginary machine language code. It is worth noting that commands of the imaginary language are not entered into the symbol table nor are operators. The symbol table is made up of an array containing TableEntry element.s Each TableEntry element contains a symbol, type, and location. The symbol is a line number, literal value, or variable name. The type can be either 'C', 'L', or 'V' for constant, line number, or variable respectively. Finally, the location contains the memory address to which the symbol is referring. The simulated memory is an array of size 100 with instructions stored in lower memory and variables and constants stored in higher memory. 

The imaginary language has the following commands:
comment
input
output
goto
prog_end
if CONDITION goto LINENUMBER
assign
