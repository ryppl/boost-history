
// + file: libs/mpl/test/transform.cpp
// + last modified: 10/jun/03

// Copyright (c) 2000-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/transform.hpp"
#include "boost/mpl/list.hpp"
#include "boost/mpl/front_inserter.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/type_traits/add_pointer.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/static_assert.hpp"

using namespace boost;
using namespace mpl;

int main()
{
    typedef list<char,short,int,long,float,double> types;
    typedef list<char*,short*,int*,long*,float*,double*> pointers;

    typedef transform< types,add_pointer<_1> >::type result;
    typedef transform_backward< types,add_pointer<_1>,front_inserter< list0<> >  >::type result2;
    
    BOOST_STATIC_ASSERT(( equal<result,pointers>::type::value ));
    BOOST_STATIC_ASSERT(( equal<result2,pointers>::type::value ));
    
    return 0;
}
