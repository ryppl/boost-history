// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/map_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
#define BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP

#include <boost/config.hpp>
#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>
#include <boost/bimap/relation/support/opposite_tag.hpp>
#include <boost/bimap/relation/support/pair_type_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>

namespace boost {
namespace bimap {

namespace detail {

// The next macro can be converted in a metafunctor to gain code robustness.

/********************************************************************************/
#define BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(                                  \
    CONTAINER_ADAPTOR, TAG,BIMAP, OTHER_ITER, CONST_OTHER_ITER                   \
)                                                                                \
::boost::bimap::container_adaptor::CONTAINER_ADAPTOR                             \
<                                                                                \
    typename BIMAP::core_type::template index<TAG>::type,                        \
    typename ::boost::bimap::support::iterator_type_by<TAG,BIMAP>::type,         \
    typename ::boost::bimap::support::const_iterator_type_by<TAG,BIMAP>::type,   \
    typename ::boost::bimap::support::OTHER_ITER<TAG,BIMAP>::type,               \
    typename ::boost::bimap::support::CONST_OTHER_ITER<TAG,BIMAP>::type,         \
    ::boost::bimap::container_adaptor::support::iterator_facade_to_base          \
    <                                                                            \
        typename ::boost::bimap::support::                                       \
            iterator_type_by<Tag,BimapType>::type,                               \
        typename ::boost::bimap::support::                                       \
            const_iterator_type_by<Tag,BimapType>::type                          \
                                                                                 \
    >,                                                                           \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::relation::support::GetPairFunctor<TAG, typename BIMAP::relation >            \
>
/********************************************************************************/

/********************************************************************************/
#define BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(                            \
    CONTAINER_ADAPTOR, TAG, BIMAP, CONST_OTHER_ITER                              \
)                                                                                \
::boost::bimap::container_adaptor::CONTAINER_ADAPTOR                             \
<                                                                                \
    const typename BIMAP::core_type::template index<TAG>::type,                  \
    typename ::boost::bimap::support::const_iterator_type_by<TAG,BIMAP>::type,   \
    typename ::boost::bimap::support::const_iterator_type_by<TAG,BIMAP>::type,   \
    typename ::boost::bimap::support::CONST_OTHER_ITER<TAG,BIMAP>::type,         \
    typename ::boost::bimap::support::CONST_OTHER_ITER<TAG,BIMAP>::type,         \
    ::boost::bimap::container_adaptor::support::iterator_facade_to_base          \
    <                                                                            \
        typename ::boost::bimap::support::                                       \
            const_iterator_type_by<Tag,BimapType>::type,                         \
        typename ::boost::bimap::support::                                       \
            const_iterator_type_by<Tag,BimapType>::type                          \
                                                                                 \
    >,                                                                           \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::relation::support::GetPairFunctor<TAG, typename BIMAP::relation >            \
>
/********************************************************************************/

#if defined(BOOST_MSVC)
/********************************************************************************/
#define BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(TYPE,TAG,BIMAP)                         \
    typedef ::boost::bimap::detail::map_view_base<                               \
        TYPE<TAG,BIMAP>,TAG,BIMAP > friend_map_view_base;                        \
    friend class friend_map_view_base;
/********************************************************************************/
#else
/********************************************************************************/
#define BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(TYPE,TAG,BIMAP)                         \
    friend class ::boost::bimap::detail::map_view_base<                          \
        TYPE<TAG,BIMAP>,TAG,BIMAP >;
/********************************************************************************/
#endif


/// \brief Common base for map views.

template< class Derived, class Tag, class BimapType>
class map_view_base
{
    typedef ::boost::bimap::container_adaptor::support::iterator_facade_to_base<
        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type

    > iterator_to_base;

    public:

    bool replace(typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type position,
                 typename ::boost::bimap::relation::support::pair_type_by
                 <
                     Tag,
                     typename BimapType::relation

                 >::type const & x)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base>()(position),
            typename BimapType::relation(x)
        );
    }

    template<typename Modifier>
    bool modify(typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type position,Modifier mod)
    {
        return derived().base().modify(

            derived().template functor<iterator_to_base>()(position),

            ::boost::bimap::detail::relation_modifier_adaptor
            <
                Modifier,
                typename BimapType::relation,
                typename ::boost::bimap::relation::support::data_extractor
                <
                    Tag,
                    typename BimapType::relation

                >::type,
                typename ::boost::bimap::relation::support::data_extractor
                <
                    typename ::boost::bimap::relation::support::opossite_tag<Tag,BimapType>::type,
                    typename BimapType::relation

                >::type

            >(mod)
        );
    }

    protected:

    typedef map_view_base map_view_base_;

    private:

    // Curiously Recurring Template interface.

    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }
};

// The following macro should we replace by a free function

/*****************************************************************************/
#define BOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(BASE)                       \
                                                                              \
template<typename LowerBounder,typename UpperBounder>                         \
std::pair<typename BASE::iterator,typename BASE::iterator>                    \
    range(LowerBounder lower,UpperBounder upper)                              \
{                                                                             \
    std::pair<                                                                \
                                                                              \
        typename BASE::base_type::iterator,                                   \
        typename BASE::base_type::iterator                                    \
                                                                              \
    > r( this->base().range(lower,upper) );                                   \
                                                                              \
    return std::pair                                                          \
    <                                                                         \
        typename BASE::iterator,                                              \
        typename BASE::iterator                                               \
    >(                                                                        \
        this->template functor<                                               \
            typename BASE::iterator_from_base                                 \
        >()                                         ( r.first ),              \
        this->template functor<                                               \
            typename BASE::iterator_from_base                                 \
        >()                                         ( r.second )              \
    );                                                                        \
}                                                                             \
                                                                              \
template<typename LowerBounder,typename UpperBounder>                         \
std::pair<typename BASE::const_iterator,typename BASE::const_iterator>        \
    range(LowerBounder lower,UpperBounder upper) const                        \
{                                                                             \
    std::pair<                                                                \
                                                                              \
        typename BASE::base_type::const_iterator,                             \
        typename BASE::base_type::const_iterator                              \
                                                                              \
    > r( this->base().range(lower,upper) );                                   \
                                                                              \
    return std::pair                                                          \
    <                                                                         \
        typename BASE::const_iterator,                                        \
        typename BASE::const_iterator                                         \
    >(                                                                        \
        this->template functor<                                               \
            typename BASE::iterator_from_base                                 \
        >()                                         ( r.first ),              \
        this->template functor<                                               \
            typename BASE::iterator_from_base                                 \
        >()                                         ( r.second )              \
    );                                                                        \
}
/*****************************************************************************/


/*****************************************************************************/
#define BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION                            \
                                                                              \
typename base_::reference front()                                             \
{                                                                             \
    return this->template functor<                                            \
        typename base_::value_from_base>()                                    \
    (                                                                         \
                const_cast                                                    \
        <                                                                     \
            typename base_::base_type::value_type &                           \
                                                                              \
        > ( this->base().front() )                                            \
    );                                                                        \
}                                                                             \
                                                                              \
typename base_::reference back()                                              \
{                                                                             \
    return this->template functor<                                            \
        typename base_::value_from_base>()                                    \
    (                                                                         \
        const_cast                                                            \
        <                                                                     \
            typename base_::base_type::value_type &                           \
                                                                              \
        >( this->base().back() )                                              \
    );                                                                        \
}                                                                             \
                                                                              \
typename base_::const_reference front() const                                 \
{                                                                             \
    return this->template functor<                                            \
        typename base_::value_from_base>()                                    \
    (                                                                         \
        this->base().front()                                                  \
    );                                                                        \
}                                                                             \
                                                                              \
typename base_::const_reference back() const                                  \
{                                                                             \
    return this->template functor<                                            \
        typename base_::value_from_base>()                                    \
    (                                                                         \
        this->base().back()                                                   \
    );                                                                        \
}                                                                             \

/*****************************************************************************/

} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
