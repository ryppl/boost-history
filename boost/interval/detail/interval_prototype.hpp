#ifndef BOOST_INTERVAL_DETAIL_INTERVAL_PROTOTYPE_HPP
#define BOOST_INTERVAL_DETAIL_INTERVAL_PROTOTYPE_HPP

namespace boost {

  namespace interval_lib {

    template<class T> struct rounded_math;
    template<class T> struct checking_strict;
    class comparison_error;

  } // namespace interval_lib

  /*
   * default traits class
   */

  template<class Rounding, class Checking>
  struct interval_policies
  {
    typedef Rounding rounding;
    typedef Checking checking;
  };

  template<class T,
	   class Policies =
	     interval_policies<interval_lib::rounded_math<T>,
			       interval_lib::checking_strict<T> >
          >
  class interval;  

} // namespace boost

#endif // BOOST_INTERVAL_DETAIL_INTERVAL_PROTOTYPE_HPP
