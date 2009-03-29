//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_SYNCHRONIZED__HPP
#define BOOST_SYNCHRO_SYNCHRONIZED__HPP

namespace boost { namespace synchro { namespace detail {
    bool no_opt_false() {return false;}
}}}

#define BOOST_SYNCHRONIZE_EXT(VAR_DECL) \
    if (bool __stop = boost::synchro::detail::no_opt_false()) {} else   \
    for (VAR_DECL;!__stop; __stop= true)

#define BOOST_SYNCHRONIZE_TYPE(TYPE, VAR, LOCKABLE) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(LOCKABLE))

#define BOOST_ADOPT_SYNCHRONIZE_TYPE(TYPE, VAR, LOCKABLE) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(LOCKABLE, boost::synchro::adopt_lock))
#define BOOST_DEFER_SYNCHRONIZE_TYPE(TYPE, VAR, LOCKABLE) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(LOCKABLE, boost::synchro::defer_lock))
#define BOOST_TRY_TO_SYNCHRONIZE_TYPE(TYPE, VAR, LOCKABLE) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(LOCKABLE, boost::synchro::try_to_lock))
#define BOOST_TRY_TO_SYNCHRONIZE_TYPE_UNTIL(TYPE, VAR, LOCKABLE, ABS_TIME) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(LOCKABLE, ABS_TIME))
#define BOOST_SYNCHRONIZE_TYPE_UNTIL(TYPE, VAR, LOCKABLE, ABS_TIME) \
    BOOST_SYNCHRONIZE_EXT(boost::synchro::unique_locker<TYPE> VAR(ABS_TIME, LOCKABLE))

#define BOOST_SYNCHRONIZE_VAR(VAR, LOCKABLE) \
    BOOST_SYNCHRONIZE_TYPE(boost::mutex, VAR, LOCKABLE)
#define BOOST_ADOPT_SYNCHRONIZE_VAR(VAR, LOCKABLE) \
    BOOST_ADOPT_SYNCHRONIZE_TYPE(boost::mutex, VAR, LOCKABLE)
#define BOOST_DEFER_SYNCHRONIZE_VAR(VAR, LOCKABLE) \
    BOOST_DEFER_SYNCHRONIZE_TYPE(boost::mutex, VAR, LOCKABLE)
#define BOOST_TRY_TO_SYNCHRONIZE_VAR(VAR, LOCKABLE) \
    BOOST_TRY_TO_SYNCHRONIZE_TYPE(boost::mutex, VAR, LOCKABLE)
#define BOOST_SYNCHRONIZE_VAR_UNTIL(VAR, LOCKABLE, ABS_TIME) \
    BOOST_SYNCHRONIZE_TYPE_UNTIL(boost::mutex, VAR, LOCKABLE, ABS_TIME)
#define BOOST_TRY_TO_SYNCHRONIZE_VAR_UNTIL(VAR, LOCKABLE, ABS_TIME) \
    BOOST_TRY_TO_SYNCHRONIZE_TYPE_UNTIL(boost::mutex, VAR, LOCKABLE, ABS_TIME)

#define BOOST_SYNCHRONIZE(LOCKABLE) \
    BOOST_SYNCHRONIZE_VAR(_, LOCKABLE)
#define BOOST_ADOPT_SYNCHRONIZE(LOCKABLE) \
    BOOST_ADOPT_SYNCHRONIZE_VAR(_, LOCKABLE)
#define BOOST_DEFER_SYNCHRONIZE(LOCKABLE) \
    BOOST_DEFER_SYNCHRONIZE_VAR(_, LOCKABLE)
#define BOOST_TRY_TO_SYNCHRONIZE(LOCKABLE) \
    BOOST_TRY_TO_SYNCHRONIZE_VAR(_, LOCKABLE)
#define BOOST_SYNCHRONIZE_UNTIL(LOCKABLE, ABS_TIME) \
    BOOST_SYNCHRONIZE_VAR_UNTIL(_, LOCKABLE, ABS_TIME)
#define BOOST_TRY_TO_SYNCHRONIZE_UNTIL(LOCKABLE, ABS_TIME) \
    BOOST_TRY_TO_SYNCHRONIZE_VAR_UNTIL(_, LOCKABLE, ABS_TIME)

#define synchronize(LOCKABLE) BOOST_SYNCHRONIZE(LOCKABLE)
#define adopt_synchronize(LOCKABLE) BOOST_ADOPT_SYNCHRONIZE(LOCKABLE)
#define defer_synchronize(LOCKABLE) BOOST_DEFER_SYNCHRONIZE(LOCKABLE)
#define try_to_synchronize(LOCKABLE) BOOST_TRY_TO_SYNCHRONIZE(LOCKABLE)
#define try_to_synchronize_until(LOCKABLE, ABS_TIME) BOOST_TRY_TO_SYNCHRONIZE_UNTIL(LOCKABLE, ABS_TIME)
#define synchronize_until(LOCKABLE, ABS_TIME) BOOST_SYNCHRONIZE_UNTIL(LOCKABLE, ABS_TIME)

#define synchronize_var(VAR, LOCKABLE) BOOST_SYNCHRONIZE_VAR(VAR, LOCKABLE)
#define adopt_synchronize_var(VAR, LOCKABLE) BOOST_ADOPT_SYNCHRONIZE_VAR(VAR, LOCKABLE)
#define defer_synchronize_var(VAR, LOCKABLE) BOOST_DEFER_SYNCHRONIZE_VAR(VAR, LOCKABLE)
#define try_to_synchronize_var(VAR, LOCKABLE) BOOST_TRY_TO_SYNCHRONIZE_VAR(VAR, LOCKABLE)
#define try_to_synchronize_var_until(VAR, LOCKABLE, ABS_TIME) BOOST_TRY_TO_SYNCHRONIZE_VAR_UNTIL(VAR, LOCKABLE, ABS_TIME)
#define synchronize_var_until(VAR, LOCKABLE, ABS_TIME) BOOST_SYNCHRONIZE_VAR_UNTIL(VAR, ABS_TIME, LOCKABLE)

#define synchronize_type(TYPE, VAR, LOCKABLE) BOOST_SYNCHRONIZE_TYPE(TYPE, VAR, LOCKABLE)
#define adopt_synchronize_type(TYPE, VAR, LOCKABLE) BOOST_ADOPT_SYNCHRONIZE_VAR(TYPE, VAR, LOCKABLE)
#define defer_synchronize_type(TYPE, VAR, LOCKABLE) BOOST_DEFER_SYNCHRONIZE_VAR(TYPE, VAR, LOCKABLE)
#define try_to_synchronize_type(TYPE, VAR, LOCKABLE) BOOST_TRY_TO_SYNCHRONIZE_VAR(TYPE, VAR, LOCKABLE)
#define try_to_synchronize_type_until(TYPE, VAR, LOCKABLE, ABS_TIME) BOOST_TRY_TO_SYNCHRONIZE_VAR_UNTIL(TYPE, VAR, LOCKABLE, ABS_TIME)
#define synchronize_type_until(TYPE, VAR, LOCKABLE, ABS_TIME) BOOST_SYNCHRONIZE_VAR_UNTIL(TYPE, VAR, ABS_TIME, LOCKABLE)


#endif
