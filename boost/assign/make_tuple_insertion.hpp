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

#ifndef BOOST_ASSIGN_MAKE_TUPLE_INSERTION_HPP
#define BOOST_ASSIGN_MAKE_TUPLE_INSERTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost
{
namespace assignment
{

//    
//#if BOOST_NO_FUNCTION_TEMPLATE_ORDERING
//
// do nothing: a default version ***** up vc6 
//
//#else 
//    

    template< typename InputIterator, typename T >
    inline void make_tuple_insertion( InputIterator& i, const T& t )
    {
        *i++ = t;
    }



    template< typename InputIterator, typename T, typename T2 >
    inline void make_tuple_insertion( InputIterator& i, const T& t, const T& t2 )
    {
        *i++ = t; *i++ = t2;
    }



    template< typename InputIterator, typename T, typename T2, typename T3 >
    inline void make_tuple_insertion( InputIterator& i, const T& t, const T2& t2, 
                                      const T3& t3 )
    {
        *i++ = t; *i++ = t2; *i++ = t3;
    }



    template< typename InputIterator, typename T, typename T2, typename T3, 
              typename T4 >
    inline void make_tuple_insertion( InputIterator& i, const T& t, const T2& t2, 
                                      const T3& t3, const T4& t4 )
    {
        *i++ = t; *i++ = t2; *i++ = t3; *i++ = t4;
    }

    
    
    template< typename InputIterator, typename T, typename T2, typename T3, 
              typename T4, typename T5 >
    inline void make_tuple_insertion( InputIterator& i, const T& t, const T2& t2,
                                      const T3& t3, const T4& t4, const T5& t5 )
    {
        *i++ = t; *i++ = t2; *i++ = t3; *i++ = t4; *i++ = t5;
    }


    template< typename InputIterator, typename T, typename T2, typename T3, 
              typename T4, typename T5, typename T6 >
    inline void make_tuple_insertion( InputIterator& i, const T& t, const T2& t2,
                                      const T3& t3, const T4& t4, const T5& t5,
                                      const T6& t6 )
    {
        *i++ = t; *i++ = t2; *i++ = t3; *i++ = t4; *i++ = t5; *i++ = t6;
    }


//#endif

} // namespace 'assignment'
} // namespace 'boost'

#endif
