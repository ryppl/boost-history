#if !defined(SIGNAL_NETWORK_CONNECT_SLOT_HPP)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/iterate.hpp>

#define SIGNAL_NETWORK_TEMPLATE_TYPENAMES typename T,
#define SIGNAL_NETWORK_TEMPLATE_TYPENAMES_LIST T,

namespace boost {
    namespace signal_network {
        namespace signet {
            namespace detail {
                
                template<typename T, typename Signature, typename Combiner, typename Group, typename GroupCompare, int Arity>
                struct connect_slot_impl;
                
#define BOOST_PP_ITERATION_PARAMS_1 (3,(0,3,<boost/signal_network/detail/connect_slot.hpp>))
#include BOOST_PP_ITERATE()
                
            } // namespace detail
        } // namespace signet
    } // namespace signal_network
} // namespace boost

#define SIGNAL_NETWORK_CONNECT_SLOT_HPP
#else // defined(BOOST_PP_IS_ITERATING)

template<typename T, typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct connect_slot_impl<T, Signature, Combiner, Group, GroupCompare, BOOST_PP_ITERATION()>
{
    static void connect_slot(boost::signal<Signature, Combiner, Group, GroupCompare> &signal, const slot_selector_t<T, Signature> &slot)
    {
        signal.connect(boost::bind
                       (static_cast<typename slot_type<T, Signature>::type>(slot.func),
                        boost::ref(slot.link) BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_ITERATION()),_)));
    }
};
#endif
#endif