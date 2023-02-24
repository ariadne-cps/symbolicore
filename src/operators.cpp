/***************************************************************************
 *            numeric/operators.cpp
 *
 *  Copyright  2008-20  Pieter Collins
 *
 ****************************************************************************/

/*
 * This file is part of SymboliCore, under the MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

namespace SymboliCore {
double add(double,double);
double sub(double,double);
double mul(double,double);
double div(double,double);
}

#include "stdlib.hpp"
#include "string.hpp"
#include "operators.hpp"
#include "macros.hpp"

namespace SymboliCore {

template<class OP, class... AS> class Expression;

OutputStream& operator<<(OutputStream& os, const OperatorCode& op) {
    return os << name(op);
}

OutputStream& operator<<(OutputStream& os, const OperatorKind& knd) {
    switch(knd) {
        case OperatorKind::VARIABLE: return os << "VARIABLE";
        case OperatorKind::COORDINATE: return os << "COORDINATE";
        case OperatorKind::NULLARY: return os << "NULLARY";
        case OperatorKind::UNARY: return os << "UNARY";
        case OperatorKind::BINARY: return os << "BINARY";
        case OperatorKind::TERNARY: return os << "TERNARY";
        case OperatorKind::SCALAR: return os << "SCALAR";
        case OperatorKind::GRADED: return os << "GRADED";
        case OperatorKind::COMPARISON: return os << "COMPARISON";
        default: return os << "UNKNOWN";
    }
}

const char* name(const OperatorCode& op) {
    switch(op) {
        case OperatorCode::CNST: return "cnst"; break;
        case OperatorCode::VAR:  return "var"; break;
        case OperatorCode::IND:  return "ind"; break;
        case OperatorCode::POS:  return "pos"; break;
        case OperatorCode::NEG:  return "neg"; break;
        case OperatorCode::REC:  return "rec"; break;
        case OperatorCode::ADD:  return "add"; break;
        case OperatorCode::SUB:  return "sub"; break;
        case OperatorCode::MUL:  return "mul"; break;
        case OperatorCode::DIV:  return "div"; break;
        case OperatorCode::SADD:  return "sadd"; break;
        case OperatorCode::SSUB:  return "ssub"; break;
        case OperatorCode::SMUL:  return "smul"; break;
        case OperatorCode::SDIV:  return "sdiv"; break;
        case OperatorCode::POW:  return "pow"; break;
        case OperatorCode::ROOT:  return "root"; break;
        case OperatorCode::NOT:  return "not"; break;
        case OperatorCode::AND:  return "and"; break;
        case OperatorCode::OR:   return "or"; break;
        case OperatorCode::XOR:  return "xor"; break;
        case OperatorCode::IMPL: return "impl"; break;
        case OperatorCode::ABS:  return "abs"; break;
        case OperatorCode::MAX:  return "max"; break;
        case OperatorCode::MIN:  return "min"; break;
        case OperatorCode::SQR:  return "sqr"; break;
        case OperatorCode::SQRT: return "sqrt"; break;
        case OperatorCode::EXP:  return "exp"; break;
        case OperatorCode::LOG:  return "log"; break;
        case OperatorCode::SIN:  return "sin"; break;
        case OperatorCode::COS:  return "cos"; break;
        case OperatorCode::TAN:  return "tan"; break;
        case OperatorCode::ASIN:  return "asin"; break;
        case OperatorCode::ACOS:  return "acos"; break;
        case OperatorCode::ATAN:  return "atan"; break;
        case OperatorCode::ITOR:  return "itor"; break;
        case OperatorCode::PULL: return "pull"; break;
        case OperatorCode::PUSH: return "push"; break;
        case OperatorCode::SGN:  return "sgn"; break;
        case OperatorCode::EQ:   return "eq"; break;
        case OperatorCode::NEQ:  return "neq"; break;
        case OperatorCode::GEQ:  return "geq"; break;
        case OperatorCode::LEQ:  return "leq"; break;
        case OperatorCode::GT:   return "gt"; break;
        case OperatorCode::LT:   return "lt"; break;
        case OperatorCode::SUBS:   return "subs"; break;
        case OperatorCode::HLF:  return "hlf"; break;
        default: return "UNKNOWN";
    }
}

const char* symbol(const OperatorCode& op) {
    switch(op) {
        case OperatorCode::POS:  return "+"; break;
        case OperatorCode::NEG:  return "-"; break;
        case OperatorCode::ADD:  return "+"; break;
        case OperatorCode::SUB:  return "-"; break;
        case OperatorCode::MUL:  return "*"; break;
        case OperatorCode::DIV:  return "/"; break;
        case OperatorCode::POW:  return "^"; break;
        case OperatorCode::SADD: return "+"; break;
        case OperatorCode::SMUL: return "*"; break;
        case OperatorCode::NOT:  return "!"; break;
        case OperatorCode::AND:  return "&"; break;
        case OperatorCode::OR:   return "|"; break;
        case OperatorCode::EQ:  return "=="; break;
        case OperatorCode::NEQ: return "!="; break;
        case OperatorCode::LEQ: return "<="; break;
        case OperatorCode::GEQ: return ">="; break;
        case OperatorCode::LT:  return "<"; break;
        case OperatorCode::GT:  return ">"; break;
        default: return "???";
    }
}

OperatorKind kind(OperatorCode op) {
    switch(op) {
        case OperatorCode::CNST:
            return OperatorKind::NULLARY;
        case OperatorCode::IND:
            return OperatorKind::COORDINATE;
        case OperatorCode::VAR:
            return OperatorKind::VARIABLE;
        case OperatorCode::ADD: case OperatorCode::SUB: case OperatorCode::MUL: case OperatorCode::DIV:
        case OperatorCode::MAX: case OperatorCode::MIN:
            return OperatorKind::BINARY;
        case OperatorCode::SADD: case OperatorCode::SSUB: case OperatorCode::SMUL: case OperatorCode::SDIV:
            return OperatorKind::SCALAR;
        case OperatorCode::POS: case OperatorCode::NEG: case OperatorCode::REC: case OperatorCode::SQR:
        case OperatorCode::SQRT: case OperatorCode::EXP: case OperatorCode::LOG:
        case OperatorCode::SIN: case OperatorCode::COS: case OperatorCode::TAN: case OperatorCode::ATAN:
        case OperatorCode::ABS:
            return OperatorKind::UNARY;
        case OperatorCode::POW: case OperatorCode::ROOT:
            return OperatorKind::GRADED;
        case OperatorCode::AND: case OperatorCode::OR:
            return OperatorKind::BINARY;
        case OperatorCode::NOT:
            return OperatorKind::UNARY;
        case OperatorCode::EQ: case OperatorCode::NEQ: case OperatorCode::LEQ: case OperatorCode::GEQ: case OperatorCode::LT: case OperatorCode::GT:
            return OperatorKind::COMPARISON;
        default:
            SYMBOLICORE_FAIL_MSG("Cannot deduce kind of operator "<<op);
    }
}

OperatorKind get_kind(OperatorCode op) {
    return kind(op);
}


} // namespace SymboliCore

