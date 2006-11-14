// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/vector_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

#include <boost/bimap/container_adaptor/vector_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/bimap/container_adaptor/detail/comparison_adaptor.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also const_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class vector_map_view
:
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by, const_reverse_iterator_type_by
    ),

    public ::boost::bimap::detail::map_view_base< vector_map_view<Tag,BimapType>,Tag,BimapType >
{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by, const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(vector_map_view,Tag,BimapType);

    typedef typename ::boost::bimap::relation::support::data_extractor
    <
        Tag,
        typename BimapType::relation

    >::type key_from_base_value;

    public:

    vector_map_view(typename base_::base_type & c) :
        base_(c) {}

    vector_map_view & operator=(const vector_map_view & v) { this->base() = v.base(); return *this; }

    BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION

    // Lists operations

    void splice(typename base_::iterator position, vector_map_view & x)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base()
        );
    }

    void splice(typename base_::iterator position, vector_map_view & x, typename base_::iterator i)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base(),
            this->template functor<typename base_::iterator_to_base>()(i)
        );
    }

    void splice(typename base_::iterator position, vector_map_view & x,
                typename base_::iterator first, typename base_::iterator last)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base(),
            this->template functor<typename base_::iterator_to_base>()(first),
            this->template functor<typename base_::iterator_to_base>()(last)
        );
    }

    void remove(typename ::boost::call_traits< typename base_::value_type >::param_type value)
    {
        this->base().remove(
            this->template functor<typename base_::value_to_base>()(value)
        );
    }

    template<typename Predicate>
    void remove_if(Predicate pred)
    {
        this->base().remove_if(
            ::boost::bimap::container_adaptor::detail::unary_check_adaptor
            <
                Predicate,
                typename base_::base_type::value_type,
                key_from_base_value

            >( pred, key_from_base_value() )
        );
    }

    void unique()
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::equal_to<typename base_::key_type>,
                typename base_::base_type::value_type,
                key_from_base_value

            >( std::equal_to<typename base_::key_type>(), key_from_base_value() )
        );
    }

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred)
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                BinaryPredicate,
                typename base_::base_type::value_type,
                key_from_base_value

            >( binary_pred, key_from_base_value() )
        );
    }

    void merge(vector_map_view & x)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::key_type>,
                typename base_::base_type::value_type,
                key_from_base_value

            >( std::less<typename base_::key_type>(), key_from_base_value() )
        );
    }

    template <typename Compare>
    void merge(vector_map_view & x, Compare comp)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename base_::base_type::value_type,
                key_from_base_value

            >( comp, key_from_base_value() )
        );
    }

    void sort()
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::key_type>,
                typename base_::base_type::value_type,
                key_from_base_value

            >( std::less<typename base_::key_type>(), key_from_base_value() )
        );
    }

    template <typename Compare>
    void sort(Compare comp)
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename base_::base_type::value_type,
                key_from_base_value

            >( comp, key_from_base_value() )
        );
    }

    void reverse()
    {
        this->base().reverse();
    }

    // Rearrange Operations

    void relocate(typename base_::iterator position, typename base_::iterator i)
    {
        this->base().relocate(
            this->template functor<typename base_::iterator_to_base>()(position),
            this->template functor<typename base_::iterator_to_base>()(i)
        );
    }

    void relocate(typename base_::iterator position,
                  typename base_::iterator first, typename base_::iterator last)
    {
        this->base().relocate(
            this->template functor<typename base_::iterator_to_base>()(position),
            this->template functor<typename base_::iterator_to_base>()(first),
            this->template functor<typename base_::iterator_to_base>()(last)
        );
    }

};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

