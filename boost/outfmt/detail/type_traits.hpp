// (C) Copyright 2003: Jonathan Turkanis; Reece H. Dunn

// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. This 
// software is provided "as is" without express or implied warranty, and 
// with no claim as to its suitability for any purpose.

// 
// 1. Defines the following type traits templates. Each models 
//    MPL::IntegralConstant and has the form: 
//    template<typename T> struct is_xxx.
//   
//       is_std_complex
//       is_std_pair
//       is_std_deque
//       is_std_list
//       is_std_map
//       is_std_multimap
//       is_std_set
//       is_std_multiset
//       is_std_string
//       is_std_vector
//       is_slist                 // RHD: added support for the SGI slist container
//       is_hash_set
//       is_hash_multiset
//       is_hash_map
//       is_hash_multimap
//       is_compressed_pair
//       is_rational
//       is_interval
//       is_quaternion
//       is_octonion
//
//       is_container (subsumes is_std_deque through is_hash_multimap)
//       is_pair (subsumes is_std_complex, is_std_pair, is_ratioanl, etc.)
//       is_nary (is_quaternion and is_octonion)
//

#ifndef BOOST_IO_STREAM_TRAITS_HPP_INCLUDED
#define BOOST_IO_STREAM_TRAITS_HPP_INCLUDED       
                
//--------------STL headers containing types to be analyzed-------------------//

#  include <complex>   
#  include <deque>    
#  include <list>      
#  include <map>       
#  include <set>       
#  include <vector>    
#  include <utility>   // pair

//--------------Non-standard STL headers containing types to be analyzed------//

#  if defined(BOOST_IOFM__HASH_CONTAINERS)
#     include <hash_map>
#     include <hash_set>
#  endif
#  if defined(BOOST_HAS_SLIST) // RHD: added support for the SGI slist container
#     include <slist>
#  endif

//--------------Boost headers containing types to be analyzed-----------------//

#  include <boost/compressed_pair.hpp>           
#  include <boost/rational.hpp>                 

#  if !defined(BOOST_IOFM__NO_LIB_INTERVAL)
#     include <boost/numeric/interval/interval.hpp>
#  endif
#  if !defined(BOOST_IOFM__NO_LIB_QUATERNION)
#     include <boost/math/quaternion.hpp>           
#  endif
#  if !defined(BOOST_IOFM__NO_LIB_OCTONION)
#     include <boost/math/octonion.hpp>             
#  endif

//--------------Headers used to perform the type-analysis---------------------//
 
#include <boost/config.hpp>       // BOOST_STATIC_CONSTANT, 
                                  // BOOST_DINKUMWARE_STDLIB,
                                  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION,
                                  // BOOST_NO_INTRINSIC_WCHAR_T.
                      
#include <boost/outfmt/detail/select.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>               // is_same, is_base_and_derived.
#include <boost/type_traits/detail/yes_no_type.hpp>    

namespace boost { namespace io { 
                
//--------------is_a----------------------------------------------------------//

//
// Description: Metafunction returning true if U is the same as or derived 
//      from T.
//
template<typename T, typename U>
struct is_a : public mpl::or_< is_base_and_derived<U, T>, is_same<U, T> > { };

//--------------or_-----------------------------------------------------------//

// mpl::or_ has two few paramters.
// RHD: extended or_ to support more parameters for future/custom extensions
template< typename T1,
          typename T2,
          typename T3 = mpl::false_,
          typename T4 = mpl::false_,
          typename T5 = mpl::false_,
          typename T6 = mpl::false_,
          typename T7 = mpl::false_,
          typename T8 = mpl::false_,
          typename T9 = mpl::false_,
          typename T10 = mpl::false_,
          typename T11 = mpl::false_,
          typename T12 = mpl::false_,
          typename T13 = mpl::false_,
          typename T14 = mpl::false_,
          typename T15 = mpl::false_,
          typename T16 = mpl::false_
        >
struct or_ 
    : public mpl::or_<
                 mpl::or_< T1,  T2,  T3,  T4  >,
                 mpl::or_< T5,  T6,  T7,  T8  >,
                 mpl::or_< T9,  T10, T11, T12 >,
                 mpl::or_< T13, T14, T15, T16 >
             >
    { };

//--------------type traits helpers-------------------------------------------//

#define BOOST_IO_MAKE_TRAITS(name, helper) \
template<typename T> \
struct name { \
    static const T* io; \
    struct type { \
       BOOST_STATIC_CONSTANT( bool, value = (sizeof(helper(io)) == \
                                             sizeof(type_traits::yes_type)) ); \
    }; \
    BOOST_STATIC_CONSTANT(bool, value = type::value); \
}; 

#define BOOST_IO_MAKE_TRAITS_HELPERS_1(helper, type) \
template<typename T> \
type_traits::yes_type helper(const type<T>*); \
type_traits::no_type helper(...);    

#define BOOST_IO_MAKE_TRAITS_HELPERS_2(helper, type) \
template<typename T, typename U> \
type_traits::yes_type helper(const type<T, U>*); \
type_traits::no_type helper(...);    

#define BOOST_IO_MAKE_TRAITS_HELPERS_3(helper, type) \
template<typename T, typename U, typename V> \
type_traits::yes_type helper(const type<T, U, V>*); \
type_traits::no_type helper(...);    

#define BOOST_IO_MAKE_TRAITS_HELPERS_4(helper, type) \
template<typename T, typename U, typename V, typename W> \
type_traits::yes_type helper(const type<T, U, V, W>*); \
type_traits::no_type helper(...);    

// RHD: fixed the macro -- , X> ==> , typename X>
#define BOOST_IO_MAKE_TRAITS_HELPERS_5(helper, type) \
template<typename T, typename U, typename V, typename W, typename X> \
type_traits::yes_type helper(const type<T, U, V, W, X>*); \
type_traits::no_type helper(...);    

namespace detail {

    //--------------Standard types--------------------------------------------//
                                          
BOOST_IO_MAKE_TRAITS_HELPERS_1(is_complex_helper, std::complex)
BOOST_IO_MAKE_TRAITS_HELPERS_2(is_pair_helper, std::pair)
BOOST_IO_MAKE_TRAITS_HELPERS_2(is_deque_helper, std::deque)
BOOST_IO_MAKE_TRAITS_HELPERS_2(is_list_helper, std::list)
BOOST_IO_MAKE_TRAITS_HELPERS_4(is_map_helper, std::map)
BOOST_IO_MAKE_TRAITS_HELPERS_4(is_multimap_helper, std::multimap)
BOOST_IO_MAKE_TRAITS_HELPERS_3(is_set_helper, std::set)
BOOST_IO_MAKE_TRAITS_HELPERS_3(is_multiset_helper, std::multiset)
BOOST_IO_MAKE_TRAITS_HELPERS_3(is_string_helper, std::basic_string)
BOOST_IO_MAKE_TRAITS_HELPERS_2(is_vector_helper, std::vector)

    //--------------Non-standard containers-----------------------------------//

// Note: The SGI versions each have an additional template parameter.

#ifdef BOOST_IOFM__HASH_CONTAINERS
#ifdef BOOST_DINKUMWARE_STDLIB
    BOOST_IO_MAKE_TRAITS_HELPERS_3 \
        (is_hash_set_helper, std::hash_set)
    BOOST_IO_MAKE_TRAITS_HELPERS_3 \
        (is_hash_multiset_helper, std::hash_multiset)
    BOOST_IO_MAKE_TRAITS_HELPERS_4 \
        (is_hash_map_helper, std::hash_map)
    BOOST_IO_MAKE_TRAITS_HELPERS_4 \
        (is_hash_multimap_helper, std::hash_multimap)
#else // #ifdef BOOST_DINKUMWARE_STDLIB
    BOOST_IO_MAKE_TRAITS_HELPERS_4 \
        (is_hash_set_helper, std::hash_set)
    BOOST_IO_MAKE_TRAITS_HELPERS_4 \
        (is_hash_multiset_helper, std::hash_multiset)
    BOOST_IO_MAKE_TRAITS_HELPERS_5 \
        (is_hash_map_helper, std::hash_map)
    BOOST_IO_MAKE_TRAITS_HELPERS_5 \
        (is_hash_multimap_helper, std::hash_multimap)
#endif // #ifdef BOOST_DINKUMWARE_STDLIB
#endif // #ifdef BOOST_IOFM__HASH_CONTAINERS

#if defined(BOOST_HAS_SLIST) // RHD: added support for the SGI slist container
   BOOST_IO_MAKE_TRAITS_HELPERS_2(is_slist_helper, std::slist)
#endif

    //--------------Boost types-----------------------------------------------//

BOOST_IO_MAKE_TRAITS_HELPERS_2(is_compressed_pair_helper, boost::compressed_pair)
BOOST_IO_MAKE_TRAITS_HELPERS_1(is_rational_helper, boost::rational)
    
#ifndef BOOST_IOFM__NO_LIB_INTERVAL
    BOOST_IO_MAKE_TRAITS_HELPERS_2(is_interval_helper, boost::numeric::interval)
#endif // #ifndef BOOST_IOFM__NO_LIB_INTERVAL

#ifndef BOOST_IOFM__NO_LIB_QUATERNION
   BOOST_IO_MAKE_TRAITS_HELPERS_1(is_quaternion_helper, boost::math::quaternion)
#endif // #ifndef BOOST_IOFM__NO_LIB_QUATERNION

#ifndef BOOST_IOFM__NO_LIB_OCTONION
   BOOST_IO_MAKE_TRAITS_HELPERS_1(is_octonion_helper, boost::math::octonion)
#endif // #ifndef BOOST_IOFM__NO_LIB_OCTONION

}               // End namespace detail.

//--------------is_xxx metafunctions------------------------------------------//

    //--------------Standard types--------------------------------------------//

BOOST_IO_MAKE_TRAITS(is_std_complex, detail::is_complex_helper)
BOOST_IO_MAKE_TRAITS(is_std_pair, detail::is_pair_helper)
BOOST_IO_MAKE_TRAITS(is_std_deque, detail::is_deque_helper)
BOOST_IO_MAKE_TRAITS(is_std_list, detail::is_list_helper)
BOOST_IO_MAKE_TRAITS(is_std_map, detail::is_map_helper)
BOOST_IO_MAKE_TRAITS(is_std_multimap, detail::is_multimap_helper)
BOOST_IO_MAKE_TRAITS(is_std_set, detail::is_set_helper)
BOOST_IO_MAKE_TRAITS(is_std_multiset, detail::is_multiset_helper)
BOOST_IO_MAKE_TRAITS(is_std_string, detail::is_string_helper)
BOOST_IO_MAKE_TRAITS(is_std_vector, detail::is_vector_helper)

    //--------------Non-standard containers-----------------------------------//

#ifdef BOOST_IOFM__HASH_CONTAINERS
    BOOST_IO_MAKE_TRAITS(is_hash_set, detail::is_hash_set_helper)
    BOOST_IO_MAKE_TRAITS(is_hash_multiset, detail::is_hash_multiset_helper)
    BOOST_IO_MAKE_TRAITS(is_hash_map, detail::is_hash_map_helper)
    BOOST_IO_MAKE_TRAITS(is_hash_multimap, detail::is_hash_multimap_helper)
#else // #ifdef BOOST_IOFM__HASH_CONTAINERS
    template<typename T> struct is_hash_set : public mpl::false_ { };
    template<typename T> struct is_hash_multiset : public mpl::false_ { };
    template<typename T> struct is_hash_map : public mpl::false_ { };
    template<typename T> struct is_hash_multimap : public mpl::false_ { };
#endif

#if defined(BOOST_HAS_SLIST) // RHD: added support for the SGI slist container
   BOOST_IO_MAKE_TRAITS(is_slist, detail::is_slist_helper)
#else
   template<typename T> struct is_slist : public mpl::false_ { };
#endif

    //--------------Boost-types-----------------------------------------------//

BOOST_IO_MAKE_TRAITS(is_compressed_pair, detail::is_compressed_pair_helper)
BOOST_IO_MAKE_TRAITS(is_rational, detail::is_rational_helper)

#ifndef BOOST_IOFM__NO_LIB_INTERVAL
    BOOST_IO_MAKE_TRAITS(is_interval, detail::is_interval_helper)
#else  // #ifndef BOOST_IOFM__NO_LIB_INTERVAL
    template<typename T> struct is_interval : public mpl::false_ { };
#endif // #ifndef BOOST_IOFM__NO_LIB_INTERVAL

#ifndef BOOST_IOFM__NO_LIB_QUATERNION
    BOOST_IO_MAKE_TRAITS(is_quaternion, detail::is_quaternion_helper)
#else  // #ifndef BOOST_IOFM__NO_LIB_QUATERNION
    template<typename T> struct is_quaternion : public mpl::false_ { };
#endif // #ifndef BOOST_IOFM__NO_LIB_QUATERNION

#ifndef BOOST_IOFM__NO_LIB_OCTONION
   BOOST_IO_MAKE_TRAITS(is_octonion, detail::is_octonion_helper)
#else  // #ifndef BOOST_IOFM__NO_LIB_OCTONION
    template<typename T> struct is_octonion : public mpl::false_ { };
#endif // #ifndef BOOST_IOFM__NO_LIB_OCTONION

template<typename T>
struct is_container
    : public or_< 
                 is_std_deque<T>,
                 is_std_list<T>,
                 is_std_map<T>,
                 is_std_multimap<T>,
                 is_std_set<T>,
                 is_std_multiset<T>,
                 is_std_string<T>,
                 is_std_vector<T>,
                 is_slist<T>,          // RHD: added support for the SGI slist container
                 is_hash_set<T>,
                 is_hash_multiset<T>,
                 is_hash_map<T>,
                 is_hash_multimap<T>
             >
    { };

template<typename T>
struct is_pair
    : public or_< 
                 is_std_complex<T>,
                 is_std_pair<T>,
                 is_compressed_pair<T>,
                 is_rational<T>,
                 is_interval<T>
             >
    { };

template<typename T>
struct is_nary : public or_< is_quaternion<T>, is_octonion<T> > { };

} }             // End namespaces io, boost.

#endif          // #ifndef BOOST_IO_STREAM_TRAITS_HPP_INCLUDED
