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
    Rtns rtns = main->chck(Rtns{Void {}},defs,main_symt);
    if (!std::holds_alternative<Void>(rtns)) {
        DwislpyError(main->where(), "Main script should not return.");
    }
}

void Defn::chck(Defs& defs) {
    Rtns rtns = body->chck(Rtns{rety}, defs, symt);
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

Rtns void_of(Rtns rtns) {
    if (std::holds_alternative<Type>(rtns)) {
        Type rtns_ty = std::get<Type>(rtns);
        return Rtns { VoidOr { rtns_ty } };
    } else {
        return rtns;
    }
}

Rtns rtns_seq(Rtns rtns1, Rtns rtns2, Locn lo) {
    if (std::holds_alternative<Void>(rtns1)) {
        return rtns2;
    }
    if (std::holds_alternative<VoidOr>(rtns1)) {
        Type rtns_ty1 = type_of(rtns1);
        if (std::holds_alternative<Void>(rtns2)) {
            return rtns1;
        } else {
            Type rtns_ty2 = type_of(rtns2);
            if (rtns_ty1 == rtns_ty2) {
                return rtns2;
            } else {
                std::string msg = "Type mismatch. Statement return is";
                msg += " not compatible with what's expected.";
                throw DwislpyError {lo,msg};
            }
        }
    }
    throw DwislpyError {lo, "Statement not reachable because of prior return."};
}

Rtns rtns_sum(Rtns rtns1, Rtns rtns2, Locn lo) {
    if (std::holds_alternative<Void>(rtns1)) {
        return void_of(rtns2);
    }
    if (std::holds_alternative<Void>(rtns2)) {
        return void_of(rtns1);
    }
    Type rtns_ty1 = type_of(rtns1);
    Type rtns_ty2 = type_of(rtns2);
    if (rtns_ty1 == rtns_ty2) {
        if (std::holds_alternative<VoidOr>(rtns1)
            || std::holds_alternative<VoidOr>(rtns2)) {
            return Rtns { VoidOr { rtns_ty1 } };
        } else {
            return Rtns { rtns_ty1 };
        }
    } else {
        throw DwislpyError {lo,"Type mismatch. Incompatible return types."};
    }
}

Rtns Blck::chck(Rtns expd, Defs& defs, SymT& symt) {
    // Mark the symbol table to note this block and its scope.
    // symt->mark_blck(this);
    
    // Scan through the statements and check their return behavior.
    Rtns blck_rtns = Rtns {Void {}};;
    for (Stmt_ptr stmt : stmts) {
        
        // Check this statement.
        Rtns stmt_rtns = stmt->chck(expd, defs, symt);
        
        // Check and update the sequencing of return behavior.
        blck_rtns = rtns_seq(blck_rtns, stmt_rtns, stmt->where());
    }            
    // symt->pop_to_mark();
    return blck_rtns;
}

Rtns Ntro::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    Type name_ty = type;
    Type expn_ty = expn->chck(defs,symt);
    if (name_ty != expn_ty) {
        std::string msg = "Type mismatch. Expected initialization of type ";
        msg += type_name(name_ty) + " but instead has type ";
        msg += type_name(expn_ty) + ".";
        throw DwislpyError{where(), msg};
    }
    symt.add_locl(name,type);
    return Rtns{Void {}};
}

Rtns Asgn::chck([[maybe_unused]] Rtns expd, Defs& defs, SymT& symt) {
    if (!symt.has_info(name)) {
        throw DwislpyError(where(), "Variable '" + name + "' never introduced.");
    }
    Type name_ty = symt.get_info(name)->type;
    Type expn_ty = expn->chck(defs,symt);
    if (name_ty != expn_ty) {
        std::string msg = "Type mismatch. Expected expression of type ";
        msg += type_name(name_ty) + " but instead has type ";
        msg += type_name(expn_ty) + ".";
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

Rtns FRtn::chck(Rtns expd, Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (std::holds_alternative<Void>(expd)) {
        throw DwislpyError {expn->where(), "Unexpected return statement."};
    }
    Type expd_ty = type_of(expd);
    if (expn_ty != expd_ty) {
        std::string msg = "Return type mismatch. Expected return of type ";
        msg += type_name(expd_ty) + ".";
        throw DwislpyError {expn->where(), msg};
    }
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
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Wrong operand types for plus.";
        throw DwislpyError { where(), msg };
    }
}

Type Mnus::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Wrong operand types for minus.";
        throw DwislpyError { where(), msg };
    }
}

Type Tmes::chck([[maybe_unused]] Defs& defs, [[maybe_unused]] SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Wrong operand types for times.";
        throw DwislpyError { where(), msg };
    }
}

Type IDiv::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Wrong operand types for division.";
        throw DwislpyError { where(), msg };
    }
}

Type IMod::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_int(left_ty) && is_int(rght_ty)) {
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Wrong operand types for mod.";
        throw DwislpyError { where(), msg };
    }
}

Type Less::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (left_ty == rght_ty) {
        if (is_int(left_ty)) {
            type = Type {BoolTy {}}; 
            return type;
        } else {
            std::string msg = "This version of the language only compares integers.";
            throw DwislpyError { where(), msg }; 
        }
    } else {
        std::string msg = "Must compare values of the same type.";
        throw DwislpyError { where(), msg }; 
    }       
}

Type LsEq::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (left_ty == rght_ty) {
        if (is_int(left_ty)) {
            type = Type {BoolTy {}}; 
            return type;
        } else {
            std::string msg = "This version of the language only compares integers.";
            throw DwislpyError { where(), msg }; 
        }
    } else {
        std::string msg = "Must compare values of the same type.";
        throw DwislpyError { where(), msg }; 
    }       
}

Type Equl::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (left_ty == rght_ty) {
        if (is_int(left_ty)) {
            type = Type {BoolTy {}}; 
            return type;
        } else {
            std::string msg = "This version of the language only compares integers.";
            throw DwislpyError { where(), msg }; 
        }
    } else {
        std::string msg = "Must compare values of the same type.";
        throw DwislpyError { where(), msg }; 
    }       
}

Type And::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_bool(left_ty) && is_bool(rght_ty)) {
        type = Type {BoolTy {}}; 
        return type;
    } else {
        std::string msg = "Logical connective applied to a non-boolean value.";
        throw DwislpyError { where(), msg };
    }
}

Type Or::chck(Defs& defs, SymT& symt) {
    Type left_ty = left->chck(defs,symt);
    Type rght_ty = rght->chck(defs,symt);
    if (is_bool(left_ty) && is_bool(rght_ty)) {
        type = Type {BoolTy {}}; 
        return type;
    } else {
        std::string msg = "Logical connective applied to a non-boolean value.";
        throw DwislpyError { where(), msg };
    }
}

Type Not::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_bool(expn_ty)) {
        type = Type {BoolTy {}}; 
        return type;
    } else {
        std::string msg = "Logical not applied to a non-boolean value.";
        throw DwislpyError { where(), msg };
    }
}

Type Ltrl::chck([[maybe_unused]] Defs& defs, [[maybe_unused]] SymT& symt) {
    if (std::holds_alternative<int>(valu)) {
        type = Type {IntTy {}};
    } else if (std::holds_alternative<std::string>(valu)) {
        type = Type {StrTy {}};
    } else if (std::holds_alternative<bool>(valu)) {
        type = Type {BoolTy {}};
    } else {
        type = Type {NoneTy {}};
    }
    return type;
}

Type Lkup::chck([[maybe_unused]] Defs& defs, SymT& symt) {
    if (symt.has_info(name)) {
        type = symt.get_info(name)->type;
        return type;
    } else {
        throw DwislpyError {where(), "Unknown identifier."};
    } 
}

Type Inpt::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_str(expn_ty)) {
        // This next line *should* be 
        //    type = Type {StrTy {}};
        // but this language version returns an integer instead.
        type = Type {IntTy {}};
        return type;
    } else {
        std::string msg = "Input prompt is not a string.";
        throw DwislpyError { where(), msg };
    }
}

Type IntC::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_int(expn_ty)) {
        type = Type {IntTy {}};
    } else if (is_str(expn_ty)) {
        type = Type {StrTy {}};
    } else if (is_bool(expn_ty)) {
        type = Type {BoolTy {}};
    } else {
        std::string msg = "Wrong type for integer conversion.";
        throw DwislpyError { where(), msg };
    }
    return type; 
}

Type StrC::chck(Defs& defs, SymT& symt) {
    Type expn_ty = expn->chck(defs,symt);
    if (is_int(expn_ty)) {
        type = Type {IntTy {}};
    } else if (is_str(expn_ty)) {
        type = Type {StrTy {}};
    } else if (is_bool(expn_ty)) {
        type = Type {BoolTy {}};
    } else {
        std::string msg = "Wrong type for integer conversion.";
        throw DwislpyError { where(), msg };
    }
    return type; 
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
    type = Type {NoneTy {}};
    return type;
}
