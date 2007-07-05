// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_INSTANTIATOR_HPP
#define SIGNAL_NETWORK_INSTANTIATOR_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>
#include <boost/fusion/support/is_sequence.hpp>


SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Generic component which, upon receiving a signal, instantiates an object of type Instantiation from a member object
     of type Member.
*/
template<
typename Instantiation,
typename Member,
typename Signature,
typename OutSignal=default_out_signal,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group> >
class instantiator : public filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare>
{
    typedef filter<Signature, typename OutSignal::default_normal_type, Combiner, Group, GroupCompare> base_type;
    
public:
#ifndef DOXYGEN_DOCS_ONLY
    typedef boost::fusion::unfused_inherited<instantiator<Instantiation, Member, Signature,
    typename OutSignal::default_unfused_type, Combiner, Group, GroupCompare>,
    typename mpl::vector<>::type,
    typename base_type::parameter_types > unfused;

    template <class Seq>
        struct result : public boost::enable_if<boost::fusion::traits::is_sequence<Seq>,
        typename base_type::signal_type::result_type> {};
#endif

    /** Instantiates an instance of Instantiation from the Member object.
    */
    template <class Seq>
#ifndef DOXYGEN_DOCS_ONLY
    typename boost::enable_if<boost::fusion::traits::is_sequence<Seq>,
#endif
        typename base_type::signal_type::result_type
#ifndef DOXYGEN_DOCS_ONLY
        >::type
#endif
    operator()(const Seq &seq)
    {
        Instantiation instance(member);
        return fused_out(seq);
    }
protected:
	Member member;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_INSTANTIATOR_HPP
