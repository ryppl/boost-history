// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONDITIONAL_HPP
#ifndef SIGNAL_NETWORK_GENERIC_CLASS

#define SIGNAL_NETWORK_GENERIC_CLASS conditional
#define SIGNAL_NETWORK_GENERIC_FILE <boost/dataflow/signals/component/conditional.hpp>
#define SIGNAL_NETWORK_GENERIC_TYPENAME Member
#define SIGNAL_NETWORK_GENERIC_MEMBERNAME member
#define SIGNAL_NETWORK_GENERIC_TYPENAME2 Condition
#define SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#include <boost/dataflow/signals/component/detail/generic_template.hpp>

#define SIGNAL_NETWORK_CONDITIONAL_HPP

#else // SIGNAL_NETWORK_GENERIC_CLASS

/** \brief Forwards an incoming signal if an specified condition evaluates to true.
*/
    
    /** Forwards the signal if the condition evaluates the true.
        \returns Return value of the sent signal if the condition evaluates to true,
        default constructed instance otherwise.
    */
    template <class Seq>
    typename result<conditional_impl(const Seq &)>::type
    operator()(const Seq &seq)
    {
        if (Condition()(member))
            return fused_out(seq);
        else
            return typename conditional_impl::signal_type::result_type();
    }

#endif // SIGNAL_NETWORK_GENERIC_CLASS

#endif // SIGNAL_NETWORK_CONDITIONAL_HPP
