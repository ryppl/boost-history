// Boost containers_fwd.hpp header file  -------------------------------------//
/** \file
    \brief  Forward declarations of Boost.Container components.

    Contains the forward declarations of Boost.Container's public structures,
    classes, and templates thereof, and any type aliases.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/containers> for documentation.

#ifndef BOOST_CONTAINERS_FWD_HPP
#define BOOST_CONTAINERS_FWD_HPP


namespace boost
{
/** \brief Name-space for Boost.Container

    Primary name-space for the public components of a library providing various
    container classes and class templates.
 */
namespace containers
{


//  From <boost/containers/hat.hpp>  -----------------------------------------//

template < typename T, class Container /*= std::deque<T>*/, class
 RandomNumberGenerator /*= boost::mt19937*/ >
    class hat;


}  // namespace containers
}  // namespace boost


#endif  // BOOST_CONTAINERS_FWD_HPP
