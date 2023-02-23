/***************************************************************************
 *            integer.cpp
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

#include <limits>

#include "stdlib.hpp"
#include "integer.hpp"
#include "macros.hpp"
#include "string.hpp"
#include "logical.hpp"

namespace SymboliCore {

Comparison cmp(Integer const& z1, Integer const& z2);

Integer::Integer() : _value(0) {}

Integer::Integer(String const& str) : _value(std::atoi(c_str(str))) { }

Integer::Integer(const Integer& z) : _value(z._value) { }

long int const& Integer::value() const { return _value; }

Integer& Integer::operator=(const Integer& z) {
    _value = z._value;
    return *this;
}

Integer& operator++(Integer& z) {
    z._value++;
    return z;
}

Integer& operator--(Integer& z) {
    z._value--;
    return z;
}

Integer& operator+=(Integer& z1, Integer const& z2) {
    z1._value += z2._value;
    return z1;
}

Integer& operator-=(Integer& z1, Integer const& z2) {
    z1._value -= z2._value;
    return z1;
}

Integer& operator*=(Integer& z1, Integer const& z2) {
    z1._value *= z2._value;
    return z1;
}

Integer nul(Integer const&) {
    return Integer(0);
}

Integer pos(Integer const& z) {
    return z;
}

Integer neg(Integer const& z) {
    return Integer(-z._value);
}

Natural sqr(Integer const& z) {
    return Natural(static_cast<unsigned long int>(z._value*z._value));
}

Integer add(Integer const& z1, Integer const& z2) {
    return Integer(z1._value+z2._value);
}

Integer sub(Integer const& z1, Integer const& z2) {
    return Integer(z1._value-z2._value);
}

Integer mul(Integer const& z1, Integer const& z2) {
    return Integer(z1._value*z2._value);
}

Integer quot(Integer const& z1, Integer const& z2) {
    return Integer(z1._value/z2._value);
}

Integer rem(Integer const& z1, Integer const& z2) {
    return Integer(z1._value % z2._value);
}

Integer pow(Integer const& z, Nat m) {
    unsigned long int lm=m;
    return Integer(static_cast<int>(std::pow(z._value,lm)));
}

Integer min(Integer const& z1,Integer const& z2) {
    return (z1<z2)?z1:z2;
}

Integer max(Integer const& z1,Integer const& z2) {
    return (z1>z2)?z1:z2;
}

Natural abs(Integer const& z) {
    return Natural(static_cast<unsigned long int>(std::abs(z._value)));
}

Natural max(Natural const& z1,Natural const& z2) {
    return (z1>z2)?z1:z2;
}

Natural min(Natural const& z1,Natural const& z2) {
    return (z1<z2)?z1:z2;
}

Bool is_nan(Integer const&) {
    return false;
}

Bool is_inf(Integer const&) {
    return false;
}

Bool is_finite(Integer const&) {
    return true;
}

Bool is_zero(Integer const& z) {
    return z._value==0;
}

Sign sgn(Integer const& z) {
    if (z._value > 0) return Sign::POSITIVE;
    else if (z._value < 0) return Sign::NEGATIVE;
    else return Sign::ZERO;
}

Comparison cmp(Integer const& z1, Integer const& z2) {
    auto c=z1._value-z2._value;
    return c==0 ? Comparison::EQUAL : (c>0?Comparison::GREATER:Comparison::LESS);
}

Comparison cmp(Integer const& z1, Int const& n2) {
    return cmp(z1,Integer(n2));
}

Boolean eq(Integer const& z1, Integer const& z2) {
    return z1._value==z2._value;
}

Boolean lt(Integer const& z1, Integer const& z2) {
    return z1._value < z2._value;
}

String Integer::literal() const {
    return to_string(_value);
}

OutputStream& operator<<(OutputStream& os, Integer const& z) {
    return os << z.literal();
}

template<> String class_name<Integer>() { return "Integer"; }

template<> String class_name<Natural>() { return "Natural"; }

OutputStream& operator<<(OutputStream& os, Sign s) {
    return os << ( (s==Sign::ZERO) ? "ZERO" : (s==Sign::NEGATIVE) ? "NEGATIVE" : "POSITIVE" );
}


} // namespace SymboliCore
