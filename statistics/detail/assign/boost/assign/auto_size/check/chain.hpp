//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_adapter.hpp                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CHAIN_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_CHAIN_ER_2010_HPP
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/auto_size/chain/chain_convert.hpp>
#include <boost/assign/auto_size/check/iterator.hpp>
#include <boost/assign/auto_size/check/copy_iterator.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp> // MUST be included

#ifndef BOOST_ASSIGN_CHECK_EQUAL
#error
#endif

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    // Usage
    // BOOST_AUTO(tmp,ref_list_of(a)(b)(c)(d));
    // check_chain(tmp,cref_list_of(e)(f)(g)(h),
    //   a,b,c,d,e,f,g,h);

    // TODO const C1.
    template<typename C,typename C1,typename T>
    void check_chain(C& coll,const C1& coll1,
        T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h
    )
    {   
            a = 1, b = 5, c = 3, 
            d = 4, e = 2, f = 9, 
            g = 0, h = 7;

        typedef boost::array<T,4> ar_;
        ar_ ar; 
        ar[0] = e; 
        ar[1] = f;
        ar[2] = g;
        ar[3] = h;
//        BOOST_AUTO(tmp1,coll.chain_convert(ar));
//        BOOST_AUTO(tmp2,coll.chain_convert(coll1));
//        check_iterator(tmp1,a,b,c,d,e,f,g,h);
//        check_iterator(tmp2,a,b,c,d,e,f,g,h);
//        check_copy_iterator(tmp1,a,b,c,d,e,f,g,h);
    }

}// auto_size
}// detail
}// assign
}// boost

#endif
