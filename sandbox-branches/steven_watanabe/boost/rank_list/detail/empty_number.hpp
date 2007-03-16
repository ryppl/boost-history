//  boost/rank_list/detail/empty_number.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/empty_number.hpp
  -----------------------

  Class intended to be used as default parameter when features
  like NPSV or stable_sort are not wanted. Compilers should
  optimize away memory and operations of this class.
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_EMPTY_NUMBER_HPP_
#define _BOOST_RANK_LIST_EMPTY_NUMBER_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    namespace detail
    {

//////////////////////////////////////////////////////////////////

class empty_number   // Class supporting some numeric operators
{                    // but containing no number at all
  public:

    empty_number (int n=0) { }
    empty_number (std::size_t n) { }

    empty_number operator+ (const empty_number &) const
    { return empty_number(); }

    empty_number operator- (const empty_number &) const
    { return empty_number(); }

    empty_number operator+= (const empty_number &)
    { return empty_number(); }

    empty_number operator-= (const empty_number &)
    { return empty_number(); }

    empty_number & operator++ () { return *this; }
    empty_number & operator-- () { return *this; }
    empty_number operator++ (int) { return *this; }
    empty_number operator-- (int) { return *this; }
};

inline bool operator== (const empty_number &, const empty_number &)
{ return true; }

inline bool operator!= (const empty_number &, const empty_number &)
{ return false; }

inline bool operator< (const empty_number &, const empty_number &)
{ return false; }

inline bool operator> (const empty_number &, const empty_number &)
{ return false; }

inline bool operator<= (const empty_number &, const empty_number &)
{ return true; }

inline bool operator>= (const empty_number &, const empty_number &)
{ return true; }

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

