%{
    #include <string>
    #include <iostream>
    #include <fstream>
    #include "foo.hh"

    #undef YY_DECL
    #define YY_DECL int foo::Lexer::yylex(void)

    #define YY_USER_ACTION advance_location(yytext);
    
    #define yyterminate() return 0
    #define YY_NO_UNISTD_H

    void foo::Lexer::advance_location(std::string txt) {
        start_line = line;
        start_column = column;
        for (char c: txt) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        }
    }
    
    void foo::Lexer::do_foo(void) {
        std::cout << "FOO:" << start_line << ":" << start_column << std::endl;
    }
%}

%option debug
%option nodefault
%option noyywrap
%option yyclass="foo::Lexer"
%option c++

EOLN    \r\n|\n\r|\n|\r

%s ERROR
    
%%

%{

%}

   
foo{EOLN} { do_foo(); }

" "       { }
        
<<EOF>>   { return 0; }

. {
    std::string txt { yytext };
    std::cerr << "Unexpected \"" << txt << "\" in input." << std::endl;
    return -1;
}
    
%%

int main(int argc, char** argv) {
    std::string src_name { argv[1] };
    std::ifstream ins { src_name };
    foo::Lexer lexer { &ins };
    return lexer.yylex();
}
