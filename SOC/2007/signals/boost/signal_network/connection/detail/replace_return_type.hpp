// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_REPLACE_RETURN_TYPE_HPP
#define SIGNAL_NETWORK_REPLACE_RETURN_TYPE_HPP

namespace boost { namespace signals {
        
namespace detail {        

    template<typename Signature, typename T>
    struct replace_return_type : public
        boost::function_types::function_type<
            typename boost::mpl::push_front<
                typename boost::function_types::parameter_types<Signature>::type,
                T
            >::type
        >
    {};
    
} // namespace detail

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_REPLACE_RETURN_TYPE_HPP

