%{
    #include <string>
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include "balanced-bracks.hh"

    #undef YY_DECL
    #define YY_DECL int balanced_bracks::Lexer::yylex(void)

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
    
    void balanced_bracks::Lexer::process(std::string txt) {
        current = current + txt;
    }
    
    void balanced_bracks::Lexer::report(bool accepted) {
        std::cout << remove_EOLNs(current) << " ";
        if (accepted && vector.empty()) {
            std::cout << " YES";
        } else {   
            std::cout << " NO";
        }
        std::cout << std::endl;
        current = "";
        vector.clear();
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

<INITIAL>"("     { vector.push_back('('); BEGIN(INITIAL); }
<INITIAL>"{"     { vector.push_back('{'); BEGIN(INITIAL); }
<INITIAL>"["     { vector.push_back('['); BEGIN(INITIAL); }
<INITIAL>"<"     { vector.push_back('<'); BEGIN(INITIAL); }
<INITIAL>")"     {
    if (!vector.empty() && vector.back() == '(') {
        vector.pop_back();
        BEGIN(INITIAL);
    } else {
        BEGIN(TRAP);
    }
}
<INITIAL>"}"     {
    if (!vector.empty() && vector.back() == '{') {
        vector.pop_back();
        BEGIN(INITIAL);
    } else {
        BEGIN(TRAP);
    }
}
<INITIAL>"]"     {
    if (!vector.empty() && vector.back() == '[') {
        vector.pop_back();
        BEGIN(INITIAL);
    } else {
        BEGIN(TRAP);
    }
}
<INITIAL>">"     {
    if (!vector.empty() && vector.back() == '<') {
        vector.pop_back();
        BEGIN(INITIAL);
    } else {
        BEGIN(TRAP);
    }
}
<TRAP>["("|")"]   { BEGIN(TRAP); }
<TRAP>["{"|"}"]   { BEGIN(TRAP); }
<TRAP>("["|"]")   { BEGIN(TRAP); } 
<TRAP>["<"|">"]   { BEGIN(TRAP); }
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
    balanced_bracks::Lexer lexer { &ins };
    return lexer.yylex();
}
