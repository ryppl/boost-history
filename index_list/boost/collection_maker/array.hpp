/*  Boost CollectionMaker library array.hpp header file.
 *
 *                    Copyright Pierre-Andre Noel 2011.
 *         Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *                  http://www.boost.org/LICENSE_1_0.txt)
 */
/** @file    array.hpp
 *  @brief   Provide boost::array collections.
 *  @author  Pierre-Andre Noel
 *  @since   2011-03-24
 *  @version $Id$
 */

#ifndef BOOST_COLLECTION_MAKER_ARRAY_HPP
#define BOOST_COLLECTION_MAKER_ARRAY_HPP

#include <boost/array.hpp>
#include <boost/assert.hpp>

namespace boost
{
  namespace collection_maker
  {
    template< std::size_t Size = 1 >
    class array
    {
      public:
        template < typename T >
        class collection_of
        {
          public:
            /// The collection type to use.
            typedef boost::array< T, Size > type;
            /// Size must match the array's static size.
            static type make(const typename type::size_type n)
            { BOOST_ASSERT(n == Size); return type(); }
            /// An array cannot be resized.
            static void resize(type&, const typename type::size_type n)
            { BOOST_ASSERT(n == Size); }
            /// Inform if the collection has static size.
            static const bool is_static_size = true;
        };
    };
  }  // namespace collection_maker
} //  namespace boost

#endif //  BOOST_COLLECTION_MAKER_ARRAY_HPP
