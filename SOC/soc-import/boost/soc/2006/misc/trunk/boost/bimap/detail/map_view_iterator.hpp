// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file detail/map_view_iterator.hpp
/// \brief Iterator adaptors from multi index to bimap.

#ifndef BOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP
#define BOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP

// Boost
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/iterator/detail/enable_if.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/bimap/relation/support/get_pair_functor.hpp>

namespace boost {
namespace bimap {
namespace detail {


/** \brief Iterator adaptor from multi index to bimap.

This is class is based on transform iterator from Boost.Iterator that is
modified to allow serialization. It has been specialized for this
library, and EBO optimization was applied to the functor.

                                                                      **/

template
<
    class Tag, class Relation,
    class CoreIterator,
    class Reference,
    class ValueType
>
struct map_view_iterator :

    public iterator_adaptor
    <
        map_view_iterator< Tag, Relation, CoreIterator, Reference, ValueType >,
        CoreIterator,
        typename remove_reference<Reference>::type,
        ::boost::use_default,
        Reference

    >,
    protected ::boost::bimap::relation::support::GetPairFunctor<Tag,Relation>

{
    typedef iterator_adaptor
    <
        map_view_iterator< Tag, Relation, CoreIterator, Reference, ValueType >,
        CoreIterator,
        typename remove_reference<Reference>::type,
        boost::use_default,
        Reference

    > base_;

    typedef ::boost::bimap::relation::support::GetPairFunctor<Tag,Relation> get_pair_functor;

    public:

    // The best way will be to pass the correct "value_type" to iterator_adaptor and
    // to set the "pointer" to Reference*, but iterator_adaptor and iterator_facade
    // defines "pointer" as value_type* and do not allow this to be changed.

    typedef ValueType value_type;

    map_view_iterator() {}

    map_view_iterator(CoreIterator const& iter)
      : base_(iter) {}

    // If multi_index change the iterator == const_iterator scheme, this
    // function have to be rewritted

    map_view_iterator(map_view_iterator const & iter)
      : base_(iter.base()) {}

    private:

    friend class iterator_core_access;

    typename base_::reference dereference() const
    {
        return get_pair_functor::operator()(*this->base());
    }

    #ifndef BOOST_BIMAP_DISABLE_SERIALIZATION

    // Serialization support

    BOOST_SERIALIZATION_SPLIT_MEMBER();

    friend class ::boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar << ::boost::serialization::make_nvp("mi_iterator",this->base());
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        CoreIterator iter;
        ar >> ::boost::serialization::make_nvp("mi_iterator",iter);
        this->base_reference() = iter;
    }

    #endif // BOOST_BIMAP_DISABLE_SERIALIZATION
};


} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP


