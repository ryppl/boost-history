//  (C) Copyright Chintan Rao H 2008. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DSEARCH_PAT_KEY_TRAITS_HPP
#define BOOST_DSEARCH_PAT_KEY_TRAITS_HPP

#include<string>
#include<vector>

namespace boost{
namespace dsearch{


///key traits describing extra traits required for patricia.
class pat_string_traits {
	public:
	/// element type = unsigned type. \n
	/// each element in string (ie char) should correspond to element_type.
	/// ie unsigned char.
	typedef unsigned char  element_type ; 
	/// Const_iterator used by patricia to iterate through elements. 
	/// It can be forward or random access.
	typedef std::string::const_iterator const_iterator ;
	
	/// Returns the begin iterator of a key.
	/**
	 	\returns const_iterator positioned at begining of the string.
	 	\param key whose begin is to found
	 */
	static inline const_iterator begin (std::string const &key)
	{
		return key.begin();
	}
	
	/// Returns the end of a key.
	/**
		\returns const_iterator positioned at the end.
		\param key whose end is to found
	 */
	static inline const_iterator end (std::string const &key)
	{
		return key.end();
	}

	/// Returns the size of the key.
	/**
		\returns size of the key.
		\param key whose size is to found
	 */
	static inline std::size_t size(std::string const &key)
	{
		return key.size();
	}
	
	/// Deference iterator to corresponding element_type , ie unsigned type.
	/**
		\returns unsigned integer of element_type corresponding to the iterator.
		\param it: iterator corresponding to which element_type if to be found
	 */
	static inline element_type  get_element(const const_iterator &it) 
	{
		return static_cast<unsigned char>(*it);
	}
	
};

///vector key triats class.
template<typename type>
class pat_vector_traits {
	public:
	/// element type = unsigned type. \n
	typedef unsigned int  element_type ; 
	/// Const_iterator used by patricia to iterate through elements. 
	/// It can be forward or random access.
	typedef typename std::vector<type>::const_iterator const_iterator ;

	/// Returns the begin iterator of a key.
	/**
	 	\returns const_iterator positioned at begining of the string.
	 	\param key whose begin is to found
	 */
	static inline const_iterator begin ( std::vector<type> const &key )
	{
		return key.begin();
	}
	
	/// Returns the end of a key.
	/**
		\returns const_iterator positioned at the end.
		\param key whose end is to found
	 */
	static inline const_iterator end (std::vector<type> const &key)
	{
		return key.end();
	}

	/// Returns the size of the key.
	/**
		\returns size of the key.
		\param key whose size is to found
	 */
	static inline std::size_t size(std::vector<type> const &key)
	{
		return key.size();
	}
	
	/// Deference iterator to corresponding element_type , ie unsigned type.
	/**
		\returns unsigned integer of element_type corresponding to the iterator.
		\param it: iterator corresponding to which element_type if to be found
	 */
	static inline element_type  get_element(const const_iterator &it) 
	{
		return static_cast<unsigned int>(*it);
	}
};

}
}

#endif //BOOST_DSEARCH_PAT_KEY_TRAITS_HPP

