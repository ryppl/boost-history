// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PARAMETER_AUX__RESULT_OF0_DWA2005511_HPP
# define BOOST_PARAMETER_AUX__RESULT_OF0_DWA2005511_HPP

// A metafunction returning the result of invoking a nullary function
// object of the given type.

#if !defined(BOOST_NO_SFINAE) && !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

# include <boost/utility/result_of.hpp>
namespace boost { namespace parameter { namespace aux { 
template <class F>
struct result_of0 : result_of<F()>
{};

}}} // namespace boost::parameter::aux_

#else

namespace boost { namespace parameter { namespace aux { 
template <class F>
struct result_of0
{
    typedef typename F::result_type type;
};

}}} // namespace boost::parameter::aux_

#endif 


#endif // BOOST_PARAMETER_AUX__RESULT_OF0_DWA2005511_HPP
