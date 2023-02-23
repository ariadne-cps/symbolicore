/***************************************************************************
 *            real.hpp
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

/*! \file real.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_REAL_HPP
#define SYMBOLICORE_REAL_HPP


#include "typedefs.hpp"
#include "metaprogramming.hpp"
#include "sign.hpp"
#include "logical.hpp"
//#include "vector.hpp"

namespace SymboliCore {

class Real
{
  private:
    double _value;
  public:
    typedef ApproximateTag Paradigm; //!< <p/>
  public:
    Real(); //!< Default constructor yielding \c 0.0
    explicit Real(String const&); //!< Construct from a string literal
    Real(double val); //!< Construct from raw double
    Real(const Real&); //!< Copy constructor
    Real& operator=(const Real&); //!< %Assignment
    String literal() const; //!< A string literal
    double const& value() const; //!< Builtin value

    //!@{
    //! \name Arithmetic operators
    friend Real operator+(Real const& r) { return pos(r); } //!< <p/>
    friend Real operator-(Real const& r) { return neg(r); } //!< <p/>
    friend Real operator+(Real const& r1, Real const& r2) { return add(r1,r2); } //!< <p/>
    friend Real operator-(Real const& r1, Real const& r2) { return sub(r1,r2); } //!< <p/>
    friend Real operator*(Real const& r1, Real const& r2) { return mul(r1,r2); } //!< <p/>
    friend Real operator/(Real const& r1, Real const& r2) { return div(r1,r2); }; //!< <p/>
    friend Real& operator+=(Real& r1, Real const& r2); //!< <p/>
    friend Real& operator-=(Real& r1, Real const& r2); //!< <p/>
    friend Real& operator*=(Real& r1, Real const& r2); //!< <p/>
    //!@}

    //!@{
    //! \name Comparison operators
    friend Boolean operator==(Real const& r1, Real const& r2) { return eq(r1,r2); } //!< <p/>
    friend Boolean operator!=(Real const& r1, Real const& r2) { return !eq(r1,r2); } //!< <p/>
    friend Boolean operator<=(Real const& r1, Real const& r2) { return !lt(r2,r1); } //!< <p/>
    friend Boolean operator>=(Real const& r1, Real const& r2) { return !lt(r1,r2); } //!< <p/>
    friend Boolean operator< (Real const& r1, Real const& r2) { return lt(r1,r2); } //!< <p/>
    friend Boolean operator> (Real const& r1, Real const& r2) { return lt(r2,r1); } //!< <p/>
    //!@}

    //!@{
    //! \name Arithmetic operations

    friend Real nul(Real const& r); //!< zero \a 0.
    friend Real pos(Real const& r); //!< Identity \a +r.
    friend Real neg(Real const& r); //!< Negative \a -r.
    friend Real hlf(Real const& r); //!< Half value \a r/2.
    friend Real sqr(Real const& r); //!< Square \a r<sup>2</sup>.
    friend Real add(Real const& r1, Real const& r2); //!< \brief Sum \a r1+r2.
    friend Real sub(Real const& r1, Real const& r2); //!< \brief Difference \a r1-r2.
    friend Real mul(Real const& r1, Real const& r2); //!< \brief Product \a r1×r2.
    friend Real div(Real const& r1, Real const& r2); //!< \brief Rounded quotient \a r1÷r2.
    friend Real rec(Real const& r); //!< Reciprocal \a 1/r.
    friend Real fma(Real const& r1, Real const& r2, Real const& r3); //!< \brief Fused multiply-and-add \a r1×r2+r3.
    friend Real pow(Real const& r, Nat m); //!< \brief Power \a r<sup>m</sup>.
    friend Real pow(Real const& r, Int n); //!< \brief Power \a r<sup>n</sup>.
    //!@}


    //!@{
    //! \name Algebraic and transcendental operations
    friend Real sqrt(Real const& r);
    friend Real exp(Real const& r);
    friend Real log(Real const& r);
    friend Real sin(Real const& r);
    friend Real cos(Real const& r);
    friend Real tan(Real const& r);
    friend Real asin(Real const& r);
    friend Real acos(Real const& r);
    friend Real atan(Real const& r);
    //!@}

    //!@{
    //! \name Lattice operations
    friend Real abs(Real const& r); //!< Absolute value \a |r|.
    friend Real min(Real const& r1, Real const& r2); //!< Minimum \a r1∧r2.
    friend Real max(Real const& r1, Real const& r2); //!< Maximum \a r1∨r2.
    //!@}

    //!@{
    //! \name Comparison operations
    friend Sign sgn(Real const& r); //!< The sign of \a r.
    friend Comparison cmp(Real const& w1, Real const& w2); //!< Compares which of \a w1 and \a w2 is larger.
    friend Boolean eq(Real const& w1, Real const& w2); //!< Tests if \a w1 is equal to \a w2.
    friend Boolean lt(Real const& w1, Real const& w2); //!< Tests if \a w1 is less than \a w2.
    //!@}

    //!@{
    //! \name Special value tests
    friend Bool is_nan(Real const& r); //!< Tests whether \a r is NaN (not-a-number).
    friend Bool is_inf(Real const& r); //!< Tests whether \a r is ±∞.
    friend Bool is_finite(Real const& r); //!< Tests whether \a r is finite.
    friend Bool is_zero(Real const& r); //!< Tests whether \a r is zero.
    //!@}

    //!@{
    //! \name Input/output operations
    friend OutputStream& operator<<(OutputStream& os, Real const& r); //!< <p/>
    //!@}
};

} // namespace SymboliCore

#endif