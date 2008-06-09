#ifndef BOOST_DSEARCH_KEY_TRAITS_HPP
#define BOOST_DSEARCH_KEY_TRAITS_HPP

#include<string>
#include<vector>

namespace boost{
namespace dsearch{

//I think this will need only has key_trait_value_tag and key_trait_no_value_tag.
//compare function can always be presumed to exists
struct key_trait_compare_tag {};
struct key_trait_value_tag {};
struct key_trait_compare_value_tag: public key_trait_compare_tag,public key_trait_value_tag{};

template<class Key>
class default_iterator_traits{
	public:
	typedef typename Key::const_iterator const_iterator;
	typedef Key key_type;
	static inline const_iterator begin(const Key &key)
	{
		return key.begin();
	}

	static inline const_iterator end(const Key &key)
	{
		return key.end();
	}
};


//this will lessen the burden for trie_node. 
class string_traits: public default_iterator_traits<std::string>{
	public:
	typedef key_trait_compare_value_tag container_category;
	typedef std::string::const_iterator const_iterator;
	typedef std::string key_type;
	typedef char element_type;

	struct lt_element
	{
	  bool operator()(element_type e1, element_type e2) const
	    {
	        return e1 < e2;
	    }
	};

	typedef lt_element key_compare;

	enum {
		max=255
	};

	static std::size_t get_value(const element_type &e1)
	{
		return static_cast<unsigned char>(e1);
	}

	static element_type get_element(const std::size_t &t)
	{
		return (char)t;
	}
	
	//returns a string of element_type
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

