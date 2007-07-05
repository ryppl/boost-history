// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONDITIONAL_HPP
#define SIGNAL_NETWORK_CONDITIONAL_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/support/is_sequence.hpp>


SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Forwards an incoming signal if an specified condition evaluates to true.
*/
template<
typename Condition,
typename Member,
typename Signature,
typename OutSignal=default_out_signal,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group> >
class conditional : public filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare>
{
    typedef filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare> base_type;
    
public:
    typedef boost::fusion::unfused_inherited<conditional<Condition, Member, Signature, OutSignal, Combiner, Group, GroupCompare>,
    typename mpl::vector<>::type,
    typename boost::function_types::parameter_types<Signature> > unfused;
        
    template <class Seq>
        struct result : public boost::enable_if<boost::fusion::traits::is_sequence<Seq>,
        typename base_type::signal_type::result_type> {};
    
    /** Forwards the signal if the condition evaluates the true.
        \returns Return value of the sent signal if the condition evaluates to true,
        default constructed instance otherwise.
    */
    template <class Seq>
#ifndef DOXYGEN_DOCS_ONLY
        typename boost::enable_if<boost::fusion::traits::is_sequence<Seq>,
#endif
        typename base_type::signal_type::result_type
#ifndef DOXYGEN_DOCS_ONLY
        >::type
#endif
    operator()(const Seq &vec_par)
    {
        if (Condition()(member))
            return fused_out(vec_par);
        else
            return typename base_type::signal_type::result_type();
    }
protected:
    Member member;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_CONDITIONAL_HPP