#include<boost/dsearch/key_traits.hpp>
#include<boost/dsearch/trie_array_node.hpp>
#include<boost/dsearch/trie.hpp>
#include<boost/dsearch/node_cursor.hpp>
#include<assert.h>
#include<string>
#include<memory>
#include<iostream>

using namespace boost::dsearch;

template<class T>
void test_trie()
{
	T tr;
	typename T::cursor c;
	std::string str="hello";
	tr.insert( std::make_pair(str,1) );
	tr.insert( std::make_pair("he",2) );

	assert( tr["hello"]==1 );
	assert( !tr.find("hell") );
	assert( !tr.find("helli") );
	assert(tr.find("he"));
	tr.erase("he");
	assert( !tr.find("he") );
	assert( tr.find("hello") );
	tr.erase("hello");
	assert( !tr.find("hello") );
	c=tr.root();
	tr.insert( std::make_pair("he",2) );
	tr.insert( std::make_pair("hello",3) );
	assert( (*c).find('h')!=(*c).end() );
	assert( (*c).find('e')==(*c).end() );
	assert( (*c.begin()).find('e')!=(*c).end() ) ;
	assert( c.begin().begin()->find('l')!=(*c).end() ) ;

	tr.insert( std::make_pair("it",3) );
	c=tr.root().begin();
	c++;
	std::cout<<"GRR.............AT THE END"<<std::endl;
	tr.begin();
	tr.clear();

//	tr.clear();
	//assert(c->find('h')!=c.begin()->end());
	//assert(c.begin()->find('e')!=c.begin()->end());
}



int main(void)
{
	typedef trie<std::string,int,trie_array_node,string_traits> trie_type;
	test_trie<trie_type>();
	return 0;
}

#if 0
int main2(void)
{
/*	trie<std::string,int,trie_array_node,string_traits> a;
	a.empty();
	a.insert(std::make_pair("hello",1));
	a.erase("hello");
	a.find("hello");
	a["hello"]=10;*/
	return 0;
}

int main3(void)
{
	/*trie_cursor<trie_array_node<int,int,string_traits> > c;
	c++;
	c--;
	c.begin();
	c.end();
	*c;
	c.empty();*/
	return 0;
}
#endif
