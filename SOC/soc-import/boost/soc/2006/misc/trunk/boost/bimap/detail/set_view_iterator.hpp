// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/set_view_iterator.hpp
/// \brief Iterator adaptors from multi-index to bimap.

#ifndef BOOST_BIMAP_DETAIL_SET_VIEW_ITERATOR_HPP
#define BOOST_BIMAP_DETAIL_SET_VIEW_ITERATOR_HPP

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

template< class CoreIterator > struct set_view_iterator;

template< class CoreIterator >
struct set_view_iterator_base
{
    typedef iterator_adaptor
    <
        set_view_iterator< CoreIterator >,
        CoreIterator,
        typename remove_reference
        <
            typename CoreIterator::value_type::above_view_reference

        >::type,
        ::boost::use_default,
        typename CoreIterator::value_type::above_view_reference

    > type;
};

template< class CoreIterator >
struct set_view_iterator : public set_view_iterator_base<CoreIterator>::type
{
    typedef typename set_view_iterator_base<CoreIterator>::type base_;

    public:

    // The best way will be to pass the correct "value_type" to iterator_adaptor and
    // to set the "pointer" to Reference*, but iterator_adaptor and iterator_facade
    // defines "pointer" as value_type* and do not allow this to be changed.

    typedef typename CoreIterator::value_type::above_view value_type;

    set_view_iterator() {}

    set_view_iterator(CoreIterator const& iter)
      : base_(iter) {}

    set_view_iterator(set_view_iterator const & iter)
      : base_(iter.base()) {}

    typename base_::reference dereference() const
    {
        return const_cast<typename base_::base_type::value_type*>(&(*this->base()))->get_view();
    }

    private:

    friend class iterator_core_access;

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


template< class CoreIterator > struct const_set_view_iterator;

template< class CoreIterator >
struct const_set_view_iterator_base
{
    typedef iterator_adaptor
    <
        set_view_iterator< CoreIterator >,
        CoreIterator,
        typename remove_reference
        <
            typename CoreIterator::value_type::above_view_reference

        >::type,
        ::boost::use_default,
        typename CoreIterator::value_type::above_view_reference

    > type;
};

template< class CoreIterator >
struct const_set_view_iterator : public const_set_view_iterator_base<CoreIterator>::type
{
    typedef typename const_set_view_iterator_base<CoreIterator>::type base_;

    public:

    // The best way will be to pass the correct "value_type" to iterator_adaptor and
    // to set the "pointer" to Reference*, but iterator_adaptor and iterator_facade
    // defines "pointer" as value_type* and do not allow this to be changed.

    typedef typename CoreIterator::value_type::above_view value_type;

    const_set_view_iterator() {}

    const_set_view_iterator(CoreIterator const& iter)
      : base_(iter) {}

    const_set_view_iterator(const_set_view_iterator const & iter)
      : base_(iter.base()) {}

    const_set_view_iterator(set_view_iterator<CoreIterator> i)
      : base_(i.base()) {}

    typename base_::reference dereference() const
    {
        return this->base()->get_view();
    }

    private:

    friend class iterator_core_access;

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


