//////////////////////////////////////////////////////////////////////////////
// cluster_call_center.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CLUSTER_CALL_CENTER_HPP_ER_2009
#define BOOST_MATH_IFGT_CLUSTER_CALL_CENTER_HPP_ER_2009
namespace boost{namespace math{namespace ifgt{
    template<typename Cc>
    struct cluster_call_center{
        typedef typename Cc::center_type center_type;
        typedef const center_type&  result_type;
        typedef const Cc&           argument_type;
        cluster_call_center(){}
        result_type operator()(argument_type arg)const{
            return arg.center();
        }
    };
}}}
#endif
