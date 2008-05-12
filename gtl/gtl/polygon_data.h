/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
template <typename T>
class polygon_data {
public:
  typedef T coordinate_type;
  typedef typename std::vector<coordinate_type>::const_iterator iterator_type;

  inline polygon_data(){;} //do nothing default constructor

  /// initialize a polygon from x,y values, it is assumed that the first is an x
  /// and that the input is a well behaved polygon
  template<class iT>
  inline polygon_data& set(iT input_begin, iT input_end) {
    coords_.clear();  //just in case there was some old data there
    while(input_begin != input_end) {
       coords_.insert(coords_.end(), *input_begin);
       ++input_begin;
    }
    return *this;
  }

  /// copy constructor (since we have dynamic memory)
  inline polygon_data(const polygon_data& that) : coords_(that.coords_) {}
  
  /// assignment operator (since we have dynamic memory do a deep copy)
  inline polygon_data& operator=(const polygon_data& that) {
    coords_ = that.coords_;
    return *this;
  }

  template <typename T2>
  inline polygon_data& operator=(const T2& rvalue);

  /// get begin iterator, returns a pointer to a const Unit
  inline iterator_type begin() const { return coords_.begin(); }

  /// get end iterator, returns a pointer to a const Unit
  inline iterator_type end() const { return coords_.end(); }

  inline std::size_t size() const { return coords_.size(); }

private:
  std::vector<coordinate_type> coords_; 
};

