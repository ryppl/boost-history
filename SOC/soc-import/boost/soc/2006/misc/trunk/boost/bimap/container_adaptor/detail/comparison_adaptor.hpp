// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/detail/comparison_adaptor.hpp
/// \brief Comparison adaptor.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_COMPARISON_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_COMPARISON_ADAPTOR_HPP

#include <boost/call_traits.hpp>
#include <functional>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Comparison adaptor
/**

A simple comparison adaptor.
                                                                                    **/

// TODO
// * The comparison adaptor can be optimized for NewType equal to
//   CompareFunctor::argument_firsttype
// * Use ConceptCheck to ensure the validity of CompareFunctor

template < class CompareFunctor, class NewType, class Converter >
struct comparison_adaptor : std::binary_function<NewType,NewType,bool>
{
    comparison_adaptor( CompareFunctor c, Converter conv ) : comp(c), converter(conv) {}

    bool operator()( typename call_traits<NewType>::param_type x,
                     typename call_traits<NewType>::param_type y)
    {
        return comp(
            converter(x),
            converter(y)
        );
    }

    private:

    // TODO
    // Apply EBO optimization

    CompareFunctor  comp;
    Converter       converter;
};

} // namespace detail
} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_COMPARISON_ADAPTOR_HPP


