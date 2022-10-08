%{
    #include <string>
    #include <iostream>
    #include <fstream>
    #include "FALexer.hh"

    #undef YY_DECL
    #define YY_DECL int FALexer::yylex(void)

    #define YY_USER_ACTION process(yytext);
    
    #define yyterminate() return 0
    #define YY_NO_UNISTD_H

    std::string remove_EOLNs(std::string txt) {
        int ending = txt.length();
        while (txt[ending-1] == '\r' || txt[ending-1] == '\n') {
            ending--;
        }
        return txt.substr(0,ending);
    }
    
    void FALexer::process(std::string txt) {
        current = current + txt;
    }
    
    void FALexer::report(bool accepted) {
        std::cout << remove_EOLNs(current) << " ";
        if (accepted) {
            std::cout << " YES";
        } else {   
            std::cout << " NO";
        }
        std::cout << std::endl;
        current = "";
        BEGIN(0);
    }

%}

%option debug
%option nodefault
%option noyywrap
%option yyclass="Lexer"
%option c++

%s SAWa SAWab ACCEPT
     
EOLN    \r\n|\n\r|\n|\r

%%

%{

%}

<INITIAL>{EOLN}  { report(true); }
<INITIAL>a       { BEGIN(SAWa); }
<INITIAL>b       { BEGIN(INITIAL); }
<INITIAL>c       { BEGIN(INITIAL); }
<SAWa>a          { BEGIN(SAWa); }
<SAWa>b          { BEGIN(SAWab); }
<SAWa>c          { BEGIN(INITIAL); }
<SAWa>{EOLN}     { report(true); }
<SAWab>a         { BEGIN(ACCEPT); }
<SAWab>b         { BEGIN(SAWab); }
<SAWab>c         { BEGIN(SAWab); }
<ACCEPT>a        { BEGIN(ACCEPT); }
<ACCEPT>b        { BEGIN(INITIAL); }
<ACCEPT>c        { BEGIN(SAWab); }
<*>{EOLN}        { report(false); }

<<EOF>> {
    return 0;
}

. {
    std::string txt { yytext };
    std::cerr << "Unexpected \"" << txt << "\" in input." << std::endl;
    return -1;
}
    
%%

int main(int argc, char** argv) {
    std::string src_name { argv[1] };
    std::ifstream ins { src_name };
    FALexer lexer { &ins };
    return lexer.yylex();
}
