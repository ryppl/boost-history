/*
Open questions:
1. How do I pass parameters into decl_grammar (& the OutputDelegate)?
2. This grammar now takes in a scanner that does cpp lexing for us.
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

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "parser/generator.h"

using namespace std;
using namespace boost::spirit;
using namespace boost::wave;

struct decl_grammar : public grammar<decl_grammar> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  decl, var_decl, type_decl, id, semi;
		
		definition (decl_grammar const& self) {

			// these are obviously insufficient for production use, but that'll
			// come later.
			
			id = ch_p(T_IDENTIFIER);
			semi = ch_p(T_SEMICOLON);
			
			var_decl = id >> id >> semi;
			
			// completely ignoring typedef and function defs right now.
			type_decl = ( ch_p(T_STRUCT)  | ch_p(T_CLASS) ) >> id 
						>> semi;
			
			decl = *(var_decl | type_decl);
		}
		rule<ScannerT> const& start () { return var_decl; }
	};
};

static std::vector<std::string> execute (context_t&, OutputDelegate&);

std::vector<std::string>
Generator::
execute() {
	return ::execute(c,d);
}

static
std::vector<std::string>
execute (context_t& ctx, OutputDelegate& del) {
	std::vector<std::string>  result;
	decl_grammar g;
	if (parse (ctx.begin (), ctx.end (), g, space_p).full) {
		puts ("parsed");
	} else {
		puts ("not parsed.");
	}
	return result;
}
