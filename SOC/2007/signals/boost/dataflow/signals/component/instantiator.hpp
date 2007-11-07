// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_INSTANTIATOR_HPP

#ifndef SIGNAL_NETWORK_GENERIC_CLASS

#define SIGNAL_NETWORK_GENERIC_CLASS instantiator
#define SIGNAL_NETWORK_GENERIC_FILE <boost/dataflow/signals/component/instantiator.hpp>
#define SIGNAL_NETWORK_GENERIC_TYPENAME Member
#define SIGNAL_NETWORK_GENERIC_MEMBERNAME member
#define SIGNAL_NETWORK_GENERIC_TYPENAME2 Instantiation
#define SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#include <boost/dataflow/signals/component/detail/generic_template.hpp>

#define SIGNAL_NETWORK_INSTANTIATOR_HPP

#else // SIGNAL_NETWORK_GENERIC_CLASS

/** \brief Generic component which, upon receiving a signal, instantiates an object of type Instantiation from a member object
     of type Member.
*/

    /** Instantiates an instance of Instantiation from the Member object.
    */
    template <class Seq>
    typename result<instantiator_impl(const Seq &)>::type
    operator()(const Seq &seq)
    {
        Instantiation instance(member);
        return fused_out(seq);
    }

#endif // SIGNAL_NETWORK_GENERIC_CLASS

#endif // SIGNAL_NETWORK_INSTANTIATOR_HPP
