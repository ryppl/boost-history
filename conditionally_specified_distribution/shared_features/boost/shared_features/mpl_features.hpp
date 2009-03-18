/////////////////////////////////////////////////////////////////////////////
// mpl_features.hpp
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
#ifndef BOOST_SHARED_FEATURES_MPL_FEATURES_HPP_ER_2009
#define BOOST_SHARED_FEATURES_MPL_FEATURES_HPP_ER_2009
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/version.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/protect.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/shared_features/depends_on.hpp>
namespace boost{ namespace shared_features{
namespace detail{
    template<typename Features>
    struct as_feature_list
        : mpl::transform_view<Features,as_feature<mpl::_1> >
    {};

    template<typename Map,typename Feature>
    struct insert_feature
        : mpl::eval_if<
            mpl::has_key<Map, typename feature_of<Feature>::type>,
            mpl::identity<Map>,
            mpl::insert<
                Map,
                mpl::pair<
                    typename feature_of<Feature>::type,
                    Feature
                >
            >
        >
    {};

    template<typename Map,typename Feature>
    struct insert_dependencies
        : mpl::fold<//Recall : depends_on<A,B,C>::dependencies
            as_feature_list<typename Feature::dependencies>,
            Map,
            insert_dependencies<
                insert_feature<mpl::_1,mpl::_2>,
                mpl::_2
            >
        >
    {};

    template<typename Map,typename Features>
    struct insert_sequence
        : mpl::fold<
            as_feature_list<Features>,
            Map,
            insert_feature<mpl::_1,mpl::_2>
        >
    {};

    template<typename AskedFeatures>
    struct features{

        typedef typename mpl::fold<
            as_feature_list<AskedFeatures>,
            mpl::map0<>,
            mpl::if_<
                mpl::is_sequence<mpl::_2>,
                insert_sequence<mpl::_1,mpl::_2>,
                insert_feature<mpl::_1,mpl::_2>
            >
        >::type explicit_map;

        typedef typename mpl::fold<
            explicit_map,
            explicit_map,
            insert_dependencies<mpl::_1,mpl::second<mpl::_2> >
        >::type map;

        typedef typename mpl::insert_range<
            mpl::vector<>,
            mpl::end<mpl::vector<> >::type,
            mpl::transform_view<map,mpl::second<mpl::_1> >
        >::type vector;

        typedef typename mpl::sort<
            vector,
            is_dependent_on<mpl::_2,mpl::_1>
        >::type sorted_vector;

    };
}//detail
}//shared_features
}//boost

#endif // DEPENDENCIES_HPP_INCLUDED
