//////////////////////////////////////////////////////////////////////////////
// detail::fusion::serialization::map.hpp                             		//
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_SERIALIZATION_MAP_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_SERIALIZATION_MAP_HPP_ER_2009
#include <boost/fusion/include/make_map.hpp>
#include <boost/statistics/detail/fusion/serialization/save.hpp>
#include <boost/statistics/detail/fusion/serialization/load.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost{ namespace serialization{

	// A generic function restricted by enable_if<is_sequence<S>,void>
    // causes ambiguous compile error. So, apparently, each type of sequence and
    // each length within requires an overoad.

	// n = 1

    template<class Archive,typename K1,typename D1>
    void serialize(
        Archive & ar,
        typename boost::fusion::result_of::make_map<K1,D1>::type& t,
        const unsigned int file_version
    )
    {
        split_free(ar, t, file_version);
    }

	// n = 2

    template<class Archive,typename K1,typename K2,typename D1,typename D2>
    void serialize(
        Archive & ar,
        typename boost::fusion::result_of::make_map<K1,K2,D1,D2>::type& t,
        const unsigned int file_version
    )
    {
        split_free(ar, t, file_version);
    }

	// n = 3

    template<class Archive,
    	typename K1,typename K2,typename K3,
        typename D1,typename D2,typename D3
    >
    void serialize(
        Archive & ar,
        typename boost::fusion::result_of::make_map<K1,K2,K3,D1,D2,D3>::type& t,
        const unsigned int file_version
    )
    {
        split_free(ar, t, file_version);
    }

}}

#endif
