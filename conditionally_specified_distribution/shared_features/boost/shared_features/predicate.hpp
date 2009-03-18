/////////////////////////////////////////////////////////////////////////////
// predicate.hpp
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
#ifndef BOOST_SHARED_FEATURES_PREDICATE_HPP_ER_2009
#define BOOST_SHARED_FEATURES_PREDICATE_HPP_ER_2009
#include <boost/mpl/contains.hpp>
#include <boost/shared_features/depends_on.hpp>
namespace boost{namespace shared_features{
    namespace detail{
        template<typename W>
        struct feature_tag{
            typedef typename W::feature_tag type;
        };
    }


    template<typename Feature>
    struct matches_feature{

        template<typename Wrapper>
        struct apply
        : is_same<
            typename feature_of<typename as_feature<Feature>::type>::type,
            typename feature_of<typename as_feature<typename
                detail::feature_tag<Wrapper>::type>::type>::type
        >
        {};
    };

    template<typename Features,typename Wrapper>
    struct contains_feature_of{

        typedef mpl::transform_view<
            Features,
            feature_of<as_feature<mpl::_> >
        > features_list;

        typedef typename
            feature_of<typename detail::feature_tag<Wrapper>::type>::type
        the_feature;

        typedef
            typename mpl::contains<features_list, the_feature>::type
        type;

    };


}
}

#endif // BOOST_SHARED_FEATURES_PREDICATE_HPP_ER_2009
