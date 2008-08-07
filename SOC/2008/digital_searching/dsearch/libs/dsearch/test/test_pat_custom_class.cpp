#include<list>
#include<vector>
#include<boost/dsearch/patricia.hpp>
#include<boost/dsearch/pat_key_traits.hpp>
#include<boost/test/minimal.hpp>

using namespace boost::minimal_test;
using namespace boost::dsearch;

template<class T>
class custom_class {
	public:
	typedef T container_type;
	typedef typename container_type::iterator iterator ;
	typedef typename container_type::const_iterator const_iterator ;
	container_type con;
	
	explicit custom_class(container_type ll )
	{
		con=ll;
	}

	const_iterator begin() const
	{
		return con.begin();
	}
	
	const_iterator end() const
	{
		return con.end();
	}
	
	std::size_t size1() const
	{
		return con.size();
	}

};

template <class T>
class custom_class_traits {
	public:
	typedef T custom_class_type;
	typedef unsigned int  element_type ; 
	typedef typename custom_class_type::const_iterator const_iterator ;

	static inline const_iterator begin (custom_class_type const &key)
	{
		return key.begin();
	}
	
	static inline const_iterator end (custom_class_type const &key)
	{
		return key.end();
	}
	
	static inline std::size_t size( custom_class_type const &key)
	{
		return key.size1();
	}
	
	static inline element_type  get_element(const const_iterator &it) 
	{
		return static_cast<unsigned int>(*it);
	}
};

template<class T>
void container_test()
{
	typedef custom_class< T > cc;
	typedef patricia<cc, float, custom_class_traits<cc> > pat_type;

	T ll;
	pat_type pat_trie;

	ll.push_back(0);
	ll.push_back(1);
	
	pat_trie[ cc(ll) ]=2;
	pat_trie.erase( cc(ll) );
	
	pat_trie.prefix_range( cc(ll) );
	pat_trie.find ( cc(ll) );
	pat_trie.lower_bound ( cc(ll) );
	pat_trie.upper_bound ( cc(ll) );
	std::distance ( pat_trie.begin(), pat_trie.end() );
	BOOST_REQUIRE ( pat_trie.rbegin() == pat_trie.rend() );
	std::distance ( pat_trie.rbegin(), pat_trie.rend() );
}

int test_main(int, char**)
{
	std::cout<<"container_test<std::list<int> >();"<<std::endl;
	container_test<std::list<int> >();
	
	std::cout<<"container_test<std::vector<int> >();"<<std::endl;
	container_test<std::vector<int> >();

	std::cout<<"container_test<std::vector<unsigned char> >();"<<std::endl;
	container_test<std::vector<unsigned char> >();
	
	return 0;
}
