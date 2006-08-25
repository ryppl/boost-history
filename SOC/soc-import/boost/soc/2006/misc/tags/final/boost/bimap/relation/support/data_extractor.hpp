// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/support/data_extractor.hpp
/// \brief Data extraction functor.

#ifndef BOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::relation::support::data_extractor

\brief Data extraction functor.

\ingroup relation_group
                                                                    **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace relation {
namespace support {

template< class Tag, class Relation >
struct data_extractor_implementation;

template< class Relation >
struct data_extractor_implementation< member_at::left, Relation > :
    public std::unary_function<Relation,typename Relation::left_value_type>
{
    typename Relation::left_value_type const & operator()(Relation const & rel) const
    {
        return rel.left;
    }

    typename Relation::left_value_type       & operator()(Relation       & rel) const
    {
        return rel.left;
    }
};

template< class Relation >
struct data_extractor_implementation< member_at::right, Relation > :
    public std::unary_function<Relation,typename Relation::right_value_type>
{
    typename Relation::right_value_type const & operator()(Relation const & rel) const
    {
        return rel.right;
    }

    typename Relation::right_value_type       & operator()(Relation       & rel) const
    {
        return rel.right;
    }
};

template< class Tag, class Relation >
struct data_extractor
{
    typedef data_extractor_implementation
    <
        typename member_with_tag<Tag,Relation>::type,
        Relation

    > type;
};

} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP

