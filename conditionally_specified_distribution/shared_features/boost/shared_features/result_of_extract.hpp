/////////////////////////////////////////////////////////////////////////////
// result_of_extract.hpp
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
#ifndef BOOST_SHARED_FEATURES_RESULT_OF_EXTRACT_HPP_ER_2009
#define BOOST_SHARED_FEATURES_RESULT_OF_EXTRACT_HPP_ER_2009
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits.hpp>
#include <boost/parameter/parameters.hpp>
//#include <boost/parameter/keyword.hpp>
#include <boost/parameter/binding.hpp>
#include <boost/parameter/value_type.hpp>
#include <boost/utility/remove_qualifier.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/set.hpp>
namespace boost{
namespace shared_features{


    template<typename Args>
    struct args_to_set
    : boost::parameter::value_type<//binding
        typename utility::remove_qualifier<Args>::type,
        boost::shared_features::tag::kwd_set
    >
    {
    };


    template<typename Set,typename T>
    struct result_of_extract_given_set{
        typedef typename Set::template result_of_extract<T>::type type;
    };

    template<typename Args,typename T>
    struct result_of_extract
    : result_of_extract_given_set<
        typename args_to_set<Args>::type,
        T
    >
    {
    };

}
}
#endif // RESULT_OF_EXTRACT_HPP_INCLUDED
