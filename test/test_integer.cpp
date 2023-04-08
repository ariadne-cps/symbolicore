/***************************************************************************
 *            test_integer.cpp
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

#include "utility/test.hpp"
#include "utility/string.hpp"
#include "integer.hpp"
#include "logical.hpp"

using namespace std;
using namespace SymboliCore;

class TestInteger
{
  public:
    void test();
  private:
    void test_concept();
    void test_literal();
    void test_constructors();
    void test_arithmetic();
    void test_comparisons();
};

void TestInteger::test()
{
    UTILITY_TEST_CALL(test_constructors());
    UTILITY_TEST_CALL(test_comparisons());
    UTILITY_TEST_CALL(test_literal());
    UTILITY_TEST_CALL(test_arithmetic());
}


void TestInteger::test_literal() {
    Integer z(3);
    UTILITY_TEST_EQUALS(z,3);
    UTILITY_TEST_EQUALS(z,Integer(3));
    UTILITY_TEST_EQUALS(100000,Integer(100000));
    UTILITY_TEST_EQUAL(1000000000000,sqr(Integer(1000000)));
    UTILITY_TEST_EQUALS(1000000000000-sqr(Integer(1000000)),0);
    UTILITY_TEST_EQUALS(4294967295,4294967295u);
    UTILITY_TEST_EQUALS(-2147483647,-2147483647);
    UTILITY_TEST_EQUALS(4294967295,Integer(4294967295u));
    UTILITY_TEST_EQUALS(4611686016279904256,Integer(2147483647)*2147483647+2147483647);
}

void TestInteger::test_constructors() {
    int m=2147483647;
    unsigned int um=2147483647u;
    unsigned long int ulm=um;
    unsigned long long int ullm=um;
    int n=-2147483647;
    long int ln=n;
    long long int lln=n;
    String sn="-2147483647";
    String sz="314159265358979323846264";

    UTILITY_TEST_ASSERT((not Constructible<Integer,float>));
    UTILITY_TEST_ASSERT((not Constructible<Integer,double>));

    UTILITY_TEST_CONSTRUCT(Integer,zum,(um));
    UTILITY_TEST_EQUALS(zum.value(),m);
    UTILITY_TEST_CONSTRUCT(Integer,zulm,(ulm));
    UTILITY_TEST_EQUALS(zulm.value(),m);
    UTILITY_TEST_CONSTRUCT(Integer,zullm,(ullm));
    UTILITY_TEST_EQUALS(zullm.value(),m);
    UTILITY_TEST_CONSTRUCT(Integer,zn,(n));
    UTILITY_TEST_EQUALS(zn.value(),n);
    UTILITY_TEST_CONSTRUCT(Integer,zln,(ln));
    UTILITY_TEST_EQUALS(zln.value(),n);
    UTILITY_TEST_CONSTRUCT(Integer,zlln,(lln));
    UTILITY_TEST_EQUALS(zlln.value(),n);
    UTILITY_TEST_CONSTRUCT(Integer,zsn,(sn));
    UTILITY_TEST_EQUALS(zsn.value(),n);

    UTILITY_TEST_CONSTRUCT(Integer,z1,(0));
    UTILITY_TEST_EQUALS(z1.value(),0);
    UTILITY_TEST_CONSTRUCT(Integer,z2,(-3));
    UTILITY_TEST_EQUALS(z2.value(),-3);
    UTILITY_TEST_CONSTRUCT(Integer,z3,(ullm*ullm+ullm));
    UTILITY_TEST_EQUALS(z3,zum*zum+zum);
    UTILITY_TEST_CONSTRUCT(Integer,z4,(lln*lln+lln));
    UTILITY_TEST_EQUALS(z4,zn*zn+zn);
}

void TestInteger::test_arithmetic() {
    UTILITY_TEST_EQUALS(+Integer(-5),-5);
    UTILITY_TEST_EQUALS(-Integer(-5), 5);
    UTILITY_TEST_EQUALS(Integer(-5)+Integer(2),-3);
    UTILITY_TEST_EQUALS(Integer(-5)-Integer(2),-7);
    UTILITY_TEST_EQUALS(Integer(-5)*Integer(2),-10);

    UTILITY_TEST_EQUALS(pos(Integer(-5)),-5);
    UTILITY_TEST_EQUALS(neg(Integer(-5)), 5);
    UTILITY_TEST_EQUALS(sqr(Integer(-5)),25);
    UTILITY_TEST_EQUALS(pow(Integer(-5),3u),-125);


    UTILITY_TEST_EQUALS((Integer)max(Integer(5),Integer(3)),5);
    UTILITY_TEST_EQUALS((Integer)max(Integer(-5),Integer(-3)),-3);
    UTILITY_TEST_EQUALS((Integer)min(Integer(5),Integer(3)),3);
    UTILITY_TEST_EQUALS((Integer)min(Integer(-5),Integer(-3)),-5);
    UTILITY_TEST_EQUALS(abs(Integer(-5)),5);
    UTILITY_TEST_EQUALS(abs(Integer( 0)),0);
    UTILITY_TEST_EQUALS(abs(Integer(+5)),5);
}

void TestInteger::test_comparisons() {
    UTILITY_TEST_COMPARE(Integer(3),==,3);
    UTILITY_TEST_COMPARE(3,==,Integer(3));

    UTILITY_TEST_COMPARE(Integer(2),==,Integer(2));
    UTILITY_TEST_COMPARE(Integer(0),==,Integer(-0));
    UTILITY_TEST_COMPARE(Integer(2),!=,Integer(-2));
    UTILITY_TEST_COMPARE(Integer(2),!=,Integer(-3));
    UTILITY_TEST_COMPARE(Integer(2),<=,Integer(23));
    UTILITY_TEST_COMPARE(Integer(2),<=,Integer(3));
    UTILITY_TEST_COMPARE(Integer(2),>=,Integer(2));
    UTILITY_TEST_COMPARE(Integer(2),>=,Integer(-3));
    UTILITY_TEST_COMPARE(Integer(2),< ,Integer(3));
    UTILITY_TEST_COMPARE(Integer(2),> ,Integer(-3));
}

int main() {
    UTILITY_TEST_CLASS(Integer,TestInteger());

    return UTILITY_TEST_FAILURES;
}
