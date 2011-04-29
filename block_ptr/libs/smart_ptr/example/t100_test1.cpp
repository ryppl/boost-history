/**
	@file
	regex_test1.cpp
*/

#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/block_ptr.hpp>

#include "t100.h"

using namespace std;
using namespace boost;
using boost::detail::sh::neuron_sight;


int main(int argv, char * argc[])
{
    block_ptr<neuron_sight> t100 = new block<neuron_sight>("I eat ([a-z]+) then drink ([a-z]+)");
	t100->sub_[0].second = new block<neuron_sight>("beef|chicken");
	t100->sub_[1].second = new block<neuron_sight>("vodka|water");

    cout << (* t100)("I eat beef then drink wine") << endl;
    cout << (* t100)("I eat beef then drink wine") << endl;
}
