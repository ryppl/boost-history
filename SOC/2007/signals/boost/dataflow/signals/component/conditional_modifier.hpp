// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONDITIONAL_MODIFIER_HPP

#ifndef SIGNAL_NETWORK_GENERIC_CLASS

#define SIGNAL_NETWORK_GENERIC_CLASS conditional_modifier
#define SIGNAL_NETWORK_GENERIC_FILE <boost/dataflow/signals/component/conditional_modifier.hpp>
#define SIGNAL_NETWORK_GENERIC_TYPENAME Modification
#define SIGNAL_NETWORK_GENERIC_MEMBERNAME modification

#include <boost/dataflow/signals/component/detail/generic_template.hpp>

#define SIGNAL_NETWORK_CONDITIONAL_MODIFIER_HPP

#else // SIGNAL_NETWORK_GENERIC_CLASS

    /** Applies the Modification object to the received signal parameters.
    */
    template <typename Args>
    struct result;

    template <typename F, typename Seq>
    struct result<F(const Seq &seq)> : public boost::enable_if<
        boost::fusion::traits::is_sequence<Seq>,
        typename base_type::signal_type::result_type>
    {};

    template <class Seq>
    typename result<conditional_modifier_impl(const Seq &)>::type
    operator()(const Seq &seq)
    {
        typename boost::result_of<Modification(const Seq &)>::type optional
        = modification.operator()<Seq>(seq);
        if (optional)
            return fused_out(*optional);
    }

#endif // SIGNAL_NETWORK_GENERIC_CLASS

#endif // SIGNAL_NETWORK_CONDITIONAL_MODIFIER_HPP
