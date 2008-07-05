#ifndef BOOST_DSEARCH_KEY_TRAITS_HPP
#define BOOST_DSEARCH_KEY_TRAITS_HPP

#include<string>
#include<vector>

namespace boost{
namespace dsearch{


///tag for key_traits. this means that the trait has only compare function
struct key_trait_compare_tag {};
///tag for key_traits. this means that the trait has only get_value function
struct key_trait_value_tag {};
///tag for key_traits. this the trait both compare function and get_value function
struct key_trait_compare_value_tag: public key_trait_compare_tag,public key_trait_value_tag{};

///traits of the string to be specfied by the user.
/**
 \note I think this will need only has key_trait_value_tag and key_trait_no_value_tag. 
 		compare function can always be presumed to exists.
*/
class string_traits{
	public:
	/// category of the trait
	typedef key_trait_compare_value_tag trait_category;
	/// const_iterator for iterating through the key.
	typedef std::string::const_iterator const_iterator;
	/// type of key.
	typedef std::string key_type;
	/// type of element.
	typedef char element_type;

	/// compare function object.
	/// this thing should ways hold true. 
	/// if e1<e2 ==> get_value(e1) < get_value(e2)
	struct lt_element
	{
		///behaves like a < operator
		/**
		 \returns e1<e2.
		 \param e1 is the first element.
		 \param e2 is the second element.
		 */	
	   bool operator()(element_type e1, element_type e2) const
	    {
	        return e1 < e2;
	    }
	};

	/// typedef as the key compare function obecjt so that higher level classes
	/// can make use of this typedef.
	typedef lt_element key_compare;
	
	///maximum value of a charecter
	enum {
		max=255
	};

	/// get the value correspoding to a particular charecter
	/**
	 \returns e1 converted to std::size_t
	 \param e1 is the element whose value is to be got.
	 \note if e1<e2 ==> get_value(e1) < get_value(e2)
	 */
	static std::size_t get_value(const element_type &e1)
	{
		return static_cast<unsigned char>(e1);
	}

	/// get element back from the value.
	/**
	 \returns element type corresponding to t
	 \param t should be converted back to element_type
	 */
	static element_type get_element(const std::size_t &t)
	{
		return (char)t;
	}

	/// returns begin of the key.
	/**
	  \returns const_iterator pointing to begin of a key.
	  \param key is the thing whose begin iterator is should be returns
	 */
	static inline const_iterator begin(const key_type &key)
	{
		return key.begin();
	}
	
	/// returns end of the key.
	/**
	 \returns const_iterator pointing to begin of a key.
	 \param key is the key whose end is to be found.
	 */
	static inline const_iterator end(const key_type &key)
	{
		return key.end();
	}

	/// returns a string from a sequence of element_types
	/**
	 \returns the regenerated string
	 \param beg is the begin() of the sequence
	 \param end is the end() of the sequence 
	 */
	template<typename value_it>
	static key_type get_key(const value_it &beg,const value_it &end)
	{
		key_type str;
		value_it it=beg;
		while(it!=end)
		{
			str.push_back((char)*it);
			it++;
		}
		return str;
	}
};
}
}
#endif // BOOST_DSEARCH_KEY_TRAITS_HPP

