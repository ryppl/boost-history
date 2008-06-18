#ifndef BOOST_SERIALIZE_FUSION_VECTOR_HPP
#define BOOST_SERIALIZE_FUSION_VECTOR_HPP

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// vector.hpp - non-intrusive serialization of boost::fusion::vector
//
// copyright (c) 2005   
// troy d. straszheim <troy@resophonic.com>
// http://www.resophonic.com
//
// copyright 2007 Stjepan Rajko
// 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
//

#include <boost/fusion/container/vector/vector.hpp>

namespace boost {
  namespace serialization {
    namespace detail {
      namespace vector {

	// primary template, serializes a vector in the
	// constructor.   Class is called like a template function.
	template <class Archive, class Tuple, int i=0, bool stop=false>
	struct serialize_impl
	{
	  serialize_impl(Archive &ar, Tuple& t)
	  { 
	    char tag[3] = "T?";
	    tag[1] = '0' + i; 
        ar & make_nvp(tag, boost::fusion::at_c<i>(t));
	    serialize_impl<Archive, Tuple, i+1, 
            i+1 < boost::fusion::result_of::size<Tuple>::value ? false : true>(ar, t);
	  }
	};

	// no-op called by the primary template when "i" is out of
	// bounds.
	template <class Archive, class Tuple, int i>
	struct serialize_impl<Archive, Tuple, i, true>
	{
	  serialize_impl(Archive &ar, Tuple& t) { }
	};

      } // namespace vector
    } // namespace detail

    // 
    // primary interface functions.  Serialize a vector...
    //
    template <class Archive, 
	      typename T0, typename T1, typename T2, typename T3, typename T4,
	      typename T5, typename T6, typename T7, typename T8, typename T9>
    void 
    serialize (Archive & ar,
	       boost::fusion::vector<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> & t,
	       unsigned int version)
    {
      detail::vector::serialize_impl<
        Archive,
        boost::fusion::vector<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
      >(ar, t); 
    }
    
    //
    // Serialize a "null" vector is a no-op
    //
    template <class Archive>
    void
    serialize (Archive & ar, 
	       boost::fusion::vector<> & nullvector, 
	       unsigned int version)
    {
    }
  } // namespace serialization
} // namespace boost

#endif // BOOST_SERIALIZE_FUSION_VECTOR_HPP
