
#ifndef _BOOST_OVERLOAD_DETAIL__IF_METAFUNCTION_HPP_
#define _BOOST_OVERLOAD_DETAIL__IF_METAFUNCTION_HPP_


namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// if metafunction

template< bool C, typename T, typename E  >
struct if_c
{
    typedef T type;
};

template<typename T, typename E >
struct if_c<false, T, E>
{
    typedef E type;
};

template< typename C, typename T, typename E  >
struct if_ : if_c<C::value, T, E >
{
};

} } } // end namespaces

#endif // _BOOST_OVERLOAD_DETAIL__IF_METAFUNCTION_HPP_

