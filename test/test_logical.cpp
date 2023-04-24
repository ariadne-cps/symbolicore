/***************************************************************************
 *            test_logical.cpp
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
#include "helper/macros.hpp"
#include "logical.hpp"
#include "sequence.hpp"

using namespace SymboliCore;
using namespace Helper;

class TestLogical
{
  public:
    void test();
  private:
    void test_concept();
    void test_conversion_to_bool();
    void test_conversion();
    void test_disjunction();
};

int main() {
    HELPER_TEST_CLASS(TestLogical,TestLogical());
    return HELPER_TEST_FAILURES;
}

void
TestLogical::test()
{
    HELPER_TEST_CALL(test_conversion_to_bool());
    HELPER_TEST_CALL(test_conversion());
    HELPER_TEST_CALL(test_disjunction());
}

void
TestLogical::test_concept()
{
    // Check to see if we can perform operations on computational and specification logical types
    LogicalType<ExactTag> xl(true);
    LogicalType<EffectiveTag> el(true);
    LogicalType<ValidatedTag> vl(true);
    Effort eff(0);

    vl=el.check(eff);
    vl=check(el,eff);

    vl=indeterminate;
    vl=LogicalType<ValidatedTag>(LogicalValue::LIKELY);

    xl = xl && xl;
    el = xl && el;
    vl = xl && vl;
    el = el && el;
    vl = vl && vl;
}

void
TestLogical::test_conversion_to_bool()
{
    HELPER_TEST_CONCEPT(Convertible<Boolean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<Sierpinskian,bool>);
    HELPER_TEST_CONCEPT(not Convertible<NegatedSierpinskian,bool>);
    HELPER_TEST_CONCEPT(not Convertible<Kleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<LowerKleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<UpperKleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<ValidatedKleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<ValidatedUpperKleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<ValidatedLowerKleenean,bool>);
    HELPER_TEST_CONCEPT(not Convertible<ApproximateKleenean,bool>);
}

void
TestLogical::test_conversion()
{
    if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        HELPER_TEST_NOTIFY("EffectiveTag logical types may be converted to values using default Effort.");
    } else if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        HELPER_TEST_NOTIFY("EffectiveTag logical types may be explicitly converted to values using default Effort.");
    } else {
        HELPER_TEST_NOTIFY("EffectiveTag logical types cannot be converted to values; the Effort used must be specified.");
    }

    try {
        if(decide(indeterminate)) {
            HELPER_TEST_NOTIFY("decide(...) is true on INDETERMINATE value.");
        } else {
            HELPER_TEST_NOTIFY("decide(...) is false on INDETERMINATE value.");
        }
    } catch(...) {
        HELPER_TEST_NOTIFY("decide(...) is throws error on INDETERMINATE value.");
    }

    HELPER_TEST_CONCEPT(not Convertible<Indeterminate,Boolean>);
    HELPER_TEST_CONCEPT(Convertible<Indeterminate,Sierpinskian>);
    HELPER_TEST_CONCEPT(Convertible<Indeterminate,NegatedSierpinskian>);
    HELPER_TEST_CONCEPT(Convertible<Indeterminate,Kleenean>);
    HELPER_TEST_CONCEPT(Convertible<Indeterminate,LowerKleenean>);
    HELPER_TEST_CONCEPT(Convertible<Indeterminate,UpperKleenean>);
    HELPER_TEST_CONCEPT(Same<decltype(indeterminate and Sierpinskian(true)),Sierpinskian>);
    HELPER_TEST_CONCEPT(Same<decltype(indeterminate and Kleenean(true)),Kleenean>);

    HELPER_TEST_CONSTRUCT(LogicalType<ValidatedTag>,vl,(LogicalValue::LIKELY))
    HELPER_TEST_EQUAL(definitely(vl),false);
    HELPER_TEST_EQUAL(possibly(vl),true);
    HELPER_TEST_EQUAL(decide(vl),true);

    HELPER_TEST_CONSTRUCT(LogicalType<ValidatedTag>,vi,(LogicalValue::INDETERMINATE))
    HELPER_TEST_EQUAL(definitely(vl),false);
    HELPER_TEST_EQUAL(possibly(vl),true);
}

void
TestLogical::test_disjunction()
{
    Sequence<LowerKleenean> seq([](unsigned int n){return n==2 ? LowerKleenean(true) : LowerKleenean(indeterminate);});
    HELPER_TEST_ASSIGN_CONSTRUCT(LowerKleenean, some, disjunction(seq));
    HELPER_TEST_ASSERT(possibly(not some.check(2_eff)));
    HELPER_TEST_ASSERT(definitely(some.check(3_eff)));
    HELPER_TEST_ASSERT(definitely(some.check(4_eff)));

    HELPER_TEST_ASSIGN_CONSTRUCT(
        UpperKleenean, all, conjunction(Sequence<UpperKleenean>([](unsigned int n){return n==2 ? UpperKleenean(false) : UpperKleenean(indeterminate);})));
    HELPER_TEST_ASSERT(possibly(all.check(2_eff)));
    HELPER_TEST_ASSERT(not possibly(all.check(3_eff)));
    HELPER_TEST_ASSERT(definitely(not all.check(4_eff)));

}

