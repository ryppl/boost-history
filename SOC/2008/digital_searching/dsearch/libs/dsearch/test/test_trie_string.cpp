#include<boost/dsearch/key_traits.hpp>
#include<boost/dsearch/trie_array_node.hpp>
#include<boost/dsearch/trie.hpp>
#include<assert.h>
#include<string>
#include<memory>
#include<iostream>
#include<boost/test/minimal.hpp>
using namespace boost::minimal_test;


using namespace boost::dsearch;
using std::make_pair;

/*template<class T>
void print(const T &tr)
{
	T::iterator it=tr.begin();
	void
}*/

//testing insert function
template<class T>
T test_insert()
{
	T tr;
	
	BOOST_CHECK( tr.find("hello")==tr.end() );
	BOOST_CHECK( tr.find("")==tr.end() );

	tr.insert( make_pair("hello",1) );
	tr.insert( make_pair("world",2) );
	tr.insert( make_pair("",3) );
	tr.insert( make_pair("hel",4) );
	tr.insert( make_pair("wor",5) );
	tr.insert( make_pair("worry",6) );
	tr.insert( make_pair("hellish",7) );

	BOOST_CHECK( tr.find("hello")!=tr.end() );
	BOOST_CHECK( *tr.find("hello")==1 );

	BOOST_CHECK( tr.find("world")!=tr.end() );
	BOOST_CHECK( *tr.find("world")==2 );

	BOOST_CHECK( tr.find("")!=tr.end() );
	BOOST_CHECK( *tr.find("")== 3 );

	BOOST_CHECK( tr.find("hel")!=tr.end() );
	BOOST_CHECK( *tr.find("hel")== 4 );

	BOOST_CHECK( tr.find("wor")!=tr.end() );
	BOOST_CHECK( *tr.find("wor")==5 );

	BOOST_CHECK( tr.find("worry")!=tr.end() );
	BOOST_CHECK( *tr.find("worry")==6 );

	BOOST_CHECK( tr.find("hellish")!=tr.end() );
	BOOST_CHECK( *tr.find("hellish")==7 );
	
	return tr;
}

//testing copy contructor
template <class T>
void test_copy(T tr)
{
	BOOST_CHECK( tr.find("hello")!=tr.end() );
	BOOST_CHECK( *tr.find("hello")==1 );

	BOOST_CHECK( tr.find("world")!=tr.end() );
	BOOST_CHECK( *tr.find("world")==2 );

	BOOST_CHECK( tr.find("")!=tr.end() );
	BOOST_CHECK( *tr.find("")== 3 );

	BOOST_CHECK( tr.find("hel")!=tr.end() );
	BOOST_CHECK( *tr.find("hel")== 4 );

	BOOST_CHECK( tr.find("wor")!=tr.end() );
	BOOST_CHECK( *tr.find("wor")==5 );

	BOOST_CHECK( tr.find("worry")!=tr.end() );
	BOOST_CHECK( *tr.find("worry")==6 );

	BOOST_CHECK( tr.find("hellish")!=tr.end() );
	BOOST_CHECK( *tr.find("hellish")==7 );

	//checking not found cases
	BOOST_CHECK( tr.find("arbitrary")==tr.end() );
	BOOST_CHECK( tr.find("a")==tr.end() );
	BOOST_CHECK( tr.find("helli")==tr.end() );
	BOOST_CHECK( tr.find("worldy")==tr.end() );
}

template<class T>
void test_bound(T tr)
{
	//""=3<"hel"=4<"hellish"=7<"hello"=1<"wor"=5<"world"=2<"worry"=6
	BOOST_CHECK(tr.upper_bound("")==tr.find(""));
	BOOST_CHECK(tr.upper_bound("a")==tr.find("hel"));
	BOOST_CHECK(tr.upper_bound("hel")==tr.find("hel"));
	BOOST_CHECK(tr.upper_bound("hell")==tr.find("hellish"));
	BOOST_CHECK(tr.upper_bound("hellish")==tr.find("hellish"));
	BOOST_CHECK(tr.upper_bound("hellk")==tr.find("hello"));
	BOOST_CHECK(tr.upper_bound("hello")==tr.find("hello"));
	BOOST_CHECK(tr.upper_bound("hellp")==tr.find("wor"));
	BOOST_CHECK(tr.upper_bound("i")==tr.find("wor"));
	BOOST_CHECK(tr.upper_bound("w")==tr.find("wor"));
	BOOST_CHECK(tr.upper_bound("wor")==tr.find("wor"));
	BOOST_CHECK(tr.upper_bound("work")==tr.find("world"));
	BOOST_CHECK(tr.upper_bound("worm")==tr.find("worry"));
	BOOST_CHECK(tr.upper_bound("worx")==tr.end());
	BOOST_CHECK(tr.upper_bound("x")==tr.end());

	BOOST_CHECK(tr.lower_bound("y")==tr.find("worry"));
	BOOST_CHECK(tr.lower_bound("worx")==tr.find("worry"));
	BOOST_CHECK(tr.lower_bound("worm")==tr.find("world"));
	BOOST_CHECK(tr.lower_bound("work")==tr.find("wor"));
	BOOST_CHECK(tr.lower_bound("wor")==tr.find("wor"));
	BOOST_CHECK(tr.lower_bound("w")==tr.find("hello"));
	BOOST_CHECK(tr.lower_bound("i")==tr.find("hello"));
	BOOST_CHECK(tr.lower_bound("hellp")==tr.find("hello"));
	BOOST_CHECK(tr.lower_bound("hellk")==tr.find("hellish"));
	BOOST_CHECK(tr.lower_bound("hellish")==tr.find("hellish"));
	BOOST_CHECK(tr.lower_bound("hell")==tr.find("hel"));
	BOOST_CHECK(tr.lower_bound("hel")==tr.find("hel"));
	BOOST_CHECK(tr.lower_bound("a")==tr.find(""));
	BOOST_CHECK(tr.lower_bound("")==tr.find(""));

	tr.erase("");//made sure at least this works :)
	BOOST_CHECK(tr.lower_bound("a")==tr.end());
	BOOST_CHECK(tr.lower_bound("")==tr.end());
	BOOST_CHECK(tr.lower_bound("abcd")==tr.end());

}

template<class T>
void test_erase(T tr)
{
	tr.erase("hello");
	BOOST_CHECK( tr.find("hello")==tr.end() );
	
	tr.erase("hel");
	BOOST_CHECK( tr.find("hel")==tr.end() );
	
	tr.erase("hellish");
	BOOST_CHECK( tr.find("hellish")==tr.end() );
	
	tr.erase("worry");
	BOOST_CHECK( tr.find("worry")==tr.end() );
	
	tr.erase("world");
	BOOST_CHECK( tr.find("world")==tr.end() );

	tr.erase("wor");
	BOOST_CHECK( tr.find("wor")==tr.end() );

	tr.erase("");
	BOOST_CHECK( tr.find("")==tr.end() );

	BOOST_CHECK(tr.empty());
}

template<class T>
void test_iteration(T tr)
{
	//"" 3<"hel" 4<"hellish" 7<"hello" 1<"wor" 5<"world" 2<"worry" 6
	std::cout<<"bloody gods"<<std::endl;
	typename T::iterator it=tr.begin();

	std::cout<<"test_iteration:"<<*(tr.find(""))<<std::endl;
	BOOST_CHECK(it==tr.find(""));
	it++;

	BOOST_CHECK(it==tr.find("hel"));
	it++;

	BOOST_CHECK(it==tr.find("hellish"));
	it++;

	BOOST_CHECK(it==tr.find("hello"));
	it++;

	BOOST_CHECK(it==tr.find("wor"));
	it++;

	BOOST_CHECK(it==tr.find("world"));
	it++;

	BOOST_CHECK(it==tr.find("worry"));
	it++;

	BOOST_CHECK(it==tr.end());
	it--;

	BOOST_CHECK(it==tr.find("worry"));
	it--;

	BOOST_CHECK(it==tr.find("world"));
	it--;

	BOOST_CHECK(it==tr.find("wor"));
	it--;

	BOOST_CHECK(it==tr.find("hello"));
	it--;

	BOOST_CHECK(it==tr.find("hellish"));
	it--;

	BOOST_CHECK(it==tr.find("hel"));
	it--;

	BOOST_CHECK(it==tr.find(""));
	it--;

	BOOST_CHECK(it==tr.end());
}

template<class T>
void test_copy_simple()
{
	T tr,tr1;
	tr1.insert(make_pair("h",1));
	tr1.insert(make_pair("w",1));
	tr=tr1;
}

template<class T>
void test_erase_iterator()
{
	T tr;
	typename T::iterator it;
	typename T::const_iterator cit;

	tr.insert(make_pair("hello",1));
	it=tr.find("hello");
	tr.erase(it);
	BOOST_CHECK(tr.find("hello")==tr.end());

	//it=cit; //TODO:MAKE THIS NOT COMPILE
	cit=it; //should compile properly

	//"" 3<"hel" 4<"hellish" 7<"hello" 1<"wor" 5<"world" 2<"worry" 6
	tr=test_insert<T>();

	it=tr.find("hello");
	tr.erase(it);
	BOOST_CHECK(tr.find("hello")==tr.end());


	it=tr.find("wor");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("wor")==tr.end());

	it=tr.find("hellish");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("hellish")==tr.end());

	it=tr.find("hel");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("hel")==tr.end());

	it=tr.find("world");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("world")==tr.end());

	it=tr.find("worry");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("worry")==tr.end());

	it=tr.find("");
	BOOST_CHECK(it!=tr.end());
	tr.erase(it);
	BOOST_CHECK(tr.find("")==tr.end());
	
	BOOST_CHECK( tr.empty() );
}

template<class T>
void test_erase_range(T tr)
{
	typename T::iterator it;
	std::cout<<"test erase"<<std::endl;

//	"" 3<"hel" 4<"hellish" 7<"hello" 1<"wor" 5<"world" 2<"worry" 6
//	tr.erase(tr.begin(),tr.end());	
	it=tr.begin();
	std::cout<<"grrr..."<<*it<<std::endl;
	++it;
	std::cout<<"grrrr..."<<*it<<std::endl;
	++it;
	std::cout<<"grrr..."<<*it<<std::endl;
	++it;
	std::cout<<"grrr..."<<*it<<std::endl;

	tr.erase(tr.begin(),it);
	//BOOST_CHECK( tr.empty() );
}

int test_main(int,char **)
{
	typedef trie<std::string,int,trie_array_node,string_traits> trie_type;
	trie_type tr;
	test_copy_simple<trie_type>();
	tr=test_insert<trie_type>();

	test_copy(tr);
	test_bound(tr);
	test_erase(tr);
	test_iteration(tr);
	test_erase_iterator<trie_type>();
	
	//test_erase_range(tr);

	return 0;
}

