// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MODIFIER_HPP

#ifndef SIGNAL_NETWORK_GENERIC_CLASS

#define SIGNAL_NETWORK_GENERIC_CLASS modifier
#define SIGNAL_NETWORK_GENERIC_FILE <boost/dataflow/signals/component/modifier.hpp>
#define SIGNAL_NETWORK_GENERIC_TYPENAME Modification
#define SIGNAL_NETWORK_GENERIC_MEMBERNAME modification
#define SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#include <boost/dataflow/signals/component/detail/generic_template.hpp>

#define SIGNAL_NETWORK_MODIFIER_HPP

#else // SIGNAL_NETWORK_GENERIC_CLASS

    /** Applies the Modification object to the received signal parameters.
        */
    template <class Seq>
    typename result<modifier_impl(const Seq &)>::type
    operator()(const Seq &seq)
    {
        return fused_out(modification(seq));
    }

#endif // SIGNAL_NETWORK_GENERIC_CLASS

#endif // SIGNAL_NETWORK_MODIFIER_HPP
