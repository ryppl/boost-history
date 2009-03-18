/////////////////////////////////////////////////////////////////////////////
// parameter.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
// Disclaimer :
//
// This code is a minor modification to Boost.Accumulator and serves a
// particular purpose not readily provided by the original library.
// Contact the Boost library for copyright implications.
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHARED_FEATURES_PARAMETER_HPP_INCLUDED
#define BOOST_SHARED_FEATURES_PARAMETER_HPP_INCLUDED
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
namespace boost{namespace shared_features{

namespace tag{
    struct kwd_set;
    struct kwd_do_initialize;
}

namespace{
    static	::boost::parameter::keyword<tag::kwd_set>& kwd_set =
        ::boost::parameter::keyword<tag::kwd_set>::get();

    static	::boost::parameter::keyword<tag::kwd_do_initialize>&
        kwd_do_initialize =
        ::boost::parameter::keyword<tag::kwd_do_initialize>::get();
}

namespace detail{

    typedef parameter::parameters<
        parameter::required<tag::kwd_set>
    > params;

}

}
}

#endif
