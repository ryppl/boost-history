//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_PP_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_PP_ER_2010_HPP

#include <boost/range/size.hpp>
#include <boost/range/size_type.hpp>
#define BOOST_ASSIGN_V2_CHECK_AUX_CONTAINER_SIZE\
		typedef boost::range_size<R>::type size_type;\
        size_type n1 = cont.size();\
        size_type n2 = boost::range_size( r );\
        BOOST_ASSIGN_V2_CHECK( n2 >= n1 );\
/**/        

#include <boost/range/iterator_range.hpp>
#define BOOST_ASSIGN_V2_CHECK_AUX_CONTAINER_ELEM_AT\
		typedef typename boost::iterator_range<\
        	typename boost::range_iterator<R const>::type\
        >::type elem_at_;\
        elem_at_ elem_at = boost::make_iterator_range( r );\
/**/

#endif
