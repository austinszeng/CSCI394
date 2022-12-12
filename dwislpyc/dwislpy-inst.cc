#include "dwislpy-ast.hh"
#include "dwislpy-inst.hh"

//
// dwislpy-inst.cc
//
// This gives the `trans` and the `trans_cndn` methods for all the
// supported AST nodes. These methods convert the AST into a sequence
// of IR instructions which are subclasses of class INST as defined in
// `dwislpy-inst.hh`.
//

//
// These are global variables for the top-level labels used for
// the string constants needed for the translation. They are set
// by `Prgm::trans`.
//
std::string EOLN_STRG_LBL; // Label for '\n'
std::string TRUE_STRG_LBL; // Label for 'True'
std::string FLSE_STRG_LBL; // Label for 'False'
std::string NONE_STRG_LBL; // Label for 'None'
std::string INPT_BUFF_LBL; // Label for the 80-character `input` buffer.

//
// Prgm::trans(void)
//
// Translate each of the definitions and the main script of the program
// into their intermediate representation,
//
void Prgm::trans(void) {
    
    // Make the global symbol table shared by all the program's IR.
    //
    glbl_symt_ptr = SymT_ptr { new SymT {} };

    // Create labels for the global string constants needed.
    //
    EOLN_STRG_LBL = glbl_symt_ptr->add_strg("\n"); 
    TRUE_STRG_LBL = glbl_symt_ptr->add_strg("True"); 
    FLSE_STRG_LBL = glbl_symt_ptr->add_strg("False"); 
    NONE_STRG_LBL = glbl_symt_ptr->add_strg("None"); 
    INPT_BUFF_LBL = glbl_symt_ptr->add_strg("12345678901234567890123456789012345678901234567890123456789012345678901234567890"); 

    // Translate each definition into IR.
    // 
    for (std::pair<Name,Defn_ptr> dfpr : defs) {
        Defn_ptr defn = dfpr.second;
        defn->symt.set_parent(glbl_symt_ptr); // Set parent to global table.
        defn->trans();
    }
    
    // Translate the main script into IR labelled as `main`
    //
    main_code = INST_vec {};
    main_symt.set_parent(glbl_symt_ptr);
    std::string def_lbl = main_symt.add_labl("main");
    std::string ext_lbl = main_symt.add_labl("main_done");
    //
    main_code.push_back(INST_ptr { new LBL {def_lbl} });
    main_code.push_back(INST_ptr { new ENTER {} });
    main->trans(ext_lbl,main_symt,main_code); // Note: ext_lbl won't get used.
    main_code.push_back(INST_ptr { new LBL {ext_lbl} });
    main_code.push_back(INST_ptr { new LEAVE {} });
}

//
// Defn::trans(void)
//
// Translate a definition's body into IR. Label its prologue with
// the function name. Label its epilogue so that `return` statements
// can jump to its code to exit.
//
void Defn::trans(void) {
    std::string def_lbl = symt.add_labl(name);
    std::string ext_lbl = symt.add_labl(name+"_done");
    //
    code.push_back(INST_ptr { new LBL {def_lbl} });
    code.push_back(INST_ptr { new ENTER {} });
    body->trans(ext_lbl,symt,code);
    code.push_back(INST_ptr { new LBL {ext_lbl} });
    code.push_back(INST_ptr { new LEAVE {} }); 
}

//
// Blck::trans(exit,symt,code)
//
// Translate the statements of a block of code into IR. Any `return`
// statements should jump to the target `exit`.
//
// The translation modifies `symt` and places new references to
// IR instructions into `code`.
//
void Blck::trans(std::string exit, SymT& symt, INST_vec& code) {
    for (Stmt_ptr stmt: stmts) {
        stmt->trans(exit,symt,code);
    }
}

// * * * * * 
//
// Stmt::trans(exit,symt,code)
//
// Translate a statements of into IR. A `return` statement should
// jump to the target `exit`.
//
// The translation modifies `symt` and places new references to
// IR instructions into `code`.
//

void Ntro::trans([[maybe_unused]]std::string exit,
                 SymT& symt, INST_vec& code) {
    expn->trans(name,symt,code);
}

void Asgn::trans([[maybe_unused]]std::string exit,
                 SymT& symt, INST_vec& code) {
    expn->trans(name,symt,code);
}

void PlEq::trans([[maybe_unused]]std::string exit,
                 SymT& symt, INST_vec& code) {
    std::string srce1 = symt.add_temp(expn->type);
    code.push_back(INST_ptr {new MOV {srce1,name}});
    std::string srce2 = symt.add_temp(expn->type);
    expn->trans(srce2,symt,code); 
    code.push_back(INST_ptr {new ADD {name,srce1,srce2}});
}

void MnEq::trans([[maybe_unused]]std::string exit,
                 SymT& symt, INST_vec& code) {
    std::string srce1 = symt.add_temp(expn->type);
    code.push_back(INST_ptr {new MOV {srce1,name}});
    std::string srce2 = symt.add_temp(expn->type);
    expn->trans(srce2,symt,code);
    code.push_back(INST_ptr {new SUB {name,srce1,srce2}});
}

void FRtn::trans(std::string exit, SymT& symt, INST_vec& code) {
    std::string temp = symt.add_temp(expn->type);
    expn->trans(temp,symt,code);
    code.push_back(INST_ptr {new RTN {temp}});
    code.push_back(INST_ptr {new JMP {exit}});
}

void IfEl::trans(std::string exit, SymT& symt, INST_vec& code) {
    std::string then_lbl = symt.add_labl();
    std::string else_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    cndn->trans_cndn(then_lbl,else_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {then_lbl}});
    then_blck->trans(exit,symt,code);
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {else_lbl}});
    else_blck->trans(exit,symt,code);
    code.push_back(INST_ptr {new LBL {done_lbl}});      
}

void Whle::trans(std::string exit, SymT& symt, INST_vec& code) {
    std::string loop_lbl = symt.add_labl();
    std::string true_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    code.push_back(INST_ptr {new LBL {loop_lbl}});
    // if condition is true, jump to true_label and evaluate block
    // else, jump to done_label
    cndn->trans_cndn(true_lbl,done_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    blck->trans(exit,symt,code);
    code.push_back(INST_ptr {new JMP {loop_lbl}});
    code.push_back(INST_ptr {new LBL {done_lbl}}); 
}

void PRtn::trans(std::string exit, SymT& symt, INST_vec& code) {
    std::string temp = symt.add_temp(NoneTy {});
    code.push_back(INST_ptr {new SET {temp,0}});
    code.push_back(INST_ptr {new RTN {temp}});
    code.push_back(INST_ptr {new JMP {exit}});
}

void Pass::trans([[maybe_unused]]std::string exit,
                 [[maybe_unused]]SymT& symt, INST_vec& code) {
    code.push_back(INST_ptr {new NOP {}});
}

void PCll::trans([[maybe_unused]]std::string exit,
                 [[maybe_unused]]SymT& symt, INST_vec& code) {
    int i = 0;
    for (auto param : params) {
        std::string srce = symt.add_temp(param->type);
        param->trans(srce,symt,code);
        code.push_back(INST_ptr {new ARG {i, srce}});
        i++;
    }
    // Call f
    code.push_back(INST_ptr {new CLL {name}});
    // ignore return value of None
}

void Prnt::trans([[maybe_unused]]std::string exit,
                 SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(expn->type)) {
        std::string temp = symt.add_temp(IntTy {});
        expn->trans(temp,symt,code);
        code.push_back(INST_ptr {new PTI {temp}});
    }
    if (std::holds_alternative<StrTy>(expn->type)) {
        std::string temp = symt.add_temp(StrTy {});
        expn->trans(temp,symt,code);
        code.push_back(INST_ptr {new PTS {temp}});
    }
    if (std::holds_alternative<BoolTy>(expn->type)) {
        std::string true_lbl = symt.add_labl();
        std::string flse_lbl = symt.add_labl();
        std::string done_lbl = symt.add_labl();
        std::string temp = symt.add_temp(BoolTy {});
        //
        expn->trans_cndn(true_lbl,flse_lbl,symt,code);
        code.push_back(INST_ptr {new LBL {true_lbl}});
        code.push_back(INST_ptr {new STL {temp,TRUE_STRG_LBL}});
        code.push_back(INST_ptr {new JMP {done_lbl}});
        code.push_back(INST_ptr {new LBL {flse_lbl}});
        code.push_back(INST_ptr {new STL {temp,FLSE_STRG_LBL}});
        code.push_back(INST_ptr {new LBL {done_lbl}});
        code.push_back(INST_ptr {new PTS {temp}});        
    }
    if (std::holds_alternative<NoneTy>(expn->type)) {
        std::string dumm = symt.add_temp(NoneTy {});
        std::string temp = symt.add_temp(StrTy {});
        //
        expn->trans(dumm,symt,code);
        code.push_back(INST_ptr {new STL {temp,NONE_STRG_LBL}});
        code.push_back(INST_ptr {new PTS {temp}});        
    }
    std::string eoln = symt.add_temp(StrTy {});
    code.push_back(INST_ptr {new STL {eoln,EOLN_STRG_LBL}});
    code.push_back(INST_ptr {new PTS {eoln}});        
}

// * * * * * 
//
// Expn::trans(dest,symt,code)
//
// Translate an expression of into IR in such a way that the resulting
// value gets placed into `dest`.
//
// The translation modifies `symt` and places new references to
// IR instructions into `code`.
//
// For some `Expn` subclasses we also define:
//
//    Expn::trans_cndn(true_lbl,else_lbl,symt,code)
//
// This translates a boolean condition expressiom into IR such
// that it jumps to `then_lbl` when the expression is `True`,
// and jumps to `else_lbl` when the expression is `False`.
//

void Plus::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new ADD {dest,srce1,srce2}});
    }
}

void Mnus::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new SUB {dest,srce1,srce2}});
    }
}

void Tmes::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new MUL {dest,srce1,srce2}});
    }
}

void IDiv::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new DIV {dest,srce1,srce2}});
    }
}

void IMod::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new MOD {dest,srce1,srce2}});
    }
}

void Expn::trans_cndn([[maybe_unused]]std::string then_lbl,
                      [[maybe_unused]]std::string else_lbl,
                      [[maybe_unused]]SymT& symt,
                      [[maybe_unused]]INST_vec& code) {
    
    // This is a bogus method that should be overridden by
    // any expression that could yield a `bool` value.
    
}

// Used as a condition in a while or an if
void Less::trans_cndn(std::string then_lbl, std::string else_lbl,
                      SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(left->type)
        && std::holds_alternative<IntTy>(rght->type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new BCN {"lt",
                                          srce1,srce2,
                                          then_lbl,else_lbl}});
    }
}

// Computes an actual bool value rather than jumping to a place in code
void Less::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void LsEq::trans_cndn(std::string then_lbl, std::string else_lbl,
                      SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(left->type)
        && std::holds_alternative<IntTy>(rght->type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new BCN {"le",
                                          srce1,srce2,
                                          then_lbl,else_lbl}});
    }
}

void LsEq::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void Equl::trans_cndn(std::string then_lbl, std::string else_lbl,
                      SymT& symt, INST_vec& code) {
    if (std::holds_alternative<IntTy>(left->type)
        && std::holds_alternative<IntTy>(rght->type)) {
        std::string srce1 = symt.add_temp(left->type);
        std::string srce2 = symt.add_temp(rght->type);
        left->trans(srce1,symt,code);
        rght->trans(srce2,symt,code);
        code.push_back(INST_ptr {new BCN {"eq",
                                          srce1,srce2,
                                          then_lbl,else_lbl}});
    }
}

void Equl::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void And::trans_cndn(std::string then_lbl, std::string else_lbl,
                     SymT& symt, INST_vec& code) {
    std::string cont_lbl = symt.add_labl();
    // If left condition is false, jump to else_lbl
    left->trans_cndn(cont_lbl,else_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {cont_lbl}});    
    rght->trans_cndn(then_lbl,else_lbl,symt,code);
}

void And::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void Or::trans_cndn(std::string then_lbl, std::string else_lbl,
                     SymT& symt, INST_vec& code) {
    std::string cont_lbl = symt.add_labl();
    // If left condition is false, check right condition
    // If left condition is true, jump to then_lbl (we're done)
    left->trans_cndn(then_lbl,cont_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {cont_lbl}});    
    rght->trans_cndn(then_lbl,else_lbl,symt,code);
}

void Or::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void Not::trans_cndn(std::string then_lbl, std::string else_lbl,
                     SymT& symt, INST_vec& code) {
    expn->trans_cndn(else_lbl,then_lbl,symt,code);
}

void Not::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string true_lbl = symt.add_labl();
    std::string flse_lbl = symt.add_labl();
    std::string done_lbl = symt.add_labl();
    trans_cndn(true_lbl,flse_lbl,symt,code);
    code.push_back(INST_ptr {new LBL {true_lbl}});
    code.push_back(INST_ptr {new SET {dest,1}});
    code.push_back(INST_ptr {new JMP {done_lbl}});
    code.push_back(INST_ptr {new LBL {flse_lbl}});
    code.push_back(INST_ptr {new SET {dest,0}});
    code.push_back(INST_ptr {new LBL {done_lbl}});
}

void Ltrl::trans(std::string dest, SymT& symt, INST_vec& code) {
    if (std::holds_alternative<int>(valu)) {
        int ival = std::get<int>(valu);
        code.push_back(INST_ptr {new SET {dest,ival}});
    }
    if (std::holds_alternative<std::string>(valu)) {
        std::string sval = std::get<std::string>(valu);
        std::string strg_lbl = symt.add_strg(sval);
        code.push_back(INST_ptr {new STL {dest,strg_lbl}});
    }
    if (std::holds_alternative<bool>(valu)) {
        bool bval = std::get<bool>(valu);
        if (bval) {
            code.push_back(INST_ptr {new SET {dest,1}});
        } else {
            code.push_back(INST_ptr {new SET {dest,0}});
        }
    }
    if (std::holds_alternative<none>(valu)) {
        code.push_back(INST_ptr {new SET {dest,0}});
    }
}

void Ltrl::trans_cndn(std::string then_lbl, std::string else_lbl,
                      [[maybe_unused]]SymT& symt, INST_vec& code) { 
    bool bval = std::get<bool>(valu);
    if (bval) {
        code.push_back(INST_ptr {new JMP {then_lbl}});
    } else {
        code.push_back(INST_ptr {new JMP {else_lbl}});
    }
}

void Lkup::trans(std::string dest, [[maybe_unused]]SymT& symt,
                 INST_vec& code) {
    code.push_back(INST_ptr {new MOV {dest,name}});
}

void Lkup::trans_cndn(std::string then_lbl, std::string else_lbl,
                      [[maybe_unused]]SymT& symt, INST_vec& code) { 
    code.push_back(INST_ptr {new BCZ {"gtz",name,then_lbl,else_lbl}});
}

void Inpt::trans(std::string dest, SymT& symt, INST_vec& code) {
    std::string strg = symt.add_temp(StrTy {});
    expn->trans(strg,symt,code);
    code.push_back(INST_ptr {new PTS {strg}});
    code.push_back(INST_ptr {new GTI {dest}});
}

void IntC::trans([[maybe_unused]]std::string dest,
                 [[maybe_unused]]SymT& symt, INST_vec& code) {
    // 
}

void StrC::trans([[maybe_unused]]std::string dest,
                 [[maybe_unused]]SymT& symt, INST_vec& code) {
    // 
}

// Generate code that evaluates inputs and set args 0, 1, ... to their results
void FCll::trans([[maybe_unused]]std::string dest,
                 [[maybe_unused]]SymT& symt, INST_vec& code) {
    int i = 0;
    for (auto param : params) {
        std::string srce = symt.add_temp(param->type);
        param->trans(srce,symt,code);
        code.push_back(INST_ptr {new ARG {i, srce}});
        i++;
    }
    // Call f, get value returned by f, and place it into x
    code.push_back(INST_ptr {new CLL {name}});
    code.push_back(INST_ptr {new RTV {dest}});
}

// Functions may return a bool which can be used as conditions within while and if
void FCll::trans_cndn(std::string then_lbl, std::string else_lbl,
                      [[maybe_unused]]SymT& symt, INST_vec& code) { 
    // Generate code that evaluates f
    std::string srce = symt.add_temp(type);
    trans(srce,symt,code);
    // If f evaluates to false, branch to else block
    code.push_back(INST_ptr {new BCZ {"eqz",
                                        srce,
                                        else_lbl,then_lbl}});
}