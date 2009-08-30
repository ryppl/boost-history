//simple template parameter pack wrapper.
#ifndef BOOST_MPL_PACKAGE_HPP_INCLUDED
#define BOOST_MPL_PACKAGE_HPP_INCLUDED
//  (C) Copyright Larry Evans 2007.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/package/package0.hpp>
namespace boost
{
namespace mpl
{
      template
      < typename... Args
      >
      struct
    package
    {
            typedef
          aux::package_tag
        tag
        /**@brief
         *  Used for tag dispatching.
         */
        ;
            typedef
          package
          < Args...
          >
        type
        ;
            typedef
          type
        pkg_type
        /**@brief
         *  Used in package/package0.hpp.
         */
        ;
    };
    
}}//exit boost::mpl
#endif

