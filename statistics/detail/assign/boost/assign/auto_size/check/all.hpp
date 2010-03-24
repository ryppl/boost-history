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
#include <boost/preprocessor/cat.hpp>
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <boost/array.hpp>

#include <boost/assign/auto_size/check/constants.hpp>

#include <boost/assign/auto_size/check/array.hpp>
#include <boost/assign/auto_size/check/comparison_op.hpp>
#include <boost/assign/auto_size/check/converter.hpp>
#include <boost/assign/auto_size/check/copy_array.hpp>
#include <boost/assign/auto_size/check/copy_iterator.hpp>
#include <boost/assign/auto_size/check/example1.hpp>
#include <boost/assign/auto_size/check/fifo.hpp>
#include <boost/assign/auto_size/check/iterator.hpp>
#include <boost/assign/auto_size/check/lifo.hpp>
#include <boost/assign/auto_size/check/rebind_array.hpp>
#include <boost/assign/auto_size/check/example2.hpp>
#include <boost/assign/auto_size/check/chain_convert_r.hpp>
#include <boost/assign/auto_size/check/chain_convert_l.hpp>

#define BOOST_ASSIGN_AS_CHECK_all(fun)                                         \
namespace boost{                                                               \
namespace assign{                                                              \
namespace detail{                                                              \
namespace auto_size{                                                           \
                                                                               \
template<typename T>                                                           \
void fun(){                                                                    \
    BOOST_ASSIGN_AS_CHECK_example1                                             \
    BOOST_ASSIGN_AS_CHECK_iterator                                             \
    BOOST_ASSIGN_AS_CHECK_array                                                \
    BOOST_ASSIGN_AS_CHECK_copy_iterator                                        \
    BOOST_ASSIGN_AS_CHECK_copy_array                                           \
    BOOST_ASSIGN_AS_CHECK_rebind_array                                         \
    BOOST_ASSIGN_AS_CHECK_comparison_op                                        \
    BOOST_ASSIGN_AS_CHECK_converter(std::list<T>)                              \
    BOOST_ASSIGN_AS_CHECK_converter(std::vector<T>)                            \
    BOOST_ASSIGN_AS_CHECK_adapter_lifo(std::stack<T>)                          \
    BOOST_ASSIGN_AS_CHECK_adapter_fifo(std::queue<T>)                          \
    typedef boost::array<T,8> ar8_;                                            \
    BOOST_ASSIGN_AS_CHECK_converter(ar8_)                                      \
    BOOST_ASSIGN_AS_CHECK_converter_sorted(std::set<T>)                        \
    BOOST_ASSIGN_AS_CHECK_chain_convert_r                                      \
    BOOST_ASSIGN_AS_CHECK_chain_convert_l                                      \
    BOOST_ASSIGN_AS_CHECK_example2                                             \
}                                                                              \
void BOOST_PP_CAT(fun,_int)(){ fun<int>(); }                                   \
}                                                                              \
}                                                                              \
}                                                                              \
}                                                                              \
/**/

#endif
