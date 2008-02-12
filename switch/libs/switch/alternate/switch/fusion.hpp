// fusion.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distriuted under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost/org/LICENSE_1_0.txt)

#ifndef BOOST_SWITCH_FUSION_HPP_INCLUDED
#define BOOST_SWITCH_FUSION_HPP_INCLUDED

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost {

template<class S>
class fusion_case_t;

namespace switch_detail {

template<int N>
struct get_nth {
    template<class Iter>
    struct apply {
        typedef typename boost::fusion::result_of::next<
            typename get_nth<N - 1>::template apply<Iter>::type
        >::type type;
        static type call(const Iter& iter) {
            return(boost::fusion::next(
                get_nth<N - 1>::template apply<Iter>::call(iter)));
        }
    };
};

template<>
struct get_nth<0> {
    template<class Iter>
    struct apply {
        typedef Iter type;
        static Iter call(const Iter& iter) {
            return(iter);
        }
    };
};

template<class T, class R>
struct internal_at;

template<class T>
struct internal_size;

template<class T, class R>
struct internal_at<fusion_case_t<T>, R> {
    template<int N>
    struct apply {
        typedef typename boost::remove_cv<
            typename boost::remove_reference<
                typename boost::fusion::result_of::deref<
                    typename get_nth<N>::template apply<
                        typename boost::fusion::result_of::begin<S>::type
                    >::type
                >::type
            >::type
        >::type f_type;
        typedef typename f_type::label label;
        static R call(const fusion_case_t<T>& t) {
            return(boost::fusion::deref(get_nth<N>::template apply<
                typename boost::fusion::result_of::begin<S>::type
            >::call(boost::fusion::begin(t.get_seq())))());
        }
        static bool fallthrough() { return(f_type::fallthrough()); }
    };
};

template<class T>
struct internal_size<fusion_case_t<T> > : boost::fusion::result_of::size<T> {};

struct fusion_case_tag {};

template<class It, class End>
struct fusion_case_iterator {
    typedef typename boost::remove_cv<
        typename boost::remove_reference<
            typename boost::fusion::result_of_deref<It>::type
        >::type
    >::type::label type;
    typedef typename fusion_case_iterator<
        typename boost::fusion::result_of::next<It>::type, End
    > next;
};

template<class End>
struct fusion_case_iterator<End, End> {};

template<class T>
struct wrap {};

struct int_map_base {
    static void lookup();
};

template<class Key, int Value, class Base>
struct int_map : Base {
    static char (&lookup(wrap<Key>))[Value + 1];
    using Base::lookup;
};

template<int N>
struct make_index_map {
    template<class T, int I>
    struct apply {
        typedef int_map<typename mpl::deref<T>::type, I,
            typename make_index_map<N - 1>::template apply<
                typename mpl::next<T>::type, I + 1
            >::type
        > type;
    };
};

template<>
struct make_index_map<0> {
    template<class T, int I>
    struct apply {
        typedef int_map_base type;
    };
};

}

template<class S>
class fusion_case_t {
public:
    struct labels {
        typedef fusion_case_tag tag;
        typedef labels type;
        typedef S impl;
    };
    fusion_case_t(S& sequence) : s(sequence) {}
    S& get_seq() const { return(s); }
    template<class R, class N>
    R apply(N) {
        typedef typename make_index_map<
            mpl::size<labels>::value
        >::template apply<typename mpl::begin<labels>::type, 0>::type map;
        return(switch_detail::internal_at<fusion_case_t, R>::template apply<
            sizeof(map::lookup(wrap<N>())) - 1
        >::call());
    }
private:
    S& s;
};

template<class S>
fusion_case_t<S> fusion_case(S& s) {
    return(fusion_case_t<S>(s));
}

template<class S>
fusion_case_t<const S> fusion_case(const S& s) {
    return(fusion_case_t<const S>(s));
}

namespace mpl {

template<>
struct size_impl<boost::switch_detail::fusion_case_tag> {
    template<class T>
    struct apply : boost::fusion::result_of::size<typename T::impl> {};
};

template<>
struct begin_impl<boost::switch_detail::fusion_case_tag> {
    template<class T>
    struct apply {
        typedef boost::switch_detail::fusion_case_iterator<
            typename boost::fusion::result_of::begin<typename T::impl>::type,
            typename boost::fusion::result_of::end<typename T::impl>::type
        > type;
    };
};

template<>
struct end_impl<boost::switch_detail::fusion_case_tag> {
    template<class T>
    struct apply {
        typedef boost::switch_detail::fusion_case_iterator<
            typename boost::fusion::result_of::end<typename T::impl>::type,
            typename boost::fusion::result_of::end<typename T::impl>::type
        > type;
    };
};

}

}

#endif
