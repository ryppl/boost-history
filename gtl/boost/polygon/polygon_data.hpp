/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_DATA_HPP
#define GTL_POLYGON_DATA_HPP
namespace boost { namespace polygon{
  struct polygon_concept;
  template <typename T>
  class polygon_data : public polygon_45_data<T> {
  public:
    typedef polygon_concept geometry_type;
	
    inline polygon_data() : polygon_45_data<T>() {} //do nothing default constructor

    template<class iT>
    inline polygon_data(iT input_begin, iT input_end) : polygon_45_data<T>(input_begin, input_end) {}  

	// copy constructor (since we have dynamic memory)
    inline polygon_data(const polygon_data& that) : polygon_45_data<T>(that.coords_.begin(), that.coords_.end()) {}
  
    // assignment operator (since we have dynamic memory do a deep copy)
    inline polygon_data& operator=(const polygon_data& that) {
      this->coords_ = that.coords_;
      return *this;
    }
  };
  
}
}
#endif

