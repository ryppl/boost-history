//
// boost/join/actor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_UTILS_HPP
#define BOOST_JOIN_UTILS_HPP

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <sstream>

namespace boost {
  namespace join {

    class logger : public std::ostringstream {
    public:
      const char *name_;
      static boost::mutex mutex_; 

      logger(const char *n = 0) : name_(n) {
      }

      void msg(std::string str) {
        if (name_ == 0) return;
        boost::mutex::scoped_lock lock(mutex_);
        std::cout << "[" << name_ << "] : " << str << std::endl;
      }
      std::ostringstream & stream(void) {
        boost::mutex::scoped_lock lock(mutex_);
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
        return *(new logger(name_));
#else
        return *this;
#endif
      }
      void flush(void) {
        boost::mutex::scoped_lock lock(mutex_);
        if(name_ != 0)
          std::cout << "[" << name_ << "] " << this->str();
        this->str("");
      }
      void flushl(void) {
        boost::mutex::scoped_lock lock(mutex_);
        if(name_ != 0)
          std::cout << "[" << name_ << "] " << this->str() << std::endl;
        this->str("");
      }
      typedef void func(logger &l);
      static void end(logger &l) { 
        l.flush(); 
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
        delete &l;
#endif
      }
      static void endl(logger &l) { 
        l.flushl(); 
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
        delete &l;
#endif
      }
    };
    std::ostream &operator << (std::ostream &os, logger::func *f) {
      f((logger&)os);
      return os;
    }
    boost::mutex logger::mutex_;
  }
}

#endif

