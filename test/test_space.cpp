/***************************************************************************
 *            test_space.cpp
 *
 *  Copyright  2023  Luca Geretti
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

#include <iostream>

#include "container.hpp"
#include "stlio.hpp"
#include "real.hpp"
#include "space.hpp"

#include "test.hpp"

using namespace SymboliCore;

class TestSpace {
  public:

    void test_construction() {
        {
            Space<Boolean> spc;
            SYMBOLICORE_TEST_EQUAL(spc.dimension(),0);
        }
        {
            RealVariable x("x"), y("y");
            Space<Real> spc({x,y});
            SYMBOLICORE_TEST_EQUAL(spc.dimension(),2);
        }
    }



    void test() {
        SYMBOLICORE_TEST_CALL(test_construction());
    }
};

Int main() {
    TestSpace().test();
    return SYMBOLICORE_TEST_FAILURES;
}
