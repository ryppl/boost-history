// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGIES_TAGS_HPP
#define GGL_STRATEGIES_TAGS_HPP


namespace ggl
{

namespace strategy
{
    /*!
        \brief Indicate compiler/library user that strategy is not implemented.
        \details Strategies are defined for point types or for point type
        combinations. If there is no implementation for that specific point type, or point type
        combination, the calculation cannot be done. To indicate this, this not_implemented
        class is used as a typedef stub.

    */
    struct not_implemented {};
}




struct strategy_tag_unknown {};
struct strategy_tag_distance_point_point {};
struct strategy_tag_distance_point_segment {};

template <typename T>
struct strategy_tag
{
    typedef strategy_tag_unknown type;
};

} // namespace ggl

#endif // GGL_STRATEGIES_TAGS_HPP
