///////////////////////////////////////////////////////////////////////////////
//  keyword.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_KEYWORD_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_KEYWORD_HPP_ER_2009
#include <boost/mpl/void.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>

namespace boost{
namespace survival_data{
namespace tag{
    template<typename Id> struct covariate_{};
    template<typename Id> struct entry_time_{};
    template<typename Id> struct unit_{};
    template<typename Id> struct unit_at_time_{};
    template<typename Id> struct time_analysis_{};
}
namespace{
    template<typename Id = mpl::void_>
    struct kwd{
        static ::boost::parameter::keyword<tag::
            unit_<Id> >& unit;
        static ::boost::parameter::keyword<tag::
            unit_at_time_<Id> >& unit_at_time;
        static ::boost::parameter::keyword<tag::
            time_analysis_<Id> >& time_analysis;
        static ::boost::parameter::keyword<tag::
            entry_time_<Id> >& entry_time;
        static ::boost::parameter::keyword<tag::
            covariate_<Id> >& covariate;
    };
    template<typename Id>
    ::boost::parameter::keyword<tag::unit_<Id> >&
        kwd<Id>::unit =
            ::boost::parameter::keyword<tag::unit_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::unit_at_time_<Id> >&
        kwd<Id>::unit_at_time =
            ::boost::parameter::keyword<tag::unit_at_time_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::time_analysis_<Id> >&
        kwd<Id>::time_analysis =
            ::boost::parameter::keyword<tag::time_analysis_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::entry_time_<Id> >&
        kwd<Id>::entry_time =
            ::boost::parameter::keyword<tag::entry_time_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::covariate_<Id> >&
        kwd<Id>::covariate =
            ::boost::parameter::keyword<tag::covariate_<Id> >::get();
}//anonymous
}//survival_data
}//boost

#endif
