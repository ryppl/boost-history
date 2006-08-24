#include "driver/config.h"
#include "map/map.h"

#include <iterator>
#include <algorithm>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <boost/signals.hpp>
// #include <boost/lambda/lambda.hpp>
// #include <boost/lambda/bind.hpp>
// #include <boost/lambda/algorithm.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
namespace opt = boost::program_options;
using namespace boost::filesystem;
using namespace boost;

// clearly set up for Mac OS X 10.4.6: gcc 4.0.0
//  this'll have to get converted into a script that probes
//  the C preprocessor and GCC installation for include paths
//  and macros.
//
// Other platforms, e.g. VC, can be hardcoded as selections.

static vector<string> s_sys_inc_paths, s_inc_paths, s_force_paths;
static vector<string> s_macros, s_predef, s_undef;
static string s_suffix;
static bool s_longlong = false, s_variadics = false, s_c99 = false;

// const vector<string>&
// config::
// system_include_paths () {
// 	if (!s_sys_inc_paths.size ()) {
// 		s_sys_inc_paths.push_back ("/usr/include");
// 		s_sys_inc_paths.push_back ("/usr/include/c++/4.0.0");
// 		s_sys_inc_paths.push_back ("/usr/include/c++/4.0.0/powerpc-apple-darwin8");
// 		s_sys_inc_paths.push_back ("/usr/lib/gcc/powerpc-apple-darwin8/4.0.1/include");
// 	}
// 	return s_sys_inc_paths;
// }

void
configure_mapmanager (MapManager& map) {
	for (vector<string>::const_iterator it = s_sys_inc_paths.begin ();
	     it != s_sys_inc_paths.end ();
	     ++it) {
		map.add(path(*it));
	}
	for (vector<string>::const_iterator it = s_inc_paths.begin ();
	     it != s_inc_paths.end ();
	     ++it) {
		map.add(path(*it));
	}
	map.add (path ("."));
}

std::string configure_getsuffix () {
	return s_suffix;
}

static ostream& operator<< (ostream& o, const vector<string>& v) {
	for (vector<string>::const_iterator it = v.begin (); it != v.end (); ++it) {
		o << *it << "\n";
	}
	return o;
}

void configure_context (context_t& context){
	using namespace boost;
	using namespace boost::wave;
	
	// static vector<string> s_sys_inc_paths, s_inc_paths, s_force_paths;
	// static vector<string> s_macros, s_undef;
	// static string s_suffix;
	// static bool s_longlong, s_variadics, s_c99;
	if (s_longlong)
		context.set_language (enable_long_long(context.get_language ()));
	
	if (s_variadics)
		context.set_language (enable_variadics(context.get_language ()));
		
	if (s_c99)
		context.set_language (
		    static_cast<language_support>(context.get_language () 
		        | support_c99));

	// this one isn't optional.
	context.set_language (language_support (~support_option_emit_line_directives
				                            & context.get_language()));

	for (vector<string>::const_iterator it = s_sys_inc_paths.begin ();
	     it != s_sys_inc_paths.end ();
	     ++it) {
		context.add_sysinclude_path (it->c_str());
	}
	
	for (vector<string>::const_iterator it = s_inc_paths.begin ();
	     it != s_inc_paths.end ();
	     ++it) {
		context.add_include_path (it->c_str());
	}

	context_t::iterator_type first = context.begin();
	vector<string>::const_reverse_iterator cit, rend;
	rend = s_force_paths.rend ();
	for (vector<string>::const_reverse_iterator cit = s_force_paths.rbegin(); 
		 cit != rend; /**/)	{
		string filename(*cit);
		first.force_include(filename.c_str(), ++cit == rend);
	}
	
	for (vector<string>::const_iterator it = s_macros.begin ();
	     it != s_macros.end ();
	     ++it) {
		context.add_macro_definition (it->c_str());
	}
	
	for (vector<string>::const_iterator it = s_undef.begin ();
	     it != s_undef.end ();
	     ++it) {
		context.remove_macro_definition (it->c_str());
	}
}



namespace config_util {
//	boost::signal<void ()> s_reset_inputs;

    // Additional command line parser which interprets '@something' as an 
    // option "config-file" with the value "something".


/*
    // class, which keeps include file information read from the command line
    class include_paths {
    	signals::connection c;
    public:
        include_paths() : seen_separator(false) {
        	c = s_reset_inputs.connect (bind(&include_paths::reset, this));
        }
		~include_paths () { c.disconnect (); }
		
        vector<string> paths;       // stores user paths
        vector<string> syspaths;    // stores system paths
        bool seen_separator;        // command line contains a '-I-' option

		void reset () { seen_separator = false; }

        // Function which validates additional tokens from command line.
        static void 
        validate(boost::any &v, vector<string> const &tokens) {
            if (v.empty())
                v = boost::any(include_paths());

            include_paths *p = boost::any_cast<include_paths>(&v);

            BOOST_ASSERT(p);
            // Assume only one path per '-I' occurrence.
            string const& t = opt::validators::get_single_string(tokens);
            if (t == "-") {
            // found -I- option, so switch behaviour
                p->seen_separator = true;
            } 
            else if (p->seen_separator) {
            // store this path as a system path
                p->syspaths.push_back(t); 
            } 
            else {
            // store this path as an user path
                p->paths.push_back(t);
            }            
        }
    }; */
}

static inline pair<string, string> 
at_option_parser(string const&s)
{
	if ('@' == s[0]) 
		return std::make_pair(string("config-file"), s.substr(1));
	else
		return pair<string, string>();
}

struct token_marker {};

void validate (boost::any& v,
               const vector<string>& values,
               token_marker *, int) {
	opt::validators::check_first_occurrence(v);
	bool marked = false;
	for (vector<string>::const_iterator it = values.begin ();
	     it != values.end ();
	     ++it) {
		if (*it == "-")
			marked = true;
		else {
			if (marked)
				s_inc_paths.push_back(*it);
			else
				s_sys_inc_paths.push_back(*it);
		}
	}
}

//
// Sets up internal tables for configuring the context and mapmanager
// later on.
// returns a list of filenames.
vector<string> configure (int args, const char **argv) {
	// based mostly off of Hartmut Kaiser's boost/tools/wave/cpp.cpp.
	// a lot of it copy-pasted directly.
	opt::options_description desc_generic ("Options allowed anywhere.");
	desc_generic.add_options () 
		("output,o", opt::value<string>(),
			 "specify the name of the translated source file [arg], "
			 "or to stdout [-]")
		("suffix,s", opt::value<string>(&s_suffix)->default_value("_gen"),
			 "suffix used for generated .h and .map files.  Defaults to _gen")
		("include,I", opt::value<token_marker>()->composing (),
			 "an additional include directory")
		("sysinclude,S", opt::value<vector<string> >()->composing (),
			 "an additional system include directory")
		("forceinclude,F", opt::value<vector<string> >()->composing (),
			 "force inclusion of specified file")
		("define,D", opt::value<vector<string> >()->composing(), 
			"specify a macro to define (as macro[=[value]])")
		("predefine,P", opt::value<vector<string> >()->composing(), 
			"specify a macro to predefine (as macro[=[value]])")
		("undefine,U", opt::value<vector<string> >()->composing(), 
			"specify a macro to undefine")
		("long_long", "enable long long support in C++ mode")
		("variadics", "enable certain C99 extensions in C++ mode")
		("c99", "enable C99 mode (implies --variadics)")
		;
		

    opt::options_description desc_cmdline ("Options allowed on the command line only");
    desc_cmdline.add(desc_generic);
	desc_cmdline.add_options()
		("help,h", "print out program usage (this message)")
		("version,v", "print the version number")
		("copyright,c", "print out the copyright statement")
		("config-file", opt::value<string>(), 
			"specify a config file (alternatively: @filepath)")
		("input-file", opt::value<vector<string> >(),
		    "a file to process")
		;
		
	opt::positional_options_description pos_opts;
	pos_opts.add("input-file", -1);
	
	opt::variables_map vm;
	opt::store (opt::command_line_parser(args,(char **)argv)
	            .options(desc_cmdline)
	            .positional(pos_opts)
	            .extra_parser(at_option_parser)
	            .run (),
	            vm);
	if (vm.count("config-file")) {
		ifstream ifs (vm["config-file"].as<string>().c_str());
		if (!ifs) {
			cout << "Could not open the config file\n";
		} else {
			// Read the whole file into a string
			stringstream ss;
			ss << ifs.rdbuf();
			// Split the file content
			char_separator<char> sep("\n\r");
			tokenizer<char_separator<char> > tok(ss.str(), sep);
			vector<string> args;
			copy(tok.begin(), tok.end(), back_inserter(args));
//			cout << "Input file { \n" << args << "}" << endl;
			// Parse the file and store the options
			opt::store(opt::command_line_parser(args)
			     .options(desc_generic)
			     .run(), 
			    vm);            
		}
	}
	opt::notify (vm);
	
	// now extract the relevant data, and configure
	if (vm.count ("forceinclude"))
		s_force_paths = vm["forceinclude"].as<vector<string> >();
	vector<string> paths;		
	if (vm.count ("sysinclude")) {
		paths = vm["sysinclude"].as<vector<string> >();
		copy (paths.begin (), paths.end (), back_inserter(s_sys_inc_paths));
	}
	
	if (vm.count ("define"))
		s_macros = vm["define"].as<vector<string> >();
		
	if (vm.count ("predefine")) {
		paths = vm["predefine"].as<vector<string> >();
		copy (paths.begin (), paths.end (), back_inserter(s_macros));
	}
	if (vm.count ("undefine"))
		s_undef = vm["undefine"].as<vector<string> >();
	s_longlong = vm.count ("long_long") > 0;
	s_c99 = vm.count ("c99") > 0;
	s_variadics = (vm.count ("variadics") > 0) || s_c99;
	return vm["input-file"].as<vector<string> >();
}


/*
template<class Context>
void configure_context (Context& c) {
	typedef std::vector<std::string>::const_iterator  vec_iter_t;
	typedef std::map<std::string, std::string>::const_iterator map_iter_t;
	
	using namespace boost;
	using namespace boost::wave;
	
	c.set_language (enable_long_long(c.get_language()));
	c.set_language (enable_variadics(c.get_language()));
	c.set_language (language_support (support_option_convert_trigraphs 
	                                  | c.get_language()));
	c.set_language (language_support (~support_option_emit_line_directives
				                      & c.get_language()));
// 	c.set_language (language_support (support_option_no_character_validation 
// 	                                  | c.get_language()));
	
	// add in all the system include paths.
	for (vec_iter_t it = config::system_include_paths().begin();
	     it != config::system_include_paths().end();
	     ++it) {
	     c.add_sysinclude_path(it->c_str());
	}
	
	// and now the macros.
	for (map_iter_t it = config::macros().begin();
	     it != config::macros().end();
	     ++it) {
	    try {
			if (it->second.length ())
				c.add_macro_definition( str(boost::format("%s=%s") 
										 % it->first % it->second), 
										 true);
			else
				c.add_macro_definition( it->first, true );
		} catch (...) {
			std::cout << "Failed adding macro definition " 
			          << it->first << "=" << it->second << std::endl;
		}
	}
}*/


/*
1. touch foo.cpp
2. cpp -dM foo.cpp
3. sed/gawk/perl your way to this.
Godspeed.

const map<string,string>&
config::
macros () {
	if (!s_macros.size ()) {
		s_macros["__DBL_MIN_EXP__"]="(-1021)";
		s_macros["__FLT_MIN__"]="1.17549435e-38F";
		s_macros["__CHAR_BIT__"]="8";
		s_macros["__WCHAR_MAX__"]="2147483647";
		s_macros["__DBL_DENORM_MIN__"]="4.9406564584124654e-324";
		s_macros["__FLT_EVAL_METHOD__"]="0";
		s_macros["__DBL_MIN_10_EXP__"]="(-307)";
		s_macros["__FINITE_MATH_ONLY__"]="0";
		s_macros["__GNUC_PATCHLEVEL__"]="1";
		s_macros["__SHRT_MAX__"]="32767";
		s_macros["__LDBL_MAX__"]="1.79769313486231580793728971405301e+308L";
		s_macros["__APPLE_CC__"]="5247";
		s_macros["__UINTMAX_TYPE__"]="long long unsigned int";
		s_macros["__LDBL_MAX_EXP__"]="1024";
		s_macros["__SCHAR_MAX__"]="127";
		s_macros["__USER_LABEL_PREFIX__"]="_";
		s_macros["__STDC_HOSTED__"]="1";
		s_macros["__LDBL_HAS_INFINITY__"]="1";
		s_macros["__DBL_DIG__"]="15";
		s_macros["__FLT_EPSILON__"]="1.19209290e-7F";
		s_macros["__LDBL_MIN__"]="2.00416836000897277799610805135016e-292L";
		s_macros["__ppc__"]="1";
		s_macros["__strong"]="";
		s_macros["__APPLE__"]="1";
		s_macros["__DECIMAL_DIG__"]="33";
		s_macros["__LDBL_HAS_QUIET_NAN__"]="1";
		s_macros["__DYNAMIC__"]="1";
		s_macros["__GNUC__"]="4";
		s_macros["__DBL_MAX__"]="1.7976931348623157e+308";
		s_macros["__DBL_HAS_INFINITY__"]="1";
		s_macros["__weak"]="";
		s_macros["__DBL_MAX_EXP__"]="1024";
		s_macros["__LONG_LONG_MAX__"]="9223372036854775807LL";
		s_macros["__GXX_ABI_VERSION"]="1002";
		s_macros["__FLT_MIN_EXP__"]="(-125)";
		s_macros["__DBL_MIN__"]="2.2250738585072014e-308";
		s_macros["__DBL_HAS_QUIET_NAN__"]="1";
		s_macros["__REGISTER_PREFIX__"]="";
		s_macros["__NO_INLINE__"]="1";
		s_macros["_ARCH_PPC"]="1";
		s_macros["__FLT_MANT_DIG__"]="24";
		s_macros["__VERSION__"]="\"4.0.1 (Apple Computer, Inc. build 5247)\"";
		s_macros["__BIG_ENDIAN__"]="1";
		s_macros["__SIZE_TYPE__"]="long unsigned int";
		s_macros["__FLT_RADIX__"]="2";
		s_macros["__LDBL_EPSILON__"]="4.94065645841246544176568792868221e-324L";
		s_macros["__NATURAL_ALIGNMENT__"]="1";
		s_macros["__FLT_HAS_QUIET_NAN__"]="1";
		s_macros["__FLT_MAX_10_EXP__"]="38";
		s_macros["__LONG_MAX__"]="2147483647L";
		s_macros["__FLT_HAS_INFINITY__"]="1";
		s_macros["_BIG_ENDIAN"]="1";
		s_macros["__LDBL_MANT_DIG__"]="106";
		s_macros["__WCHAR_TYPE__"]="int";
		s_macros["__FLT_DIG__"]="6";
		s_macros["__INT_MAX__"]="2147483647";
		s_macros["__LONG_DOUBLE_128__"]="1";
		s_macros["__FLT_MAX_EXP__"]="128";
		s_macros["__DBL_MANT_DIG__"]="53";
		s_macros["__WINT_TYPE__"]="int";
		s_macros["__LDBL_MIN_EXP__"]="(-968)";
		s_macros["__MACH__"]="1";
		s_macros["__LDBL_MAX_10_EXP__"]="308";
		s_macros["__DBL_EPSILON__"]="2.2204460492503131e-16";
		s_macros["__INTMAX_MAX__"]="9223372036854775807LL";
		s_macros["__FLT_DENORM_MIN__"]="1.40129846e-45F";
		s_macros["__PIC__"]="1";
		s_macros["__FLT_MAX__"]="3.40282347e+38F";
		s_macros["__FLT_MIN_10_EXP__"]="(-37)";
		s_macros["__INTMAX_TYPE__"]="long long int";
		s_macros["__GNUC_MINOR__"]="0";
		s_macros["__DBL_MAX_10_EXP__"]="308";
		s_macros["__LDBL_DENORM_MIN__"]="4.94065645841246544176568792868221e-324L";
		s_macros["__PTRDIFF_TYPE__"]="int";
		s_macros["__LDBL_MIN_10_EXP__"]="(-291)";
		s_macros["__LDBL_DIG__"]="31";
		s_macros["__POWERPC__"]="1";
	}
	return s_macros;
}
*/