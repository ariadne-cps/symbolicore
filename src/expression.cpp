/***************************************************************************
 *            expression.cpp
 *
 *  Copyright  2023  Pieter Collins
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

#include "helper/array.hpp"
#include "helper/string.hpp"
#include "helper/container.hpp"
#include "sign.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "vector.hpp"
#include "assignment.hpp"
#include "constant.hpp"
#include "variable.hpp"
#include "valuation.hpp"
#include "operators.hpp"
#include "templates.hpp"
#include "expression.hpp"
#include "templates.tpl.hpp"
#include "expression.tpl.hpp"

namespace SymboliCore {

using Helper::range;
using std::shared_ptr;

template class Expression<Boolean>;
template class Expression<Kleenean>;
template class Expression<String>;
template class Expression<Integer>;
template class Expression<Real>;

template bool before<Real>(Expression<Real> const& e1, Expression<Real> const& e2);
template size_t count_nodes<Real>(const Expression<Real>& e);
template size_t count_distinct_nodes<Real>(const Expression<Real>& e);
template size_t count_distinct_node_pointers<Real>(const Expression<Real>& e);

Expression<Boolean> operator&&(Expression<Boolean> const& e1, Expression<Boolean> const& e2) {
    return make_expression<Boolean>(AndOp(),e1,e2); }
Expression<Boolean> operator||(Expression<Boolean> const& e1, Expression<Boolean> const& e2) {
    return make_expression<Boolean>(OrOp(),e1,e2); }
Expression<Boolean> operator!(Expression<Boolean> const& e) {
    return make_expression<Boolean>(NotOp(),e); }


Expression<Kleenean> operator&&(Expression<Kleenean> const& e1, Expression<Kleenean> const& e2) {
    return make_expression<Kleenean>(AndOp(),e1,e2); }
Expression<Kleenean> operator||(Expression<Kleenean> const& e1, Expression<Kleenean> const& e2) {
    return make_expression<Kleenean>(OrOp(),e1,e2); }
Expression<Kleenean> operator!(Expression<Kleenean> const& e) {
    return make_expression<Kleenean>(NotOp(),e); }

Expression<Boolean> operator==(Variable<String> v1, const String& s2) {
    return make_expression<Boolean>(Equal(),Expression<String>(v1),Expression<String>::constant(s2)); }
Expression<Boolean> operator!=(Variable<String> v1, const String& s2) {
    return make_expression<Boolean>(Unequal(),Expression<String>(v1),Expression<String>::constant(s2)); }

Expression<Boolean> operator==(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Equal(),e1,e2); }
Expression<Boolean> operator!=(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Unequal(),e1,e2); }
Expression<Boolean> operator>=(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Geq(),e1,e2); }
Expression<Boolean> operator<=(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Leq(),e1,e2); }
Expression<Boolean> operator>(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Gtr(),e1,e2); }
Expression<Boolean> operator<(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Boolean>(Less(),e1,e2); }

Expression<Integer> operator+(Expression<Integer> const& e) {
    return make_expression<Integer>(Pos(),e); }
Expression<Integer> operator-(Expression<Integer> const& e) {
    return make_expression<Integer>(Neg(),e); }
Expression<Integer> operator+(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Integer>(Add(),e1,e2); }
Expression<Integer> operator-(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Integer>(Sub(),e1,e2); }
Expression<Integer> operator*(Expression<Integer> const& e1, Expression<Integer> const& e2) {
    return make_expression<Integer>(Mul(),e1,e2); }

Expression<Integer>& operator+=(Expression<Integer>& e1, Expression<Integer> const& e2) {
    return e1=e1+e2; }
Expression<Integer>& operator-=(Expression<Integer>& e1, Expression<Integer> const& e2) {
    return e1=e1-e2; }
Expression<Integer>& operator*=(Expression<Integer>& e1, Expression<Integer> const& e2) {
    return e1=e1*e2; }

Expression<Kleenean> sgn(Expression<Real> const& e) {
    return make_expression<Kleenean>(Sgn(),e); }

Expression<Kleenean> operator==(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Equal(),e1,e2); }
Expression<Kleenean> operator!=(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Unequal(),e1,e2); }
Expression<Kleenean> operator>=(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Geq(),e1,e2); }
Expression<Kleenean> operator<=(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Leq(),e1,e2); }
Expression<Kleenean> operator>(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Gtr(),e1,e2); }
Expression<Kleenean> operator<(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Kleenean>(Less(),e1,e2); }


Expression<Real> operator+(Expression<Real> const& e) {
    return make_expression<Real>(Pos(),e); }
Expression<Real> operator-(Expression<Real> const& e) {
    return make_expression<Real>(Neg(),e); }
Expression<Real> operator+(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Add(),e1,e2); }
Expression<Real> operator-(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Sub(),e1,e2); }
Expression<Real> operator*(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Mul(),e1,e2); }
Expression<Real> operator/(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Div(),e1,e2); }

Expression<Real>& operator+=(Expression<Real>& e1, Expression<Real> const& e2) {
    return e1=e1+e2; }
Expression<Real>& operator-=(Expression<Real>& e1, Expression<Real> const& e2) {
    return e1=e1-e2; }
Expression<Real>& operator*=(Expression<Real>& e1, Expression<Real> const& e2) {
    return e1=e1*e2; }
Expression<Real>& operator/=(Expression<Real>& e1, Expression<Real> const& e2) {
    return e1=e1/e2; }

Expression<Real> add(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Add(),e1,e2); }
Expression<Real> sub(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Sub(),e1,e2); }
Expression<Real> mul(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Mul(),e1,e2); }
Expression<Real> div(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Div(),e1,e2); }
Expression<Real> pow(Expression<Real> const& e, int n) {
    return make_expression<Real>(Pow(),e,n); }

Expression<Real> nul(Expression<Real> const&) {
    return make_expression<Real>(Real(0)); }
Expression<Real> pos(Expression<Real> const& e) {
    return make_expression<Real>(Pos(),e); }
Expression<Real> neg(Expression<Real> const& e) {
    return make_expression<Real>(Neg(),e); }
Expression<Real> rec(Expression<Real> const& e) {
    return make_expression<Real>(Rec(),e); }
Expression<Real> sqr(Expression<Real> const& e) {
    return make_expression<Real>(Sqr(),e); }
Expression<Real> hlf(Expression<Real> const& e) {
    return make_expression<Real>(Hlf(),e); }
Expression<Real> sqrt(Expression<Real> const& e) {
    return make_expression<Real>(Sqrt(),e); }
Expression<Real> exp(Expression<Real> const& e) {
    return make_expression<Real>(Exp(),e); }
Expression<Real> log(Expression<Real> const& e) {
    return make_expression<Real>(Log(),e); }
Expression<Real> sin(Expression<Real> const& e) {
    return make_expression<Real>(Sin(),e); }
Expression<Real> cos(Expression<Real> const& e) {
    return make_expression<Real>(Cos(),e); }
Expression<Real> tan(Expression<Real> const& e) {
    return make_expression<Real>(Tan(),e); }
Expression<Real> asin(Expression<Real> const& e) {
    return make_expression<Real>(Asin(),e); }
Expression<Real> acos(Expression<Real> const& e) {
    return make_expression<Real>(Acos(),e); }
Expression<Real> atan(Expression<Real> const& e) {
    return make_expression<Real>(Atan(),e); }

Expression<Real> max(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Max(),e1,e2); }
Expression<Real> min(Expression<Real> const& e1, Expression<Real> const& e2) {
    return make_expression<Real>(Min(),e1,e2); }
Expression<Real> abs(Expression<Real> const& e) {
    return make_expression<Real>(Abs(),e); }

template String evaluate(const Expression<String>& e, const Valuation<String>& x);
template Integer evaluate(const Expression<Integer>& e, const Valuation<Integer>& x);
template Real evaluate(const Expression<Real>& e, const Valuation<Real>& x);
template Boolean evaluate(const Expression<Boolean>& e, const Valuation<String>& x);
template Boolean evaluate(const Expression<Boolean>& e, const Valuation<Integer>& x);
template Kleenean evaluate(const Expression<Kleenean>& e, const Valuation<Real>& x);

template Real evaluate(Expression<Real> const&, Map<Identifier, Real> const&);


template Set<Identifier> arguments(const Expression<Boolean>& e);
template Set<Identifier> arguments(const Expression<Kleenean>& e);
template Set<Identifier> arguments(const Expression<Real>& e);


template Expression<Kleenean> substitute(const Expression<Kleenean>& e, const Variable<Kleenean>& v, const Kleenean& c);
template Expression<Kleenean> substitute(const Expression<Kleenean>& e, const Variable<Real>& v, const Real& c);
template Expression<Real> substitute(const Expression<Real>& e, const Variable<Real>& v, const Real& c);
template Expression<Real> substitute(const Expression<Real>& e, const Variable<Real>& v, const Expression<Real>& c);
template Expression<Real> substitute(const Expression<Real>& e, const List< Assignment< Variable<Real>, Expression<Real> > >& c);
template Vector<Expression<Real>> substitute(const Vector<Expression<Real>>& e, const List< Assignment< Variable<Real>, Expression<Real> > >& c);
template Expression<Kleenean> substitute(const Expression<Kleenean>& e, const List< Assignment< Variable<Real>, Expression<Real> > >& c);

template Expression<Real> simplify(const Expression<Real>& e);
template Expression<Kleenean> simplify(const Expression<Kleenean>& e);

template void eliminate_common_subexpressions(Expression<Real>&);
template void eliminate_common_subexpressions(Vector<Expression<Real>>&);


template<class VIS, class A, class... OPS> decltype(auto) visit_symbolic(VIS vis, Symbolic<OperatorVariant<OPS...>,A> s) {
    return s.op().accept([&s,&vis](auto op){return vis(op,s.arg());}); }
template<class VIS, class A1, class A2, class... OPS> decltype(auto) visit_symbolic(VIS vis, Symbolic<OperatorVariant<OPS...>,A1,A2> s) {
    return s.op().accept([&s,&vis](auto op){return vis(op,s.arg1(),s.arg2());}); }


namespace {
using RE=Expression<Real>; using KE=Expression<Kleenean>;

RE _indicator(Sgn, RE e, Sign sign) { if(sign==Sign::POSITIVE) { return e; } else { return -e; } }
RE _indicator(Geq, RE e1, RE e2, Sign sign) { if(sign==Sign::POSITIVE) { return e1-e2; } else { return e2-e1; } }
RE _indicator(Gtr, RE e1, RE e2, Sign sign) { return _indicator(Geq(),e1,e2,sign); }
RE _indicator(Leq, RE e1, RE e2, Sign sign) { return _indicator(Geq(),e1,e2,-sign); }
RE _indicator(Less, RE e1, RE e2, Sign sign) { return _indicator(Leq(),e1,e2,sign); }
RE _indicator(Equal op, RE e1, RE e2, Sign) { HELPER_FAIL_MSG("Cannot compute indicator function of expression " << op(e1,e2)); }
RE _indicator(Unequal op, RE e1, RE e2, Sign) { HELPER_FAIL_MSG("Cannot compute indicator function of expression " << op(e1,e2)); }

RE _indicator(AndOp, KE e1, KE e2, Sign sign) { return min(indicator(e1,sign),indicator(e2,sign)); }
RE _indicator(OrOp, KE e1, KE e2, Sign sign) { return max(indicator(e1,sign),indicator(e2,sign)); }
RE _indicator(NotOp, KE e, Sign sign) { return neg(indicator(e,sign)); }

Expression<Real> indicator(ConstantExpressionNode<Kleenean> e, Sign sign) {
    Kleenean value=( sign==Sign::POSITIVE ? e.value() : !e.value() );
    ValidatedKleenean checked_value = value.check(Effort::get_default());
    if(definitely(checked_value)) { return Expression<Real>::constant(+1); }
    else if(not possibly(checked_value)) {  return Expression<Real>::constant(-1); }
    else { return Expression<Real>::constant(0); } }
Expression<Real> indicator(VariableExpressionNode<Kleenean> e, Sign) {
    HELPER_FAIL_MSG("Cannot compute indicator function of expression " << e); }
Expression<Real> indicator(UnaryExpressionNode<Kleenean> e, Sign sign) {
    return e.op().accept([&](auto op){return _indicator(op,e.arg(),sign);}); }
Expression<Real> indicator(BinaryExpressionNode<Kleenean> e, Sign sign) {
    return e.op().accept([&](auto op){return _indicator(op,e.arg1(),e.arg2(),sign);}); }
Expression<Real> indicator(UnaryExpressionNode<Kleenean,Real> e, Sign sign) {
    return e.op().accept([&](auto op){return _indicator(op,e.arg(),sign);}); }
Expression<Real> indicator(BinaryExpressionNode<Kleenean,Real,Real> e, Sign sign) {
    return e.op().accept([&](auto op){return _indicator(op,e.arg1(),e.arg2(),sign);}); }
}

Expression<Real> indicator(Expression<Kleenean> e, Sign sign) {
    return e.node_ref().accept([&](auto en){return indicator(en,sign);});
}



template bool is_constant(const Expression<Real>&, const Real&);
template bool is_constant(const Expression<Kleenean>&, const Kleenean&);

template bool is_variable(const Expression<Real>&, const Variable<Real>&);
template bool identical(const Expression<Real>&, const Expression<Real>&);

template bool is_constant_in(const Expression<Real>& e, const Set<Variable<Real>>& spc);

bool is_affine_in(const Expression<Real>& e, const Set<Variable<Real>>& spc) {
    return e.node_ref().accept([&spc](auto en){return is_affine_in(en,spc);});
}

bool is_affine_in(const Vector<Expression<Real>>& e, const Set<Variable<Real>>& spc) {
    for (auto i : range(e.size())) {
        if (not is_affine_in(e[i],spc)) return false;
    }
    return true;
}

bool is_polynomial_in(const Expression<Real>& e, const Set<Variable<Real>>& spc) {
    return e.node_ref().accept([&spc](auto en){return is_polynomial_in(en,spc);});
}

bool is_polynomial_in(const Vector<Expression<Real>>& e, const Set<Variable<Real>>& spc) {
    for (auto i : range(e.size())) {
        if (not is_polynomial_in(e[i],spc)) return false;
    }
    return true;
}

bool is_constant_in(const Expression<Real>& e, const Variable<Real>& var) { return is_constant_in(e,Set<RealVariable>{var}); }

namespace {
typedef Expression<Real> RE; typedef Expression<Real> const& REcr;
typedef Variable<Real> const& RVcr; typedef Constant<Real> const& RCcr;

inline bool _is_additive_in(Add, REcr e1, REcr e2, RVcr var) {
    return (is_additive_in(e1,var) && is_constant_in(e2,var)) || (is_constant_in(e1,var) && is_additive_in(e2,var)); }
inline bool _is_additive_in(Sub, REcr e1, REcr e2, RVcr var) {
    return is_additive_in(e1,var) && is_constant_in(e2,var); }
inline bool _is_additive_in(Variant<Mul,Div,Max,Min>, REcr , REcr , RVcr ) { return false; }
template<class... OPS> inline bool _is_additive_in(OperatorVariant<OPS...> const& ops, REcr e1, REcr e2, RVcr var) {
    return ops.accept([&](auto op){return _is_additive_in(op,e1,e2,var);}); }

inline bool is_additive_in(RCcr, RVcr) { return true; }
inline bool is_additive_in(RVcr, RVcr) { return true; }
template<class OP> inline bool is_additive_in(Symbolic<OP,RE> const&, RVcr) { return false; }
template<class OP> inline bool is_additive_in(Symbolic<OP,RE,int> const&, RVcr) { return false; }
template<class OP> inline bool is_additive_in(Symbolic<OP,RE,RE> const& e, RVcr var) { return _is_additive_in(e._op,e._arg1,e._arg2,var); }
}

bool is_additive_in(const Expression<Real>& e, const Variable<Real>& var) {
    return e.node_ref().accept([&](auto en){return is_additive_in(en,var);});
}

bool is_additive_in(const Vector<Expression<Real>>& ev, const Set<Variable<Real>>& spc) {
    // We treat the vector of expressions as additive in spc if each variable in spc appears at most once in all expressions,
    // with a constant value of 1
    // (FIXME: this simplifies the case of a constant multiplier, for which would need to rescale the variable)
    // (FIXME: more generally, this simplifies the case of a diagonalisable matrix of constant multipliers)

    for (auto v : spc) {
        bool already_found = false;
        bool already_found_one = false;
        for (auto i : range(ev.size())) {
            const Expression<Real>& e = ev[i];
            auto der = simplify(derivative(e, v));
            if (not is_constant_in(e,v)) {
                if (already_found) {
                    return false;
                } else {
                    already_found = true;
                    if (is_additive_in(e,v)) {
                        if (already_found_one) {
                            return false;
                        } else {
                            already_found_one = true;
                        }
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

namespace {

template<class OP> constexpr bool _identical(OP,OP) { return true; }
template<class OP1, class OP2> constexpr bool _identical(OP1,OP2) { return false; }

constexpr bool _opposite(Geq,Leq) { return true; }
constexpr bool _opposite(Leq,Geq) { return true; }
constexpr bool _opposite(Gtr,Less) { return true; }
constexpr bool _opposite(Less,Gtr) { return true; }
template<class OP1, class OP2> constexpr bool _opposite(OP1,OP2) { return false; }

bool opposite(BinaryComparisonOperator ops1, BinaryComparisonOperator ops2) {
    return ops1.accept([&ops2](auto op1){return ops2.accept([&op1](auto op2){return _opposite(op1,op2);});}); }
bool identical(BinaryComparisonOperator ops1, BinaryComparisonOperator ops2) {
    return ops1.accept([&ops2](auto op1){return ops2.accept([&op1](auto op2){return _identical(op1,op2);});}); }
}

bool opposite(Expression<Kleenean> e1, Expression<Kleenean> e2) {
    auto* e1cp = std::get_if<BinaryExpressionNode<Kleenean,Real>>(&e1.node_ref());
    auto* e2cp = std::get_if<BinaryExpressionNode<Kleenean,Real>>(&e2.node_ref());

    if (e1cp && e2cp) {
        if (identical(e1cp->op(),e2cp->op())) {
            return identical(e1.arg1(),e2.arg2()) && identical(e1.arg2(),e2.arg1());
        } else if (opposite(e1cp->op(),e2cp->op())) {
            return identical(e1.arg1(),e2.arg1()) && identical(e1.arg2(),e2.arg2());
        }
    }
    return false;
}

namespace {
Expression<Real> derivative(const Constant<Real>&, Variable<Real>) { return Expression<Real>(Real(0)); }
Expression<Real> derivative(const Variable<Real>& e, Variable<Real> v) { return Expression<Real>(Real(e==v ?1:0)); }
}

Expression<Real> derivative(const Expression<Real>& e, Variable<Real> v)
{
    return e.node_ref().accept([&v](auto en){return derivative(en,v);});
}

} // namespace SymboliCore
