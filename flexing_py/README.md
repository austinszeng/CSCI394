# Homework 2: Python indentation
- Flex
    - reg expression (next Monday)
    - finite automata (next Monday)
    - foodentation (Wednesday)
    - slpy + functions, loops, and ifs (FLIPY) [all due Friday]
- Homework 3 is a bison-pased parser and interpreter for FLIPY
- Can work in pairs for FLIPY

PART 1:
- Flex file
- Scans through file and produces C code by building finite state machine that does maximal munch according to reg expressions down below
- If pattern is found in file, code gets triggered
- any code relied on code below is put above: methods and helper functions
- scans source code line and line and see which pattern matches
- things that start with a binary sequence and end with eoln, get judged by yes or no depending on top pattern or catch all pattern

- Usually looks like
    preamble
    named pattern (definitions) section
%%
    [rules section] has pattern {triggered code} ordered from top to bottom in order of precedence (remembering maximal munch)
        - Flex patterns are syntax for regexps
        ^ start of line
        $ end of line; isn't it just {EOLN}
        triggered-code gets cut and paste inside yylex method code
%%
    typically main (gets stitched on at end of flex file)

make oh-one-one
- call to flex and call to g++

(b|a+c|c)
    - a+c: at least one a followed by c?

Use pragma once instead of endif 
for class RELexer
    extend yyFlexLexer define constructor that calls default constructor for thing it's extending; if adding more features to scanner, need to initialize more things
    define yylex, which produces body in method
    producing a cc scanner using flex; inherit yyFlexLexer, define what yylex looks like and define own methods and instance variables to enhance what I'm doing

** use patty or download version of flex**

PART 2:
<INITIAL>1 {BEGIN(INITIAL); }
if pattern matches and im in state, then follow code; follow idiom where switching from this state to another state
laying out transition table
special way of laying out accept states: when hit EOLN and in accept state, call report(true), otherwise report(false)
build up current string so when reaching eoln we can report string
identify and process things without them being reg langs

class Lexer
left_open counter
instance variable left_open, so can report whether thing is accepted (can also just scan through thing built)
for initial state, count up one and stay in that state if )
if more than 0 (, decrement counter and stay in state.
When right paren and no left paren, then go intro trap state and never emerge

PART 3: Foodentation
doesn't follow previous parts; scanner reads entire text file
Rule: only matters if youre consistent to prior indentation when dedenting

scanner that produces normal token stream as long with indents/dedents
<stmt> ::= while <expn> : [INDENT] <blck> [DEDENT]

- baz: need to do a put back where things that match pattern (tkns) get put back into scanner state as not having been consumed because need to dedent first?

- foo lexer mimics bison: yylex, consume stuff+emit a token (but cant do that with dedents, so need to dechomp/ vomit)

-