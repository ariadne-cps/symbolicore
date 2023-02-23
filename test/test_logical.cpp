/***************************************************************************
 *            test_logical.cpp
 *
 *  Copyright  2023  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "metaprogramming.hpp"
#include "logical.hpp"
#include "sequence.hpp"
#include "macros.hpp"

#include "test.hpp"

using namespace SymboliCore;

class TestLogical
{
  public:
    Void test();
  private:
    Void test_concept();
    Void test_conversion_to_bool();
    Void test_conversion();
    Void test_disjunction();
};

int main() {
    SYMBOLICORE_TEST_CLASS(TestLogical,TestLogical());
    return SYMBOLICORE_TEST_FAILURES;
}

Void
TestLogical::test()
{
    SYMBOLICORE_TEST_CALL(test_conversion_to_bool());
    SYMBOLICORE_TEST_CALL(test_conversion());
    SYMBOLICORE_TEST_CALL(test_disjunction());
}

Void
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

Void
TestLogical::test_conversion_to_bool()
{
    SYMBOLICORE_TEST_CONCEPT(Convertible<Boolean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<Sierpinskian,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<NegatedSierpinskian,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<Kleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<LowerKleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<UpperKleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<ValidatedKleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<ValidatedUpperKleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<ValidatedLowerKleenean,Bool>);
    SYMBOLICORE_TEST_CONCEPT(not Convertible<ApproximateKleenean,Bool>);
}

Void
TestLogical::test_conversion()
{
    if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        SYMBOLICORE_TEST_NOTIFY("EffectiveTag logical types may be converted to values using default Effort.");
    } else if(Convertible<LogicalType<EffectiveTag>,LogicalType<ValidatedTag>>) {
        SYMBOLICORE_TEST_NOTIFY("EffectiveTag logical types may be explicitly converted to values using default Effort.");
    } else {
        SYMBOLICORE_TEST_NOTIFY("EffectiveTag logical types cannot be converted to values; the Effort used must be specified.");
    }

    try {
        if(decide(indeterminate)) {
            SYMBOLICORE_TEST_NOTIFY("decide(...) is true on INDETERMINATE value.");
        } else {
            SYMBOLICORE_TEST_NOTIFY("decide(...) is false on INDETERMINATE value.");
        }
    } catch(...) {
        SYMBOLICORE_TEST_NOTIFY("decide(...) is throws error on INDETERMINATE value.");
    }

    SYMBOLICORE_TEST_CONCEPT(not Convertible<Indeterminate,Boolean>);
    SYMBOLICORE_TEST_CONCEPT(Convertible<Indeterminate,Sierpinskian>);
    SYMBOLICORE_TEST_CONCEPT(Convertible<Indeterminate,NegatedSierpinskian>);
    SYMBOLICORE_TEST_CONCEPT(Convertible<Indeterminate,Kleenean>);
    SYMBOLICORE_TEST_CONCEPT(Convertible<Indeterminate,LowerKleenean>);
    SYMBOLICORE_TEST_CONCEPT(Convertible<Indeterminate,UpperKleenean>);
    SYMBOLICORE_TEST_CONCEPT(Same<decltype(indeterminate and Sierpinskian(true)),Sierpinskian>);
    SYMBOLICORE_TEST_CONCEPT(Same<decltype(indeterminate and Kleenean(true)),Kleenean>);

    SYMBOLICORE_TEST_CONSTRUCT(LogicalType<ValidatedTag>,vl,(LogicalValue::LIKELY))
    SYMBOLICORE_TEST_EQUAL(definitely(vl),false);
    SYMBOLICORE_TEST_EQUAL(possibly(vl),true);
    SYMBOLICORE_TEST_EQUAL(decide(vl),true);

    SYMBOLICORE_TEST_CONSTRUCT(LogicalType<ValidatedTag>,vi,(LogicalValue::INDETERMINATE))
    SYMBOLICORE_TEST_EQUAL(definitely(vl),false);
    SYMBOLICORE_TEST_EQUAL(possibly(vl),true);
}

Void
TestLogical::test_disjunction()
{
    Sequence<LowerKleenean> seq([](Nat n){return n==2 ? LowerKleenean(true) : LowerKleenean(indeterminate);});
    SYMBOLICORE_TEST_ASSIGN_CONSTRUCT(LowerKleenean, some, disjunction(seq));
    SYMBOLICORE_TEST_ASSERT(possibly(not some.check(2_eff)));
    SYMBOLICORE_TEST_ASSERT(definitely(some.check(3_eff)));
    SYMBOLICORE_TEST_ASSERT(definitely(some.check(4_eff)));

    SYMBOLICORE_TEST_ASSIGN_CONSTRUCT(
        UpperKleenean, all, conjunction(Sequence<UpperKleenean>([](Nat n){return n==2 ? UpperKleenean(false) : UpperKleenean(indeterminate);})));
    SYMBOLICORE_TEST_ASSERT(possibly(all.check(2_eff)));
    SYMBOLICORE_TEST_ASSERT(not possibly(all.check(3_eff)));
    SYMBOLICORE_TEST_ASSERT(definitely(not all.check(4_eff)));

}

