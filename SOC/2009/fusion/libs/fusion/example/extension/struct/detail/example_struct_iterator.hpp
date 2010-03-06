/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_EXAMPLE_STRUCT_ITERATOR_HPP
#define EXTENSION_STRUCT_DETAIL_EXAMPLE_STRUCT_ITERATOR_HPP

#include <boost/fusion/support.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/add_const.hpp>

namespace example
{
    template<typename StructRef, int Pos>
    struct example_struct_iterator
      : boost::fusion::iterator_base<example_struct_iterator<StructRef, Pos> >
    {
        typedef StructRef struct_type;
        typedef boost::mpl::int_<Pos> index;

        typedef example::example_struct_iterator_tag fusion_tag;
        struct category
          : boost::fusion::random_access_traversal_tag
          , boost::fusion::associative_tag
        {};

        example_struct_iterator(StructRef struct_)
          : struct_(struct_)
        {}

        StructRef struct_;
    };
}

#endif
