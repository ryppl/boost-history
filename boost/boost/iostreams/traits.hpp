// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// 
// Contains metafunctions io_char, io_category and io_mode used for deducing 
// the i/o category and i/o mode of a model of Filter or Device.
//
// Also contains several utility metafunctions, functions and macros.
//

#ifndef BOOST_IOSTREAMS_IO_TRAITS_HPP_INCLUDED
#define BOOST_IOSTREAMS_IO_TRAITS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <iosfwd>            // stream types, char_traits.
#include <boost/config.hpp>  // partial spec, deduced typename.
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/bool_trait_def.hpp> 
#include <boost/iostreams/detail/config/wide_streams.hpp>
#include <boost/iostreams/detail/is_iterator_range.hpp>    
#include <boost/iostreams/detail/select.hpp>        
#include <boost/iostreams/detail/select_by_size.hpp>      
#include <boost/iostreams/detail/wrap_unwrap.hpp>       
#include <boost/iostreams/traits_fwd.hpp> 
#include <boost/mpl/bool.hpp>   
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>      
#include <boost/mpl/int.hpp>  
#include <boost/mpl/or.hpp>                         
#include <boost/range/iterator_range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/type_traits/is_convertible.hpp>     

namespace boost { namespace iostreams {        

//------------------Definitions of predicates for streams and stream buffers--//

#ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //--------------------------------//

BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_istream, std::basic_istream, 2)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_ostream, std::basic_ostream, 2)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_iostream, std::basic_iostream, 2)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_streambuf, std::basic_streambuf, 2)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_stringstream, std::basic_stringstream, 3)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_stringbuf, std::basic_stringbuf, 3)

#else // #ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //-----------------------//

BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_istream, std::istream, 0)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_ostream, std::ostream, 0)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_iostream, std::iostream, 0)
BOOST_IOSTREAMS_BOOL_TRAIT_DEF(is_streambuf, std::streambuf, 0)

#endif // #ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //----------------------//

template<typename T>
struct is_std_io
    : mpl::or_< is_istream<T>, is_ostream<T>, is_streambuf<T> >
    { };
                    
//------------------Definitions of io_char------------------------------------//

namespace detail {

template<typename T>
struct member_char_type { typedef typename T::char_type type; };

} // End namespace detail.

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //---------------------------//
# ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //-------------------------------//

template<typename T>
struct io_char 
    : detail::member_char_type<
          typename detail::unwrapped_type<T>::type
      > 
    { };

# else // # ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //---------------------//

template<typename T>
struct io_char {
    typedef typename detail::unwrapped_type<T>::type U;
    typedef typename 
            mpl::eval_if<
                is_std_io<U>,
                mpl::identity<char>,
                detail::member_char_type<U>
            >::type type;
};

# endif // # ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //--------------------//

template<typename Iter>
struct io_char< iterator_range<Iter> > {
    typedef typename iterator_value<Iter>::type type;
};

#else // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //------------------//

template<typename T>
struct io_char {
    template<typename U>
    struct get_value_type {
        typedef typename range_value<U>::type type;
    };
    typedef typename 
            mpl::eval_if<
                is_iterator_range<T>,
                get_value_type<T>,
                detail::member_char_type<
                    BOOST_DEDUCED_TYPENAME detail::unwrapped_type<T>::type
                >
            >::type type;
};

#endif // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //-----------------//

//------------------Definitions of io_category--------------------------------//

namespace detail {

template<typename T>
struct member_io_category { typedef typename T::io_category type; };

} // End namespace detail.

template<typename T>
struct io_category {
    template<typename U>
    struct member_io_category { 
        typedef typename U::io_category type; 
    };
    typedef typename detail::unwrapped_type<T>::type U;
    typedef typename  
            mpl::eval_if<
                is_std_io<U>,
                iostreams::select<  // Disambiguation for Tru64
                    is_iostream<U>,   iostream_tag, 
                    is_istream<U>,    istream_tag, 
                    is_ostream<U>,    ostream_tag,
                    is_streambuf<U>,  streambuf_tag
                >,
                detail::member_io_category<U>
            >::type type;      
};

//------------------Definition of get_category--------------------------------//

// 
// Returns an object of type io_category<T>::type.
// 
template<typename T>
inline typename io_category<T>::type get_category(const T&) 
{ typedef typename io_category<T>::type category; return category(); }

//------------------Definition of io_int--------------------------------------//

template<typename T>
struct io_int { 
#ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES
    typedef std::char_traits<
                BOOST_DEDUCED_TYPENAME io_char<T>::type
            > traits_type;      
    typedef typename traits_type::int_type type; 
#else  
    typedef int                            type; 
#endif
};

//------------------Definition of mode----------------------------------------//

namespace detail {

template<int N> struct io_mode_impl;

#define BOOST_IOSTREAMS_MODE_HELPER(tag_, id_) \
    case_<id_> io_mode_impl_helper(tag_); \
    template<> struct io_mode_impl<id_> { typedef tag_ type; }; \
    /**/
BOOST_IOSTREAMS_MODE_HELPER(input, 1)
BOOST_IOSTREAMS_MODE_HELPER(output, 2)
BOOST_IOSTREAMS_MODE_HELPER(bidirectional, 3)
BOOST_IOSTREAMS_MODE_HELPER(input_seekable, 4)
BOOST_IOSTREAMS_MODE_HELPER(output_seekable, 5)
BOOST_IOSTREAMS_MODE_HELPER(seekable, 6)
BOOST_IOSTREAMS_MODE_HELPER(dual_seekable, 7)
BOOST_IOSTREAMS_MODE_HELPER(bidirectional_seekable, 8)
BOOST_IOSTREAMS_MODE_HELPER(dual_use, 9)
#undef BOOST_IOSTREAMS_MODE_HELPER

template<typename T>
struct io_mode_id {
    typedef typename io_category<T>::type category;
    BOOST_SELECT_BY_SIZE(int, value, detail::io_mode_impl_helper(category()));
};

} // End namespace detail.

template<typename T> // Borland 5.6.4 requires this circumlocution.
struct io_mode : detail::io_mode_impl< detail::io_mode_id<T>::value > { };
                    
//------------------Definition of is_device, is_filter and is_direct----------//

namespace detail {

template<typename T, typename Tag>
struct has_trait_impl {
    typedef typename io_category<T>::type category;
    BOOST_STATIC_CONSTANT(bool, value = (is_convertible<category, Tag>::value));
};

template<typename T, typename Tag>
struct has_trait 
    : mpl::bool_<has_trait_impl<T, Tag>::value>
    { }; 

} // End namespace detail.

template<typename T>
struct is_device : detail::has_trait<T, device_tag> { };

template<typename T>
struct is_filter : detail::has_trait<T, filter_tag> { };

template<typename T>
struct is_direct : detail::has_trait<T, direct_tag> { };
                    
//------------------Definition of BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS----------//

#define BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr) \
    typedef Tr                              traits_type; \
    typedef typename traits_type::int_type  int_type; \
    typedef typename traits_type::off_type  off_type; \
    typedef typename traits_type::pos_type  pos_type; \
    /**/

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_IO_TRAITS_HPP_INCLUDED
