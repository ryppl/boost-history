// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SLOT_TYPE_HPP
#define SIGNAL_NETWORK_SLOT_TYPE_HPP

// Constructs a type corresponding to pointer to member of T with signature Signature.
// e.g. slot_type<some_class, void(float)>::type is void (some_class::*) (float)

#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/push_front.hpp>

namespace boost {  namespace signals {

namespace detail {
        
    template <typename Signature, typename T>
    struct slot_type
    {
        typedef
            typename boost::function_types::member_function_pointer<
                typename boost::mpl::push_front<
                    typename boost::mpl::push_front<
                        typename boost::function_types::parameter_types<Signature>::type, T
                    >::type, typename boost::function_types::result_type<Signature>::type
                >::type
            >::type type;
    };
        
} // namespace detail

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_SLOT_TYPE_HPP
