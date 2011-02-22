//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-20111

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_INTEGER_ENDIAN_CONVERT_HPP
#define BOOST_INTEGER_ENDIAN_CONVERT_HPP

#include <boost/integer/endian/endianness.hpp>
#include <boost/integer/endian/domain_map.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/integer/endian/endian_view.hpp>

namespace boost {
namespace integer {
namespace endianness {
    template <typename EndianTarget, typename EndianSource, typename T>
    inline void convert_to_from(T& r);

    template <typename EndianTarget, typename EndianSource, typename T>
    inline void convert_to_from_impl(T& r);

    namespace integer_detail {
        template <typename EndianTarget, typename EndianSource, typename T, 
                    bool IsFundamental  = is_fundamental<T>::value,
                    bool IsSeq          = fusion::traits::is_sequence<T>::value
        >
        struct convert_to_from;

        template <
            typename ItTarget, typename EndTarget, 
            typename ItSource, typename EndSource>
        struct convert_to_from_seq_loop {
            template <typename It, typename End>
            static void apply(It& it, End& end) {
                boost::integer::endianness::convert_to_from_impl<
                    typename mpl::deref<ItTarget>::type, 
                    typename mpl::deref<ItSource>::type, 
                    typename remove_reference<typename remove_cv<typename fusion::result_of::deref<It>::type>::type >::type
                >
                (fusion::deref(it));
                convert_to_from_seq_loop<
                    typename mpl::next<ItTarget>::type, EndTarget,
                    typename mpl::next<ItSource>::type, EndSource
                >::apply(fusion::next(it), end);
            }
        };

        template <typename EndTarget, typename EndSource>
        struct convert_to_from_seq_loop<EndTarget, EndTarget, EndSource, EndSource> {
            template <typename It, typename End>
            static void apply(It& , End& ) {
            }
        };

        template <typename EndianTarget, typename EndianSource, typename T>
        struct convert_to_from_seq {
            static void apply(T& r) {
                convert_to_from_seq_loop<
                    typename mpl::begin<EndianTarget>::type, 
                    typename mpl::end<EndianTarget>::type, 
                    typename mpl::begin<EndianSource>::type, 
                    typename mpl::end<EndianSource>::type >
                    ::apply(fusion::begin(r), fusion::end(r));

            }
        };

        // same endianess do nothiong
        template <typename  E, typename T, bool IsFundamental, bool IsSeq>
        struct convert_to_from<E, E, T, IsFundamental, IsSeq> {
            static void apply(T& ) {
            }
        };

        template <typename  E, typename T>
        struct convert_to_from<E, E, T, false, false> {
            static void apply(T& ) {
            }
        };

        template <typename  E, typename T>
        struct convert_to_from<E,E,T,false,true> {
            static void apply(T& ) {
            }
        };

        // on fundamentals apply endian views
        template <typename T>
        struct convert_to_from<endianness::big, endianness::little, T, true, false> {
            static void apply(T& r) {
                as_big(r)=as_little(r);
            }
        };
        // on fundamentals apply endian views
        template <typename T>
        struct convert_to_from<endianness::little, endianness::big, T, true, false> {
            static void apply(T& r) {
                as_little(r)=as_big(r);
            }
        };
        template <typename EndianTarget, typename EndianSource, typename T>
        struct convert_to_from<EndianTarget, EndianSource, T, false, true>
        : convert_to_from_seq<EndianTarget,EndianSource,T> {};

        template <typename EndianTarget, typename EndianSource, typename T>
        struct convert_to_from<EndianTarget, EndianSource, T, false, false> {
            static void apply(T& ) {
            }
            //~ template <typename T>
            //~ static void apply(T& r) {
                //~ //use ADL to find this
                //~ convert_to_from_impl<EndianTarget,EndianSource,T>(r);
            //~ }
        };

    } // namespace integer_detail

template <typename EndianTarget, typename EndianSource, typename T>
inline void convert_to_from_impl(T& r) {
    integer_detail::convert_to_from<
            EndianTarget, 
            EndianSource, 
            T
    >::apply(r);
}

template <typename EndianTarget, typename EndianSource, typename T>
void convert_to_from(T& r) {
    integer_detail::convert_to_from<
            typename endianness::domain_map<EndianTarget,T>::type, 
            typename endianness::domain_map<EndianSource,T>::type, 
            T
    >::apply(r);
}

template <typename Endian> 
struct to {
    typedef Endian type;
};

template <typename Endian> 
struct from {
    typedef Endian type;
};

template <typename Endian> 
struct is_to : mpl::false_ {};
template <typename Endian> 
struct is_to<to<Endian> > : mpl::true_ {};

template <typename Endian> 
struct is_from : mpl::false_ {};
template <typename Endian> 
struct is_from<from<Endian> > : mpl::true_ {};


template <typename Endian1, typename Endian2, typename T>
void convert(T& r) {
    typedef 
        typename mpl::if_<is_to<Endian1>, typename Endian1::type,
             typename mpl::if_<is_to<Endian2>, typename Endian2::type,
                void
             >::type
    >::type EndianTarget;
    typedef 
        typename mpl::if_<is_from<Endian1>, typename Endian1::type,
             typename mpl::if_<is_from<Endian2>, typename Endian2::type,
                void
             >::type
    >::type EndianSource;
        
    integer_detail::convert_to_from<
            typename endianness::domain_map<EndianTarget,T>::type, 
            typename endianness::domain_map<EndianSource,T>::type, 
            T
    >::apply(r);
}


//~ template <typename EndianTarget, typename EndianSource, typename T, typename N>
//~ void convert_to_from(T[N] a) {
    //~ for (unsigned int i=0; i<N; ++i) {
        //~ convert_to_from<EndianTarget, EndianSource, T>(a[i]);
    //~ }
//~ }

template <typename EndianSource, typename T>
void convert_from(T& r) {
    integer_detail::convert_to_from<
        typename endianness::domain_map<endianness::native,T>::type,      
        typename endianness::domain_map<EndianSource,T>::type, 
        T>::apply(r);
}

template <typename EndianTarget, typename T>
void convert_to(T& r) {
    integer_detail::convert_to_from<
        typename endianness::domain_map<EndianTarget,T>::type, 
        typename endianness::domain_map<endianness::native, T>::type, 
        T>::apply(r);
}

} // namespace endianness
} // namespace integer
} // namespace boost


#endif // BOOST_INTEGER_ENDIAN_CONVERT_HPP
