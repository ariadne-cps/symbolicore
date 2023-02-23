/***************************************************************************
 *            space.cpp
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

/*! \file space.cpp
 *  \brief Spaces formed by variables.
 */

#include <cstdarg>
#include <iosfwd>
#include <iostream>

#include "macros.hpp"
#include "real.hpp"
#include "variable.hpp"
#include "space.hpp"

namespace SymboliCore {

template<class T> Space<T>::Space() : _variables() { }

template<class T> Space<T>::Space(const List<VariableType>& vl) {
    for(SizeType i=0; i!=vl.size(); ++i) {
        this->append(vl[i]);
    }
}

template<class T> Space<T>::Space(const List<Identifier>& vl) {
    for(SizeType i=0; i!=vl.size(); ++i) {
        this->append(VariableType(vl[i]));
    }
}

template<class T> Space<T>::Space(const InitializerList<VariableType>& vl) {
    for(SizeType i=0; i!=vl.size(); ++i) {
        this->append(vl.begin()[i]);
    }
}

template<class T> Bool Space<T>::operator==(const Space<T>& other) const {
    return this->_variables==other._variables; }

template<class T> Bool Space<T>::operator!=(const Space<T>& other) const {
    return !(*this == other); }

template<class T> SizeType Space<T>::size() const {
    return _variables.size(); }

template<class T> SizeType Space<T>::dimension() const {
    return _variables.size(); }

template<class T> const typename Space<T>::VariableType Space<T>::operator[](SizeType i) const {
    return VariableType(_variables.at(i)); }

template<class T> const typename Space<T>::VariableType Space<T>::variable(SizeType i) const {
    return VariableType(_variables.at(i)); }

template<class T> List<Identifier> Space<T>::variable_names() const {
    return this->_variables; }

template<class T> List<typename Space<T>::VariableType> Space<T>::variables() const {
    return List<VariableType>(this->_variables); }

template<class T> Map<Identifier,SizeType> Space<T>::indices_from_names() const {
    Map<Identifier,SizeType> indices;
    for(SizeType i=0; i!=this->_variables.size(); ++i) {
        SYMBOLICORE_ASSERT_MSG(!indices.has_key(_variables[i]),"Repeated variable "<<_variables[i]<<" in space "<<_variables)
            indices.insert(this->_variables[i],i);
    }
    return indices;
}
template<class T> Map<typename Space<T>::VariableType,SizeType> Space<T>::indices() const {
    Map<VariableType,SizeType> indices;
    for(SizeType i=0; i!=this->_variables.size(); ++i) {
        SYMBOLICORE_ASSERT_MSG(!indices.has_key(VariableType(_variables[i])),"Repeated variable "<<_variables[i]<<" in space "<<_variables)
        indices.insert(VariableType(this->_variables[i]),i);
    }
    return indices;
}

template<class T> Bool Space<T>::contains(const typename Space<T>::VariableType& v) const {
    for(SizeType i=0; i!=_variables.size(); ++i) {
        if(v.name()==_variables[i]) { return true; } }
    return false; }
template<class T> Bool Space<T>::contains(const Set<typename Space<T>::VariableType>& vs) const {
    for(auto v : vs) {
        if(!this->contains(v)) { return false; } }
    return true; }
template<class T> SizeType Space<T>::operator[](const typename Space<T>::VariableType& v) const {
    return this->index(v); }
template<class T> SizeType Space<T>::operator[](const Identifier& n) const {
    return this->index(n); }
template<class T> SizeType Space<T>::index(const typename Space<T>::VariableType& v) const {
    for(SizeType i=0; i!=_variables.size(); ++i) {
        if(v.name()==_variables[i]) { return i; } }
    SYMBOLICORE_ASSERT_MSG(false,"Variable "<<v<<" is not in the Space "<<*this);
    return _variables.size(); }
template<class T> SizeType Space<T>::index(const Identifier& n) const {
    for(SizeType i=0; i!=_variables.size(); ++i) {
        if(n==_variables[i]) { return i; } }
    SYMBOLICORE_ASSERT_MSG(false,"Variable named "<<n<<" is not in the Space "<<*this);
    return _variables.size(); }
template<class T> Space<T>& Space<T>::insert(const typename Space<T>::VariableType& v) {
    for(SizeType i=0; i!=_variables.size(); ++i) {
        if(_variables[i]==v.name()) { return *this; } }
    _variables.push_back(v.name()); return *this; }

template<class T> Space<T>& Space<T>::adjoin(const Space<T>& spc) {
    for(SizeType i=0; i!=spc._variables.size(); ++i) { this->insert(VariableType(spc._variables[i])); }
    return *this;
}

template<class T> Space<T>& Space<T>::append(const VariableType& v) {
    for(SizeType i=0; i!=_variables.size(); ++i) {
        SYMBOLICORE_ASSERT_MSG(_variables[i]!=v.name(),"Variable "<<v<<" is already a variable of the StateSpace "<<*this);
    }
    _variables.push_back(v.name()); return *this;
}

template class Space<Real>;

template<class T> Variable<T> variable(const Identifier& s) { return Variable<T>(s); }
template<class T> Space<T> variables(const List<Identifier>& s) { return Space<T>(s); }

SizeType dimension(const Space<Real>& spc)
{
    return spc.size();
}

Space<Real> real_space(const List<Identifier>& vars)
{
    return Space<Real>(vars);
}

List<Identifier> variable_names(const List<Variable<Real>>& vars)
{
    List<Identifier> nms; nms.reserve(vars.size()); for(SizeType i=0; i!=vars.size(); ++i) { nms.append(vars[i].name()); } return nms;
}

List<Identifier> variable_names(const Space<Real>& spc)
{
    return spc.variable_names();
}

template class Space<Boolean>;

} // namespace SymboliCore
