//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_CHRONO_CHECK_CLOCK_NOW_HPP
#define BOOST_CHRONO_CHECK_CLOCK_NOW_HPP

#include <boost/chrono.hpp>
#include <boost/system/system_error.hpp>
#include <boost/detail/lightweight_test.hpp>

template <typename Clock>
void check_clock_now()
{
	typename Clock::time_point t1 = Clock::now();
}

template <typename Clock>
void check_clock_now_ec()
{
    boost::system::error_code ec;
    typename Clock::time_point t1 = Clock::now(ec);
    BOOST_TEST(ec.value()==0);
}

template <typename Clock>
void check_clock_now_throws()
{
    typename Clock::time_point t1 = Clock::now(boost::throws());
}

template <typename Clock>
void check_clock_now_err(int err)
{
	Clock::set_errno(err);
	try {
	    typename Clock::time_point t1 = Clock::now();
	} catch (boost::system::system_error& ex) {
		BOOST_TEST(ex.code().value()==err);
//		BOOST_TEST(ex.code().category() == BOOST_CHRONO_SYSTEM_CATEGORY);
//		BOOST_TEST(std::string(ex.what()) == std::string("errored_clock"));
	}
	Clock::set_errno(0);
}

template <typename Clock>
void check_clock_now_ec_err(int err)
{
    Clock::set_errno(err);
    boost::system::error_code ec;
    typename Clock::time_point t1 = Clock::now(ec);
    BOOST_TEST(ec.value()==err);
//  BOOST_TEST(ec.category() == BOOST_CHRONO_SYSTEM_CATEGORY);
    Clock::set_errno(0);
}

template <typename Clock>
void check_clock_now_throws_err(int err)
{
    Clock::set_errno(err);
    try {
        typename Clock::time_point t1 = Clock::now(boost::throws());
        BOOST_TEST(0&&"exception not thown");
    } catch (boost::system::system_error& ex) {
        BOOST_TEST(ex.code().value()==err);
//      BOOST_TEST(ex.code().category() == BOOST_CHRONO_SYSTEM_CATEGORY);
//      BOOST_TEST(std::string(ex.what()) == std::string("errored_clock"));
    }
    Clock::set_errno(0);
}

#endif
