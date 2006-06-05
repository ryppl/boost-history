/*
Open questions:
1. How do I pass parameters into decl_grammar (the OutputDelegate)?
*/

#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;
using namespace boost::spirit;


//
// This is _certainly_ staying in a .cpp file, to avoid recompiling
// all this metacode.
struct decl_grammar : public grammar<decl_grammar> {
	template<typename ScannerT>
	struct definition {
		rule<ScannerT>  id, decl, type, var_decl, type_decl, body;
		symbols<> cv_qual, lifetime_qual, sign_qual, size_qual, scope_qual;
		symbols<> reserved_word;
		
		definition (decl_grammar const& self) {
			cout << "definition(self)" << endl;
			body = lexeme_d [
			          ( ch_p('{') >> ( ~ch_p('{') & ~ch_p('}') ) >> '}' ) 
				    ]
			        | ( ch_p('{') >> body >> '}')
			     ;

			reserved_word = 
				"asm", "auto", "bool", "break", "case", "catch",
				"char", "class", "const", "const_cast",
				"continue", "default", "delete", "do", "double",
				"dynamic_cast", "else", "enum", "explicit",
				"extern", "false", "float", "for", "friend",
				"goto", "if", "inline", "int", "long", "mutable",
				"namespace", "new", "operator", "private",
				"protected", "public", "register",
				"reinterpret_cast", "return", "short", "signed",
				"sizeof", "static", "static_cast", "struct",
				"switch", "template", "this", "throw", "true",
				"try", "typedef", "typeid", "typename", "union",
				"unsigned", "using", "virtual", "void",
				"volatile", "wchar_t", "while"; 

			// essentially a direct copy from Hartmut Kaiser's c_grammar.cpp
			id =
			 lexeme_d[
                    ((alpha_p | '_') >> *(alnum_p | '_' ))
                    // this part prevents reserved words, but allows
                    // identifiers beginning with reserved_words.
                   - (reserved_word >> anychar_p - (alnum_p | '_' ))
                ];
			
			cv_qual = "const", "volatile";
			lifetime_qual = "static", "auto";
			sign_qual = "unsigned", "signed";
			size_qual = "long", "short";
			scope_qual = "register", "extern";
			
			type = id | ( *(cv_qual | lifetime_qual | sign_qual 
			                | size_qual | scope_qual) 
			              >> ("float" | "double" | "int" | size_qual 
			                  | sign_qual) );
			
			// these are obviously insufficient for production use, but that'll
			// come later.
			var_decl = type >> id >> ';';
			// completely ignoring typedef and function defs right now.
			type_decl = ( str_p("struct")  | str_p ("class") ) >> id 
			             >> !body >> ';';
			
			decl = *(var_decl | type_decl);
			
			BOOST_SPIRIT_DEBUG_RULE(id);
			BOOST_SPIRIT_DEBUG_RULE(var_decl);
			BOOST_SPIRIT_DEBUG_RULE(type_decl);
			BOOST_SPIRIT_DEBUG_RULE(body);
			//BOOST_SPIRIT_DEBUG_NODE(reserved_word);
			BOOST_SPIRIT_DEBUG_RULE(decl);

		}
		rule<ScannerT> const& start () { return var_decl; }
	};
};

std::vector<std::string>
Generator::
execute() {
	return execute(c,d);
}

static
std::vector<std::string>
execute (context_t& ctx, OutputDelegate& del) {
//char *start, char *end) {
	std::vector<std::string>  result;
	decl_grammar g;
	context_t::iterator_type first, last;
	first = ctx.begin ();
	last = ctx.end ();
	if (parse (first, last, g, space_p).full) {
		puts ("parsed");
	} else {
		puts ("not parsed.");
	}
	return result;
}

/*
int main (int args, char ** argv) {
	decl_grammar g;
	int fd;
	struct stat st;
	char * buf;
	if (args < 2) {
		puts ("interactive mode");
		buf = new char[16384];
		while (!feof(stdin)) {
			printf (">");
			gets(buf);
			if (strlen(buf))
				execute(buf, buf+strlen(buf));
		}
	} else {
		if (   ((fd = open (argv[1], O_RDONLY)) < 0)
		    || (fstat (fd, &st) < 0)
		    || ((buf = (char*) mmap (0, st.st_size, PROT_READ, 0, fd, 0)) < 0))
		{
			perror (argv[1]);
			return 1;
		} else {
			printf ("reading input from %s\n", argv[1]);
			execute(buf, buf+st.st_size);
		}
	}

	return 0;
}
*/
