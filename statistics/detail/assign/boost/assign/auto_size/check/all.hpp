//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::check_all.hpp                                 //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ALL_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_ALL_ER_2010_HPP
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <boost/preprocessor/cat.hpp>
#include <boost/array.hpp>
#include <boost/range/algorithm/max_element.hpp>

//#include <boost/assign/auto_size/chain/mpl_check.hpp>

#include <boost/assign/auto_size/check/array.hpp>
//#include <boost/assign/auto_size/check/chain.hpp>
#include <boost/assign/auto_size/check/converter.hpp>
#include <boost/assign/auto_size/check/copy_array.hpp>
#include <boost/assign/auto_size/check/copy_iterator.hpp>
//#include <boost/assign/auto_size/check/example1.hpp>
#include <boost/assign/auto_size/check/fifo.hpp>
#include <boost/assign/auto_size/check/iterator.hpp>
#include <boost/assign/auto_size/check/lifo.hpp>
#include <boost/assign/auto_size/check/rebind_array.hpp>
//#include <boost/assign/auto_size/check/ref_list_of_caller.hpp>

#define BOOST_ASSIGN_AS_CHECK_all(fun)                                         \
namespace boost{                                                               \
namespace assign{                                                              \
namespace detail{                                                              \
namespace auto_size{                                                           \
                                                                               \
template<typename T>                                                           \
void fun(){                                                                    \
    BOOST_ASSIGN_AS_CHECK_iterator                                             \
    BOOST_ASSIGN_AS_CHECK_array                                                \
    BOOST_ASSIGN_AS_CHECK_copy_iterator                                        \
    BOOST_ASSIGN_AS_CHECK_copy_array                                           \
    BOOST_ASSIGN_AS_CHECK_rebind_array                                         \
    BOOST_ASSIGN_AS_CHECK_converter(std::list<T>)                              \
    BOOST_ASSIGN_AS_CHECK_converter(std::vector<T>)                            \
    typedef boost::array<T,8> ar8_;                                            \
    BOOST_ASSIGN_AS_CHECK_converter(ar8_)                                      \
    BOOST_ASSIGN_AS_CHECK_converter_sorted(std::set<T>)                        \
    BOOST_ASSIGN_AS_CHECK_adapter_lifo(std::stack<T>)                          \
    BOOST_ASSIGN_AS_CHECK_adapter_fifo(std::queue<T>)                          \
}                                                                              \
void fun(){ fun<int>(); }                                                      \
}                                                                              \
}                                                                              \
}                                                                              \
}                                                                              \
/**/

/*
    boost::assign::detail::chain_mpl_check::compound();
    {	
        val_ a, b, c, d, e, f, g, h;
        BOOST_AUTO(tmp1,ref_list_of(a)(b)(c)(d));
        check_chain(
           tmp1,
           cref_list_of(e)(f)(g)(h),
           a,b,c,d,e,f,g,h);
    }
    // TODO comparison operators
}

*/


#endif


