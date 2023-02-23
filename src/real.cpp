/***************************************************************************
 *            real.cpp
 *
 *  Copyright  2023  Luca Geretti
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
#include "real.hpp"
#include "macros.hpp"
#include "string.hpp"
#include "logical.hpp"

namespace SymboliCore {

Comparison cmp(Real const& r1, Real const& r2);

Real::Real() : _value(0.0) { }

Real::Real(double val) : _value(val) { }

Real::Real(String const& str) {
    SizeType sz;
    _value = stod(str,&sz);
}

Real::Real(const Real& r) : _value(r._value) { }

double const& Real::value() const { return _value; }

Real& Real::operator=(const Real& r) {
    _value = r._value;
    return *this;
}

Real& operator+=(Real& r1, Real const& r2) {
    r1._value += r2._value;
    return r1;
}

Real& operator-=(Real& r1, Real const& r2) {
    r1._value -= r2._value;
    return r1;
}

Real& operator*=(Real& r1, Real const& r2) {
    r1._value *= r2._value;
    return r1;
}

Real nul(Real const&) {
    return Real(0.0);
}

Real pos(Real const& r) {
    return r;
}

Real neg(Real const& r) {
    return Real(-r._value);
}

Real hlf(Real const& r) {
    return Real(r._value/2);
}

Real sqr(Real const& r) {
    return r*r;
}

Real add(Real const& r1, Real const& r2) {
    return Real(r1._value+r2._value);
}

Real sub(Real const& r1, Real const& r2) {
    return Real(r1._value-r2._value);
}

Real mul(Real const& r1, Real const& r2) {
    return Real(r1._value*r2._value);
}

Real div(Real const& r1, Real const& r2) {
    return Real(r1._value/r2._value);
}

Real rec(Real const& r) {
    return Real(1.0/r._value);
}

Real pow(Real const& r, Nat m) {
    unsigned long int lm=m;
    return Real(std::pow(r._value,lm));
}

Real pow(Real const& r, Int n) {
    auto ln=static_cast<unsigned long int>(n);
    return Real(std::pow(r._value,ln));
}

Real min(Real const& r1,Real const& r2) {
    return Real(std::min(r1._value,r2._value));
}

Real max(Real const& r1,Real const& r2) {
    return Real(std::max(r1._value,r2._value));
}

Real abs(Real const& r) {
    return Real(std::abs(r._value));
}

Real sqrt(Real const& r) {
    return Real(std::sqrt(r._value));
}

Real exp(Real const& r) {
    return Real(std::exp(r._value));
}

Real log(Real const& r) {
    return Real(std::log(r._value));
}

Real sin(Real const& r) {
    return Real(std::sin(r._value));
}

Real cos(Real const& r) {
    return Real(std::cos(r._value));
}

Real tan(Real const& r) {
    return Real(std::tan(r._value));
}

Real asin(Real const& r) {
    return Real(std::asin(r._value));
}

Real acos(Real const& r) {
    return Real(std::acos(r._value));
}

Real atan(Real const& r) {
    return Real(std::atan(r._value));
}

Bool is_nan(Real const& r) {
    return std::isnan(r._value);
}

Bool is_inf(Real const& r) {
    return std::isinf(r._value);
}

Bool is_finite(Real const& r) {
    return not is_inf(r);
}

Bool is_zero(Real const& r) {
    return r._value==0;
}

Sign sgn(Real const& r) {
    if (r._value > 0) return Sign::POSITIVE;
    else if (r._value < 0) return Sign::NEGATIVE;
    else return Sign::ZERO;
}

Comparison cmp(Real const& r1, Real const& r2) {
    auto c=r1._value-r2._value;
    return c==0 ? Comparison::EQUAL : (c>0?Comparison::GREATER:Comparison::LESS);
}

Comparison cmp(Real const& r1, double const& d) {
    return cmp(r1,Real(d));
}

Boolean eq(Real const& r1, Real const& r2) {
    return r1._value==r2._value;
}

Boolean lt(Real const& r1, Real const& r2) {
    return r1._value < r2._value;
}

String Real::literal() const {
    return to_string(_value);
}

OutputStream& operator<<(OutputStream& os, Real const& r) {
    return os << r.literal();
}

template<> String class_name<Real>() { return "Real"; }

} // namespace SymboliCore
