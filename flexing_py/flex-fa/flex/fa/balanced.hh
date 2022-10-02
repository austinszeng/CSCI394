#ifndef _BALANCED_HH
#define _BALANCED_HH

#include <iostream>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace balanced {
class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream *in) :
        yyFlexLexer { in },
        current { "" },
        left_open { 0 }
    { }
    virtual ~Lexer() { }
    
    //get rid of override virtual function warning
    using FlexLexer::yylex;

    // Critical method for supporting Bison parsing.
    virtual int yylex(void);
    // YY_DECL defined in .ll
    // Method body created by flex in .cc

private:
    int left_open;
    void report(bool accepted);
    //
    std::string current; // Holds the characters processed so far.
    void process(std::string txt);
};
    
} // end of namespace balanced

#endif 
