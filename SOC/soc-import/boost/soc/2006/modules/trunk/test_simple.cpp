#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/wave.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

typedef boost::wave::cpplexer::lex_token<> token_t;
typedef boost::wave::cpplexer::lex_iterator<token_t> lex_iter_t;
typedef boost::wave::context<std::string::iterator, lex_iter_t>  context_t;

using namespace std;
using namespace boost::spirit;
using namespace boost::wave;

struct decl_grammar : public grammar<decl_grammar> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  var_decl;
		
		definition (decl_grammar const& self) {
			var_decl = ch_p(T_IDENTIFIER) >> ch_p(T_IDENTIFIER) 
			           >> ch_p(T_SEMICOLON);
			BOOST_SPIRIT_DEBUG_RULE(var_decl);
		}
		rule<ScannerT> const& start () { return var_decl; }
	};
};

struct skip_grammar : public grammar<skip_grammar> {
    template <typename ScannerT>
    struct definition
    {
        definition(skip_grammar const& /*self*/) {
			skip = ch_p(T_SPACE) | ch_p(T_NEWLINE);        
			BOOST_SPIRIT_DEBUG_RULE(skip);
        }

        rule<ScannerT> skip;

        rule<ScannerT> const& start() const { return skip; }
    };
};

int main(int args, char **argv) {
	assert(args > 1);
	ifstream f(argv[1]);
	string instring;
	f.unsetf(ios::skipws);
	
	instring = string(istreambuf_iterator<char>(f.rdbuf()),
				  istreambuf_iterator<char>());
				  
	context_t ctx (instring.begin (), instring.end (), argv[1]);
	decl_grammar g;
	skip_grammar s;
	if (parse (ctx.begin (), ctx.end (), g, s).hit) {
		puts ("parsed");
	} else {
		puts ("not parsed.");
	}
}
