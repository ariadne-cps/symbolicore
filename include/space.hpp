/***************************************************************************
 *            space.hpp
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

/*! \file symbolic/space.hpp
 *  \brief Spaces formed by variables.
 */

#ifndef SYMBOLICORE_SPACE_HPP
#define SYMBOLICORE_SPACE_HPP

#include <cstdarg>
#include <iosfwd>
#include <iostream>

#include "helper/macros.hpp"
#include "helper/container.hpp"
#include "variable.hpp"

namespace SymboliCore {

using Helper::List;
using Helper::Map;
using Helper::Set;

template<class T> class Space;
template<class T> ostream& operator<<(ostream& os, const Space<T>& spc);

//! \brief A space defined as a list of real variables.
//! \relates Space
typedef Space<Real> RealSpace;

//! \brief A space defined as a list of named variables of type \a T.
//! Allows conversion between the indexed space \a T<sup>n</sup> and the space \a T<sup>V</sup> defined by named variables \a V.
//!
//! \details The main computational functionality is based around Euclidean space \f$\R^n\f$.
//! However, it is more convenient to define sets and functions symbolically in terms of named variables.
//! A Space<Real> provides an ordering of variables which allows conversion between the two kinds of representation.
//!
//! The main conversion allowed is between \ref Valuation of type \a T<sup>V</sup> for a set of \a n variables \a V,
//! and a \ref Vector or type \a T<sup>n</sup>.
//! It also allows conversion between sets defined in terms of named variables and those defined in terms of numbered indices,
//! such as \ref VariablesBox and \ref Box.
//! In general, the conversion operations are provided by the symbolic class.
//!
//! \b Example
//! \snippet tutorials/symbolic_usage.cpp Space_usage
//! \see Variable
template<class T> class Space
{
  public:
    typedef Variable<T> VariableType;
  public:
    //! \brief The trivial space \f$T^0\f$.
    Space();
    Space(const List<VariableType>& vl);
    Space(const List<Identifier>& vl);
    Space(const initializer_list<VariableType>& vl);

    bool operator==(const Space<T>& other) const;
    bool operator!=(const Space<T>& other) const;

    //! \brief The dimension of the space.
    size_t size() const;
    //! \brief The dimension of the space.
    size_t dimension() const;
    //! \brief The \a i<sup>th</sup> named variable.
    const VariableType operator[](size_t i) const;
    //! \brief The \a i<sup>th</sup> named variable.
    const VariableType variable(size_t i) const;

    //! \brief A list giving ordered variables.
    List<Identifier> variable_names() const;
    //! \brief A list giving ordered variables.
    List<VariableType> variables() const;
    //! \brief A map giving the index of a given variable.
    Map<Identifier,size_t> indices_from_names() const;
    //! \brief A map giving the index of a given variable.
    Map<VariableType,size_t> indices() const;

    //! \brief Tests if the variable \a v is in the space.
    bool contains(const VariableType& v) const;
    //! \brief Tests if all the variables \a vs is in the space.
    bool contains(const Set<VariableType>& vs) const;
    //! \brief The index of the named variable \a v.
    size_t operator[](const VariableType& v) const;
    size_t operator[](const Identifier& n) const;
    //! \brief The index of the named variable \a v.
    size_t index(const VariableType& v) const;
    size_t index(const Identifier& n) const;
    //! \brief Append the named variable \a v to the variables defining the space; ignores if the variable is already in the space.
    Space<T>& insert(const VariableType& v);
    //! \brief Adjoins the variables in \a spc.
    Space<T>& adjoin(const Space<T>& spc);
    //! \brief Append the named variable \a v to the variables defining the space.
    Space<T>& append(const VariableType& v);
  private:
    List<Identifier> _variables;
};

template<class T> ostream& operator<<(ostream& os, const Space<T>& spc) { return os << spc.variables(); }

template<class T> Space<T> join(const Space<T>& spc1, const Space<T>& spc2) {
    Space<T> r(spc1); r.adjoin(spc2); return r; }
template<class T> Space<T> join(const Space<T>& spc1, const Variable<T>& var2) {
    Space<T> r(spc1); r.append(var2); return r; }

size_t dimension(const Space<Real>& spc);
List<Identifier> variable_names(const Space<Real>& spc);
List<Identifier> variable_names(const List<Variable<Real>>& spc);
Space<Real> real_space(const List<Identifier>& vars);

template<class T> Variable<T> variable(const Identifier& s);
template<class T> Space<T> variables(const List<Identifier>& s);

} // namespace SymboliCore

#endif /* SYMBOLICORE_SPACE_HPP */
