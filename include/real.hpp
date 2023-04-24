/***************************************************************************
 *            real.hpp
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

/*! \file real.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_REAL_HPP
#define SYMBOLICORE_REAL_HPP

#include "using.hpp"
#include "helper/metaprogramming.hpp"
#include "sign.hpp"
#include "logical.hpp"

namespace SymboliCore {

class Integer;

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
    Real(Integer const& i); //!< Costruct from Integer
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
    friend Real pow(Real const& r, unsigned int m); //!< \brief Power \a r<sup>m</sup>.
    friend Real pow(Real const& r, int n); //!< \brief Power \a r<sup>n</sup>.
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
    friend Comparison cmp(Real const& r1, Real const& r2); //!< Compares which of \a r1 and \a r2 is larger.
    friend Boolean eq(Real const& r1, Real const& r2); //!< Tests if \a r1 is equal to \a r2.
    friend Boolean lt(Real const& r1, Real const& r2); //!< Tests if \a r1 is less than \a r2.
    //!@}

    //!@{
    //! \name Special value tests
    friend bool is_nan(Real const& r); //!< Tests whether \a r is NaN (not-a-number).
    friend bool is_inf(Real const& r); //!< Tests whether \a r is ±∞.
    friend bool is_finite(Real const& r); //!< Tests whether \a r is finite.
    friend bool is_zero(Real const& r); //!< Tests whether \a r is zero.
    //!@}

    friend bool same(Real const& r1, Real const& r2); //!< Test equivalence of representation.

    //!@{
    //! \name Input/output operations
    friend ostream& operator<<(ostream& os, Real const& r); //!< <p/>
    //!@}
};

} // namespace SymboliCore

#endif
