// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef SIGNAL_NETWORK_GENERIC_CLASS

#include <boost/dataflow/signal/component/filter.hpp>

#ifndef SIGNAL_NETWORK_GENERIC_USE_TEMPLATED
#include <boost/dataflow/signal/component/detail/unfused_inherited.hpp>
#define SIGNAL_NETWORK_GENERIC_UNFUSED_BASE unfused_inherited
#else
#include <boost/dataflow/signal/component/detail/unfused_inherited_templated.hpp>
#define SIGNAL_NETWORK_GENERIC_UNFUSED_BASE unfused_inherited_templated
#endif

#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/support/is_sequence.hpp>

#define SIGNAL_NETWORK_GENERIC_CLASS_IMPL BOOST_PP_CAT(SIGNAL_NETWORK_GENERIC_CLASS,_impl)

namespace boost { namespace signals {

namespace detail
{
    /** \brief fused implementation of conditional modifier
    */
    template<
    typename SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
    typename SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif    
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
    typename Group = int,
    typename GroupCompare = std::less<Group> >
    class BOOST_PP_CAT(SIGNAL_NETWORK_GENERIC_CLASS,_impl) : public filter<Signature, typename OutSignal::filter_type, Combiner, Group, GroupCompare>
{
protected:
    typedef filter<Signature, typename OutSignal::filter_type, Combiner, Group, GroupCompare> base_type;

public:    
    SIGNAL_NETWORK_GENERIC_CLASS_IMPL() {}
    template<typename T1>
    SIGNAL_NETWORK_GENERIC_CLASS_IMPL(const T1 &t1) : SIGNAL_NETWORK_GENERIC_MEMBERNAME(t1) {}
    template<typename T1, typename T2>
    SIGNAL_NETWORK_GENERIC_CLASS_IMPL(const T1 &t1, const T2 &t2) : SIGNAL_NETWORK_GENERIC_MEMBERNAME(t1, t2) {}

#   ifdef SIGNAL_NETWORK_GENERIC_STANDARD_RESULT
    template <class FArgs>
    struct result;
    
    template<typename T, typename Seq>
    struct result<T(const Seq &)> : public boost::enable_if<boost::fusion::traits::is_sequence<Seq>,
        typename base_type::signal_type::result_type> {};
#   endif // SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#   include SIGNAL_NETWORK_GENERIC_FILE
    
protected:
    SIGNAL_NETWORK_GENERIC_TYPENAME SIGNAL_NETWORK_GENERIC_MEMBERNAME;
};
}

/** \brief Passes the incoming signal to a member modifier, and optionally forwards the returned result.
*/
template<
typename SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
typename SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
typename Signature,
typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
typename Group = int,
typename GroupCompare = std::less<Group> >
class SIGNAL_NETWORK_GENERIC_CLASS
    : public boost::fusion::SIGNAL_NETWORK_GENERIC_UNFUSED_BASE
        <detail::SIGNAL_NETWORK_GENERIC_CLASS_IMPL<
            SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
            SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
            Signature, OutSignal, Combiner, Group, GroupCompare>,
        typename boost::function_types::parameter_types<Signature>::type >
{
    typedef boost::fusion::SIGNAL_NETWORK_GENERIC_UNFUSED_BASE
        <detail::SIGNAL_NETWORK_GENERIC_CLASS_IMPL<
            SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
            SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
            Signature, OutSignal, Combiner, Group, GroupCompare>,
        typename boost::function_types::parameter_types<Signature>::type > base_type;
public:
    SIGNAL_NETWORK_GENERIC_CLASS() {}
    template<typename T1>
    SIGNAL_NETWORK_GENERIC_CLASS(const T1 &t1) : base_type(t1) {}
    template<typename T1, typename T2>
    SIGNAL_NETWORK_GENERIC_CLASS(const T1 &t1, const T2 &t2) : base_type(t1, t2) {}
};

} } // namespace boost::signals

#undef SIGNAL_NETWORK_GENERIC_CLASS
#undef SIGNAL_NETWORK_GENERIC_USE_TEMPLATED
#undef SIGNAL_NETWORK_GENERIC_FILE
#undef SIGNAL_NETWORK_GENERIC_TYPENAME
#undef SIGNAL_NETWORK_GENERIC_TYPENAME2
#undef SIGNAL_NETWORK_GENERIC_MEMBERNAME
#undef SIGNAL_NETWORK_GENERIC_STANDARD_RESULT
#undef SIGNAL_NETWORK_GENERIC_UNFUSED_BASE


#endif