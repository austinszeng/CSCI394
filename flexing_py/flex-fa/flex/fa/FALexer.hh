#ifndef _FALEXER_HH
#define _FALEXER_HH

#include <iostream>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class FALexer : public yyFlexLexer {
public:
    FALexer(std::istream *in) : yyFlexLexer { in }, current { "" } { }
    virtual ~FALexer() { }
    
    //get rid of override virtual function warning
    using FlexLexer::yylex;

    // Critical method for supporting Bison parsing.
    virtual int yylex(void);
    // YY_DECL defined in .ll
    // Method body created by flex in .cc

private:
    std::string current; // Holds the characters processed so far.
    void process(std::string txt);
    void report(bool accepted);
};

#endif 
