#ifndef BOOST_FTMPL_UNWRAP_HPP
#define BOOST_FTMPL_UNWRAP_HPP

namespace boost { namespace ftmpl {

template< typename T >
struct unwrap;

template< typename T >
struct unwrap< T& >
{
    typedef T type;
};

#ifdef BOOST_HAS_RELAXED_TYPENAME

#define BOOST_FTMPL_UNWRAP( expr )                                             \
typename ::boost::ftmpl::unwrap< decltype( expr ) >::type

#else

#define BOOST_FTMPL_UNWRAP( expr )                                             \
::boost::ftmpl::unwrap< decltype( ( expr ) ) >::type

#endif

#define BOOST_FTMPL_UNWRAP_T( expr )                                           \
typename ::boost::ftmpl::unwrap< decltype( ( expr ) ) >::type


template< typename T >
struct unwrap_value;

#define BOOST_FTMPL_UNWRAP_VALUE( expr )                                       \
::boost::ftmpl::unwrap_value< decltype( expr ) >::value

} }

#endif // BOOST_FTMPL_UNWRAP_HPP
