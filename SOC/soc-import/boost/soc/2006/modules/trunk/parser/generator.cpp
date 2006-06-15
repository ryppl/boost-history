#define BOOST_SPIRIT_DEBUG
#include "generator.h"
#include "decl_grammar.h"

#include <iostream>
#include <fstream>

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
