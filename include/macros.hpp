/***************************************************************************
 *            macros.hpp
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


/*! \file macros.hpp
 *  \brief
 */



#ifndef SYMBOLICORE_MACROS_HPP
#define SYMBOLICORE_MACROS_HPP

#include <sstream>
#include <stdexcept>

#include "typedefs.hpp"

#define SYMBOLICORE_USING_CONSTRUCTORS(Class,Base) \
    template<class T,typename std::enable_if<std::is_convertible<T,Base>::value,int>::type=0> \
    Class(const T& t) : Base(t) { } \
    template<class T,typename std::enable_if<std::is_constructible<T,Base>::value and not std::is_convertible<T,Base>::value,int>::type=0> \
    explicit Class(const T& t) : Base(t) { } \
    template<class ...Args> Class(Args&&... args) : Base(std::forward<Args>(args)...) { } \

#define SYMBOLICORE_THROW(except,func,msg)          \
    { \
        StringStream ss; \
        ss << #except " in " << func << ": " << msg;    \
        throw except(ss.str()); \
    } \

#define SYMBOLICORE_ASSERT(expression) \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<__FUNCTION__,"Assertion `" << #expression << "' failed."); \
        } \
    } \


#ifndef NDEBUG
#define SYMBOLICORE_DEBUG_ASSERT_MSG(expression,error) \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"Assertion `" << #expression << "' failed.\n"<<"  "<<error); \
        } \
    } \

#else
#define SYMBOLICORE_DEBUG_ASSERT_MSG(expression,error) \
    { }
#endif


#ifndef NDEBUG
#define SYMBOLICORE_DEBUG_ASSERT(expression) \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<__FUNCTION__,"Assertion `" << #expression << "' failed."); \
        } \
    } \

#else
#define SYMBOLICORE_DEBUG_ASSERT(expression) \
    { }
#endif


#define SYMBOLICORE_PRECONDITION_MSG(expression,error)             \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"Precondition `" << #expression << "' failed.\n"<<"  "<<error); \
        } \
    } \

#define SYMBOLICORE_PRECONDITION(expression)             \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"Precondition `" << #expression << "' failed."); \
        } \
    } \

#ifndef NDEBUG
#define SYMBOLICORE_DEBUG_PRECONDITION(expression) \
    { \
        bool result = static_cast<bool>(expression); \
        if(!result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<__FUNCTION__,"Precondition `" << #expression << "' failed."); \
        } \
    } \

#else
#define SYMBOLICORE_DEBUG_PRECONDITION(expression) \
    { }
#endif

#define SYMBOLICORE_FAIL_MSG(error)             \
    { \
        SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"ErrorTag "<<error); \
    } \

#define SYMBOLICORE_ASSERT_MSG(expression,error)             \
    { \
        bool assertion_result = static_cast<bool>(expression); \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"Assertion `" << #expression << "' failed.\n"<<"  "<<error); \
        } \
    } \

#define SYMBOLICORE_ASSERT_EQUAL(expression1,expression2)    \
    { \
        bool assertion_result = static_cast<bool>((expression1) == (expression2));       \
        if(!assertion_result) { \
            SYMBOLICORE_THROW(std::runtime_error,__FILE__<<":"<<__LINE__<<": "<<SYMBOLICORE_PRETTY_FUNCTION,"Assertion `" << #expression1 << "==" << #expression2 << "' failed.\n"<<"  "<<expression1<<" != "<<expression2); \
        } \
    } \

#define SYMBOLICORE_NOT_IMPLEMENTED                 \
    throw std::runtime_error(StringType("Not implemented: ")+SYMBOLICORE_PRETTY_FUNCTION);

#define SYMBOLICORE_DEPRECATED(fn,msg)          \
    static bool first_time=true; \
    if(first_time) { \
        first_time=false; \
        std::cerr << "DEPRECATED: Function " << #fn << " is deprecated. " << #msg << std::endl; \
    } \

#define SYMBOLICORE_NOTIFY(msg)          \
    {                                                                \
        std::cerr << "NOTIFICATION: " << msg << "" << std::endl;                \
    }

#define SYMBOLICORE_WARN(msg)          \
    {                                                                \
        std::cerr << "WARNING: " << msg << "" << std::endl;                \
    }

#define SYMBOLICORE_WARN_ONCE(msg)          \
    static bool first_time=true; \
    if(first_time) { \
        first_time=false; \
        std::cerr << "WARNING: " << msg << "" << std::endl; \
    } \

#define SYMBOLICORE_ERROR(msg)          \
    {                                                                \
        std::cerr << "ERROR: " << msg << "" << std::endl;                \
    }
                                                                  \
#if defined(linux) || defined(__linux) || defined(__linux__)
#define SYMBOLICORE_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define SYMBOLICORE_PRETTY_FUNCTION __FUNCTION__
#elif defined(darwin) || defined(__darwin) || defined(__darwin__) || defined(__APPLE__)
#define SYMBOLICORE_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#define SYMBOLICORE_PRETTY_FUNCTION ""
#endif


#endif // SYMBOLICORE_MACROS_HPP
