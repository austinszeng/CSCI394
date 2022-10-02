%{
    #include <string>
    #include <iostream>
    #include <fstream>
    #include "RELexer.hh"

    #undef YY_DECL
    #define YY_DECL int RELexer::yylex(void)

    #define yyterminate() return 0
    #define YY_NO_UNISTD_H
    
    std::string remove_EOLNs(std::string txt) {
        int ending = txt.length();
        while (txt[ending-1] == '\r' || txt[ending-1] == '\n') {
            ending--;
        }
        return txt.substr(0,ending);
    }
        
%}

%option debug
%option nodefault
%option noyywrap
%option yyclass="Lexer"
%option c++
    
EOLN    \r\n|\n\r|\n|\r

%%

%{

%}
    
^[01]*011[01]*{EOLN} {
    std::cout << remove_EOLNs(yytext) << " YES" << std::endl;
}
    
^[01]*{EOLN} {
    std::cout << remove_EOLNs(yytext) << " NO" << std::endl;
}

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
    RELexer lexer { &ins };
    return lexer.yylex();
}
