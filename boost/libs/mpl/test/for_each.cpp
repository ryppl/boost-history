//-----------------------------------------------------------------------------
// boost mpl/test/for_each.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/list.hpp"
#include "boost/mpl/range_c.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/bind.hpp"

#include <vector>
#include <iostream>
#include <typeinfo>
#include <cassert>

namespace mpl = boost::mpl;
using mpl::_;

struct printer
{
    printer(std::ostream& s) : f_stream(&s) {}
    template< typename U > void operator()(mpl::identity<U>)
    {
        *f_stream << typeid(U).name() << '\n';
    }

 private:
    std::ostream* f_stream;
};

#ifdef __ICL
# pragma warning(disable:985)
#endif

int main()
{
    typedef mpl::list<char,short,int,long,float,double> types;
    mpl::for_each< types,mpl::make_identity<_> >(printer(std::cout));

    typedef mpl::range_c<int,0,10> numbers;
    std::vector<int> v;
    mpl::for_each<numbers,mpl::_>(
          boost::bind(&std::vector<int>::push_back, &v, _1)
        );
    
    for (int i = 0; i < v.size(); ++i)
        assert(v[i] == i);

    return 0;
}
