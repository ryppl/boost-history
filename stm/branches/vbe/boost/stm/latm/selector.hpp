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

#ifndef BOOST_STM_LATM_SELECTOR__HPP
#define BOOST_STM_LATM_SELECTOR__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------

#if defined(BOOST_STM_LATM_STATIC_FULL_MODE)
#include <boost/stm/latm/static_full_mode.hpp>
#elif defined(BOOST_STM_LATM_STATIC_TM_MODE)
#include <boost/stm/latm/static_tm_mode.hpp>
#elif defined(BOOST_STM_LATM_STATIC_TX_MODE)
#include <boost/stm/latm/static_tx_mode.hpp>
#elif defined(BOOST_STM_LATM_DYNAMIC_MODE)
#include <boost/stm/latm/dynamic_mode.hpp>
#else
#include <boost/stm/latm/all_in_one_mode.hpp>
#endif

#endif // BOOST_STM_LATM_SELECTOR__HPP
