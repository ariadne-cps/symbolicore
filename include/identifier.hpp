/***************************************************************************
 *            identifier.hpp
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

/*! \file identifier.hpp
 *  \brief Strings used as names for constants and variables.
 */

#ifndef SYMBOLICORE_IDENTIFIER_HPP
#define SYMBOLICORE_IDENTIFIER_HPP

namespace SymboliCore {

//! \brief A class representing the name of a constant or variable.
//! \details A proxy for a standard string; used to distinguish a string used as a variable name from a value.
//! \sa Constant, Variable, String
class Identifier : public std::string
{
  public:
    Identifier() : std::string() { }
    Identifier(const char* cstr) : std::string(cstr) { }
    //! \brief Construct an identifier from a standard string.
    Identifier(const std::string& str) : std::string(str) { }
};

} // namespace SymboliCore

#endif /* SYMBOLICORE_IDENTIFIER_HPP */
