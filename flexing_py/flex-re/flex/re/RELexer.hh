#ifndef _RELEXER_HH
#define _RELEXER_HH

#include <iostream>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class RELexer : public yyFlexLexer {
public:
    RELexer(std::istream *in) : yyFlexLexer { in } { }
    virtual ~RELexer() { }
    
    //get rid of override virtual function warning
    using FlexLexer::yylex;

    // Critical method for supporting Bison parsing.
    virtual int yylex(void);
    // YY_DECL defined in .ll
    // Method body created by flex in .cc
};

#endif 
