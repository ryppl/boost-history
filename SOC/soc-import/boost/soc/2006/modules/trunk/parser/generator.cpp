#include "setup.h"
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
			BOOST_SPIRIT_DEBUG_RULE(skip);
		}
		rule<ScannerT> const& start () { return skip; }
	};

};

static vector<string> execute (context_t&, OutputDelegate&);

vector<string>
SourceGenerator::
execute() {
	return ::execute(c,d);
}

static
std::vector<std::string>
execute (context_t& ctx, OutputDelegate& del) {
	std::vector<std::string> retval;
	decl_grammar g (del);
	skip_parser s;
	// default to emit in the source code.
//	del.push_source ();
	if (parse (ctx.begin (), ctx.end (), g, s).hit) {
		cout << "parsed" << endl;
	} else {
		cout << "not parsed." << endl;
	}
	return g.m_ids;
}
