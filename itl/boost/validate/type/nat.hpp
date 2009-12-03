/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <stdio.h>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/type_traits/value_size.hpp>

#define ITL_LOCATION(message) location(__FILE__,__LINE__,message)

namespace boost{namespace itl
{

class cnat
{
public:
    cnat(): _value(0) {}
    cnat(int val): _value(val<0 ? 0 : val){}
    cnat(const cnat& val): _value(val._value){}

    int value()const { return _value; }

    cnat& operator++(){ ++_value; return *this; }
    const cnat operator++(int){ cnat that = *this; ++_value; return that; }

    cnat& operator--(){ if(_value > 0)--_value; return *this; }
    const cnat operator--(int){ cnat that = *this; --_value; return that; }

    cnat& operator += (const cnat& right){ _value += right._value; return *this; }

    cnat& operator -= (const cnat& right)
    { 
        _value = right._value > _value ? 0 : (_value - right._value); 
        return *this; 
    }

    operator unsigned int()const{ return (_value); }

    std::string as_string()const { return to_string<int>::apply(_value); }

private:
    int _value;
};

//inline cnat operator + (const cnat& left, const cnat& right){return cnat(left)+=right;}
//inline cnat operator - (const cnat& left, const cnat& right){return cnat(left)-=right;}

inline bool operator == (const boost::itl::cnat& left, const boost::itl::cnat& right){ return left.value() == right.value(); }
inline bool operator <  (const boost::itl::cnat& left, const boost::itl::cnat& right){ return left.value() < right.value(); }

template<>inline std::string type_to_string<cnat>::apply() { return "cnat"; }

template<> inline std::size_t value_size<cnat>::apply(const cnat& value) 
{ return value; }

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits> &operator<<
(std::basic_ostream<CharType, CharTraits> &stream, itl::cnat const& right)
{
    return stream << right.value();
}

}} // namespace itl boost

