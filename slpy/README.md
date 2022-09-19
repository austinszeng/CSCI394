# Straight-Line Python

## Progress Report
Implemented 
- --dump, 
    - 
- right-associative power (**) operator, 
    - Extended lex method in slpy-lex.cc to "tokenize" ** as a token 
    - Created a Powr class, which is a subclass of Expn, with eval, output, and dump methods in slpy-ast.*
    - Modified parseMult and created parseExpt method in slpy-parse.*
        - parseMult now calls parseExpt instead of parseLeaf
        - parseExpt calls parseLeaf and utilizes recursion for right-associativity
- print with multiple arguments, 
    - Added "," token in slpy-lex.cc
    - Modified the Prnt class (subclass of Stmt) to hold a vector of expressions, changing its methods accordingly, in slpy-ast.*
    - Extended parseStmt method in slpy-parse.cc to handle the vector of expressions in slpy-parse.cc
- "+=" update statement
    - Extended lexical analyzer to recognize "+=" tokens in slpy-lex.cc
    - Created a new Stmt subclass, Updt, which closely follows Asgn's class definitions and methods, slightly modifying the exec method, in slpy-ast.*
    - Extended parseStmt method in slpy-parse.cc to differentiate the Asgn (=) and Updt (+=) operators

# Ideas for future progress
- Like output does, use an indent argument to recursively indent parse tree representation for dump