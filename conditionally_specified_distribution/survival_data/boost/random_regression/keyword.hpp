///////////////////////////////////////////////////////////////////////////////
//  keyword.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_RANDOM_KEYWORD_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_RANDOM_KEYWORD_HPP_ER_2009
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>

namespace boost{
namespace random_regression{

namespace tag{
    template<typename Id> struct coefficients_{};
}

namespace{
    template<typename Id>
    struct kwd{
        static ::boost::parameter::keyword<
            tag::coefficients_<Id> >& coefficients;
    };
    template<typename Id>
    ::boost::parameter::keyword<tag::coefficients_<Id> >&
            kwd<Id>::coefficients =
            ::boost::parameter::keyword<tag::coefficients_<Id> >::get();
}

}//random_regression
}//boost

#endif
