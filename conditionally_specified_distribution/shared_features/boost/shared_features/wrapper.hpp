/////////////////////////////////////////////////////////////////////////////
// wrapper.hpp
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
#ifndef BOOST_SHARED_FEATURES_WRAPPER_HPP_ER_2009
#define BOOST_SHARED_FEATURES_WRAPPER_HPP_ER_2009
#include <boost/mpl/apply.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/shared_features/mpl_features.hpp>
namespace boost{namespace shared_features{
namespace detail{



template<typename Impl,typename Feature>
struct wrapper : Impl{
    typedef Feature feature_tag;
    wrapper(wrapper const &that)
    : Impl(*static_cast<Impl const *>(&that))
    {}

    template<typename Args>
    wrapper(Args const & args)
    : Impl(args)
    {}
};


template<typename Feature>
struct to_wrapper{
    typedef wrapper<
        typename Feature::impl,
        Feature
    > type;
};


template<typename Features>
struct wrappers{
    typedef features<Features> features;
    typedef typename features::sorted_vector sorted_vector;

    typedef typename mpl::transform<sorted_vector,
        to_wrapper<mpl::_1>
    >::type type;
};

}
}
}


#endif // WRAPPER_HPP_INCLUDED
