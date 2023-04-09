/***************************************************************************
 *            constant.hpp
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

/*! \file constant.hpp
 *  \brief Named constants
 */

#ifndef SYMBOLICORE_CONSTANT_HPP
#define SYMBOLICORE_CONSTANT_HPP

#include "utility/string.hpp"
#include "using.hpp"
#include "identifier.hpp"

namespace SymboliCore {

//! \brief A named constant of type \a T.
//! \details
//! \par \b Example
//! \snippet tutorials/symbolic_usage.cpp Constant_usage
//! \see Variable, Expression
template<class T> class Constant
    : public T
{
  public:
    //! The constant with value \a value, displayed as the value itself.
    explicit Constant(const T& value) : T(value), _name("") { }
    //! The constant with name \a name and value \a value.
    explicit Constant(const Identifier& name, const T& value) : T(value), _name(name) { }
    //! The name of the constant.
    const Identifier& name() const { return _name; }
    //! The value of the constant.
    const T& value() const { return *this; }
    const T& val() const { return *this; }
    //! Write to an output stream.
    friend OutputStream& operator<<(OutputStream& os, Constant<T> const& c) {
        if (c.name().empty()) { return os << c.value(); } else { return os << c._name << "(=" << c.value() << ")"; } }
  private:
    Identifier _name;
};

template<> class Constant<String>
    : public String
{
  public:
    explicit Constant(const String& value) : String(value) { }
    const Identifier& name() const { return static_cast<const Identifier&>(static_cast<const std::string&>(*this)); }
    const String& value() const { return *this; }
    const String& val() const { return *this; }
};

} // namespace SymboliCore

#endif /* SYMBOLICORE_CONSTANT_HPP */
