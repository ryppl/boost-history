//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_MOVE__HPP
#define BOOST_STM_MOVE__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#if BUILD_MOVE_SEMANTICS
#include <type_traits>
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

#if BUILD_MOVE_SEMANTICS
bool const kDracoMoveSemanticsCompiled = true;
#else
bool const kDracoMoveSemanticsCompiled = false;
#endif
    
#if BUILD_MOVE_SEMANTICS
template <class T>
inline typename std::remove_reference<T>::type&& draco_move(T &&t)
{
   return t;
}
#endif

} // namespace core
}
#endif // BOOST_STM_DATATYPES__HPP


