/***************************************************************************
 *            test_real.cpp
 *
 *  Copyright  2023  Luca Geretti, Pieter Collins
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

#include "logical.hpp"
#include "real.hpp"
#include "test.hpp"

using namespace std;
using namespace SymboliCore;

class TestReal
{
  public:
    void test();
  private:
    void test_concept();
    void test_arithmetic();
    void test_transcendental();
    void test_comparison();
};

void TestReal::test()
{
    SYMBOLICORE_TEST_CALL(test_arithmetic());
    SYMBOLICORE_TEST_CALL(test_transcendental());
}

void TestReal::test_concept() {
    Real x,y;
    x=Real(); x=Real(1); x=Real(1.0);
    x=Real(1);
    y=+x; y=-x; y=x+x; y=x-x; y=x*x; y=x/x;
    y=pow(x,2u); y=pow(x,2);
    y=sqr(x); y=rec(x); y=sqrt(x);
    y=exp(x); y=log(x);
    y=sin(x); y=cos(x); y=tan(x);
}

void TestReal::test_arithmetic() {
    Real x(2.5);
    Real y(4.0);
    SYMBOLICORE_TEST_EQUALS(x, 2.5);
    SYMBOLICORE_TEST_EQUALS(y, 4.0);
    SYMBOLICORE_TEST_EQUALS(+x, 2.5);
    SYMBOLICORE_TEST_EQUALS(-x,-2.5);
    SYMBOLICORE_TEST_EQUALS(x+y, 6.5);
    SYMBOLICORE_TEST_EQUALS(x-y,-1.5);
    SYMBOLICORE_TEST_EQUALS(x*y,10.0);
    SYMBOLICORE_TEST_EQUALS(x/y,0.625);
    SYMBOLICORE_TEST_EQUALS(add(x,y), 6.5);
    SYMBOLICORE_TEST_EQUALS(sub(x,y),-1.5);
    SYMBOLICORE_TEST_EQUALS(mul(x,y),10.0);
    SYMBOLICORE_TEST_EQUALS(div(x,y),0.625);
    SYMBOLICORE_TEST_EQUALS(pow(x,3u),15.625);
    SYMBOLICORE_TEST_EQUALS(pow(x,3),15.625);
    SYMBOLICORE_TEST_EQUALS(pos(x),+2.5);
    SYMBOLICORE_TEST_EQUALS(neg(x),-2.5);
    SYMBOLICORE_TEST_EQUALS(hlf(x),1.25);
    SYMBOLICORE_TEST_EQUALS(sqr(x),6.25);
    SYMBOLICORE_TEST_EQUALS(rec(y),0.25);
}

void TestReal::test_transcendental() {
    double eps = 1e-16;
    double ax = 2.5;
    Real x(ax);
    SYMBOLICORE_TEST_EQUALS(sqrt(Real(4)),2.0);
    SYMBOLICORE_TEST_EQUALS(exp(Real(0)),1.0);
    SYMBOLICORE_TEST_EQUALS(log(Real(1)),0.0);

    SYMBOLICORE_TEST_WITHIN(sqrt(x),sqrt(ax),eps);
    SYMBOLICORE_TEST_WITHIN(exp(x),exp(ax),8*eps);
    SYMBOLICORE_TEST_WITHIN(log(x),log(ax),eps);
    SYMBOLICORE_TEST_WITHIN(sin(x),sin(ax),2*eps);
    SYMBOLICORE_TEST_WITHIN(cos(x),cos(ax),2*eps);
    SYMBOLICORE_TEST_WITHIN(tan(x),tan(ax),3*eps);
    SYMBOLICORE_TEST_WITHIN(atan(x),atan(ax),eps);
}

int main() {
    TestReal().test();
    return SYMBOLICORE_TEST_FAILURES;
}
