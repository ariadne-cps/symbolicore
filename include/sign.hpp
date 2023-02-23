/***************************************************************************
 *            sign.hpp
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

/*! \file sign.hpp
 *  \brief
 */


#ifndef SYMBOLICORE_SIGN_HPP
#define SYMBOLICORE_SIGN_HPP

#include <iosfwd>
#include "macros.hpp"

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
        default: SYMBOLICORE_FAIL_MSG("Unhandled Comparison for output streaming.");
    }
    return os;
}

static const Comparison LESS = Comparison::LESS;
static const Comparison EQUAL = Comparison::EQUAL;
static const Comparison GREATER = Comparison::GREATER;
static const Comparison INCOMPARABLE = Comparison::INCOMPARABLE;

} // namespace SymboliCore

#endif /* SYMBOLICORE_SIGN_HPP */
