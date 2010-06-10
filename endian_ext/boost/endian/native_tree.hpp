//  Boost endian_view.hpp header file -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/endian

//--------------------------------------------------------------------------------------//


#ifndef BOOST_ENDIAN_NATIVE_TREE__HPP
#define BOOST_ENDIAN_NATIVE_TREE__HPP

#include <boost/endian/endian.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/void.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/is_sequence.hpp>

namespace boost {


namespace endian {

    namespace endian_detail {

        template <typename T,
            bool isFundamental  = is_fundamental<T>::value,
            bool IsSeq          = fusion::traits::is_sequence<T>::value
        >
        struct native_tree_impl;

    }

    /// By default the shared endianess of a type depends on whether it is fundamental and or a fusion sequence.
    template <typename T>
    struct native_tree : endian_detail::native_tree_impl<T> {};

    namespace endian_detail {

        // fundamental types are native
        template <typename T, bool IsSeq>
        struct native_tree_impl<T, true, IsSeq> {
            typedef native type;
        };

        // other type which has not been explicitly declared is undefined,
        template <typename T>
        struct native_tree_impl<T, false, false> {};

        template <typename SharedTree, typename It, typename End>
        struct native_tree_loop {
        private:
            typedef
            typename remove_reference<
                typename remove_cv<
                    typename fusion::result_of::deref<It>::type
                >::type
            >::type it_type;
        public:
            typedef typename native_tree_loop<
                typename mpl::push_back<SharedTree, typename native_tree<it_type>::type>::type,
                typename fusion::result_of::next<It>::type,
                End
            >::type type;
        };

        // When iteration ends, accumulated SharedTree
        template <typename SharedTree, typename End>
        struct native_tree_loop<SharedTree,End,End> {
            typedef SharedTree type;
        };

        // fusion sequences are mixed if not all the elements habve the same endianess,
        // otherwise the endianess of all the types.
        template <typename Seq>
        struct native_tree_impl<Seq, false, true> :
            native_tree_loop< mpl::vector<>,
                            typename fusion::result_of::begin<Seq>::type,
                            typename fusion::result_of::end<Seq>::type
                    >
        {};

    }
    /// All the elements of an array are seen with the same endianess.
    template <typename T, std::size_t N>
    struct native_tree<T[N]> {
        typedef typename native_tree<T>::type type;
    };

} // namespace endian

} // namespace boost



#endif // BOOST_ENDIAN_NATIVE_TREE__HPP
