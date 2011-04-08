/*  Boost CollectionMaker library vector.hpp header file.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    vector.hpp
 *  @brief   Provide std::vector collections.
 *  @author  Pierre-Andre Noel
 *  @since   2011-03-24
 *  @version $Id$
 */

#ifndef BOOST_COLLECTION_MAKER_VECTOR_HPP
#define BOOST_COLLECTION_MAKER_VECTOR_HPP

#include <vector>

namespace boost
{
  namespace collection_maker
  {
    class vector
    {
      public:
        template < typename T >
        class collection_of
        {
          public:
            /// The collection type to use.
            typedef std::vector< T > type;
            /// Vector has a constructor that accept the size.
            static type make(const typename type::size_type n)
            { return type(n); }
            /// Call vector::resize(n).
            static void resize(type& x, const typename type::size_type n)
            { x.resize(n); }
            /// Inform if the collection has static size.
            static const bool is_static_size = false;
        };
    };
  }  // namespace collection_maker
} //  namespace boost

#endif //  BOOST_COLLECTION_MAKER_VECTOR_HPP
