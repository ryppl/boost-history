////////////////////////////////////////////////////////////////////////////
// keyword.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SHARED_FEATURES_KEYWORD_HPP_ER_2009
#define BOOST_SHARED_FEATURES_KEYWORD_HPP_ER_2009
#include <boost/mpl/void.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>

namespace boost{
namespace shared_features{

namespace tag{
    template<typename Id> struct scalar_{};
    template<typename Id> struct container_{};
    template<typename Id> struct position_{};
}
namespace {
    template<typename Id = mpl::void_>
    class kwd{
        typedef tag::scalar_<Id>     scalar_t;
        typedef tag::container_<Id>  container_t;
        typedef tag::position_<Id>   position_t;
        public:
        typedef Id id_type;
        static ::boost::parameter::keyword<scalar_t>&    scalar;
        static ::boost::parameter::keyword<container_t>& container;
        static ::boost::parameter::keyword<position_t>&  position;
    };

    template<typename Id>
		::boost::parameter::keyword<tag::scalar_<Id> >&
            kwd<Id>::scalar =
            ::boost::parameter::keyword<tag::scalar_<Id> >::get();
    template<typename Id>
		::boost::parameter::keyword<tag::container_<Id> >&
            kwd<Id>::container =
            ::boost::parameter::keyword<tag::container_<Id> >::get();
    template<typename Id>
		::boost::parameter::keyword<tag::position_<Id> >&
            kwd<Id>::position =
            ::boost::parameter::keyword<tag::position_<Id> >::get();
}//anonymous
}//state
}//boost


#endif // TAG_HPP_INCLUDED
