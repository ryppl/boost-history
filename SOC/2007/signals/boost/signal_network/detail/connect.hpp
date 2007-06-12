#if !defined(SIGNAL_NETWORK_CONNECT_HPP)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/enum_shifted_params.hpp>
#include <boost/bind.hpp>

#define SIGNAL_NETWORK_TEMPLATE_TYPENAMES typename T,
#define SIGNAL_NETWORK_TEMPLATE_TYPENAMES_LIST T,

namespace boost {
    namespace signal_network {
        namespace signet {
            namespace detail {
                
                template<typename T, typename Signature, typename Combiner, typename Group, typename GroupCompare, int Arity>
                struct connect_impl;
                
#define BOOST_PP_ITERATION_PARAMS_1 (3,(0,3,<boost/signal_network/detail/connect.hpp>))
#include BOOST_PP_ITERATE()

            } // namespace detail
        } // namespace signet
    } // namespace signal_network
} // namespace boost

#define SIGNAL_NETWORK_CONNECT_HPP
#else // defined(BOOST_PP_IS_ITERATING)

template<typename T, typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct connect_impl<T, Signature, Combiner, Group, GroupCompare, BOOST_PP_ITERATION()>
{
    static void connect(boost::signal<Signature, Combiner, Group, GroupCompare> &signal, T &link)
    {
        signal.connect(boost::bind
                       (static_cast<typename slot_type<T, Signature>::type>( &T::operator() ),
                        boost::ref(link) BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_ITERATION()),_)));
    }
};
#endif
#endif