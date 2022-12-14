#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <functional>
#include <iostream>
#include <exception>
#include <algorithm>
#include <sstream>

#include "slpy-ast.hh"
#include "slpy-util.hh"

//
// slpy-ast.cc
//
// Below are the implementations of methods of AST nodes. They are organized
// into groups. The first group represents the SLPY interpreter using
//
//    Prgm::run, Blck::exec, Stmt::exec, Expn::eval
//
// The second group AST::output performs pretty printing of SLP code.
//

// * * * * *
// The SLPY interpreter
//

//
// Prgm::run, Blck::exec, Stmt:: exec
//
//  - execute SLPY statements, changing the runtime context mapping
//    variables to their current values.
//

void Prgm::run(void) const {
    Ctxt main_ctxt { };
    main->exec(main_ctxt);
}

void Blck::exec(Ctxt& ctxt) const {
    for (Stmt_ptr s : stmts) {
        s->exec(ctxt);
    }
}

void Asgn::exec(Ctxt& ctxt) const {
    ctxt[name] = expn->eval(ctxt);
}

void Updt::exec(Ctxt& ctxt) const {
    ctxt[name] = ctxt[name] + expn->eval(ctxt);
}

void Pass::exec(Ctxt& ctxt) const {
    // does nothing!
}
  
void Prnt::exec(Ctxt& ctxt) const {
    for (Expn_ptr e : expns) {
        std::cout << e->eval(ctxt) << " ";
    }
    std::cout << std::endl;
}

//
// Expn::eval
//
//  - evaluate SLPY expressions within a runtime context to determine their
//    (integer) value.
//

int Plus::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv + rv);
}

int Mnus::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv - rv);
}

int Tmes::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    return (lv * rv);
}

int IDiv::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    if (rv == 0) {
        throw SlpyError { where(), "Run-time error: division by 0."};
    } else {
        return (lv / rv);
    } 
}

int IMod::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    if (rv == 0) {
        throw SlpyError { where(), "Run-time error: division by 0."};
    } else {
        return (lv % rv);
    } 
}

int Powr::eval(const Ctxt& ctxt) const {
    int lv = left->eval(ctxt);
    int rv = rght->eval(ctxt);
    int total = lv;
    if (rv == 0){
        return 1;
    } else if (rv == 1){
        return lv;
    } else {
        while (rv != 1){
            total = total * lv;
            rv = rv - 1;
        }
    }
    return (total);
}

int Nmbr::eval(const Ctxt& ctxt) const {
    return valu;
}

int Lkup::eval(const Ctxt& ctxt) const {
    if (ctxt.count(name) > 0) {
        return ctxt.at(name);
    } else {
        throw SlpyError { where(), "Run-time error: variable '" + name +"' not defined."};
    }
}

int Inpt::eval(const Ctxt& ctxt) const {
    int vl;
    std::cout << prpt;
    std::cin >> vl;
    return vl;
}

int IntC::eval(const Ctxt& ctxt) const {
    //
    // The integer conversion operation does nothing in this
    // version of SLPY.
    //
    return expn->eval(ctxt);
}


// * * * * *
//
// AST::output
//
// - Pretty printer for SLPY code represented in an AST.
//
// The code below is an implementation of a pretty printer. For each case
// of an AST node (each subclass) the `output` method provides the means for
// printing the code of the SLPY construct it represents.
//
//

void Prgm::output(std::ostream& os) const {
    main->output(os);
}

void Blck::output(std::ostream& os, std::string indent) const {
    for (Stmt_ptr s : stmts) {
        s->output(os,indent);
    }
}

void Blck::output(std::ostream& os) const {
    for (Stmt_ptr s : stmts) {
        s->output(os);
    }
}

void Stmt::output(std::ostream& os) const {
    output(os,"");
}

void Asgn::output(std::ostream& os, std::string indent) const {
    os << indent;
    os << name << " = ";
    expn->output(os);
    os << std::endl;
}

void Updt::output(std::ostream& os, std::string indent) const {
    os << indent;
    os << name << " += ";
    expn->output(os);
    os << std::endl;
}

void Pass::output(std::ostream& os, std::string indent) const {
    os << indent << "pass" << std::endl;
}

void Prnt::output(std::ostream& os, std::string indent) const {
    os << indent;
    os << "print";
    os << "(";
    for (Expn_ptr e : expns) {
        e->output(os);
        os << ", ";
    }
    os << '\b';  //Cursor moves 1 position backwards
    os << '\b';
    os << ")";
    os << std::endl;
}

void Plus::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " + ";
    rght->output(os);
    os << ")";
}

void Mnus::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " - ";
    rght->output(os);
    os << ")";
}

void Tmes::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " * ";
    rght->output(os);
    os << ")";
}

void IDiv::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " // ";
    rght->output(os);
    os << ")";
}

void IMod::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " % ";
    rght->output(os);
    os << ")";
}

void Powr::output(std::ostream& os) const {
    os << "(";
    left->output(os);
    os << " ** ";
    rght->output(os);
    os << ")";
}

void Nmbr::output(std::ostream& os) const {
    os << std::to_string(valu);
}

void Lkup::output(std::ostream& os) const {
    os << name;
}

void Inpt::output(std::ostream& os) const {
    os << "input(\"" << re_escape(prpt) << "\")";
}

void IntC::output(std::ostream& os) const {
    os << "int(";
    expn->output(os);
    os << ")";
}

// * * * * *
//
// AST::dump
//
// - Parse tree representation for SLPY code for each
// AST subclass
//
//

void Prgm::dump(std::ostream& os, std::string indent) const {
    os << indent << "Prgm" << std::endl;
    main->dump(os, indent + "    ");
}

void Blck::dump(std::ostream& os, std::string indent) const {
    os << indent << "Blck" << std::endl;
    for (Stmt_ptr s : stmts) {
        s->dump(os, indent + "    ");
    }
}

void Stmt::dump(std::ostream& os, std::string indent) const {
    os << "Stmt" << std::endl;
    dump(os, "");
}

void Asgn::dump(std::ostream& os, std::string indent) const {
    os << indent << "Asgn" << std::endl;
    os << indent + "    " << name << std::endl;
    expn->dump(os, indent + "    ");
}

void Updt::dump(std::ostream& os, std::string indent) const {
    os << indent << "Updt" << std::endl;
    os << indent + "    " << name << std::endl;
}

void Pass::dump(std::ostream& os, std::string indent) const {
    os << indent << "Pass" << std::endl;
}

void Prnt::dump(std::ostream& os, std::string indent) const {
    os << indent << "Prnt" << std::endl;
    for (Expn_ptr e : expns) {
        e->dump(os, indent + "    ");
    }
}

void Plus::dump(std::ostream& os, std::string indent) const {
    os << indent << "Plus" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void Mnus::dump(std::ostream& os, std::string indent) const {
    os << indent << "Mnus" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void Tmes::dump(std::ostream& os, std::string indent) const {
    os << indent << "Tmes" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void IDiv::dump(std::ostream& os, std::string indent) const {
    os << indent << "IDiv" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void IMod::dump(std::ostream& os, std::string indent) const {
    os << indent << "IMod" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void Powr::dump(std::ostream& os, std::string indent) const {
    os << indent << "Powr" << std::endl;
    left->dump(os, indent + "    ");
    rght->dump(os, indent + "    ");
}

void Nmbr::dump(std::ostream& os, std::string indent) const {
    os << indent << "Nmbr" << std::endl;
    os << indent + "    " << std::to_string(valu) << std::endl;
}

void Lkup::dump(std::ostream& os, std::string indent) const {
    os << indent <<"Lkup" << std::endl;
    os << indent + "    " << name << std::endl;
}

void Inpt::dump(std::ostream& os, std::string indent) const {
    os << indent << "Inpt" << std::endl;
    os << indent + "    " << "\"" << prpt << "\"" << std::endl;
}

void IntC::dump(std::ostream& os, std::string indent) const {
    os << indent << "IntC" << std::endl;
    expn->dump(os, indent + "    ");
}