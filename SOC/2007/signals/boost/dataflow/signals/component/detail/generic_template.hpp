// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef SIGNAL_NETWORK_GENERIC_CLASS

#include <boost/dataflow/signals/component/fusion_filter.hpp>

#ifndef SIGNAL_NETWORK_GENERIC_USE_TEMPLATED
#include <boost/dataflow/signals/component/detail/unfused_inherited.hpp>
#define SIGNAL_NETWORK_GENERIC_UNFUSED_BASE unfused_inherited
#else
#include <boost/dataflow/signals/component/detail/unfused_inherited_templated.hpp>
#define SIGNAL_NETWORK_GENERIC_UNFUSED_BASE unfused_inherited_templated
#endif

#include <boost/mpl/vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/support/is_sequence.hpp>

#define SIGNAL_NETWORK_GENERIC_CLASS_IMPL BOOST_PP_CAT(SIGNAL_NETWORK_GENERIC_CLASS,_impl)

namespace boost { namespace signals {

namespace detail
{
template<
    typename Derived,
    typename SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
    typename SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif    
    typename Signature,
    typename OutSignal,
    typename SignalArgs
    >
class BOOST_PP_CAT(SIGNAL_NETWORK_GENERIC_CLASS,_impl)
    : public fusion_filter<
        Derived,
        Signature,
        mpl::vector<
            Signature,
            typename fused_signal_type<Signature>::signature_type
        >,
        OutSignal,
        SignalArgs>
{
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
        typename SIGNAL_NETWORK_GENERIC_CLASS_IMPL::signal_type::result_type> {};
#   endif // SIGNAL_NETWORK_GENERIC_STANDARD_RESULT

#   include SIGNAL_NETWORK_GENERIC_FILE
    
protected:
    SIGNAL_NETWORK_GENERIC_TYPENAME SIGNAL_NETWORK_GENERIC_MEMBERNAME;
};
}

template<
    typename Derived,
    typename SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
    typename SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
    typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type >
class SIGNAL_NETWORK_GENERIC_CLASS
    : public boost::fusion::SIGNAL_NETWORK_GENERIC_UNFUSED_BASE
        <detail::SIGNAL_NETWORK_GENERIC_CLASS_IMPL<
            Derived,
            SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
            SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
            Signature, OutSignal, SignalArgs>,
        typename boost::function_types::parameter_types<Signature>::type >
{
    typedef boost::fusion::SIGNAL_NETWORK_GENERIC_UNFUSED_BASE
        <detail::SIGNAL_NETWORK_GENERIC_CLASS_IMPL<
            Derived,
            SIGNAL_NETWORK_GENERIC_TYPENAME,
#ifdef SIGNAL_NETWORK_GENERIC_TYPENAME2
            SIGNAL_NETWORK_GENERIC_TYPENAME2,
#endif
            Signature, OutSignal, SignalArgs>,
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
