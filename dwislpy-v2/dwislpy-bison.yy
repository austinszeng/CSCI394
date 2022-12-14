%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {DWISLPY}
%define api.parser.class {Parser}
%define parse.error verbose
    
%code requires{
    
    #include "dwislpy-ast.hh"

    namespace DWISLPY {
        class Driver;
        class Lexer;
    }

    #define YY_NULLPTR nullptr

}

%parse-param { Lexer  &lexer }
%parse-param { Driver &main  }
    
%code{

    #include <sstream>
    #include "dwislpy-util.hh"    
    #include "dwislpy-main.hh"

    #undef yylex
    #define yylex lexer.yylex
    
}


%define api.value.type variant
%define parse.assert
%define api.token.prefix {Token_}

%locations

%token               EOFL  0  
%token               EOLN
%token               INDT
%token               DEDT
%token               PASS "pass"
%token               PRNT "print"
%token               INPT "input"
%token               INTC "int"
%token               STRC "str"
%token               ASGN "="
%token               PLEQ "+="
%token               MNEQ "-="
%token               IFST "if"
%token               ELSE "else"
%token               WHLE "while"
%token               PLUS "+"
%token               MNUS "-"
%token               TMES "*"
%token               IDIV "//"
%token               IMOD "%"
%token               IAND "and"
%token               IOR "or"
%token               LESS "<"
%token               LSEQ "<="
%token               EQUL "=="
%token               INOT "not"
%token               LPAR "(" 
%token               RPAR ")"
%token               COLN ":"
%token               NONE "None"
%token               TRUE "True"
%token               FALS "False"
%token <int>         NMBR
%token <std::string> NAME
%token <std::string> STRG

%type <Prgm_ptr> prgm
%type <Blck_ptr> blck
%type <Stmt_vec> stms
%type <Stmt_ptr> stmt
%type <Expn_ptr> expn

%%

%start main;


%left IOR;
%left IAND;
%left INOT;

%nonassoc LESS;
%nonassoc LSEQ;
%nonassoc EQUL;

%left PLUS MNUS;
%left TMES IMOD IDIV;
    
main:
  prgm {
      main.set($1);
  }
;

prgm:
  blck {
      Defs ds { };
      Blck_ptr b = $1; 
      $$ = Prgm_ptr { new Prgm {ds, b, b->where()} };
  }   
;

blck:
  stms {
      Stmt_vec ss = $1;
      $$ = Blck_ptr { new Blck {ss, ss[0]->where()} };
  }
;

stms:
  stms stmt {
      Stmt_vec ss = $1;
      ss.push_back($2);
      $$ = ss;
  }
| stmt {
      Stmt_vec ss { };
      ss.push_back($1);
      $$ = ss;
  }
;
  
stmt: 
  NAME ASGN expn EOLN {
      $$ = Asgn_ptr { new Asgn {$1,$3,lexer.locate(@2)} };
  }
| NAME PLEQ expn EOLN {
      $$ = PlEq_ptr { new PlEq {$1,$3,lexer.locate(@2)} };
  }
| NAME MNEQ expn EOLN {
      $$ = MnEq_ptr { new MnEq {$1,$3,lexer.locate(@2)} };
  }
| IFST expn COLN EOLN INDT blck DEDT ELSE COLN EOLN INDT blck DEDT {
      $$ = IfSt_ptr { new IfSt {$2, lexer.locate(@1), $6, $12} };
  }
| IFST expn COLN EOLN INDT blck DEDT ELSE COLN EOLN INDT blck EOFL {
      $$ = IfSt_ptr { new IfSt {$2, lexer.locate(@1), $6, $12} };
  }
| WHLE expn COLN EOLN INDT blck DEDT {
      $$ = Whle_ptr { new Whle {$2, lexer.locate(@1), $6} };
  }
| WHLE expn COLN EOLN INDT blck EOFL {
      $$ = Whle_ptr { new Whle {$2, lexer.locate(@1), $6} };
  }
| PASS EOLN {
      $$ = Pass_ptr { new Pass {lexer.locate(@1)} };
  }
| PRNT LPAR expn RPAR EOLN {
      $$ = Prnt_ptr { new Prnt {$3,lexer.locate(@1)} };
  }
;

expn:
  expn PLUS expn {
      $$ = Plus_ptr { new Plus {$1,$3,lexer.locate(@2)} };
  }
| expn MNUS expn {
      $$ = Mnus_ptr { new Mnus {$1,$3,lexer.locate(@2)} };
  }
| expn TMES expn {
      $$ = Tmes_ptr { new Tmes {$1,$3,lexer.locate(@2)} };
  }
| expn IDIV expn {
      $$ = IDiv_ptr { new IDiv {$1,$3,lexer.locate(@2)} };
  }
| expn IMOD expn {
      $$ = IMod_ptr { new IMod {$1,$3,lexer.locate(@2)} };
  }
| expn IAND expn {
      $$ = IAnd_ptr { new IAnd {$1,$3,lexer.locate(@2)} };
  }
| expn IOR expn {
      $$ = IOr_ptr { new IOr {$1,$3,lexer.locate(@2)} };
  }
| expn LESS expn {
      $$ = Less_ptr { new Less {$1,$3,lexer.locate(@2)} };
  }
| expn LSEQ expn {
      $$ = LsEq_ptr { new LsEq {$1,$3,lexer.locate(@2)} };
  }
| expn EQUL expn {
      $$ = Equl_ptr { new Equl {$1,$3,lexer.locate(@2)} };
  }
| INOT expn {
      $$ = INot_ptr { new INot {$2, lexer.locate(@1)} };
  }
| NMBR {
      $$ = Ltrl_ptr { new Ltrl {Valu {$1},lexer.locate(@1)} };
  }
| STRG {
      $$ = Ltrl_ptr { new Ltrl {Valu {de_escape($1)},lexer.locate(@1)} };
  }
| TRUE {
      $$ = Ltrl_ptr { new Ltrl {Valu {true},lexer.locate(@1)} };
  }
| FALS {
      $$ = Ltrl_ptr { new Ltrl {Valu {false},lexer.locate(@1)} };
  }
| NONE {
      $$ = Ltrl_ptr { new Ltrl {Valu {None},lexer.locate(@1)} };
  }    
| INPT LPAR expn RPAR {
      $$ = Inpt_ptr { new Inpt {$3,lexer.locate(@1)} };
  }
| INTC LPAR expn RPAR {
      $$ = IntC_ptr { new IntC {$3,lexer.locate(@1)} };
  }
| STRC LPAR expn RPAR {
      $$ = StrC_ptr { new StrC {$3,lexer.locate(@1)} };
  }
| NAME {
      $$ = Lkup_ptr { new Lkup {$1,lexer.locate(@1)} };
  }
| LPAR expn RPAR {
      $$ = $2;
  }
;

%%
       
void DWISLPY::Parser::error(const location_type &loc, const std::string &msg) {
    throw DwislpyError { lexer.locate(loc), msg };
}
