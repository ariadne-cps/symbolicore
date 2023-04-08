/***************************************************************************
 *            sign.hpp
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

/*! \file sign.hpp
 *  \brief
 */


#ifndef SYMBOLICORE_SIGN_HPP
#define SYMBOLICORE_SIGN_HPP

#include <iosfwd>
#include "utility/macros.hpp"
#include "typedefs.hpp"

using namespace Utility;

namespace SymboliCore {

using OutputStream = std::ostream;

//! \brief The sign of a numerical value.
enum class Sign : ComparableEnumerationType { NEGATIVE=-1, ZERO=0, POSITIVE=+1 };
OutputStream& operator<<(OutputStream& os, Sign s);
inline Sign operator-(Sign s) { return Sign(-static_cast<ComparableEnumerationType>(s)); }
inline Sign operator*(Sign s1, Sign s2) { return Sign(static_cast<ComparableEnumerationType>(s1)*static_cast<ComparableEnumerationType>(s2)); }

//! \brief The result of a comparison operation.
enum class Comparison : ComparableEnumerationType { LESS=-1, EQUAL=0, GREATER=+1, INCOMPARABLE=ComparableEnumerationType(-128) };

inline OutputStream& operator<<(OutputStream& os, Comparison const& cmp) {
    switch(cmp) {
        case Comparison::LESS: os << "LESS";  break;
        case Comparison::EQUAL: os << "EQUAL";  break;
        case Comparison::GREATER: os << "GREATER";  break;
        case Comparison::INCOMPARABLE: os << "INCOMPARABLE"; break;
        default: UTILITY_FAIL_MSG("Unhandled Comparison for output streaming.");
    }
    return os;
}

static const Comparison LESS = Comparison::LESS;
static const Comparison EQUAL = Comparison::EQUAL;
static const Comparison GREATER = Comparison::GREATER;
static const Comparison INCOMPARABLE = Comparison::INCOMPARABLE;

} // namespace SymboliCore

#endif /* SYMBOLICORE_SIGN_HPP */
