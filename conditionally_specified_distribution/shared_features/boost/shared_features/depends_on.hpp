/////////////////////////////////////////////////////////////////////////////
// depends_on.hpp
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
#ifndef BOOST_SHARED_FEATURES_DEPENDS_ON_HPP_ER_2009
#define BOOST_SHARED_FEATURES_DEPENDS_ON_HPP_ER_2009
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
namespace boost{namespace shared_features
{
    template<typename Feature>
    struct as_feature{
        typedef Feature type;
    };

    template<typename Feature>
    struct feature_of{
        typedef Feature type;
    };

namespace detail
{
    template<typename A,typename B>
    struct is_dependent_on : is_base_and_derived<B,A>{};

    template<typename Features>
    struct depends_on_base
        : mpl::inherit_linearly<
            typename mpl::sort<
                Features,is_dependent_on<mpl::_1,mpl::_2> >::type,
            //don't inherit multiply from a feature
            mpl::if_<
                  is_dependent_on<mpl::_1,mpl::_2>
                , mpl::_1
                , mpl::inherit<mpl::_1,mpl::_2>
            >
        >::type
    {};
}

    // Usage: struct X : depends_on<A,B,C>{};
    template<typename Features>
    struct depends_on : detail::depends_on_base<
            typename mpl::transform<Features, as_feature<mpl::_1>
        >::type
    >
    {
        typedef typename
            mpl::transform<Features, as_feature<mpl::_1> >::type
                                                        dependencies;
    };

    struct depends_on0 : depends_on<mpl::vector0<> >
    {};

    template<typename Feature>
    struct depends_on1 : depends_on<mpl::vector1<Feature> >
    {};

    template<typename F1,typename F2>
    struct depends_on2 : depends_on<mpl::vector2<F1,F2> >
    {};

}
}
#endif
