// Boost.Tokenmap
//
// Copyright (c) 2009 Slawomir Lisznianski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can cause buffer overruns or other possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable alternative functions, and VC++ 8.0's own libraries use the
//  functions in question. So turn off the warnings.
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

// Boost.Tokenmap Example
//-----------------------------------------------------------------------------

#include <boost/config.hpp>

//[ code_simple_tokenmap

#include <boost/tokenmap.hpp>

// holds session information (ie. connected client's context)
struct session { };

int main()
{
  typedef boost::tokenmap<session, uint32_t> tokenamp_type;

  tokenamp_type sessions(2000,      // hint on number of concurrent sessions
                         0.7,       // load factor, if exceeded, the container resizes
                         time(NULL) // seed for the pseudo-random number generator
                         );

  // insert by-value; on return v1 holds a std::pair<token, session*>
  tokenamp_type::value_type v1 = 
    sessions.insert( session() );

  // insert by-auto_ptr
  tokenamp_type::value_type v2 = 
    sessions.insert( std::auto_ptr<session>(new session()) );

  // search for session instance
  tokenamp_type::mapped_type * m1 = sessions.find( v1.first );

  // pop (remove) element from the container
  std::auto_ptr<map_type::mapped_type> e1 = 
    sessions.pop( v1.first );

  // checks if element identified by token exists
  bool found = sessions.exists( v1.first );

  return 0;
}
//]

