//  Boost endian_type.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//

#ifndef BOOST_INTEGER_ENDIAN_TYPE_HPP
#define BOOST_INTEGER_ENDIAN_TYPE_HPP

#include <boost/integer/endian/endian.hpp>

#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/value_of_data.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace boost {
namespace integer {

namespace integer_detail {

template <typename T, 
    bool isFundamental  = is_fundamental<T>::value, 
    bool IsSeq          = fusion::traits::is_sequence<T>::value
    >
struct endian_type_impl;

}

template <typename T>
struct endian_type : integer_detail::endian_type_impl<T> {
};

namespace integer_detail {

//~ // fundamental types are native    
template <typename T, bool IsSeq>
struct endian_type_impl<T, true, IsSeq> {
    typedef endianness::native type;    
};

// we can consider that any other type which has not been explicitly declared is native, but it is more conservative to state that it could be mixted.
template <typename T>
struct endian_type_impl<T, false, false> {
    typedef endianness::mixed type;
};

// if temp is equal the endian_type of the *it continue otherwise mixed
template <typename res, typename It, typename End
> struct endian_type_loop {
private:    
    typedef 
        typename remove_reference<
            typename remove_cv<
                typename fusion::result_of::deref<It>::type
            >::type 
        >::type it_type;
public:    
    typedef typename mpl::if_<is_same<typename endian_type< it_type >::type,res>,
        typename endian_type_loop<res,typename fusion::result_of::next<It>::type, End>::type,
        endianness::mixed
    >::type type;    
};

// endian_type of mixed is mixed
template <typename It, typename End>
struct endian_type_loop<endianness::mixed,It,End> {
    typedef endianness::mixed type;
};

// temp when iteration ends
template <typename temp, typename End>
struct endian_type_loop<temp,End,End> {
    typedef temp type;
};

// fusion sequences are mixed if not all the elements habve the same endianess, 
// otherwise the endianess of all the types.
template <typename Seq>
struct endian_type_impl<Seq, false, true> {
private:    
    typedef typename remove_reference<
            typename remove_cv<
                typename fusion::result_of::front<Seq>::type
            >::type 
    >::type front_type;

public:        
    typedef typename endian_type_loop<typename endian_type< front_type >::type, 
                    typename fusion::result_of::begin<Seq>::type, 
                    typename fusion::result_of::end<Seq>::type 
    >::type type;
};


}


template <typename T>
struct endian_type<T*> {
    // it is undefined the endianess of a pointer
};

template <typename T, std::size_t N>
struct endian_type<T[N]> : endian_type<T> {};

// endianess of endian<E,T,n_bits,A> is E
template <
    typename E,
    typename T, 
    std::size_t n_bits,
    BOOST_SCOPED_ENUM(alignment) A
> struct endian_type<endian_pack<E,T,n_bits,A> >
{
    typedef E type;
};

// endianess of endian<E,T,n_bits,A> is E
template <
    typename E,
    typename T, 
    std::size_t n_bits,
    BOOST_SCOPED_ENUM(alignment) A
> struct endian_type<endian<E,T,n_bits,A> >  
{
    typedef E type;
};

template <typename T>
struct is_native : 
    is_same<typename endian_type<T>::type, endianness::native>
{};

template <typename T>
struct is_big : 
    is_same<typename endian_type<T>::type, endianness::big>
{};

template <typename T>
struct is_little : 
    is_same<typename endian_type<T>::type, endianness::little>
{};

template <typename T>
struct is_mixed : 
    is_same<typename endian_type<T>::type, endianness::mixed>
{};


} // namespace integer
} // namespace boost

#endif // BOOST_ENDIAN_HPP
