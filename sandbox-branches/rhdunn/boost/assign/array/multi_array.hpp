/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_ARRAY_MULTI_ARRAY_HPP
#define BOOST_ASSIGN_ARRAY_MULTI_ARRAY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/assign/fixed_size_assigner.hpp>
#include <boost/assign/make_tuple_insertion.hpp>
#include <boost/assign/fixed_size_tuple_assigner.hpp>
#include <boost/multi_array.hpp>
#include <boost/multi_array_ref.hpp>

namespace boost
{
namespace assignment
{
    template< typename V, std::size_t D, typename A, typename V2 > 
    inline fixed_size_assigner<V, typename boost::multi_array<V,D,A>::iterator>
    operator<<( multi_array<V,D,A>& c, const V2& v )
    {
        return fixed_size_assigner<V, 
            typename multi_array<V,D,A>::iterator>( c.begin(), c.end(), v );
    }



    template< typename V, std::size_t D, typename A, typename V2 > 
    inline fixed_size_assigner<V, typename multi_array_ref<V,D,A>::iterator>
    operator<<( multi_array_ref<V,D,A>& c, const V2& v )
    {
        return fixed_size_assigner<V, 
            typename multi_array_ref<V,D,A>::iterator>( c.begin(), c.end(), v );
    }



    template< typename V, std::size_t D, typename A, typename V2 > 
    inline fixed_size_tuple_assigner<typename multi_array<V,D,A>::iterator>
    assign_all( multi_array<V,D,A>& c, const V2& v )
    {
        return fixed_size_tuple_assigner< 
            typename multi_array<V,D,A>::iterator>( c.begin(), c.end() );
    }



    template< typename V, std::size_t D, typename A, typename V2 > 
    inline fixed_size_tuple_tuple_assigner<typename multi_array_ref<V,D,A>::iterator>
    assign_all( multi_array_ref<V,D,A>& c, const V2& v )
    {
        return fixed_size_tuple_assigner< 
            typename multi_array_ref<V,D,A>::iterator>( c.begin(), c.end() );
    }

}   
}

#endif
