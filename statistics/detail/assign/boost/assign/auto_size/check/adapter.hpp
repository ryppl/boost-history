//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_adapter.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ADAPTER_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ADAPTER_ER_2010_HPP
#include <stack>
#include <queue>

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    template<typename C1,typename T>
    void check_adapter(std::queue<T>& elems,const C1& coll,
        const T& a,const T& b,const T& c,const T& d,const T& e,const T& f,
        const T& g,const T& h
    )
    {
        check_fifo(elems,coll,a,b,c,d,e,f,g,h);
    }

    template<typename C1,typename T>
    void check_adapter(std::stack<T>& elems,const C1& coll,
        const T& a,const T& b,const T& c,const T& d,const T& e,const T& f,
        const T& g,const T& h
    )
    {
        check_lifo(elems,coll,a,b,c,d,e,f,g,h);
    }

    // Overload as needed

}// auto_size
}// detail
}// assign
}// boost

#endif
