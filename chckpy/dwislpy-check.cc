#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "dwislpy-check.hh"
#include "dwislpy-ast.hh"
#include "dwislpy-util.hh"

bool is_int(Type type) {
    return std::holds_alternative<IntTy>(type);
}

bool is_str(Type type) {
    return std::holds_alternative<StrTy>(type);
}

bool is_bool(Type type) {
    return std::holds_alternative<BoolTy>(type);
}

bool is_None(Type type) {
    return std::holds_alternative<NoneTy>(type);
}

bool operator==(Type type1, Type type2) {
    if (is_int(type1) && is_int(type2)) {
        return true;
    }
    if (is_str(type1) && is_str(type2)) {
        return true;
    }
    if (is_bool(type1) && is_bool(type2)) {
        return true;
    }
    if (is_None(type1) && is_None(type2)) {
        return true;
    }
    return false;
}

bool operator!=(Type type1, Type type2) {
    return !(type1 == type2);
}

std::string type_name(Type type) {
    if (is_int(type)) {
        return "int";
    }
    if (is_str(type)) {
        return "str";
    }
    if (is_bool(type)) {
        return "bool";
    } 
    if (is_None(type)) {
        return "None";
    }
    return "wtf";
}

unsigned int Defn::arity(void) const {
    return symt.get_frmls_size();
}

Type Defn::returns(void) const {
    return rety;
}

SymInfo_ptr Defn::formal(int i) const {
    return symt.get_frml(i);
}

void Prgm::chck(void) {
    for (std::pair<Name,Defn_ptr> dfpr : defs) {
        dfpr.second->chck(defs);
    }
    // Check the main script with Blck::chck
    // Modifies main_sympt accordingly
    Rtns rtns = main->chck(Rtns{Void {}},defs,main_symt);
    if (!std::holds_alternative<Void>(rtns)) {
        DwislpyError(main->where(), "Main script should not return.");
    }
}

void Defn::chck(Defs& defs) {
    // Pass initial 'symt' to the body->chck call
    Rtns rtns = body->chck(Rtns{rety}, defs, symt);
    // specify return behavior according to 'rety'
    if (std::holds_alternative<Void>(rtns)) {
        throw DwislpyError(body->where(), "Definition body never returns.");
    }
    if (std::holds_alternative<VoidOr>(rtns)) {
        throw DwislpyError(body->where(), "Definition body might not return.");
    }
}

Type type_of(Rtns rtns) {
    if (std::holds_alternative<VoidOr>(rtns)) {
        return std::get<VoidOr>(rtns).type;
    }
    if (std::holds_alternative<Type>(rtns)) {
        return std::get<Type>(rtns);
    }
    return Type {NoneTy {}}; // Should not happen.
}

Rtns Blck::chck(Rtns expd, Defs& defs, SymT& symt) {
    
    // Scan through the statements and check their return behavior.
    Rtns last_stmt;
    bool fallThrough = true; 
    for (Stmt_ptr stmt : stmts) {
        
        // Check this statement.
        Rtns stmt_rtns = stmt->chck(expd, defs, symt);

        // Type of statement should be Void or VoidOr
        if (std::holds_alternative<Type>(stmt_rtns) && fallThrough) {
            fallThrough = false;
        } else if (fallThrough == false) {
            throw DwislpyError(where(), "Only last statement should return.");
        }

        last_stmt = stmt_rtns;
    }          
    // Check if last statement/ return statement returns expected type  
    if (type_of(last_stmt) != type_of(expd)) {
        throw DwislpyError(where(), "Incorrect return type.");
    }

    return expd; 
}

Rtns Asgn::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    if (!symt.has_info(name)) {
        throw DwislpyError(where(), "Variable '" + name + "' never introduced.");
    }
    Type name_ty = symt.get_info(name)->type;
    Type expn_ty = expn->chck(defs,symt);
    if (name_ty != expn_ty) {
        std::string msg = "Type mismatch. Expected expression of type ";
        msg += type_name(name_ty) + ".";
        throw DwislpyError {expn->where(), msg};
    }
    return Rtns {Void {}};
}

Rtns PlEq::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    if (!symt.has_info(name)) {
        throw DwislpyError(where(), "Variable '" + name + "' never introduced.");
    }
    Type name_ty = symt.get_info(name)->type;
    Type expn_ty = expn->chck(defs,symt);
    if (name_ty != expn_ty) {
        std::string msg = "Type mismatch. Expected expression of type ";
        msg += type_name(name_ty) + ".";
        throw DwislpyError {expn->where(), msg};
    }
    return Rtns {Void {}};
} 

Rtns MnEq::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    if (!symt.has_info(name)) {
        throw DwislpyError(where(), "Variable '" + name + "' never introduced.");
    }
    Type name_ty = symt.get_info(name)->type;
    Type expn_ty = expn->chck(defs,symt);
    if (name_ty != expn_ty) {
        std::string msg = "Type mismatch. Expected expression of type ";
        msg += type_name(name_ty) + ".";
        throw DwislpyError {expn->where(), msg};
    }
    return Rtns {Void {}};
} 

Rtns Pass::chck([[maybe_unused]] Rtns expd,
                [[maybe_unused]] Defs& defs,
                [[maybe_unused]] SymT& symt) {
    return Rtns {Void {}};
}

Rtns Prnt::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    [[maybe_unused]] Type expn_ty = expn->chck(defs,symt);
    return Rtns {Void {}};
}

Rtns Ntro::chck([[maybe_unused]] Rtns expd, [[maybe_unused]] Defs& defs, SymT& symt) {
    // Record info about newly-introduced variable into symbol table symt
    symt.add_locl(name, type);
    return Rtns {Void {}};
}

Rtns FRtn::chck(Rtns expd, Defs& defs, SymT& symt) {
    // check the expression of the return, examine its type
    Type expn_ty = expn->chck(defs,symt);
    Type expd_ty = type_of(expd);
    // compare that type with the return behavior expected by expd
    // if mistmatch. raise an error
    if (expn_ty != expd_ty) {
        std::string msg = "Type mismatch. Expected expression of type ";
        msg += type_name(type_of(expd)) + ".";
        throw DwislpyError {expn->where(), msg};
    }
    // else, return behavior of this function return statement. 
    return expd;
}

Rtns PRtn::chck(Rtns expd,
                [[maybe_unused]] Defs& defs, [[maybe_unused]] SymT& symt) {
    if (std::holds_alternative<Void>(expd)) {
        throw DwislpyError {where(), "Unexpected return statement."};
    }
    Type expd_ty = type_of(expd);
    if (!is_None(expd_ty)) {
        throw DwislpyError {where(), "A procedure does not return a value."};
    }
    return Rtns {Type {NoneTy {}}};
}

Rtns PCll::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    //
    // This should look up a procedure's definition. It should check:
    // * that the correct number of arguments are passed.
    // * that the return type is None
    // * that each of the argument expressions type check
    // * that the type of each argument matches the type signature
    //
    for (std::pair<Name,Defn_ptr> dfpr : defs) {
        if (dfpr.first == name) {
            Defn_ptr dptr = dfpr.second;
            if (params.size() != dptr->symt.get_frmls_size()) {
                std::string msg = "Incorrect number of arguments.";
                throw DwislpyError { where(), msg };
            };
            for (unsigned int i=0; i<symt.get_frmls_size(); i++) {
                if(symt.get_frml(i) != dptr->symt.get_frml(i)){
                    std::string msg = "Argument ";
                    msg += std::to_string(i) + " is an incorrect type.";
                    throw DwislpyError {where(), msg};
                }
            }
            if (dptr->rety == type_of(expd)) {
                return expd;
            }
        }
    }
    return Rtns {Void {}};
}

Rtns IfEl::chck(Rtns expd, Defs& defs, SymT& symt) {
    //
    // This should check that the condition is a bool.
    // It should check each of the two blocks return behavior.
    // It should summarize the return behavior.
    //
    [[maybe_unused]] Type cndn_ty = cndn->chck(defs,symt);
    // For some reason, this gives an error vv (either way, i don't think it's needed)
    // if (!is_bool(cndn_ty)) {
    //     throw DwislpyError {where(), "The if-statement is not of type bool."};
    // }
    Type then_blck_ty = type_of(then_blck->chck(expd,defs,symt));
    Type else_blck_ty = type_of(else_blck->chck(expd,defs,symt));
    Type expd_ty = type_of(expd);
    if (then_blck_ty == expd_ty and else_blck_ty == expd_ty) {
        return expd;
    } else {
        return Rtns {VoidOr {expd_ty}};
    }
}


Rtns Whle::chck(Rtns expd, Defs& defs, SymT& symt) {
    //
    // This should check that the condition is a bool.
    // It should check the block's return behavior.
    // It should summarize the return behavior. It should be Void
    // or VoidOr because loop bodies don't always execute.
    // 
    Type cndn_ty = cndn->chck(defs,symt);
    if (!is_bool(cndn_ty)) {
        throw DwislpyError {where(), "The while-statement condition is not of type bool."};
    }

    Type blck_ty = type_of(blck->chck(expd,defs,symt));
    Type expd_ty = type_of(expd);
    if (blck_ty == expd_ty) {
        return Rtns {VoidOr {expd_ty}};
    } else {
        return Rtns {Void {}};
    }
}


Type Plus::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else if (is_str(left_ty) && is_str(rght_ty)) {
        return Type {StrTy {}};
    } else {
        std::string msg = "Wrong operand types for plus.";
        throw DwislpyError { where(), msg };
    }
}

Type Mnus::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else {
        std::string msg = "Wrong operand types for minus.";
        throw DwislpyError { where(), msg };
    }
    return Type {IntTy {}}; 
}

Type Tmes::chck([[maybe_unused]] Defs& defs, [[maybe_unused]] SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};;
    } else {
        std::string msg = "Wrong operand types for times.";
        throw DwislpyError { where(), msg };
    }
    return Type {IntTy {}}; 
}

Type IDiv::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else {
        std::string msg = "Wrong operand types for division.";
        throw DwislpyError { where(), msg };
    }
    return Type {IntTy {}}; 
}

Type IMod::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else {
        std::string msg = "Wrong operand types for mod.";
        throw DwislpyError { where(), msg };
    }
    return Type {IntTy {}}; 
}

Type Less::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else if (is_str(left_ty) && is_str(rght_ty)) {
        return Type {StrTy {}};
    } else if (is_bool(left_ty) && is_bool(rght_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong operand types for less than.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type LsEq::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else if (is_str(left_ty) && is_str(rght_ty)) {
        return Type {StrTy {}};
    } else if (is_bool(left_ty) && is_bool(rght_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong operand types for less than.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type Equl::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        return Type {IntTy {}};
    } else if (is_str(left_ty) && is_str(rght_ty)) {
        return Type {StrTy {}};
    } else if (is_bool(left_ty) && is_bool(rght_ty)) {
        return Type {BoolTy {}};
    } else if (is_None(left_ty) && is_None(rght_ty)) {
        return Type {NoneTy {}};
    } else {
        std::string msg = "Wrong operand types for logical equals.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type And::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_bool(left_ty) && is_bool(rght_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong operand types for logical and.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type Or::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_bool(left_ty) && is_bool(rght_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong operand types for logical or.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type Not::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_bool(expn_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong operand types for logical not.";
        throw DwislpyError { where(), msg };
    }
    return Type {BoolTy {}}; 
}

Type Ltrl::chck([[maybe_unused]] Defs& defs, [[maybe_unused]] SymT& symt) {
    if (std::holds_alternative<int>(valu)) {
        return Type {IntTy {}};
    } else if (std::holds_alternative<std::string>(valu)) {
        return Type {StrTy {}};
    } if (std::holds_alternative<bool>(valu)) {
        return Type {BoolTy {}};
    } else {
        return Type {NoneTy {}};
    } 
}

Type Lkup::chck([[maybe_unused]] Defs& defs, SymT& symt) {
    if (symt.has_info(name)) {
        return symt.get_info(name)->type;
    } else {
        throw DwislpyError {where(), "Unknown identifier."};
    } 
}

Type Inpt::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_str(expn_ty)) {
        return Type {StrTy {}};
    } else {
        std::string msg = "Wrong type for input.";
        throw DwislpyError { where(), msg };
    }
    return Type {StrTy {}}; 
}

Type IntC::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_int(expn_ty)) {
        return Type {IntTy {}};
    } else if (is_str(expn_ty)) {
        return Type {StrTy {}};
    } else if (is_bool(expn_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong type for integer conversion.";
        throw DwislpyError { where(), msg };
    }
    return Type {IntTy {}}; 
}

Type StrC::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_int(expn_ty)) {
        return Type {IntTy {}};
    } else if (is_str(expn_ty)) {
        return Type {StrTy {}};
    } else if (is_bool(expn_ty)) {
        return Type {BoolTy {}};
    } else {
        std::string msg = "Wrong type for integer conversion.";
        throw DwislpyError { where(), msg };
    }
    return Type {StrTy {}}; 
}


Type FCll::chck(Defs& defs, SymT& symt) {
    //
    // This should look up a function's definition. It should check:
    // * that the correct number of arguments are passed.
    // * that each of the argument expressions type check
    // * that the type of each argument matches the type signature
    // It should report the return type of the function.
    //
    for (std::pair<Name,Defn_ptr> dfpr : defs) {
        if (dfpr.first == name) {
            Defn_ptr dptr = dfpr.second;
            if (params.size() != dptr->symt.get_frmls_size()) {
                std::string msg = "Incorrect number of arguments.";
                throw DwislpyError { where(), msg };
            };
            for (unsigned int i=0; i<symt.get_frmls_size(); i++) {
                if(symt.get_frml(i) != dptr->symt.get_frml(i)){
                    std::string msg = "Argument ";
                    msg += std::to_string(i) + " is an incorrect type.";
                    throw DwislpyError {where(), msg};
                }
            }
            return dptr->rety;
        }
    }
    return Type {NoneTy {}};
}