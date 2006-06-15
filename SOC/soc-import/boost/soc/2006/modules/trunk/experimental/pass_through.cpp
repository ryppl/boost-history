/*
Experiment: pass-through text on a grammar.
Problem Description:
Spirit's parser backtracks as needed (wonderful!), but this also means
that we need a 'commit' phase to accept a matched text as proper, before
passing it through to the next phase.

Theory:
- Tree parser: go ahead and wait for final parse, then traverse the 
tree and change it as desired before pushing it out to the next stage.
 - This'd work fine with ANTLR's tree parsers, is there something
   similar with Spirit?  No tree parsers in spirit, as far as I can see.
- Screw the tree parser.  The tokens have the text within them, and
the actions of the parse rules can do the work for me. 

I'll work on this in a bit.
*/

#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_grammar_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <boost/program_options.hpp>

#include <vector>
#include <string>
#include <fstream>

using namespace boost::spirit;
using namespace boost::wave;
using namespace std;
using namespace boost::filesystem;

namespace opt = boost::program_options;


struct match : public 


int main (int args, char **argv) {
	opt::options_description desc ("Allowed options");
	desc.add_options ()
	     ("help", "produce help message")
	     ("include,I", opt::value< vector<string> >(), "add include path")
	;
	
	opt::variables_map vm;
	opt::store(opt::parse_command_line(args, argv, desc), vm);
	opt::notify(vm);
	
	vector<string> paths = vm["include"].as< vector<string> >();
	
	
}
