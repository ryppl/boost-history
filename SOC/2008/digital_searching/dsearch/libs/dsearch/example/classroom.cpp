// Copyright 2008 Chintan Rao H.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include<boost/dsearch/patricia.hpp>
#include<iostream>
#include<algorithm>
#include<string>

using boost::dsearch::patricia;
using boost::dsearch::pat_string_traits;

template<class T>
void print_range(T begin,T end)
{
	for(T it=begin; it!=end; ++it)
		std::cout<<(*it).first<<" Roll No: "<<(*it).second<<std::endl;
	std::cout<<std::endl;
}

int main(int argc, char** argv)
{
	typedef patricia<std::string,std::string,pat_string_traits>   pat_type;
	pat_type classroom;
	typedef pat_type::iterator iterator;
	std::pair<iterator,iterator> prefix_pair;
	iterator found_it;

	classroom [ "Anirudh Santhiar" ] = "05co06";
	classroom [ "Anup C Shan" ] = "05co11";
	classroom [ "Pongu Somit Gupta" ] = "05co60";
	classroom [ "Bharath Ravi" ] = "05co48";
	classroom [ "Bala Subramanyam" ] = "05co18";
	classroom [ "S Devika Nair" ] = "05co54";
	classroom [ "S A Mahima" ] = "05co35";

	std::cout << "all of them" <<std::endl;
	print_range ( classroom.begin() , classroom.end() );

	prefix_pair=classroom.prefix_range("A");
	std::cout << "names which begin with A" <<std::endl;
	print_range ( prefix_pair.first , prefix_pair.second );

	prefix_pair=classroom.prefix_range("B");
	std::cout << "names which begin with B" <<std::endl;
	print_range ( prefix_pair.first , prefix_pair.second );

	prefix_pair=classroom.prefix_range("S ");
	std::cout << "names which begin with 'S '"<<std::endl;
	print_range ( prefix_pair.first , prefix_pair.second );

	classroom.erase ( "Bala Subramanyam" );
	
	classroom.insert ( std::make_pair(std::string("Chimpanzee" ),std::string("05co19" ) ) );
	
	found_it=classroom.find("Chimpanzee");
	std::cout<<(*found_it).first<<" Roll No: "<<(*found_it).second<<std::endl<<std::endl;	

	std::cout << "all of them" <<std::endl;
	print_range ( classroom.begin() , classroom.end() );

	return 0;
}
