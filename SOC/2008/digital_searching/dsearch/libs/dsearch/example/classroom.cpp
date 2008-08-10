#include<boost/dsearch/patricia.hpp>
#include<boost/dsearch/pat_key_traits.hpp>
#include<iostream>
#include<string>

using boost::dsearch::patricia;
using boost::dsearch::pat_string_traits;

int main(int argc, char** argv)
{
	typedef patricia<std::string,std::string,pat_string_traits>   pat_type;
	pat_type classroom;
	typedef pat_type::iterator iterator;

	classroom [ "Anirudh Santhiar" ] = "05co06";
	classroom [ "Anup C Shan" ] = "05co11";
	classroom [ "Pongu Somit Gupta" ] = "05co60";
	classroom [ "Bharath Ravi" ] = "05co48";
	classroom [ "Bala Subramanyam" ] = "05co18";
	classroom [ "S Devika Nair" ] = "05co54";
	classroom [ "Mahima A S"] = "05co35";
	
	for(iterator it=classroom.begin(); it!=classroom.end(); ++it)
		std::cout<<(*it).first<<"-->"<<(*it).second<<std::endl;
	
	return 0;
}
