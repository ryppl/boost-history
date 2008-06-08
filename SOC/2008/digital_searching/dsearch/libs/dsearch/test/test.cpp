#include<boost/dsearch/key_traits.hpp>
#include<boost/dsearch/trie_array_node.hpp>
#include<boost/dsearch/trie.hpp>
#include<assert.h>
#include<string>
#include<memory>
#include<iostream>

using namespace boost::dsearch;

//RATHER INCOMPLETE TEST CLASS 
//REMOVE THIS FILE
template<class T>
void test_trie()
{
	T tr;
	typename T::cursor c;
	typename T::iterator tr_it;
	typedef typename T::const_iterator const_iterator;
	const_iterator tr_const_it;

	std::string str="hello";
	tr.insert( std::make_pair(str,1) );
	tr.insert( std::make_pair("he",2) );

	assert( tr["hello"] == 1 );
	assert( tr.find("hell") == tr.end() );
	assert( tr.find("helli") == tr.end() );
	assert(tr.find("he") != tr.end() );
	tr.erase("he");
	assert( tr.find("he") == tr.end() );
	assert( tr.find("hello") != tr.end() );
	tr.erase("hello");
	assert( tr.find("hello") == tr.end() );
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
	tr.begin();
	tr.clear();

	std::cout<<"GRR...AT THE END"<<std::endl;
	tr.begin(); //should not segfault.
	tr["hello"]=10;
	tr["heat"]=9;
	tr_it=tr.begin();
	assert((*tr_it)==9);
	++tr_it;
	std::cout<<*tr_it<<std::endl;
	assert((*tr_it)==10);
	--tr_it;
	assert((*tr_it)==9);
	++tr_it;
	assert((*tr_it)==10);
	tr["h"]=11;
	tr["he"]=12;
	tr_it=tr.begin();
	assert(*tr_it==11);
	tr_it++;
	assert(*tr_it==12);
	tr_it++;
	assert(*tr_it==9);
	tr_it++;
	assert(*tr_it==10);
	assert(tr_it==(--tr.end()));

	tr_it=tr.end();
	--tr_it;
	std::cout<<*tr_it<<std::endl;
	assert(*tr_it==10);

	assert(tr_it==tr_it);
	assert(tr.find("hello")==tr_it);

	//std::cout<<*tr_it<<std::endl; //segfaults cannot dereference end
	assert(++tr_it==tr.end());

	//std::cout<<*tr.upper_bound("helloi")<<std::endl;
	assert(tr.upper_bound("helloi")==tr.end());
	assert(tr.upper_bound("hello")==tr.find("hello"));
	assert(tr.upper_bound("hell")==tr.find("hello"));
	assert(tr.upper_bound("")==tr.begin());
	assert(tr.upper_bound("he")==tr.find("he"));
	assert(tr.upper_bound("hea")==tr.find("heat"));
	assert(tr.upper_bound("heb")==tr.find("hello"));
	assert(tr.upper_bound("hex")==tr.end());
	assert(tr.upper_bound("heata")==tr.find("hello"));
	assert(tr.upper_bound("x")==tr.end());

	assert(tr.lower_bound("hex")==tr.find("hello"));
	assert(tr.lower_bound("helloi")==tr.find("hello"));
	assert(tr.lower_bound("hell")==tr.find("heat"));
	assert(tr.lower_bound("hel")==tr.find("heat"));
	assert(tr.lower_bound("heb")==tr.find("heat"));
	assert(tr.lower_bound("heat")==tr.find("heat"));
	assert(tr.lower_bound("hea")==tr.find("he"));
	assert(tr.lower_bound("a")==tr.end());
	
	tr.clear();

	//tr_const_it=const_iterator(tr.begin());
	//tr_const_it++;

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
