#ifndef _BALANCED_BRACKS_HH
#define _BALANCED_BRACKS_HH

#include <iostream>

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#include <vector>
#endif

namespace balanced_bracks {
class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream *in) :
        yyFlexLexer { in },
        current { "" },
        vector { }
    { }
    virtual ~Lexer() { }
    
    //get rid of override virtual function warning
    using FlexLexer::yylex;

    // Critical method for supporting Bison parsing.
    virtual int yylex(void);
    // YY_DECL defined in .ll
    // Method body created by flex in .cc

private:
    std::vector<char> vector;
    void report(bool accepted);
    //
    std::string current; // Holds the characters processed so far.
    void process(std::string txt);
};
    
} // end of namespace balanced_bracks

#endif 
