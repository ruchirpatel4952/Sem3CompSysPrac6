#include <sstream>
#include <string>

#include "VMTranslator.h"

using namespace std;

/* ─────────────────────────────────────────────────────────── */
/* Utilities                                                   */
/* ─────────────────────────────────────────────────────────── */

static int LABEL_COUNTER = 0;   // used to emit unique TRUE_x / END_x labels

/* ─────────────────────────────────────────────────────────── */
/*     P U S H   /   P O P                                    */
/* ─────────────────────────────────────────────────────────── */

/** Generate Hack assembly for  push <segment> <index> */
string VMTranslator::vm_push(string segment, int index) {
    ostringstream out;

    if (segment == "constant") {                      /* ---- constant ---- */
        out << '@' << index   << '\n'
            << "D=A\n";
    } else if (segment == "local" || segment == "argument" ||
               segment == "this"  || segment == "that") {      /* LCL/ARG/THIS/THAT */
        string base = (segment == "local")    ? "LCL"  :
                       (segment == "argument") ? "ARG"  :
                       (segment == "this")     ? "THIS" : "THAT";
        out << '@' << base   << '\n' << "D=M\n"
            << '@' << index  << '\n' << "A=D+A\n" << "D=M\n";
    } else if (segment == "temp") {                   /* ---- temp ---- */
        out << '@' << (5 + index) << '\n' << "D=M\n";
    } else if (segment == "pointer") {                /* ---- pointer ---- */
        out << '@' << (3 + index) << '\n' << "D=M\n";
    } else if (segment == "static") {                 /* ---- static ---- */
        out << "@Static." << index << '\n' << "D=M\n";   // «Static» prefix is fine
    }

    /* push D */
    out << "@SP\nA=M\nM=D\n@SP\nM=M+1";
    return out.str();
}

/** Generate Hack assembly for  pop <segment> <index> */
string VMTranslator::vm_pop(string segment, int index) {
    ostringstream out;

    if (segment == "local" || segment == "argument" ||
        segment == "this"  || segment == "that") {            /* LCL/ARG/THIS/THAT */
        string base = (segment == "local")    ? "LCL"  :
                       (segment == "argument") ? "ARG"  :
                       (segment == "this")     ? "THIS" : "THAT";
        out << '@' << base   << '\n' << "D=M\n"
            << '@' << index  << '\n' << "D=D+A\n"
            << "@R13\nM=D\n"                        // R13 ← target address
            << "@SP\nAM=M-1\nD=M\n"                 // D ← *--SP
            << "@R13\nA=M\nM=D\n";
    } else if (segment == "temp") {                           /* temp */
        out << "@SP\nAM=M-1\nD=M\n"
            << '@' << (5 + index) << '\n' << "M=D\n";
    } else if (segment == "pointer") {                        /* pointer */
        out << "@SP\nAM=M-1\nD=M\n"
            << '@' << (3 + index) << '\n' << "M=D\n";
    } else if (segment == "static") {                         /* static */
        out << "@SP\nAM=M-1\nD=M\n"
            << "@Static." << index << '\n' << "M=D\n";
    }
    return out.str();
}

/* ─────────────────────────────────────────────────────────── */
/*     A R I T H M E T I C   /   L O G I C                    */
/* ─────────────────────────────────────────────────────────── */

#define BINARY_OP(op)  "@SP\nAM=M-1\nD=M\nA=A-1\nM=M" op "D"
#define UNARY_OP(op)   "@SP\nA=M-1\nM=" op "M"

string VMTranslator::vm_add() { return BINARY_OP("+"); }
string VMTranslator::vm_sub() { return BINARY_OP("-"); }
string VMTranslator::vm_and() { return BINARY_OP("&"); }
string VMTranslator::vm_or () { return BINARY_OP("|"); }
string VMTranslator::vm_neg() { return UNARY_OP("-"); }
string VMTranslator::vm_not() { return UNARY_OP("!"); }

/* eq, gt, lt need unique labels */
static string two_arg_cmp(const string& jump) {
    ostringstream out;
    int id = LABEL_COUNTER++;
    out << "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n"
        << "@TRUE" << id << '\n' << "D;" << jump << '\n'
        << "@SP\nA=M-1\nM=0\n"               // false
        << "@END"  << id << '\n' << "0;JMP\n"
        << "(TRUE" << id << ")\n"
        << "@SP\nA=M-1\nM=-1\n"              // true = -1
        << "(END"  << id << ")";
    return out.str();
}
string VMTranslator::vm_eq() { return two_arg_cmp("JEQ"); }
string VMTranslator::vm_gt() { return two_arg_cmp("JGT"); }
string VMTranslator::vm_lt() { return two_arg_cmp("JLT"); }

/* ─────────────────────────────────────────────────────────── */
/*     F L O W   C O N T R O L                                */
/* ─────────────────────────────────────────────────────────── */

string VMTranslator::vm_label(string label)       { return "(" + label + ")"; }
string VMTranslator::vm_goto (string label)       { return "@" + label + "\n0;JMP"; }
string VMTranslator::vm_if   (string label) {
    ostringstream out;
    out << "@SP\nAM=M-1\nD=M\n@" << label << '\n' << "D;JNE";
    return out.str();
}

/* ─────────────────────────────────────────────────────────── */
/*   Simple function support (enough for basic tests)          */
/* ─────────────────────────────────────────────────────────── */

string VMTranslator::vm_function(string name, int n_vars) {
    ostringstream out;
    out << '(' << name << ")\n";
    for (int i = 0; i < n_vars; ++i)
        out << "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    return out.str();
}

string VMTranslator::vm_call(string /*name*/, int /*n_args*/) {
    return "// call-command not implemented in this minimal translator";
}

string VMTranslator::vm_return() {
    return "// return-command not implemented in this minimal translator";
}

/* ─────────────────────────────────────────────────────────── */
/* Constructors / destructors (nothing special needed)        */
/* ─────────────────────────────────────────────────────────── */

VMTranslator::VMTranslator()  = default;
VMTranslator::~VMTranslator() = default;
