/////////////////////////////////////////////////////////////////////////////
// parameter.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_KEYWORD_PARAMETER_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_KEYWORD_PARAMETER_HPP_ER_2009
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>

namespace boost{
namespace conditionally_specified_distribution{
namespace parameter{
namespace tag{
    template<typename Id> struct mu_{};
    template<typename Id> struct sigma_{};
    template<typename Id> struct value_{};
    template<typename Id> struct correlation_{};
			//TODO nu, alpha, beta, p, n, etc.
}
namespace{
    template<typename Id>
    class kwd{
        typedef tag::mu_<Id>            mu_t;
        typedef tag::sigma_<Id>         sigma_t;
        typedef tag::value_<Id>         value_t;
        typedef tag::correlation_<Id>   corr_t;
        public:
        typedef Id id_type;
        static ::boost::parameter::keyword<mu_t>&        mu;
        static ::boost::parameter::keyword<sigma_t>&     sigma;
        static ::boost::parameter::keyword<value_t>&     value;
        static ::boost::parameter::keyword<corr_t>&      correlation;
    };

    template<typename Id>
		::boost::parameter::keyword<tag::mu_<Id> >&
            kwd<Id>::mu =
            ::boost::parameter::keyword<tag::mu_<Id> >::get();

    template<typename Id>
    ::boost::parameter::keyword<tag::sigma_<Id> >&
            kwd<Id>::sigma =
            ::boost::parameter::keyword<tag::sigma_<Id> >::get();

    template<typename Id>
    ::boost::parameter::keyword<tag::value_<Id> >&
            kwd<Id>::value =
            ::boost::parameter::keyword<tag::value_<Id> >::get();

    template<typename Id>
    ::boost::parameter::keyword<tag::correlation_<Id> >&
            kwd<Id>::correlation =
            ::boost::parameter::keyword<tag::correlation_<Id> >::get();
}//anonymous
}//parameter
}//distribution
}//boost
#endif
