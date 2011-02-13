//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010-2011

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_ENDIAN_DOMAIN_MAP__HPP
#define BOOST_ENDIAN_DOMAIN_MAP__HPP

#include <boost/integer/endian/endianness.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/void.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost {

namespace integer {

namespace endianness {

    namespace endian_detail {

        template <typename Domain, typename T,
            bool isFundamental  = is_fundamental<T>::value,
            bool IsSeq          = fusion::traits::is_sequence<T>::value
        >
        struct domain_map_impl;

    }

    /// By default the shared endianess of a type depends on whether it is fundamental and or a fusion sequence.
    template <typename Domain, typename T>
    struct domain_map : endian_detail::domain_map_impl<Domain, T> {};

    namespace endian_detail {

        // fundamental types are native
        template <typename Domain, typename T, bool IsSeq>
        struct domain_map_impl<Domain, T, true, IsSeq> {
            typedef Domain type;
        };

        // other type which has not been explicitly declared is undefined,
        template <typename Domain, typename T>
        struct domain_map_impl<Domain, T, false, false> {};

        template <typename Domain, typename SharedTree, typename It, typename End>
        struct domain_map_loop {
        private:
            typedef
            typename remove_reference<
                typename remove_cv<
                    typename fusion::result_of::deref<It>::type
                >::type
            >::type it_type;
        public:
            typedef typename domain_map_loop<Domain, 
                typename mpl::push_back<SharedTree, typename domain_map<Domain, it_type>::type>::type,
                typename fusion::result_of::next<It>::type,
                End
            >::type type;
        };

        // When iteration ends, accumulated SharedTree
        template <typename Domain, typename SharedTree, typename End>
        struct domain_map_loop<Domain, SharedTree, End, End> {
            typedef SharedTree type;
        };

        // fusion sequences are mixed if not all the elements habve the same endianess,
        // otherwise the endianess of all the types.
        template <typename Domain, typename Seq>
        struct domain_map_impl<Domain, Seq, false, true> :
            domain_map_loop<Domain,  mpl::vector<>,
                            typename fusion::result_of::begin<Seq>::type,
                            typename fusion::result_of::end<Seq>::type
                    >
        {};

    }
    /// All the elements of an array are seen with the same endianess.
    template <typename Domain, typename T, std::size_t N>
    struct domain_map<Domain,T[N]> {
        typedef typename domain_map<Domain, T>::type type;
    };

} // namespace endianness
} // namespace integer
} // namespace boost


#endif // BOOST_ENDIAN_DOMAIN_MAP__HPP
