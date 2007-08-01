//  Boost hat.hpp header file  -----------------------------------------------//

// Copyright 2007 Daryle Walker.
// Distributed under the Boost Software License, Version 1.0. (See the
// accompanying file LICENSE_1_0.txt or a copy at
// <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/hat/> for the library's home page.

/** \file
    \brief Grouping header for hat-style random access containers

    Include declarations (and definitions) of hat containers with
    equally-weighted and arbitrarily-weighted elements.
 */

/** \dir hat
    \brief Components for \<boost/hat.hpp\>

    The header files within this directory describe containers whose elements
    can be copied/extracted in an order determined by a built-in random number
    generator.  Items within those header files are in the namespace
    <code>boost::hats</code>.  The grouping header file \<boost/hat.hpp\> places
    the item names within namespace <code>boost</code> with <code>using</code>
    directives.
 */


#ifndef BOOST_HAT_HPP
#define BOOST_HAT_HPP

// Group #includes
#include <boost/hat/hat.hpp>
#include <boost/hat/weighted_hat.hpp>


/** \brief Boost's namespace

    Almost all non-preprocessor items provided within Boost's libraries should
    be in this namespace.  (Exceptions are anything that needs to be global.)
 */
namespace boost
{

/** \brief Boost.Hat's namespace

    Namespace for the hat containers and related items.
 */
namespace hats {}


//  Re-declarations of #included items into a higher namespace  --------------//

// hat.hpp
using hats::hat;

// weighted_hat.hpp
//using hats::weighted_hat;


}  // namespace boost


#endif // BOOST_HAT_HPP
