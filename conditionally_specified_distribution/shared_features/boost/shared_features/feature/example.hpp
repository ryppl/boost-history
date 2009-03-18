////////////////////////////////////////////////////////////////////////////
// example.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_EXAMPLE_HPP_ER_2009
#define BOOST_SHARED_FEATURES_EXAMPLE_HPP_ER_2009
#include <boost/mpl/vector/vector10.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/depends_on.hpp>

namespace boost{namespace shared_features{namespace example{

    BOOST_PARAMETER_KEYWORD(tag,kwd_x_value)
    BOOST_PARAMETER_KEYWORD(tag,kwd_y_value) //actually, not used

namespace impl{
    template<typename RealType>
    struct X{

        template<typename Args>
        X(const Args& args): value_(args[kwd_x_value|(RealType)(0)]){}

        template<typename Args>
        void initialize(const Args& args){
            value_ = args[kwd_x_value];
        }

        RealType value_;
    };
    template<typename X>
    struct Y{
        typedef X x_t;
        typedef typename x_t::value_type value_type;

        template<typename Args>
        Y(const Args& args):value_(
            get_x(args[kwd_set]).value_
        ){}

        template<typename Set>
        typename Set::template result_of_extract<x_t>::type const&
        get_x(const Set& set)const{
            return set.template extract<x_t>();
        }

        value_type value_;
    };
}

namespace feature{
    template<typename RealType>
    struct X : depends_on<
        mpl::vector0<>
    >{
        typedef RealType            value_type;
        typedef impl::X<RealType>   impl;
    };
    template<typename X>
    struct Y : depends_on<
        mpl::vector1<X>
    >{
        typedef impl::Y<X> impl;
    };
}

}
}
}

#endif // BOOST_SHARED_FEATURES_EXAMPLE_HPP_ER_2009
