#include<boost/dsearch/patricia.hpp>
#include<boost/dsearch/pat_key_traits.hpp>
#include<boost/test/minimal.hpp>
#include<string>
#include<fstream>
#include<algorithm>
using namespace boost::dsearch;
using namespace boost::minimal_test;

template<class T>
void insert_test_1()
{
	T pat;
	//BOOST_CHECK ( pat.find("") == 0 ) ;

	pat.insert ( std::make_pair("",1) );
	BOOST_CHECK ( pat.find("") );

	pat.insert ( std::make_pair("raining",1) );
	BOOST_CHECK ( pat.find("raining") );

	//BOOST_CHECK ( pat.find("weather") == 0 );
	//BOOST_CHECK ( pat.find("rain") == 0 );

	pat.insert ( std::make_pair("rain",1) );
	BOOST_CHECK ( pat.find("rain") );
	BOOST_CHECK ( pat.find("") );

	pat.insert ( std::make_pair("raina",1) );
	BOOST_CHECK ( pat.find("raina") );
	BOOST_CHECK ( pat.find("rain") );

	pat.insert ( std::make_pair("weather",1) );
	BOOST_CHECK ( pat.find("weather") );
	BOOST_CHECK ( pat.find("rain") );
	BOOST_CHECK ( pat.find("raining") );

	pat.insert ( std::make_pair("weat",1) );
	BOOST_CHECK ( pat.find("weat") );

	pat.insert ( std::make_pair("weating",1) );
	BOOST_CHECK ( pat.find("weating") );

	
	BOOST_CHECK ( pat.find("weather") );
	BOOST_CHECK ( pat.find("rain") );
	BOOST_CHECK ( pat.find("raining") );
	BOOST_CHECK ( pat.find("raina") );
	BOOST_CHECK ( pat.find("weat") );
	BOOST_CHECK ( pat.find("") );

	T pat1(pat);
	std::cout<<"PAT 1"<<std::endl;
	typename T::iterator it;
	int i=0;
	for(it=pat1.begin();it!=pat1.end();it++)
	{
		i++;
		std::cout<<(*it).first<<std::endl;
	}
	std::cout<<"END PAT 1"<<std::endl;
}

template<class T>
void insert_test_2()
{
	T pat;
	typename T::iterator it;

	pat.insert(std::make_pair("bad",1));
	pat.insert(std::make_pair("bad",2));

	it=pat.begin();
	BOOST_CHECK((*it).first ==std::string("bad"));
	BOOST_CHECK( ++it == pat.end() );

	pat.insert(std::make_pair("",3));
	pat.insert(std::make_pair("",4));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	std::cout<<"the iterator points to:"<<(*it).first<<std::endl;
	BOOST_CHECK( ++it == pat.end() );

	pat.insert(std::make_pair("h",5));
	pat.insert(std::make_pair("h",6));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	BOOST_CHECK((*++it).first == "h" );
	BOOST_CHECK( ++it == pat.end() );


	pat.insert(std::make_pair("hello",7));
	pat.insert(std::make_pair("hello",8));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	BOOST_CHECK((*++it).first == "h" );
	BOOST_CHECK((*++it).first == "hello" );
	BOOST_CHECK( ++it == pat.end() );


	pat.insert(std::make_pair("hell",9));
	pat.insert(std::make_pair("hell",10));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	BOOST_CHECK((*++it).first == "h" );
	BOOST_CHECK((*++it).first == "hell" );
	BOOST_CHECK((*++it).first == "hello" );
	BOOST_CHECK( ++it == pat.end() );

	pat.insert(std::make_pair("wicked",11));
	pat.insert(std::make_pair("wicked",12));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	BOOST_CHECK((*++it).first == "h" );
	BOOST_CHECK((*++it).first == "hell" );
	BOOST_CHECK((*++it).first == "hello" );
	BOOST_CHECK((*++it).first == "wicked" );
	BOOST_CHECK( ++it == pat.end() );

	pat.insert(std::make_pair("we",13));
	pat.insert(std::make_pair("we",14));

	it=pat.begin();
	BOOST_CHECK((*it).first == "" );
	BOOST_CHECK((*++it).first == "bad" );
	BOOST_CHECK((*++it).first == "h" );
	BOOST_CHECK((*++it).first == "hell" );
	BOOST_CHECK((*++it).first == "hello" );
	BOOST_CHECK((*++it).first == "we" );
	BOOST_CHECK((*++it).first == "wicked" );
	BOOST_CHECK( ++it == pat.end() );

	BOOST_CHECK( pat.find("") );
	BOOST_CHECK( pat.find("bad") );
	BOOST_CHECK( pat.find("hello") );
	BOOST_CHECK( pat.find("h") );
	BOOST_CHECK( pat.find("wicked") );
	BOOST_CHECK( pat.find("we") );
	
	
	 // 4<<h 6<< hell 10<< hello 8<< bad 2<< wicked 12<< we 14

	const char * key[]={ "","bad", "h","hell","hello","we","wicked"};
	int data[]=        {  4,   2 ,   6,    10,     8,   14,    12  };
	
	it=pat.begin();
	int pos=0;
	for(it=pat.begin();it!=pat.end();++it)
	{
		BOOST_CHECK ( (*it).first.compare( std::string((char *)key[pos]) ) == 0 );
		BOOST_CHECK ( (*it).second == data[pos] );
		std::cout<<(*it).first<<"==="<<(*it).second<<std::endl;
		++pos;
	}
}

template<class T>
void erase_test()
{
	std::cout<<"In erase"<<std::endl;
	T pat;
	pat[""]=1;
	pat.erase("");
	BOOST_CHECK( pat.empty() );

	pat[""]=2;
	pat["a"]=3;
	BOOST_CHECK( pat.find("") );
	BOOST_CHECK( pat.find("a") );

	pat.erase("a");
	BOOST_CHECK( !pat.find("a") );
	BOOST_CHECK( pat.find("") );

	pat["c"]=4;
	BOOST_CHECK( pat.find("c") );
	
	pat["cool"]=5;
	BOOST_CHECK( pat.find("cool") );
	BOOST_CHECK( pat.find("c") );

	pat.erase("cool");
	BOOST_CHECK( !pat.find("cool") );
	BOOST_CHECK( pat.find("c") );
	BOOST_CHECK( pat.find("") );

	pat["cool"]=6;
	pat["chess"]=7;
	pat["cheese"]=8;

	BOOST_CHECK( pat.find("c") );
	BOOST_CHECK( pat.find("chess") );
	BOOST_CHECK( pat.find("cheese") );
	BOOST_CHECK( pat.find("") );

	pat.erase("chess");
	BOOST_CHECK( pat.find("c") );
	BOOST_CHECK( pat.find("cool") );
	BOOST_CHECK( pat.find("cheese") );
	BOOST_CHECK( pat.find("") );

	pat.erase("cool");
	BOOST_CHECK( pat.find("c") );
	BOOST_CHECK( pat.find("cheese") );
	BOOST_CHECK( pat.find("") );

	pat.erase("c");
	BOOST_CHECK( pat.find("") );
	BOOST_CHECK( pat.find("cheese") );

	pat.erase("");
	BOOST_CHECK( pat.find("cheese") );
	
	pat.erase("cheese");
	BOOST_CHECK( pat.empty() );


	pat["weather"]=10;
	pat["wheat"]=11;
	pat["wheats"]=12;
	pat["what"]=13;
	BOOST_CHECK( pat.find("weather") );
	BOOST_CHECK( pat.find("wheat") );
	BOOST_CHECK( pat.find("wheats") );
	BOOST_CHECK( pat.find("what") );

	pat.erase("wheats");
	BOOST_CHECK( pat.find("weather") );
	BOOST_CHECK( pat.find("wheat") );
	BOOST_CHECK( pat.find("what") );

	
	pat.erase("wheat");
	BOOST_CHECK( pat.find("weather") );
	BOOST_CHECK( pat.find("what") );
	
	
	pat.erase("what");
	BOOST_CHECK( pat.find("weather") );
	
	BOOST_CHECK(!pat.empty());
	pat.erase("weather");
	BOOST_CHECK(pat.empty()); 

	pat["wheat"]=10;
	pat["weather"]=20;
	pat.erase("wheat");
	pat.erase("weather");
	BOOST_CHECK(pat.empty());
	pat["a"]=20;
	pat[""]=10;
	pat["b"]=30;
	pat.erase("");
	pat.erase("a");
	pat.erase("b");
	BOOST_CHECK(pat.empty());
}

template<class T>
void iterator_test()
{
	T pat;
	std::cout<<"here"<<std::endl;
	assert(pat.begin()==pat.end());
	return;
}

template<class T>
void insert_test_3(char *file)
{
	T pat;
	try{
		std::ifstream fin(file);
		std::string str;
		int value=0;
		while(!fin.eof())
		{
			getline(fin,str,'\n');
			pat[str]=value;
			//pat.insert(std::make_pair( str, value ) );
			//value++;
		}
		fin.close();
	}
	catch(std::ifstream::failure e){
		std::cout<<"exception opening/reading file "<<file<<std::endl;
	}

#ifndef  NOTEST
std::cerr<<"finished inserting"<<std::endl;
	try{
		std::ifstream fin(file);
		char str[1000];
		int value=0;
		while(!fin.eof())
		{
			fin.getline(str,999);
			//std::cout<<str<<std::endl;
			//pat.insert(std::make_pair( (const char *)str, value ) );
			BOOST_CHECK( pat.find( (const char *)str ) ) ;
			value++;
		}
		fin.close();
	}
	catch(std::ifstream::failure e){
		std::cout<<"exception opening/reading file "<<file<<std::endl;
	}
#endif
}

template<class T>
void copy_test()
{
	T pat1;
	pat1["hello"]=1;
	pat1["hell"]=1;
	T pat2=T(pat1);
}

int test_main(int argc,char **argv)
{
	typedef patricia< std::string, int, pat_key_traits> pat_type;

#ifndef NOTEST
	insert_test_1<pat_type>();
	insert_test_2<pat_type>();
	erase_test<pat_type>();
	copy_test<pat_type>();
#endif
	if ( argc > 1 )
		insert_test_3<pat_type>(argv[1]);
	return 0;
}

