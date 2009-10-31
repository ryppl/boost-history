/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
itl_rational provides adapter code for boost::rational.
------------------------------------------------------------------------------*/

#ifndef BOOST_ITL_RATIONAL_HPP_JOFA_080913
#define BOOST_ITL_RATIONAL_HPP_JOFA_080913

#if defined(_MSC_VER)
#pragma warning( disable : 4800 ) //warning C4800: 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
#endif

#include <boost/rational.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>

namespace boost{namespace itl
{
	template<class Integral> 
	struct is_continuous<boost::rational<Integral> >
	{
		typedef is_continuous<boost::rational<Integral> > type;
		BOOST_STATIC_CONSTANT(bool, value = true);
	};
}} // namespace itl boost


#endif


