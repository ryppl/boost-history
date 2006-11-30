//  boost/rank_list/detail/data_provider.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/data_provider.hpp
  ------------------------

  Functors used as parametric data sources. They return pointers to
  the objects that have to be copied. Depending on the context,
  NULL means "stop copying" or "use the default constructor".

  The user of the library doesn't need to know about these
  functors. They are for private use only.

  Available data providers are:

    class null_data_provider  (use the default constructor)
    iter_data_provider        (use a rank_list or other container)
    range_data_provider       (same as iter_, but stop at "to")
    copy_data_provider        (use allways the same prototype)
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_DATA_PROVIDER_HPP_
#define _BOOST_RANK_LIST_DATA_PROVIDER_HPP_

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

template<class Ptr>          // Function object used for creating
class null_data_provider     // collections of default constructed
{                            // objects
  public:

    Ptr operator() () { return NULL; }  // Always NULL
};

//////////////////////////////////////////////////////////////////

template<class Ptr, class IT> // Function object used for copying
class iter_data_provider      // a container (or part of it)
{
#ifdef BOOST_CLASS_REQUIRE
  BOOST_CLASS_REQUIRE (IT, boost, InputIteratorConcept);
#endif

  private:

    IT it;                   // State: current position

  public:

    iter_data_provider (const IT & from) : it(from) {}

    Ptr operator() ()
    {
      Ptr p=&*it;      // Return current element and advance
      ++ it;
      return p;
    }
};

//////////////////////////////////////////////////////////////////

template<class Ptr, class IT> // Function object used for copying
class range_data_provider     // a container (or part of it)
{
#ifdef BOOST_CLASS_REQUIRE
  BOOST_CLASS_REQUIRE (IT, boost, InputIteratorConcept);
#endif

  private:

    IT it;                   // State: current position
    IT end;                  // Limit (to)

  public:

    range_data_provider (const IT & from,
                         const IT & to) : it(from), end(to) {}

    Ptr operator() ()
    {
      if (it==end)
        return NULL;

      Ptr p=&*it;      // Return current element and advance
      ++ it;
      return p;
    }
};

//////////////////////////////////////////////////////////////////

template<class Ptr>          // Function object used for creating
class copy_data_provider     // collections of copies of a given
{                            // element
  private:

    Ptr p;             // Original element

  public:

    copy_data_provider (Ptr x) : p(x) {}

    Ptr operator() () { return p; }    // Always the same
};

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

