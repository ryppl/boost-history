///////////////////////////////////////////////////////////////////////////////
//  keyword.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_GENERATOR_KEYWORD_HPP_ER_2009
#define BOOST_GENERATOR_KEYWORD_HPP_ER_2009

namespace boost{
namespace generator{
namespace tag{
    template<typename Id> struct clock_start_time_{};
    template<typename Id> struct clock_delta_time_{};
    template<typename Id> struct range_containers_{};
    template<typename Id> struct range_containers_offset_{};
}
namespace{
    template<typename Id = mpl::void_>
    struct kwd{
        static ::boost::parameter::keyword<
            tag::clock_start_time_<Id> >& clock_start_time;
        static ::boost::parameter::keyword<
            tag::clock_delta_time_<Id> >& clock_delta_time;
        static ::boost::parameter::keyword<
            tag::range_containers_<Id> >& range_containers;
        static ::boost::parameter::keyword<
            tag::range_containers_offset_<Id> >&
                                          range_containers_offset;
    };
    template<typename Id>
    ::boost::parameter::keyword<tag::clock_start_time_<Id> >&
        kwd<Id>::clock_start_time =
            ::boost::parameter::keyword<tag::clock_start_time_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::clock_delta_time_<Id> >&
        kwd<Id>::clock_delta_time =
            ::boost::parameter::keyword<tag::clock_delta_time_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::range_containers_<Id> >&
        kwd<Id>::range_containers =
            ::boost::parameter::keyword<tag::range_containers_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::range_containers_offset_<Id> >&
        kwd<Id>::range_containers_offset =
            ::boost::parameter::keyword<
                tag::range_containers_offset_<Id> >::get();
}//anonymous
}//generator
}//boost
#endif // BOOST_GENERATOR_KEYWORD_HPP_ER_2009
