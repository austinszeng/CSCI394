%{
    #include <string>
    #include <iostream>
    #include <fstream>
    #include "balanced.hh"

    #undef YY_DECL
    #define YY_DECL int balanced::Lexer::yylex(void)

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
    
    void balanced::Lexer::process(std::string txt) {
        current = current + txt;
    }
    
    void balanced::Lexer::report(bool accepted) {
        std::cout << remove_EOLNs(current) << " ";
        if (accepted && left_open == 0) {
            std::cout << "YES";
        } else {   
            std::cout << "NO";
        }
        std::cout << std::endl;
        current = "";
        left_open = 0;
        BEGIN(0);
    }
%}

%option debug
%option nodefault
%option noyywrap
%option yyclass="Lexer"
%option c++

%s TRAP
     
EOLN    \r\n|\n\r|\n|\r

%%

%{

%}

<INITIAL>"("     { left_open++; BEGIN(INITIAL); }
<INITIAL>")"     {
    if (left_open == 0) {
        BEGIN(TRAP);
    } else {
        left_open--;
        BEGIN(INITIAL);
    }
}
<TRAP>["("|")"]   { BEGIN(TRAP); }
<INITIAL>{EOLN}   { report(true); }
<TRAP>{EOLN}      { report(false); }

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
    balanced::Lexer lexer { &ins };
    return lexer.yylex();
}
