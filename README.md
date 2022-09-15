# Progress Report
- From what I've tested, my very rough outline causes seg faults when trying to do anything with variables
- My implementation of the "**" operator currently doesn't compile because "undefined reference to `vtable for Powr'" 
    - Additionally, there is an advance() method in lex that I want to modify to make a backup() method, but I have not yet figured out how. This would be useful when checking for the case because I do not believe that my conditional statements for the case '*' works. curr_char would not be able to differentiate between '*' and not '**' and the current methods we have will not allow for backup in the case of the operator just being '*'

# Ideas for future progress
- Like output does, use an indent argument to recursively indent parse tree representation for dump