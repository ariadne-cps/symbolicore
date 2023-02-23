/***************************************************************************
 *            test_paradigm.cpp
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
#include "paradigm.hpp"

#include "test.hpp"

using namespace SymboliCore;

class TestParadigm
{
  public:
    Void test();
  private:
    Void test_concept();
};

int main() {
    SYMBOLICORE_TEST_CLASS(TestParadigm,TestParadigm());
    return SYMBOLICORE_TEST_FAILURES;
}


Void
TestParadigm::test()
{
}

// Test that the type implements all operations of
// the FloatDP concept without testing correctness
Void
TestParadigm::test_concept()
{
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ApproximateTag,ApproximateTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ApproximateTag,ValidatedTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ApproximateTag,EffectiveTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ApproximateTag,ExactTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ValidatedTag,ValidatedTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ValidatedTag,EffectiveTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ValidatedTag,ExactTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<EffectiveTag,EffectiveTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<EffectiveTag,ExactTag>);
    SYMBOLICORE_TEST_CONCEPT(WeakerThan<ExactTag,ExactTag>);

    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<ValidatedTag,ApproximateTag>);
    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<EffectiveTag,ApproximateTag>);
    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<EffectiveTag,ValidatedTag>);
    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<ExactTag,ApproximateTag>);
    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<ExactTag,ValidatedTag>);
    SYMBOLICORE_TEST_CONCEPT(not WeakerThan<ExactTag,EffectiveTag>);
}

