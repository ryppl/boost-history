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
	BOOST_CHECK ( pat.find("") == 0 ) ;

	pat.insert( std::make_pair("",1) );
	BOOST_CHECK ( pat.find("") );

	pat.insert( std::make_pair("raining",1) );
	BOOST_CHECK ( pat.find("raining") );

	BOOST_CHECK ( pat.find("weather") == 0 );
	BOOST_CHECK ( pat.find("rain") == 0 );

	pat.insert( std::make_pair("rain",1) );
	BOOST_CHECK ( pat.find("rain") );

	pat.insert( std::make_pair("raina",1) );
	BOOST_CHECK ( pat.find("raina") );
	BOOST_CHECK ( pat.find("rain") );

	pat.insert( std::make_pair("weather",1) );
	BOOST_CHECK ( pat.find("weather") );
	BOOST_CHECK ( pat.find("rain") );
	BOOST_CHECK ( pat.find("raining") );

	pat.insert( std::make_pair("weat",1) );
	BOOST_CHECK ( pat.find("weat") );

	pat.insert( std::make_pair("weating",1) );
	BOOST_CHECK ( pat.find("weating") );

	
	BOOST_CHECK ( pat.find("weather") );
	BOOST_CHECK ( pat.find("rain") );
	BOOST_CHECK ( pat.find("raining") );
	BOOST_CHECK ( pat.find("raina") );
	BOOST_CHECK ( pat.find("weat") );
}

template<class T>
void insert_test_2()
{
	T pat;

	pat.insert(std::make_pair("",-1));
	pat.insert(std::make_pair("",-1));

	pat.insert(std::make_pair("bad",1));
	pat.insert(std::make_pair("bad",2));

	pat.insert(std::make_pair("h",3));
	pat.insert(std::make_pair("h",4));

	pat.insert(std::make_pair("hello",5));
	pat.insert(std::make_pair("hello",6));


	pat.insert(std::make_pair("hell",6));
	pat.insert(std::make_pair("hell",6));

	pat.insert(std::make_pair("wicked",7));
	pat.insert(std::make_pair("wicked",8));

	pat.insert(std::make_pair("we",9));
	pat.insert(std::make_pair("we",10));

	pat.insert(std::make_pair("wicked",11));
	pat.insert(std::make_pair("wicked",12));

	BOOST_CHECK( pat.find("") );
	BOOST_CHECK( pat.find("bad") );
	BOOST_CHECK( pat.find("hello") );
	BOOST_CHECK( pat.find("h") );
	BOOST_CHECK( pat.find("wicked") );
	BOOST_CHECK( pat.find("we") );

}

template<class T>
void insert_test_3(char *file)
{
	T pat;
	//try{
		std::ifstream fin(file);
		char str[1000];
		int value=0;
		while(!fin.eof())
		{
			fin.getline(str,999);
			pat.insert(std::make_pair( (const char *)str, value ) );
			//value++;
		}
		fin.close();
	/*}
	catch(std::ifstream::failure e){
		std::cout<<"exception opening/reading file "<<file<<std::endl;
	}*/

/*std::cerr<<"finished inserting"<<std::endl;
	try{
		std::ifstream fin(file);
		char str[1000];
		int value=0;
		while(!fin.eof())
		{
			fin.getline(str,999);
			pat.insert(std::make_pair( (const char *)str, value ) );
			BOOST_CHECK( pat.find( (const char *)str ) ) ;
			//value++;
		}
		fin.close();
	}
	catch(std::ifstream::failure e){
		std::cout<<"exception opening/reading file "<<file<<std::endl;
	}*/

}

int test_main(int c,char **argv)
{
	typedef patricia< std::string, int, pat_key_traits> pat_type;
//	insert_test_1<pat_type>();
//	insert_test_2<pat_type>();
	insert_test_3<pat_type>(argv[1]);
	return 0;
}

