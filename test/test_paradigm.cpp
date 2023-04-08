/***************************************************************************
 *            test_paradigm.cpp
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
#include "paradigm.hpp"

using namespace SymboliCore;

class TestParadigm
{
  public:
    Void test();
  private:
    Void test_concept();
};

int main() {
    UTILITY_TEST_CLASS(TestParadigm,TestParadigm());
    return UTILITY_TEST_FAILURES;
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
    UTILITY_TEST_CONCEPT(WeakerThan<ApproximateTag,ApproximateTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ApproximateTag,ValidatedTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ApproximateTag,EffectiveTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ApproximateTag,ExactTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ValidatedTag,ValidatedTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ValidatedTag,EffectiveTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ValidatedTag,ExactTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<EffectiveTag,EffectiveTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<EffectiveTag,ExactTag>);
    UTILITY_TEST_CONCEPT(WeakerThan<ExactTag,ExactTag>);

    UTILITY_TEST_CONCEPT(not WeakerThan<ValidatedTag,ApproximateTag>);
    UTILITY_TEST_CONCEPT(not WeakerThan<EffectiveTag,ApproximateTag>);
    UTILITY_TEST_CONCEPT(not WeakerThan<EffectiveTag,ValidatedTag>);
    UTILITY_TEST_CONCEPT(not WeakerThan<ExactTag,ApproximateTag>);
    UTILITY_TEST_CONCEPT(not WeakerThan<ExactTag,ValidatedTag>);
    UTILITY_TEST_CONCEPT(not WeakerThan<ExactTag,EffectiveTag>);
}

