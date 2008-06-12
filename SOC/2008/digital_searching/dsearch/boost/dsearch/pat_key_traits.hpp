#ifndef BOOST_DSEARCH_PAT_KEY_TRAITS_HPP
#define BOOST_DSEARCH_PAT_KEY_TRAITS_HPP

#include<string>

namespace boost{
namespace dsearch{


//because we require an unsigned type from each element
class pat_key_traits{
	public:
	typedef unsigned char  element_type; //element type = unsigned type with size 2^(8*2^n) upto n=3. ie "unsigned long long";
	typedef std::string::const_iterator const_iterator ;

	static inline const_iterator begin (std::string const &key)
	{
		return key.begin();
	}
	
	static inline const_iterator end (std::string const &key)
	{
		return key.end();
	}

	static inline element_type  get_element(const const_iterator &it) 
	{
		return static_cast<unsigned char>(*it);
	}
	//no get_key as the keys are stored as it is.
};

}
}

#endif //BOOST_DSEARCH_PAT_KEY_TRAITS_HPP

