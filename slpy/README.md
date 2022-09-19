# Straight-Line Python

## Progress Report
Implemented 
- --dump, 
    - Wrote a dump method for each of the AST node types
    - Output to standard out a textual representation of the parse tree for the SLPY program after it has been parsed
    - Uses recursion to indent according to its depth, starting at an index of 0
    - Called using --dump flag
    - Modified slpy.cc to be able to indentify --dump flag
    - Modified slpy-ast.* by writing a dump method for each AST type node that takes in a std::ostream& os and std::string indent
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