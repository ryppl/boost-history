//  boost/chrono/stopwatch_scoped.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_SCOPED_HPP
#define BOOST_CHRONO_STOPWATCH_SCOPED_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stopwatch_runner {
    public:
        typedef Stopwatch stopwatch_type;
        stopwatch_runner(stopwatch_type & a, system::error_code & ec = system::throws)
        : stopwatch_(a) {
            stopwatch_.start(ec);
        }
        ~stopwatch_runner() {
            system::error_code ec;
            stopwatch_.stop(ec);
        }
#if 0
        typename Stopwatch::duration elapsed(system::error_code & ec = system::throws)
        {
            return stopwatch_.elapsed(ec)-stopwatch_.accumulated();
        }
#endif
    private:
        stopwatch_type& stopwatch_;
        stopwatch_runner();//= delete;
        stopwatch_runner(const stopwatch_runner&); // = delete;
        stopwatch_runner& operator=(const stopwatch_runner&); // = delete;

    };

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stopwatch_suspender {
    public:
        typedef Stopwatch stopwatch_type;
        stopwatch_suspender(stopwatch_type & a, system::error_code & ec = system::throws)
        : stopwatch_(a) {
            stopwatch_.suspend(ec);
        }
        ~stopwatch_suspender() {
            system::error_code & ec;
            stopwatch_.resume(ec);
        }
    private:
        stopwatch_type& stopwatch_;
        stopwatch_suspender(); // = delete;
        stopwatch_suspender(const stopwatch_suspender&); // = delete;
        stopwatch_suspender& operator=(const stopwatch_suspender&); // = delete;
    };

//--------------------------------------------------------------------------------------//
    template <class Stopwatch> class stopwatch_resumer {
    public:
        typedef Stopwatch stopwatch_type;
        stopwatch_resumer(stopwatch_type & a, system::error_code & ec = system::throws)
        : stopwatch_(a) {
            stopwatch_.resume(ec);
        }
        ~stopwatch_resumer() {
            system::error_code & ec;
            stopwatch_.suspend(ec);
        }
    private:
        stopwatch_type& stopwatch_;
        stopwatch_resumer(); // = delete;
        stopwatch_resumer(const stopwatch_resumer&); // = delete;
        stopwatch_resumer& operator=(const stopwatch_resumer&); // = delete;
    };


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
