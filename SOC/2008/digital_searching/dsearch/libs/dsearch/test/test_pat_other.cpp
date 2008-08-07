#include<boost/dsearch/patricia.hpp>
#include<boost/dsearch/pat_key_traits.hpp>
#include<boost/test/minimal.hpp>
#include<vector>
#include<string>
#include<fstream>
#include<iterator>
#include<algorithm>

using namespace boost::dsearch;
using namespace boost::minimal_test;

int test_main ( int argc , char **argv )
{
	typedef patricia < std::vector < int > , int , pat_vector_traits<int> > pat_type ;
	pat_type pat_trie ;
	std::vector < int > vec ;
	vec.push_back ( 1 ) ;
	vec.push_back ( 2 ) ;
	pat_trie [ vec ] = 1 ;

	BOOST_CHECK ( pat_trie [ vec ] == 1 ) ;
	BOOST_CHECK ( (*pat_trie.find(vec)).second == 1 ) ;

	vec.clear();

	vec.push_back ( 2 );
	vec.push_back ( 1 );
	vec.push_back ( 3 );
	
	BOOST_CHECK ( pat_trie [ vec ] == 0 ) ;
	pat_trie [ vec ] = 2;
	
	BOOST_CHECK ( pat_trie [ vec ] == 2 ) ;
	BOOST_CHECK ( (*pat_trie.find(vec)).second == 2 ) ;
	
	vec.push_back ( 3 );
	vec.push_back ( 2 );
	vec.push_back ( 1 );
	BOOST_CHECK ( pat_trie [ vec ] == 0 ) ;
	pat_trie [ vec ] = 3;
	
	BOOST_CHECK ( pat_trie [ vec ] == 3 ) ;
	BOOST_CHECK ( (*pat_trie.find(vec)).second == 3 ) ;
	
	pat_type::iterator it=pat_trie.begin();
	std::advance(it,1);
	BOOST_CHECK ( (*it).second==2 );
	std::advance(it,-1);
	BOOST_CHECK ( (*it).second==1 );
	
	///*BOOST_CHECK ( (*/find( vec.begin() , vec.end() , std::make_pair(vec,3) );/*).second == 3 ) ;*/
	find( pat_trie.begin() , pat_trie.end() , std::make_pair<const std::vector<int>,int>(vec,3) );
	BOOST_CHECK ( count( pat_trie.begin() , pat_trie.end() , std::make_pair<const std::vector<int>,int>(vec,3) ) == 1 );
	BOOST_CHECK ( count( pat_trie.begin() , pat_trie.end() , std::make_pair<const std::vector<int>,int>(vec,1) ) == 0 );

	BOOST_CHECK(distance(pat_trie.begin(), pat_trie.end()) == static_cast<signed int>(pat_trie.size()));
	
	return 0;
}
