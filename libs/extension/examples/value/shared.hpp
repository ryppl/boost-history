/*
 * Boost.Extension
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See             
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_SHARED_HPP
#define BOOST_EXTENSION_SHARED_HPP

#include <boost/scoped_ptr.hpp>

class generic_animal_impl {
 public:
  virtual ~generic_animal_impl() {}
  virtual const std::string& get_name() const = 0;
};

template <class T>
class animal_impl : public generic_animal_impl {
 public:
  animal_impl() : impl_(new T()) {}
  virtual const std::string& get_name() const {
    return impl_->get_name();
  }
  boost::scoped_ptr<T> impl_;
};

class any_animal {
 public:
  explicit any_animal(generic_animal_impl* a) : impl_(a) {
  }
  const std::string& get_name() const {
    return impl_->get_name();
  }
  boost::scoped_ptr<generic_animal_impl> impl_;
};

#endif  // BOOST_EXTENSION_SHARED_HPP
