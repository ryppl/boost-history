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

#include "generator.h"
#include "decl_grammar.h"

using namespace std;
using namespace boost::spirit;
using namespace boost::wave;

struct skip_parser : public grammar <skip_parser> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT> skip;
		definition (skip_parser const& self) {
			skip = ( ch_p(T_SPACE) | ch_p(T_NEWLINE) | ch_p(T_SPACE2) );
		}
		rule<ScannerT> const& start () { return skip; }
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
	skip_parser s;
	if (parse (ctx.begin (), ctx.end (), g, s).hit) {
		puts ("parsed");
	} else {
		puts ("not parsed.");
	}
	return result;
}
