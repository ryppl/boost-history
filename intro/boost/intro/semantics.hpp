//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_SEMANTICS_HPP
#define BOOST_INTRO_SEMANTICS_HPP

#include <boost/mpl/set.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_polymorphic.hpp>


namespace boost{
namespace intro{

namespace detail{
struct semantics_base{};
}
    
template<class Semantics=mpl::set0<>,class IndirSemantics=mpl::na>
struct semantics : detail::semantics_base{
    typedef typename mpl::if_<
        mpl::is_sequence<Semantics>,
        Semantics,
        mpl::set1<Semantics>
    >::type set;
    typedef typename mpl::if_<
        is_same<IndirSemantics,mpl::na>,
        semantics<>,
        IndirSemantics
    >::type indir_semantics;
};

struct shared{
    template<class Set>
    struct apply : mpl::insert<Set,shared>{};
};
struct unique{
    template<class Set>
    struct apply : mpl::erase_key<Set,shared>{};
};
struct polymorphic{
    template<class Set>
    struct apply : mpl::insert<Set,polymorphic>{};
};
struct sealed{
    template<class Set>
    struct apply : mpl::erase_key<Set,polymorphic>{};
};


template<class T>
struct is_semantics
    : is_base_of<detail::semantics_base,T>{};

template<class T>
struct default_semantics{
    typedef semantics<> type;
};

template<class T>
class default_semantics<T *>{
    typedef typename default_semantics<T>::type deref_semantics;
    typedef typename shared::template apply<typename deref_semantics::set>::type shared_added;
    typedef typename mpl::if_<
        is_polymorphic<T>,
        typename polymorphic::template apply<shared_added>::type,
        shared_added
    >::type added;
public:
    typedef semantics<
        mpl::set0<>,
        semantics<
            added,
            typename deref_semantics::indir_semantics
        >
    > type;
};

template<class T>
struct default_semantics<reference<T> >{
    typedef typename default_semantics<T *>::type type;
};

typedef semantics<> shared_pointer;
typedef shared_pointer shared_reference;
typedef semantics<mpl::set0<>,semantics<unique> > unique_pointer;
typedef unique_pointer unique_reference;

}
}
#endif
