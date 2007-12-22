// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_APPLICATOR_HPP

#ifndef SIGNAL_NETWORK_GENERIC_CLASS

#define SIGNAL_NETWORK_GENERIC_CLASS applicator
#define SIGNAL_NETWORK_GENERIC_FILE <boost/dataflow/signals/component/applicator.hpp>
#define SIGNAL_NETWORK_GENERIC_TYPENAME Member
#define SIGNAL_NETWORK_GENERIC_MEMBERNAME member
#define SIGNAL_NETWORK_GENERIC_TYPENAME2 Application
#define SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#include <boost/dataflow/signals/component/detail/generic_template.hpp>

#define SIGNAL_NETWORK_APPLICATOR_HPP

#else // SIGNAL_NETWORK_GENERIC_CLASS

    /** Applies an instance of Application to the Member object.
        */    
    template <class Seq>
    typename result<applicator_impl(const Seq &)>::type
    operator()(const Seq &seq)
    {
        Application()(member);
        return fused_out(seq);
    }    

#endif // SIGNAL_NETWORK_GENERIC_CLASS

#endif // SIGNAL_NETWORK_INSTANTIATOR_HPP
