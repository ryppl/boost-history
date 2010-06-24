//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_FLAG_FIELD_HPP
#define BOOST_FLAG_FIELD_HPP



namespace boost {



/** Allows the user to simplify the creation of a bool member by simply 
 *  requiring that they specify a name and assuming that if the flag is used
 *  that the underlying type's width is going to be 1.
 */
template <typename ReturnType>
struct flag;



} // end boost

#endif
