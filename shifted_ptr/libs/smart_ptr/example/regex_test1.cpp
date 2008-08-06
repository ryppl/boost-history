/**
	@file
	regex_test1.cpp

	@note
	Brain kernel.
*/

#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/shifted_ptr.hpp>

using namespace std;
using namespace boost;


struct neuron
{
    regex e;
    vector< shifted_ptr<neuron> > sub;
    
    neuron(string const & s, shifted<neuron> * p1 = 0, shifted<neuron> * p2 = 0, shifted<neuron> * p3 = 0) 
    : 
        e(s), 
        sub(e.mark_count()) 
    {
        if (p1) sub[0] = p1;
        if (p2) sub[1] = p2;
        if (p3) sub[2] = p3;
    }

    double operator () (std::string const & input)
    {
        match_results<std::string::const_iterator> what;

        if (! regex_match(input, what, e, match_default | match_partial))
            return 0;
          
        if (! what[0].matched)
            return 0;

        double accuracy = 0;
        for (int i = 1; i < what.size(); i ++)
            if (what[i].matched)
                accuracy += (* sub[i])(what[i].str()) / (what.size() - 1);
        
        return accuracy;
    }
};

int main(int argv, char * argc[])
{
    shifted_ptr<neuron> brain = new shifted<neuron>("I eat [a-z]+ then drink [a-z+]", new shifted<neuron>("beef|chicken"), new shifted<neuron>("vodka|water"));

    cout << ((* brain)("I eat beef then drink water") > .5 ? "true" : "false") << endl;
}
