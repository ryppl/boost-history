/*-----------------------------------------------------------------------------+
Author: Joachim Faulhaber
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef  BOOST_ITL_IMPL_CONFIG_HPP_JOFA_091225
#define  BOOST_ITL_IMPL_CONFIG_HPP_JOFA_091225

/*-----------------------------------------------------------------------------+
You can choose an implementation for the basic set and map classes.
Select at most ONE of the following defines
+-----------------------------------------------------------------------------*/

//#define ITL_USE_STD_IMPLEMENTATION
//#define ITL_USE_BOOST_INTERPROCESS_IMPLEMENTATION
//#define ITL_USE_BOOST_MOVE_IMPLEMENTATION

/*-----------------------------------------------------------------------------+
NO define or ITL_USE_STD_IMPLEMENTATION: Choose std::set and std::map as
    implementing containers (default).

ITL_USE_BOOST_INTERPROCESS_IMPLEMENTATION: Choose set and map implementations 
    from boost::interprocess.

ITL_USE_BOOST_MOVE_IMPLEMENTATION: Move aware containers from boost::container
    (NEW) are used. Currently (January 2010) this is only experimental. 
    boost::move from the boost::sandbox has to be used. This is depreciated for 
    production code, as long as move aware containers are not officially 
    accepted into boost. 
+-----------------------------------------------------------------------------*/

#if defined(ITL_USE_BOOST_INTERPROCESS_IMPLEMENTATION)
#define ITL_IMPL_SPACE boost::interprocess
#elif defined(ITL_USE_BOOST_MOVE_IMPLEMENTATION)
#define ITL_IMPL_SPACE boost::container
#else
#define ITL_IMPL_SPACE std
#endif

#endif // BOOST_ITL_IMPL_CONFIG_HPP_JOFA_091225


