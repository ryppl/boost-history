// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/vector_set_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::vector.

#ifndef BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP

#include <boost/bimap/container_adaptor/vector_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/comparison_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>


namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::vector.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::vector.

See also const_set_view.
                                                                                    **/

template< class CoreIndex >
class vector_set_view
:
    public BOOST_BIMAP_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        vector_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator
    ),

    public ::boost::bimap::detail::set_view_base< vector_set_view< CoreIndex >, CoreIndex >
{
    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(vector_set_view,CoreIndex);

    typedef BOOST_BIMAP_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        vector_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator

    ) base_;

    public:

    vector_set_view(typename base_::base_type & c) :
        base_(c) {}

    vector_set_view & operator=(const vector_set_view & v) { this->base() = v.base(); return *this; }

    BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION

    // List operations

    void splice(typename base_::iterator position, vector_set_view & x)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base()
        );
    }

    void splice(typename base_::iterator position, vector_set_view & x, typename base_::iterator i)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base(),
            this->template functor<typename base_::iterator_to_base>()(i)
        );
    }

    void splice(typename base_::iterator position, vector_set_view & x,
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
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Predicate,
                typename base_::base_type::value_type,
                typename base_::value_from_base

            >( pred, this->template functor<typename base_::value_from_base>() )
        );
    }

    void unique()
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::equal_to<typename base_::value_type>,
                typename base_::base_type::value_type,
                typename base_::value_from_base

            >(
                std::equal_to<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
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
                typename base_::value_from_base

            >( binary_pred, this->template functor<typename base_::value_from_base>() )
        );
    }

    void merge(vector_set_view & x)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::value_type>,
                typename base_::base_type::value_type,
                typename base_::value_from_base

            >(
                std::less<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
        );
    }

    template <typename Compare>
    void merge(vector_set_view & x, Compare comp)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename base_::base_type::value_type,
                typename base_::value_from_base

            >( comp, this->template functor<typename base_::value_from_base>() )
        );
    }

    void sort()
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::value_type>,
                typename base_::base_type::value_type,
                typename base_::value_from_base

            >(
                std::less<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
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
                typename base_::value_from_base

            >( comp, this->template functor<typename base_::value_from_base>() )
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


#endif // BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP

