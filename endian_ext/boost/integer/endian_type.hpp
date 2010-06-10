//  Boost endian_type.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//

#ifndef BOOST_INTEGER_ENDIAN_TYPE_HPP
#define BOOST_INTEGER_ENDIAN_TYPE_HPP

#include <boost/integer/endian.hpp>

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
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    static const BOOST_SCOPED_ENUM(endianness) value= endianness::native;
#else
    typedef boost::endian::native type;
#endif    
    
};

// we can consider that any other type which has not been explicitly declared is native, but it is more conservative to state that it could be mixted.
template <typename T>
struct endian_type_impl<T, false, false> {
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    static const BOOST_SCOPED_ENUM(endianness) value= endianness::mixed;
#else
    typedef boost::endian::mixed type;
#endif    
};

// if temp is equal the endian_type of the *it continue otherwise mixed
template <
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
        BOOST_SCOPED_ENUM(endianness) res, 
#else
        typename res,
#endif    
    typename It, typename End
> struct endian_type_loop {
private:    
    typedef 
        typename remove_reference<
            typename remove_cv<
                typename fusion::result_of::deref<It>::type
            >::type 
        >::type it_type;
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    typedef typename mpl::if_c<endian_type< it_type >::value==res,
        mpl::int_<endian_type_loop<res,typename fusion::result_of::next<It>::type, End>::value>,
        mpl::int_<endianness::mixed>
    >::type res_type;    
public:    
    static const BOOST_SCOPED_ENUM(endianness) value= BOOST_SCOPED_ENUM(endianness)(res_type::value);    
#   else
public:    
    typedef typename mpl::if_<is_same<typename endian_type< it_type >::type,res>,
        typename endian_type_loop<res,typename fusion::result_of::next<It>::type, End>::type,
        boost::endian::mixed
    >::type type;    
#   endif    
};

// endian_type of mixed is mixed
template <typename It, typename End>
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
struct endian_type_loop<endianness::mixed,It,End> {
    static const BOOST_SCOPED_ENUM(endianness) value= endianness::mixed;    
};
#else
struct endian_type_loop<boost::endian::mixed,It,End> {
    typedef boost::endian::mixed type;
};
#endif    

// temp when iteration ends
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
template <BOOST_SCOPED_ENUM(endianness) temp, typename End>
struct endian_type_loop<temp,End,End> {
    static const BOOST_SCOPED_ENUM(endianness) value= temp;    
};
#else
template <typename temp, typename End>
struct endian_type_loop<temp,End,End> {
    typedef temp type;
};
#endif    

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
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    static const BOOST_SCOPED_ENUM(endianness) value= 
        endian_type_loop<endian_type< front_type >::value, 
                    typename fusion::result_of::begin<Seq>::type, 
                    typename fusion::result_of::end<Seq>::type 
    >::value;
#else
    typedef typename endian_type_loop<typename endian_type< front_type >::type, 
                    typename fusion::result_of::begin<Seq>::type, 
                    typename fusion::result_of::end<Seq>::type 
    >::type type;
#endif    

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
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    BOOST_SCOPED_ENUM(endianness) E, 
#   else
    typename E,
#   endif    
    typename T, 
    std::size_t n_bits,
    BOOST_SCOPED_ENUM(alignment) A
> struct endian_type<endian_pack<E,T,n_bits,A> >
{
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    static const BOOST_SCOPED_ENUM(endianness) value= E;
#else
    typedef E type;
#endif    
};

// endianess of endian<E,T,n_bits,A> is E
template <
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    BOOST_SCOPED_ENUM(endianness) E, 
#   else
    typename E,
#   endif    
    typename T, 
    std::size_t n_bits,
    BOOST_SCOPED_ENUM(alignment) A
> struct endian_type<endian<E,T,n_bits,A> >  
{
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    static const BOOST_SCOPED_ENUM(endianness) value= E;
#else
    typedef E type;
#endif    
};

template <typename T>
struct is_native : 
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    mpl::bool_<endian_type<T>::value== endianness::native>
#else
    is_same<typename endian_type<T>::type, boost::endian::native>
#endif    
{};

template <typename T>
struct is_big : 
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    mpl::bool_<endian_type<T>::value== endianness::big>
#else
    is_same<typename endian_type<T>::type, boost::endian::big>
#endif    
{};

template <typename T>
struct is_little : 
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    mpl::bool_<endian_type<T>::value== endianness::little>
#else
    is_same<typename endian_type<T>::type, boost::endian::little>
#endif    
{};

template <typename T>
struct is_mixed : 
#   ifdef BOOST_INTEGER_ENDIAN_USES_ENDIANNESS    
    mpl::bool_<endian_type<T>::value== endianness::mixed>
#else
    is_same<typename endian_type<T>::type, boost::endian::mixed>
#endif    
{};


} // namespace integer
} // namespace boost

#endif // BOOST_ENDIAN_HPP
