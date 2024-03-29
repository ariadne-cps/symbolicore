/***************************************************************************
 *            test_expression.cpp
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

#include "helper/test.hpp"
#include "helper/container.hpp"
#include "helper/string.hpp"
#include "real.hpp"
#include "vector.hpp"
#include "expression.hpp"
#include "assignment.hpp"
#include "valuation.hpp"
#include "space.hpp"

using namespace SymboliCore;
using namespace Helper;

class TestExpression {
    RealConstant o;
    RealVariable x,y,z;
  public:
    TestExpression()
        : o("1.0",1.0), x("x"), y("y"), z("z") {
    }

    void test_variables() {
        HELPER_TEST_CONSTRUCT(RealVariable,a,("a"));
        HELPER_TEST_ASSERT(a==RealVariable("a"));
        HELPER_TEST_ASSERT(a==RealVariable(a));
        HELPER_TEST_ASSERT(a!=RealVariable("b"));
    }

    void test_expression() {
        // Regression test for constructing Expression from 0 without being an ambiguous nullptr;
        HELPER_TEST_CONSTRUCT(IntegerExpression,ze,(0));
        HELPER_TEST_CONSTRUCT(RealExpression,re,(0));
        RealExpression(0);
        RealExpression(nullptr);
    }

    void test_write() {
        HELPER_TEST_EQUALS(to_string(+x),"+x");
        HELPER_TEST_EQUALS(to_string(-x),"-x");
        HELPER_TEST_EQUALS(to_string(x+y),"x+y");
        HELPER_TEST_EQUALS(to_string(x-y),"x-y");
        HELPER_TEST_EQUALS(to_string(x*y),"x*y");
        HELPER_TEST_EQUALS(to_string(x/y),"x/y");
        HELPER_TEST_EQUALS(to_string(pos(x)),"+x");
        HELPER_TEST_EQUALS(to_string(neg(x)),"-x");
        HELPER_TEST_EQUALS(to_string(add(x,y)),"x+y");
        HELPER_TEST_EQUALS(to_string(sub(x,y)),"x-y");
        HELPER_TEST_EQUALS(to_string(mul(x,y)),"x*y");
        HELPER_TEST_EQUALS(to_string(div(x,y)),"x/y");
        HELPER_TEST_EQUALS(to_string(neg(sub(x,y))),"-(x-y)");
        HELPER_TEST_EQUALS(to_string(sub(neg(x),y)),"-x-y");
        HELPER_TEST_EQUALS(to_string(add(x,sub(y,z))),"x+y-z");
        HELPER_TEST_EQUALS(to_string(sub(x,add(y,z))),"x-(y+z)");
        HELPER_TEST_EQUALS(to_string(sub(x,sub(y,z))),"x-(y-z)");
        HELPER_TEST_EQUALS(to_string(add(sub(x,y),z)),"x-y+z");
        HELPER_TEST_EQUALS(to_string(sub(add(x,y),z)),"x+y-z");
        HELPER_TEST_EQUALS(to_string(sub(sub(x,y),z)),"x-y-z");
        HELPER_TEST_EQUALS(to_string(add(mul(x,y),z)),"x*y+z");
        HELPER_TEST_EQUALS(to_string(mul(add(x,y),z)),"(x+y)*z");
        HELPER_TEST_EQUALS(to_string(div(x,div(y,z))),"x/(y/z)");
        HELPER_TEST_EQUALS(to_string(div(div(x,y),z)),"x/y/z");
        HELPER_TEST_EQUALS(to_string(sub(x,div(y,z))),"x-y/z");
        HELPER_TEST_EQUALS(to_string(sub(div(x,y),z)),"x/y-z");
        HELPER_TEST_EQUALS(to_string(max(x,sub(y,z))),"max(x,y-z)");
        HELPER_TEST_EQUALS(to_string(sub(x,max(y,z))),"x-max(y,z)");
    }

    void test_assignment() {
        Real zero(0), one(1);
        RealExpression e(x*y+o);

        HELPER_TEST_ASSERT((not Assignable<Variable<Real>,Expression<Real>>));
        HELPER_TEST_ASSERT((not Constructible<Assignment<Variable<Real>,Expression<Real>>,Assignment<Variable<Real>,Real>>));

        typedef Assignment<Variable<Real>,Real> ConstantRealAssignment;
        HELPER_TEST_CONSTRUCT(ConstantRealAssignment,ac,(x=one));
        HELPER_TEST_CONSTRUCT(List<ConstantRealAssignment>,lac,({x=zero,y=one}));
        HELPER_TEST_CONSTRUCT(Valuation<Real>,va,(lac));

        HELPER_TEST_CONSTRUCT(RealAssignment,a,(let(x)=one));
        HELPER_TEST_CONSTRUCT(PrimedRealAssignment,pa,(prime(x)=one));
        HELPER_TEST_CONSTRUCT(DottedRealAssignment,da,(dot(x)=one));

        HELPER_TEST_CONSTRUCT(List<RealAssignment>,la,(let({x,y,z})={zero,x,e}));
        HELPER_TEST_CONSTRUCT(List<PrimedRealAssignment>,lpa,(prime({x,y,z})={zero,x,e}));
        HELPER_TEST_CONSTRUCT(List<DottedRealAssignment>,lda,(dot({x,y,z})={zero,x,e}));
    }

    void test_parameters() {
        RealExpression expr = x;

        Map<Identifier,Real> valuation;
        Real value(-0.0626);
        valuation[x.name()] = value;

        HELPER_TEST_EQUALS(expr.kind(),OperatorKind::VARIABLE);
        HELPER_TEST_EQUALS(expr.var(),"x");
        HELPER_TEST_EQUALS(valuation[x.name()],value);

        Real result1 = evaluate(expr,valuation);

        HELPER_TEST_EQUALS(result1,value);
    }

    void test_print() {
        HELPER_TEST_CONSTRUCT(RealExpression,g,(x+3*y*z*z));

        Map<RealVariable,Real> v;
        v[x]=Real(2.0); v[y]=Real(3.0); v[z]=Real(5.0);

        HELPER_TEST_PRINT(v);
    }

    void test_identical() {
        HELPER_TEST_ASSERT(identical(RealExpression(x),RealExpression(x)));
        HELPER_TEST_ASSERT(identical(RealExpression::constant(0),RealExpression::constant(0)));
        HELPER_TEST_ASSERT(identical(RealExpression::constant(2),RealExpression::constant(2)));
        HELPER_TEST_ASSERT(identical(sin(x),sin(x)));
        HELPER_TEST_ASSERT(identical(pow(x,2),pow(x,2)));
        HELPER_TEST_ASSERT(not identical(x*y,y*x));
        HELPER_TEST_ASSERT(not identical(x+y,y+x));

        HELPER_TEST_CONSTRUCT(RealExpression,e1,(sin(pow(x,2)+y)*(y/x+1)))
        HELPER_TEST_CONSTRUCT(RealExpression,e2,(sin(pow(x,2)+y)*(y/x+1)));
        HELPER_TEST_ASSERT(identical(e1,e1));
        HELPER_TEST_ASSERT(identical(e1,e2));
    }

    void test_derivative() {
        RealExpression expr = 2*x+y;
        HELPER_TEST_PRINT(derivative(expr,x));
        HELPER_TEST_PRINT(simplify(derivative(expr,x)));
        HELPER_TEST_ASSERT(identical(simplify(derivative(expr,x)),RealExpression::constant(2)));
        RealExpression expr2 = pow(x,3);
        HELPER_TEST_PRINT(derivative(expr2,x));
        HELPER_TEST_PRINT(simplify(derivative(expr2,x)));
        HELPER_TEST_ASSERT(identical(simplify(derivative(expr2,x)),3*sqr(x)));
    }

    void test_simplify() {
        RealExpression zero(0);
        RealExpression one(1);
        RealVariable u("u");
        RealExpression expr = -u*x*y+2*x;
        RealExpression ex=x;
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(derivative(-u*x*y+2*x,x)),-u*y+2);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(pos(neg(ex))),neg(ex));
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(neg(neg(ex))),ex);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(rec(rec(ex))),ex);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(log(exp(ex))),ex);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(exp(log(ex))),ex);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(sqr(sqrt(ex))),ex);
        // Regression tests
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(sub(neg(zero),zero)),zero);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(sub(x,x)),zero);
        HELPER_TEST_BINARY_PREDICATE(identical,simplify(div(x,x)),one);
    }

    void test_ordering() {
        HELPER_TEST_ASSERT(before(RealExpression(x),RealExpression(y)));
        HELPER_TEST_ASSERT(not before(RealExpression(x),RealExpression(x)));
        HELPER_TEST_ASSERT(before(RealExpression(1),RealExpression(2)));
        HELPER_TEST_ASSERT(not before(RealExpression(5),RealExpression(2)));
        HELPER_TEST_ASSERT(before(pow(x,2),pow(x,3)));
        HELPER_TEST_ASSERT(before(pow(y,2),pow(x,3)));
        HELPER_TEST_ASSERT(before(pow(x,2),pow(x,3)));
        HELPER_TEST_ASSERT(not before(sin(y),sin(x)));
        HELPER_TEST_ASSERT(before(sqrt(x),sin(y)));
        HELPER_TEST_ASSERT(before(x+y,y+x));
        HELPER_TEST_ASSERT(not before(x+y,x+x));
        HELPER_TEST_ASSERT(not before(log(x),rec(x)));
    }

    void test_count_nodes() {
        HELPER_TEST_EQUALS(count_nodes(RealExpression(x)),1);
        HELPER_TEST_EQUALS(count_nodes(RealExpression(1)),1);
        HELPER_TEST_EQUALS(count_nodes(2*x),3);
        HELPER_TEST_EQUALS(count_nodes(x+y),3);
        HELPER_TEST_EQUALS(count_nodes(x+sqr(y)),4);
        HELPER_TEST_EQUALS(count_nodes(sin(y)),2);
        HELPER_TEST_EQUALS(count_nodes(pow(y,2)),2);
        HELPER_TEST_EQUALS(count_nodes(pow(x+cos(y),2)+cos(y)),8);
        HELPER_TEST_EQUALS(count_nodes(x+cos(x)+pow(cos(x),2)),8);
    }

    void test_count_distinct_nodes() {
        HELPER_TEST_EQUALS(count_distinct_nodes(RealExpression(x)),1);
        HELPER_TEST_EQUALS(count_distinct_nodes(x*exp(x)),3);
        HELPER_TEST_EQUALS(count_distinct_nodes(x*x),2);
        RealExpression one = 1;
        HELPER_TEST_EQUALS(count_distinct_nodes(one+cos(one)),3);
        HELPER_TEST_EQUALS(count_distinct_nodes(x*y+sqr(x*y)),5);
        HELPER_TEST_EQUALS(count_distinct_nodes(pow(x,2)*pow(x,2)),3);
        HELPER_TEST_EQUALS(count_distinct_nodes(pow(y,2)+y*y),4);
        HELPER_TEST_EQUALS(count_distinct_nodes(pow(x+cos(y),2)+cos(y)),6);
        HELPER_TEST_EQUALS(count_distinct_nodes(x+cos(x)+pow(cos(x),2)),5);
    }

    void test_count_distinct_node_pointers() {
        HELPER_TEST_EQUALS(count_distinct_node_pointers(RealExpression(x)),1);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(x*exp(x)),4);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(x*x),3);
        RealExpression one = 1;
        HELPER_TEST_EQUALS(count_distinct_node_pointers(one+cos(one)),3);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(x*y+sqr(x*y)),8);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(pow(x,2)*pow(x,2)),5);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(pow(y,2)+y*y),6);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(pow(x+cos(y),2)+cos(y)),8);
        HELPER_TEST_EQUALS(count_distinct_node_pointers(x+cos(x)+pow(cos(x),2)),8);
    }

    void test_eliminate_common_subexpressions() {
        RealExpression expr1 = x;
        HELPER_TEST_PRINT(expr1);
        eliminate_common_subexpressions(expr1);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr1),1);
        RealExpression expr2 = x*exp(x);
        HELPER_TEST_PRINT(expr2);
        eliminate_common_subexpressions(expr2);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr2),3);
        RealExpression expr3 = x*x;
        HELPER_TEST_PRINT(expr3);
        eliminate_common_subexpressions(expr3);
        eliminate_common_subexpressions(expr3);
        HELPER_TEST_PRINT(expr3);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr3),2);
        RealExpression one = 1;
        RealExpression expr4 = one+cos(one);
        HELPER_TEST_PRINT(expr4);
        eliminate_common_subexpressions(expr4);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr4),3);
        RealExpression expr5 = x*y+sqr(x*y);
        HELPER_TEST_PRINT(expr5);
        eliminate_common_subexpressions(expr5);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr5),5);
        RealExpression expr6 = pow(x,2)*pow(x,2);
        HELPER_TEST_PRINT(expr6);
        eliminate_common_subexpressions(expr6);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr6),3);
        RealExpression expr7 = pow(y,2)+y*y;
        HELPER_TEST_PRINT(expr7);
        eliminate_common_subexpressions(expr7);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr7),4);
        RealExpression expr8 = pow(x+cos(y),2)+cos(y);
        HELPER_TEST_PRINT(expr8);
        eliminate_common_subexpressions(expr8);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr8),6);
        RealExpression expr9 = x+cos(x)+pow(cos(x),2);
        HELPER_TEST_PRINT(expr9);
        eliminate_common_subexpressions(expr9);
        HELPER_TEST_EQUAL(count_distinct_node_pointers(expr9),5);
    }

    void test_substitute() {

        RealVariable u1("u1"), u2("u2");
        RealExpression expr = -u1*x*y+2*pow(x+u2,2);

        List<Assignment<RealVariable,RealExpression>> subs={{u1,u1+1},{u2,u1*x}};

        RealExpression substitution = substitute(expr,subs);

        HELPER_TEST_ASSERT(identical(substitution,-(u1+1)*x*y+2*pow(x+u1*x,2)));
    }

    void test_is_constant_in() {
        Real c(3);
        HELPER_TEST_ASSERT(is_constant_in(3*y,{x}));
        HELPER_TEST_ASSERT(is_constant_in(pow(x,2),{y}));
        HELPER_TEST_ASSERT(not is_constant_in(pow(x,2),{x}));
        HELPER_TEST_ASSERT(not is_constant_in(3*y,{y}));
        HELPER_TEST_ASSERT(not is_constant_in(0*y,{y}));
        HELPER_TEST_ASSERT(not is_constant_in((sin(2*c)-2*sin(c)*cos(c))*y,{y}));
        HELPER_TEST_ASSERT(not is_constant_in((sin(2*x)-2*sin(x)*cos(x))*y,{y}));
        HELPER_TEST_ASSERT(is_constant_in(simplify(0*y),{y}));
    }

    void test_is_additive_in() {
        RealVariable u1("u1"), u2("u2");
        HELPER_TEST_ASSERT(is_additive_in(u1,u1));
        HELPER_TEST_ASSERT(is_additive_in(x,u1));
        HELPER_TEST_ASSERT(is_additive_in(x+u1,u1));
        HELPER_TEST_ASSERT(is_additive_in(x+u1,u2));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x+u1}),{u1}));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x+u1}),{u2}));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x+u1,y+u2}),{u1,u2}));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x+u2,y+u1}),{u1,u2}));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x+u1,y}),{u1}));
        HELPER_TEST_ASSERT(is_additive_in(Vector<RealExpression>({x,y+u1}),{u1}));
        HELPER_TEST_ASSERT(not is_additive_in(Vector<RealExpression>({x,y+2*u1}),{u1}));
        HELPER_TEST_ASSERT(not is_additive_in(Vector<RealExpression>({x+u1,y+2*u2}),{u1,u2}));
        HELPER_TEST_ASSERT(not is_additive_in(Vector<RealExpression>({x+u1,y+u1}),{u1}));
        HELPER_TEST_ASSERT(not is_additive_in(Vector<RealExpression>({x*u1,y+u2}),{u1,u2}));
        HELPER_TEST_ASSERT(not is_additive_in(Vector<RealExpression>({x+u1,y+sqr(u2)}),{u1,u2}));
    }

    void test_is_affine_in() {
        HELPER_TEST_ASSERT(is_affine_in(sqr(x),{y}));
        HELPER_TEST_ASSERT(is_affine_in(neg(x),{x}));
        HELPER_TEST_ASSERT(is_affine_in(pow(x,3),{y}));
        HELPER_TEST_ASSERT(is_affine_in(pow(x,3)+y,{y}));
        HELPER_TEST_ASSERT(is_affine_in(2+3*x-5*y-x,{x,y}));
        HELPER_TEST_ASSERT(is_affine_in(3*y,{x,y}));
        HELPER_TEST_ASSERT(is_affine_in(x*y,{x}));
        HELPER_TEST_ASSERT(is_affine_in(3*x/y,{x}));
        HELPER_TEST_ASSERT(not is_affine_in(atan(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(acos(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(asin(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(cos(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(sin(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(tan(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(abs(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(log(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(sqrt(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(exp(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(rec(x),{x}))
        HELPER_TEST_ASSERT(is_affine_in(hlf(x),{x}))
        HELPER_TEST_ASSERT(is_affine_in(nul(exp(x)),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(sqr(x),{x}))
        HELPER_TEST_ASSERT(is_affine_in(neg(x),{x}))
        HELPER_TEST_ASSERT(not is_affine_in(pow(x,3),{x}));
        HELPER_TEST_ASSERT(not is_affine_in(sqr(x),{x}));
        HELPER_TEST_ASSERT(not is_affine_in(x*y,{x,y}));
        HELPER_TEST_ASSERT(not is_affine_in(x*x,{x}));
        HELPER_TEST_ASSERT(not is_affine_in(0*x*x,{x}));
        HELPER_TEST_ASSERT(not is_affine_in(x/y,{y}));
    }

    void test_is_polynomial_in() {
        HELPER_TEST_ASSERT(is_polynomial_in(1,{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(x,{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(x*x,{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(atan(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(acos(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(asin(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(cos(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(sin(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(tan(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(abs(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(log(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(sqrt(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(exp(x),{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(rec(x),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(hlf(x),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(nul(exp(x)),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(sqr(x),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(neg(x),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(pow(x,2),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(pow(x,3),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(pow(x+sqr(x),3),{x}))
        HELPER_TEST_ASSERT(is_polynomial_in(x/y,{x}))
        HELPER_TEST_ASSERT(not is_polynomial_in(x/y,{y}))
        HELPER_TEST_ASSERT(not is_polynomial_in(x/y,{x,y}))
        HELPER_TEST_ASSERT(is_polynomial_in({x,sqr(y)},{x,y}))
        HELPER_TEST_ASSERT(not is_polynomial_in({x/y,sqr(y)},{x,y}))
    }

    void test() {
        HELPER_TEST_CALL(test_variables());
        HELPER_TEST_CALL(test_expression());
        HELPER_TEST_CALL(test_write());
        HELPER_TEST_CALL(test_assignment());
        HELPER_TEST_CALL(test_parameters());
        HELPER_TEST_CALL(test_print());
        HELPER_TEST_CALL(test_identical());
        HELPER_TEST_CALL(test_derivative());
        HELPER_TEST_CALL(test_simplify());
        HELPER_TEST_CALL(test_ordering());
        HELPER_TEST_CALL(test_count_nodes());
        HELPER_TEST_CALL(test_count_distinct_nodes());
        HELPER_TEST_CALL(test_count_distinct_node_pointers());
        HELPER_TEST_CALL(test_eliminate_common_subexpressions());
        HELPER_TEST_CALL(test_substitute());
        HELPER_TEST_CALL(test_is_constant_in());
        HELPER_TEST_CALL(test_is_additive_in());
        HELPER_TEST_CALL(test_is_affine_in());
        HELPER_TEST_CALL(test_is_polynomial_in());
    }

};


int main() {
    TestExpression().test();
    return HELPER_TEST_FAILURES;
}
