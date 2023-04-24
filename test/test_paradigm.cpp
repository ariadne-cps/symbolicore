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

#include "helper/test.hpp"
#include "paradigm.hpp"

using namespace SymboliCore;

class TestParadigm
{
  public:
    void test();
  private:
    void test_concept();
};

int main() {
    HELPER_TEST_CLASS(TestParadigm,TestParadigm());
    return HELPER_TEST_FAILURES;
}


void
TestParadigm::test()
{
}

// Test that the type implements all operations of
// the FloatDP concept without testing correctness
void
TestParadigm::test_concept()
{
    HELPER_TEST_CONCEPT(WeakerThan<ApproximateTag,ApproximateTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ApproximateTag,ValidatedTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ApproximateTag,EffectiveTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ApproximateTag,ExactTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ValidatedTag,ValidatedTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ValidatedTag,EffectiveTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ValidatedTag,ExactTag>);
    HELPER_TEST_CONCEPT(WeakerThan<EffectiveTag,EffectiveTag>);
    HELPER_TEST_CONCEPT(WeakerThan<EffectiveTag,ExactTag>);
    HELPER_TEST_CONCEPT(WeakerThan<ExactTag,ExactTag>);

    HELPER_TEST_CONCEPT(not WeakerThan<ValidatedTag,ApproximateTag>);
    HELPER_TEST_CONCEPT(not WeakerThan<EffectiveTag,ApproximateTag>);
    HELPER_TEST_CONCEPT(not WeakerThan<EffectiveTag,ValidatedTag>);
    HELPER_TEST_CONCEPT(not WeakerThan<ExactTag,ApproximateTag>);
    HELPER_TEST_CONCEPT(not WeakerThan<ExactTag,ValidatedTag>);
    HELPER_TEST_CONCEPT(not WeakerThan<ExactTag,EffectiveTag>);
}

