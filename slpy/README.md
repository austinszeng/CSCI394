# Progress Report
- From what I've tested, my very rough outline causes seg faults when trying to do anything with variables
- My implementation of the '\*\*' operator currently doesn't compile because "undefined reference to `vtable for Powr'" 
    - Additionally, there is an advance() method in lex that I want to modify to make a backup() method, but I have not yet figured out how. This would be useful when checking between the '\*' and '\*\*' cases because I do not believe that my conditional statements for the case that it's under (case '\*') works. curr_char would not be able to differentiate between '\*' and '\*\*', and the current methods we have will not allow for backup in the case of the operator just being '\*'.

# Ideas for future progress
- Like output does, use an indent argument to recursively indent parse tree representation for dump

# Current questions
- In parse.cc, test how parseStmt output a print with comma tokens
    - How would you add white space between each output? Doesn't parser ignore white spaces?
- Updt doesn't work and I think it's because of exec method
        void Updt::exec(Ctxt& ctxt) const {
            ctxt[name] = expn->eval(ctxt);
        }
    - what is ctxt and ctxt[name]? how does it actually know to do the += operator?

# Remember
- ast.hh, ast.cc, parse.cc (changed back to parseLeaf for parseMult) :: commented out all Powr stuff so I can compile and test other exercises 